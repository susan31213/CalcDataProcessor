#include<iostream>
#include<string.h>
#include<fstream>
#include<cstdio>
#include<cstdlib>

using namespace std;

int BONE_NUM = 59;
int FLOAT_NUM = 16;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Useage: a.out <raw file> <label file>\n");
        exit(1);
    }

    FILE *calcFile, *labelFile;
    ofstream fout;
    calcFile = fopen(argv[1], "r");
    labelFile = fopen(argv[2], "r");

    if(!calcFile)
        cout << "Can't open .calc file!\n";
    else if(!labelFile)
        cout << "Can't open .label file!\n";
    else
    {
        char buffer[8000];

        // label
        int index;
        char name[10], last[10] = "";
        int times = 0;
        int start, end;

        // calc
        int stamp;
        float data[BONE_NUM*FLOAT_NUM+2];

        // output file
        FILE *output;

        do
        {
            // read label name
            fscanf(labelFile, "%d %s %d %d", &index, name, &start, &end);
            fgetc(labelFile);
            //printf("START: %d\nEND: %d\n======================================\n", start, end);
            printf("=========LAST LABEL: %s      LABEL: %s==========\n", last, name);
            if(!strcmp(name, last))
                times++;
            else
                times = 0;

            char filepath[30] = "output/";
            char timesStr[2];
            strcat(filepath, name);
            strcat(filepath, "_");
            sprintf(timesStr, "%d", times);
            strcat(filepath, timesStr);
            strcat(filepath, ".calc");

            printf("%s\n", filepath);

            output = fopen(filepath, "w");

            while(!feof(calcFile))
            {
                // check stamp >= start time && <= end time
                fscanf(calcFile, "%d ", &stamp);
                printf("%d\n", stamp);
                if(stamp < start)
                    fgets(buffer, sizeof(buffer), calcFile);
                else if(stamp > end)
                {
                    fgets(buffer, sizeof(buffer), calcFile);
                    fclose(output);
                    break;
                }
                else
                {
                    int i, j;
                    char c1, c2;
                    fprintf(output, "%d ", stamp);
                    for(i=0; i<BONE_NUM; i++)
                    {
                        for(j = 0; j<FLOAT_NUM; j++)
                        {
                            fscanf(calcFile, "%f%c", &data[i*FLOAT_NUM + j], &c1);

                            //printf("=================\ni: %d, j: %d    %f\n=================\n", i, j, data[i*FLOAT_NUM + j]);

                            fprintf(output, "%f", data[i*FLOAT_NUM + j]);
                            fputc(',', output);
                        }
                    }
                    fscanf(calcFile, "%f%c%f%c", &data[BONE_NUM*FLOAT_NUM], &c1, &data[BONE_NUM*FLOAT_NUM + 1], &c2);
                    fprintf(output, "%f,%f\n", data[BONE_NUM*FLOAT_NUM], data[BONE_NUM*FLOAT_NUM+1]);
                    //printf("===========LAST2: %f, %f============\n", data[BONE_NUM*FLOAT_NUM], data[BONE_NUM*FLOAT_NUM+1]);
                    fflush(output);
                }
            }
            strcpy(last, name);

        } while(!feof(labelFile));

        fclose(calcFile);
        fclose(labelFile);

    }
    return 0;
}
