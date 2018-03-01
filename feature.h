#include<iostream>
#include<cmath>
#include<string>
#include<fstream>
#include<gsl/gsl_statistics.h>
#include<gsl/gsl_blas.h>

class Feature
{
    public:
        
        // Constructor: initaillize with data array and size 
        Feature(double arr[], double time[],int const num);
        ~Feature();

        // Return size of array
        int Size();

        // Statistics function
        float Mean();
        float Var();
        float StdDev();
        float Integ();
        float RMS();
        float ZCR();
        float MCR();
        float Skew();
        float Kurt();
        float FFT(int order);
        float Entropy(int divide);
        float SMA();
        float AR(double order);

    private:
        double* vector;
        double* interval;
        int size;

};
