#include"feature.h"

using namespace std;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Usage: ./feature <*.calcA>" << endl;
        exit(1);
    }
    else
    {

        ifstream fin(argv[1]);
        if(!fin)
        {
            cout << "Can't open file!" << endl;
            exit(1);
        }
        string path(argv[1]);
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
			f.FFT(0);
			f.AR(frame-1);
			fout << f.Mean() << " " << f.Var() << " " << f.StdDev() << " " << f.Integ() << " "
                 << f.RMS() << " " << f.ZCR() << " " << f.MCR() << " " << f.Skew() << " "
                 << f.Kurt() << " " << f.FFTCoff(0) << " " << f.FFTCoff(1) << " " << f.FFTCoff(2) << " "; 
			for(int k=0; k<10; k++)
				fout << f.ARCoff(k) << " ";
			fout << f.Entropy(10) << " " << f.SMA() << " ";
            fout << endl;
            fout.flush();
        }

        fin.close();
        fout.close();
    }
    return 0;
}
