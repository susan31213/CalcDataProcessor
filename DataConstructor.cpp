#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string.h>

using namespace std;

int BONE_NUM = 59;
int FLOAT_NUM = 16;

float*** ThreeDArr(int a, int b, int c)
{
    float ***A = new float**[a];
    for(int i=0 ;i<a; i++)
    {
        A[i] = new float*[b];
        for(int j = 0; j<b; j++)
        {
            A[i][j] = new float[c];
        }
    }
    return A;
}

int main()
{
    FILE * inf, *outf;
    int frame = 0;
    float ***ptr;
    inf = fopen("output/01_0.calc", "r");

    if(!inf)
    {
        printf("File can not open.\n");
        exit(1);
    }

    char buffer[10000];
    char c;
    int stamp;
    int i,j,k;

    char* delim = ",";
    char* split;
    

    // check how many frames
    while(!feof(inf))
    {
        fgets(buffer, sizeof(buffer), inf);
        frame++;
    }
    frame--;
    fseek(inf, 0, SEEK_SET);
    printf("FRAME: %d", frame);
    
    // allocate memory
    //ptr = new float[FLOAT_NUM][BONE_NUM][frame];
    ptr = ThreeDArr(FLOAT_NUM, BONE_NUM, frame);


    // construct data
    for(k=0; k<frame; k++)
    {
        fscanf(inf, "%d", &stamp);
        fgets(buffer, sizeof(buffer), inf);
        split = strtok(buffer,delim);
        for(i=0; i<BONE_NUM; i++)
        {
            for(j=0; j<FLOAT_NUM; j++)
            {
                sscanf(split, "%f", &ptr[j][i][k]);
                split = strtok(NULL, delim);
            }
        }
        sscanf(split, ",%f,%f%c", &ptr[k*BONE_NUM*FLOAT_NUM + BONE_NUM * FLOAT_NUM], &ptr[k*BONE_NUM*FLOAT_NUM + BONE_NUM*FLOAT_NUM + 1], &c);
    }

    // output file
    outf = fopen("output/01_0_1.calc", "w");
    if(!outf)
    {
        printf("File can not create!\n");
        exit(1);
    }
    fprintf(outf, "BONE\tTYPE\tDATA(%d))\n", frame);

    for(i=0; i<BONE_NUM; i++)
        for(j=0; j<FLOAT_NUM; j++)
        {
            fprintf(outf, "%d\t%d\t", i, j);
            for(k=0; k<frame; k++)
            {
                fprintf(outf, "%f", ptr[j][i][k]);
                if(k!= frame-1)
                    fprintf(outf, ",");
                else
                    fprintf(outf,"\n");
                fflush(outf);
            }
        }

    fclose(inf);
    fclose(outf);
    delete ptr;

    return 0;
}
