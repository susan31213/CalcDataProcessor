#include "feature.h"
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_blas.h>

using namespace std;

int main(int argc, char** argv)
{
    if(/*argc != 2*/0)
    {
        cout << "Usage: ./feature <*.calcA>" << endl;
        exit(1);
    }
    else
    {
        
        ifstream fin(/*argv[1]*/"output/01_1.calcA");
        if(!fin)
        {
            cout << "Can't open file!" << endl;
            exit(1);
        }
        string path(/*argv[1]*/"output/01_1.calcA");
        path += "F";
        ofstream fout(path.c_str());
        if(!fout)
        {
            cout << "Can't create file!" << endl;
            exit(1);
        }
        fout << "BONE\tTYPE\tFEATURE" << endl;
        
        // Get data
        string line;
        int frame;
        std::getline(fin, line);
        fin >> frame;
        double interval[frame-1];
        for(int i=0; i<frame-1; i++)
            fin >> interval[i];
		cout << "FINISH GET FREAM..." << endl;
        // Calculate feature
        double input[frame];
		Feature f(input, interval, frame);
        for(int i=0; i<59*16; i++)
        {
            int boneIdx, floatIdx;
            fin >> boneIdx >> floatIdx;
            fout << boneIdx << " " << floatIdx << " ";
            for(int j=0; j<frame; j++)
            {
                fin >> input[j];
            }
			cout << "FINISH GET " << i << "th data ..." << endl;

			//fout << f.Mean() << " " << f.Var() << " " << f.StdDev() << " " << f.Integ() << " "
            //     << f.RMS() << " " << f.ZCR() << " " << f.MCR() << " " << f.Skew() << " "
            //     << f.Kurt() << " " << f.FFT(0) << " " << f.Entropy(10) << " " << f.SMA() << " ";
			int order = (frame < 10)? frame-1 : 10;
            for(int k=1; k<order; k++)
                cout << f.AR(i) << " ";
            cout << endl;
            fout.flush();
        }

        fin.close();
        fout.close();
    }
    return 0;
}
