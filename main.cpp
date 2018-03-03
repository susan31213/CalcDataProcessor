#include"feature.h"

using namespace std;


int WINDOW_SIZE = 60;

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "Usage: ./feature <01_0.calcA> <01_1.calcA> ..." << endl;
        exit(1);
    }
	
	string filename = argv[1];
	int end = filename.find("_");
	string path = "output/" + filename.substr(7,end-7) + ".calcF";
	cout << path << endl;
	ofstream fout(path.c_str());
    for(int z=1; z<argc; z++)
    {
        ifstream fin(argv[z]);
        if(!fin)
        {
            cout << "Can't open file!" << endl;
            exit(1);
        }
        if(!fout)
        {
            cout << "Can't create file!" << endl;
            exit(1);
        }

        // Get data
        string line;
        int frame;
        std::getline(fin, line);
        fin >> frame;
        double interval[(frame-1)];
        for(int i=0; i<frame-1; i++)
            fin >> interval[i];
		cout << "FINISH GET FREAM... " << frame << endl;
        double input[frame*59*6];
        for(int i=0; i<59*6; i++)
        {
            int boneIdx, floatIdx;
            fin >> boneIdx >> floatIdx;
            for(int j=0; j<frame; j++)
                fin >> input[i*frame+j];
		}
		cout << "FINISH GET " << z << "th file ..." << endl;
		
		// Calculate feature and trun it into SVM format
		int k = 0;
		double data[WINDOW_SIZE], inter[WINDOW_SIZE-1], ffta[WINDOW_SIZE], ara[11];
		while(k+WINDOW_SIZE < frame)
		{
			fout << filename.substr(7,end-7) << " ";
			int cnt = 0;
			for(int j=0; j<59*6; j++)
			{
				int check = j/6;
				if(check == 14 || check == 15 || check == 16 || check == 18 || check == 19 || check == 22 ||
					check == 26 || check == 30 || check == 34 || check == 37 || check == 38 || check == 39 ||
					check == 41 || check == 42 || check == 45 || check == 49 || check == 53 || check == 57)
				{
					for(int i=0; i<WINDOW_SIZE-1; i++)
					{
						data[i] = input[j*frame+i+k];
						inter[i] = interval[i+k];
					}
					data[WINDOW_SIZE-1] = input[j*frame+WINDOW_SIZE-1+k];
					Feature f(data, inter, WINDOW_SIZE, ffta, ara);
					f.FFT(0);
					f.AR(10);
					fout << cnt*24+1 << ":" <<  f.Mean() << " " << cnt*24+2 << ":" << f.Var() << " " << cnt*24+3 << ":" << f.StdDev() << " " << cnt*24+4 << ":" << f.Integ() << " "
						 << cnt*24+5 << ":" << f.RMS() << " " << cnt*24+6 << ":" << f.ZCR() << " " << cnt*24+7 << ":" << f.MCR() << " " << cnt*24+8 << ":" << f.Skew() << " "
						 << cnt*24+9 << ":" << f.Kurt() << " " << cnt*24+10 << ":" << f.FFTCoff(0) << " " << cnt*24+11 << ":" << f.FFTCoff(1) << " " << cnt*24+12 << ":" << f.FFTCoff(2) << " "; 
					for(int i=0; i<10; i++)
						fout << cnt*24+13+i << ":" << f.ARCoff(i) << " ";
					fout << cnt*24+23 << ":" << f.Entropy(10) << " " << cnt*24+24 << ":" << f.SMA() << " ";
					fout.flush();
					cnt++;
				}
			}
			fout << endl;
			k+=5;
		}
        fin.close();
    }
    fout.close();
    return 0;
}
