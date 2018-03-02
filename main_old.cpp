#include"feature.h"

using namespace std;

int main(int argc, char** argv)
{
	// Calculate feature
	int frame = 10;
	double input[frame] = {1,2,3,4,5,6,7,8,9,10};
	double interval[frame] = {1,1,1,1,1,1,1,1,1,1};
	Feature f(input, interval, frame);
	f.AR(frame-1);
	for(int i=0; i<frame; i++)
	    cout << f.ARCoff(i) << " ";
	cout << endl;
    return 0;
}
