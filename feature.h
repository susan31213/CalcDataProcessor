#include<iostream>
#include<cmath>

class Feature
{
    public:
        Feature(float arr[], int const num);
        int Size();
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
        float Entropy();
        float SMA();
        float AR(int argu);

    private:
        float* vector;
        int size;

};
