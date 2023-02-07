
//Modular signal analysis program
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_OF_Channels "\nEnter the number of input channels(in range of 2 to 10) or 0 to quit: "

//declaring functions' prototypes
int Setup(int *pnChan, int *pnPt);
double dRand(void);
int CollectData(int nChan, int nPt, double Data[][nPt]);
int menu(void);
void clean_buffer(void);
int Result(int nChan, double Avg[]);
int WriteBinary(int nChan, int nPt, double Data[][nPt], const char *pfname);
int ReadBinary(int nChan, int nPt, double Data[][nPt], const char * pfname);
int AvgData(int nChan, int nPt, double Data[][nPt], double Avg[]);
int Result(int nChan, double Avg[]);


int main(void){
    void *pData, *pData2;
    int nChan, nPt;
    char file[20] = "hw#13_file";
    //random num generate
    srand((unsigned int) time(0));
        //while setup function returns 0
        while((Setup(&nChan, &nPt)) == 0){
            //check if the memory WAS allocated
            if((pData = malloc(sizeof(double) * nChan * nPt)) == NULL)
                printf("Please enter the fewer numbers of channels and points.\n");
                //calling CollectData + checking if it was succesfully collected/ written/ read
            else if ((pData = malloc(sizeof(double) * nChan * nPt)) != NULL){
                if (CollectData(nChan, nPt, pData) == 0){
                    if (WriteBinary(nChan, nPt, pData, file) == 0){
                        printf("\nWriteBinary was succesful.\n");
                        //allocate the memory for ReadBinay and other fucntion below.
                        if((pData2 = malloc(sizeof(double) * nChan)) != NULL){
                            //double data2[nChan];
                            printf("Memory was allocated for Data2\n");
                            if (ReadBinary(nChan, nPt, pData, file) == 0){
                                printf("ReadBinary was successful\n");
                                AvgData(nChan, nPt, pData, pData2);
                                printf("\n");
                                Result(nChan, pData2);
                                //free allocated memory
                                free(pData2);
                            }
                        }
                        else
                            printf("Error! The memory was not allocated.\n");
                    }
                //freeing the memory allocated for the array
                free(pData);
                fflush(stdin);
                    }
            }
        }
    return 0;
}

double dRand(void){
    return rand()* 10.0 / RAND_MAX;
}

int Setup(int *pnChan, int *pnPt){
    //my user for an input
    int result;
    printf(NUM_OF_Channels);
    while (scanf("%d", pnChan) != 1 || ((*pnChan < 2 || *pnChan > 10) && *pnChan != 0)){
         clean_buffer();
         printf(NUM_OF_Channels);
    }
    //printf("%d\n", *pnChan);
   //storing the number returned by menu in the adress of pnPt.
    if (*pnChan != 0){
        *pnPt = menu();
        return 0;
    }
    else
        return -1;
}

int CollectData(int nChan, int nPt, double Data[][nPt]){

    //i num of row
    for (int i = 0; i < nChan; i++){
        // j is for the num of columns
        for (int j = 0; j < nPt; j++)
            Data[i][j] = dRand();
    }
    return 0;
}

int menu(void){
    int num;
    //give the user option of pointers
    printf("Select the number of points: \n");
    printf("2.2048  4.4096  8.8192: ");
    //scan the input and chceck if this was one of the options
    while((scanf("%d", &num) != 1) || (num != 2 && num != 4 && num != 8)){
        printf("Invalid choice.\nSelect the number of points: \n");
        printf("2.2048 4.4096  8.8192: ");
    }
    //cases of points
    switch(num){
        case 2: num = 2048;
            break;
        case 4: num = 4096;
            break;
        case 8: num = 8192;
    }
    return num;
}

void clean_buffer(void){
    while(getchar() != '\n');
}

int WriteBinary(int nChan, int nPt, double Data[][nPt], const char *pfname){
    //open the file and store it in the pointer
    FILE *fp;
    int result = 0;

    if ((fp = fopen(pfname, "wb")) == NULL){
        printf("Error! opening the file.\n");
    }
    else // writing out the num of channels
    {
        if ((result = fwrite(&nChan, sizeof(int), 1, fp)) != 0){
            //and points
            if((result = fwrite(&nPt, sizeof(int), 1, fp) != 0)){
                //write put the array of Data
                if ((result = fwrite(Data, sizeof(double) * nPt * nChan, 1, fp)) == 0)
                    printf("Failed to write the data!\n");
            }
        }
        //closing the file
        fclose(fp);
    }
    return result - 1;
}

int ReadBinary(int nChan, int nPt, double Data[][nPt], const char * pfname){
    int chan, pt, result;
    //open the file
    FILE *fp;
    if ((fp = fopen(pfname, "rb")) == NULL){
        printf("Error! opening the file.\n");
    }
    //read the num of channels and points
    if((result = fread(&chan, sizeof(int), 1, fp)) != 0){ //if the number of channels is read
        if((result = fread(&pt, sizeof(int), 1, fp)) != 0){ //if the num pf points is read
            if (nChan == chan && nPt == pt){ //if the read channels and points are equal to the original ones
                if ((result = fread(Data, sizeof(double) * nChan, 1, fp)) == 0)
                    printf("Failed to read the data!\n");
            }
        }
    }
    return result - 1;
}

int AvgData(int nChan, int nPt, double Data[][nPt], double Avg[]){

    //go over each row
    for (int i = 0; i < nChan; i++){
        double sum = 0;
        for (int j = 0; j < nPt; j++)//getting every point
            sum += Data[i][j];
        Avg[i] = sum / nPt;
    }
    return 0;
}

int Result(int nChan, double Avg[]){
    //for every channel
    for (int i = 0; i < nChan; i++)
        printf("Average of Channel %d: %f\n", (i+1), Avg[i]);
    return 0;
}
