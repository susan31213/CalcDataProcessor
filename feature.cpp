#include "feature.h"

using namespace std;

Feature::Feature(float arr[], int num)
{
    vector = new float[num];
    size = num;
    for(int i=0; i<size; i++)
        vector[i] = arr[i];
}


float Feature::Mean()
{
    float result;

    for(int i=0; i<size; i++)
        result += vector[i];

    return result/size;
}

float Feature::Var()
{
    float mean = Feature::Mean();
    float result;

    for(int i=0; i<size; i++)
    {
        result += (vector[i]-mean)*(vector[i]-mean);
    }

    // or result/(size-1)??
    return result/size;
}

float Feature::StdDev()
{
    return (float)sqrt((double)Feature::Var());
}

float Feature::Integ()
{
    float result;

    // time step = 1/60s???

    return result;
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
    return -999;

}

float Feature::Entropy()
{
    return -999;

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

float Feature::AR(int argu)
{
    return -999;

}

int main()
{
    float test[10] = {1,2,3,4,5,6,7,8,9,10};
    Feature f = Feature(test, 10);

    cout << "MEAN: " << f.Mean() << endl;
    cout << "VAR: " << f.Var() << endl;
    cout << "STDDEV: " << f.StdDev() << endl;
    cout << "INTEGR: " << f.Integ() << endl;
    cout << "RMS: " << f.RMS() << endl;
    cout << "ZCR: " << f.ZCR() << endl;
    cout << "MCR: " << f.MCR() << endl;
    cout << "SKEW: " << f.Skew() << endl;
    cout << "KURT: " << f.Kurt() << endl;
    cout << "FFT: " << f.FFT(0) << endl;
    cout << "ENTROPY: " << f.Entropy() << endl;
    cout << "SMA: " << f.SMA() << endl;
    cout << "AR: " << f.AR(0) << endl;

}







