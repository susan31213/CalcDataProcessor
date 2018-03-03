#include "feature.h"

using namespace std;

gsl_matrix *x;
gsl_matrix *efp;
gsl_matrix *ebp;
gsl_matrix *front, *tmp, *tmp2, *ef, *ef2;

Feature::Feature(double arr[], double time[], int num, double fft[], double ar[])
{
    vector = arr;
    interval = time;
    size = num;
	fftResult = fft;
	arResult = ar;
//    for(int i=0; i<size; i++)
//    {
//        vector[i] = arr[i];
//        if(i<size-1)
//            interval[i] = time[i];
//    }
}

Feature::~Feature()
{
   // free(vector);
   // free(interval);
}

float Feature::FFTCoff(int index)
{
    return (float)fftResult[index];
}

float Feature::ARCoff(int index)
{
    return (float)arResult[index];
}

float Feature::Mean()
{
    double mean = gsl_stats_mean(vector, 1, size);
    return (float)mean;
}

float Feature::Var()
{
    double var = gsl_stats_variance(vector, 1, size);
    return (float)var;
}

float Feature::StdDev()
{
    double sd = gsl_stats_sd(vector, 1, size);
    return (float)sd;
}

float Feature::Integ()
{
    double result = 0;
    for(int i=0; i<size-1; i++)
    {
        result += (vector[i]+vector[i+1])/2 * interval[i];
    }

    return (float)result;
}

float Feature::RMS()
{
    float result;
    for(int i=0; i<size; i++)
    {
        result += pow(vector[i], 2);
    }

    return (float)sqrt(result/size);
}

float Feature::ZCR()
{
    float result;
    for(int i=1; i<size; i++)
    {
        if(vector[i]*vector[i-1] < 0)
            result++;
    }

    return result/(size-1);
}

float Feature::MCR()
{
    float result;
    for(int i=1; i<size; i++)
    {
        if((vector[i] > Feature::Mean() && vector[i-1] < Feature::Mean()) || (vector[i]) < Feature::Mean() && vector[i-1] > Feature::Mean())
            result++;
    }

    return result/(size-1);
}


float Feature::Skew()
{
    float m3;

    for(int i=0; i<size; i++)
    {
        m3 += pow((vector[i] - Feature::Mean()),3);
    }
    m3 /= size;

    return m3 / pow(Feature::Var(),1.5);
}

float Feature::Kurt()
{
    float result;

    // -3 ???
    result = pow(Feature::Mean()/Feature::StdDev(), 4) - 3;

    return result;
}

float Feature::FFT(int order)
{
    for(int i=0; i < size; i++)
        fftResult[i] = vector[i];
    //order???
    gsl_fft_real_wavetable *wave;
    gsl_fft_real_workspace *work;

    wave = gsl_fft_real_wavetable_alloc(size);
    work = gsl_fft_real_workspace_alloc(size);

    gsl_fft_real_transform(fftResult, 1, size, wave, work);

    gsl_fft_real_wavetable_free(wave);
    gsl_fft_real_workspace_free(work);
    return 0;
}

float Feature::Entropy(int divide)
{
    // Calculate Probability, divied into # of divide
    gsl_vector * x = gsl_vector_alloc(size);
    for(int i=0; i<size; i++)
    {
        gsl_vector_set(x, i, vector[i]);
    }

    double max, min;
    gsl_vector_minmax(x, &min, &max);
    double slice = (max-min)/divide;
    double probInterval[divide] = {0};

	for(int i=0; i<size; i++)
    {
        for(int j=0; j<divide;j++)
        {
            if(min + slice * j > vector[i])
            {
                probInterval[j-1]++;
                break;
            }
            else if(j==divide-1)
            {
                probInterval[j]++;
            }
        }
    }
    for(int i=0; i<divide; i++)
        probInterval[i] /= size;

    // Calculate Entropy
    double result = 0;
    for(int i=0; i<size; i++)
    {
        double prob;
        for(int j=0; j<divide; j++)
        {
            if(min + slice * j > vector[i])
            {
                prob = probInterval[j-1];
                break;
            }
            else if(j==divide-1)
            {
                prob = probInterval[j];
            }
        }

        result += prob * log2(prob);
    }
	gsl_vector_free(x);
    return (float)-result;
}

float Feature::SMA()
{
    float result;

    for(int i=0; i<size; i++)
    {
        result += std::abs (vector[i]);
    }

    return result / size;
}

