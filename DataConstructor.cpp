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

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("***********************************************\n");
        printf("*                                             *\n");
        printf("*  This program make Processor's output data  *\n");
        printf("*  become every float placed according time.  *\n");
        printf("*  A channel is in a line.                    *\n");
        printf("*                                             *\n");
        printf("*          Usage: a.exe <calc file>           *\n");
        printf("*                                             *\n");
        printf("***********************************************\n");
    }

    FILE * inf, *outf;
    int frame = 0;
    float ***ptr;
    inf = fopen(argv[1], "r");

    if(!inf)
    {
        printf("File can not open.\n");
        exit(1);
    }

    char buffer[10000];
    char c;
    int stamp = 0, lastStamp = 0;
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
    int interval[frame-1];

    // construct data
    for(k=0; k<frame; k++)
    {
        
        fscanf(inf, "%d", &stamp);
        if(k>0)
        {
            interval[k-1] = stamp-lastStamp;
        }
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
        
        if(k != frame-1)
        {
            lastStamp = stamp;
        }
    }

    // output file
    char outputPath[100] = {};
    strcpy(outputPath, argv[1]);
    strcat(outputPath, "A");
    outf = fopen(outputPath, "w");
    if(!outf)
    {
        printf("File can not create!\n");
        exit(1);
    }
    fprintf(outf, "BONE\tTYPE\tDATA\n%d ", frame);
    for(i=0; i<frame-1; i++)
        fprintf(outf, "%d ", interval[i]);
    fputs("\n", outf);
    fflush(outf);

    for(i=0; i<BONE_NUM; i++)
        for(j=0; j<FLOAT_NUM; j++)
        {
            fprintf(outf, "%d\t%d\t", i, j);
            for(k=0; k<frame; k++)
            {
                fprintf(outf, "%f", ptr[j][i][k]);
                if(k!= frame-1)
                    fprintf(outf, " ");
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