float Feature::AR(double order)
{
    // via Burg method
    int i, j;
    x = gsl_matrix_calloc(size, 1);
	efp = gsl_matrix_alloc(size-1, 1);
	ebp = gsl_matrix_alloc(size-1, 1);
	// Initialization efp & ebp
    for(i=0; i<size-1; i++)
    {
        gsl_matrix_set(x, i, 0, vector[i]);
        gsl_matrix_set(efp, i, 0, vector[i+1]);
        gsl_matrix_set(ebp, i, 0, vector[i]);
    }
    gsl_matrix_set(x, size-1, 0, vector[size-1]);
    gsl_matrix *a = gsl_matrix_calloc(1, order+1);
    gsl_matrix_set(a, 0, 0, 1);

    // Calculate
    for(i=0; i<order; i++)
    {
        // Calculate the next order reflection (parcor) coefficient
        front = gsl_matrix_alloc(ebp->size1, ebp->size2);
        gsl_matrix_memcpy(front, ebp);
        gsl_matrix_scale(front, -2);
        double front_ans = 0;
		int size1 = efp->size1;
        for(j=0; j<size1/*j<size-1-i*/; j++)
            front_ans += gsl_matrix_get(front, j, 0) * gsl_matrix_get(efp, j, 0);

        double back_ans1 = 0, back_ans2 = 0;
        for(j=0; j<size1/*j<size-1-i*/; j++)
        {
            back_ans1 += gsl_matrix_get(efp, j, 0) * gsl_matrix_get(efp, j, 0);
            back_ans2 += gsl_matrix_get(ebp, j, 0) * gsl_matrix_get(ebp, j, 0);
        }
        gsl_matrix_free(front);
        double k = front_ans / (back_ans1 + back_ans2);

        // Update the forward and backward prediction errors
        tmp = gsl_matrix_alloc(efp->size1-1, 1);
        tmp2 = gsl_matrix_alloc(ebp->size1-1, 1);

        ef = gsl_matrix_alloc(efp->size1-1, 1);
        ef2 = gsl_matrix_alloc(ebp->size1-1, 1);
		int size2 = ef->size1;
        for(j=0; j<size2; j++)
        {
            gsl_matrix_set(ef, j, 0, gsl_matrix_get(efp, j+1, 0));
            gsl_matrix_set(ef2, j, 0, gsl_matrix_get(ebp, j+1, 0));
        }
        gsl_matrix_scale(ef2, k);
        gsl_matrix_add(ef, ef2);
        gsl_matrix_memcpy(tmp, ef);
        gsl_matrix_free(ef);
        gsl_matrix_free(ef2);

        ef = gsl_matrix_alloc(efp->size1-1, 1);
        ef2 = gsl_matrix_alloc(ebp->size1-1, 1);
        for(j=0; j<size2; j++)
        {
            gsl_matrix_set(ef, j, 0, gsl_matrix_get(efp, j, 0));
            gsl_matrix_set(ef2, j, 0, gsl_matrix_get(ebp, j, 0));
        }
        gsl_matrix_scale(ef, k);
        gsl_matrix_add(ef, ef2);
        gsl_matrix_memcpy(tmp2, ef);
        gsl_matrix_free(ef);
        gsl_matrix_free(ef2);

        gsl_matrix_free(efp);
        gsl_matrix_free(ebp);
        efp = gsl_matrix_alloc(tmp->size1, tmp->size2);
        ebp = gsl_matrix_alloc(tmp2->size1, tmp2->size2);
        gsl_matrix_memcpy(efp, tmp);
        gsl_matrix_memcpy(ebp, tmp2);
        gsl_matrix_free(tmp);
        gsl_matrix_free(tmp2);

        // Update the AR coeff
        tmp = gsl_matrix_alloc(1, order+1);
        gsl_matrix_memcpy(tmp, a);
        gsl_matrix_scale(tmp, k);
        for(j=0; j<=i; j++)
        {
            gsl_matrix_set(a, 0, j+1, gsl_matrix_get(a, 0, j+1) + gsl_matrix_get(tmp, 0, i-j));
        }
        gsl_matrix_free(tmp);

    }
    int size3 = a->size2;
    for(int z=0; z<size3; z++)
		arResult[z] = gsl_matrix_get(a, 0, z);
    gsl_matrix_free(x);
    gsl_matrix_free(efp);
    gsl_matrix_free(ebp);
    return 0;


}
