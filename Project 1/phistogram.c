//
//  phistogram.c
//  phistogram
//
//  Created by Huseyin Taskesen on 18.10.2018.
//  Copyright © 2018 Huseyin Taskesen. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int num=0;
int status=0;


int main (int argc, const char * argv[]) {
    if (argc < 6)
    {
        printf("you need to give at least 6 arguments\n");
        exit(1);
    }
    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    int numberOfBins = atoi(argv[3]);
    int numProcess = atoi(argv[4]);
    int binWidth = (upper-lower) / numberOfBins;
    int occurence[numberOfBins];
    

        for(int i =0; i<numberOfBins;i++){
        occurence[i] = 0;
    }
    pid_t pid, wpid;
    int result[numberOfBins];
    char intermediateFile[numberOfBins];
    //clock begins
    clock_t begin = clock();
    
    for (int i = 0 ; i < numProcess; i++){
        
        
        
       sprintf(intermediateFile,"Intermediate_%d",i);
    
        pid = fork();
        if(pid==0)
            {
            //open file
            FILE *file = fopen(argv[5+i], "r");
           if(file == 0) {
                perror("fopen");
                exit(1);
            }
            //finding the interval for the current child
            for (int counter = 0 ; counter < numberOfBins ; counter++)
            {
                //fscanf(file,"%d",&num);
                int cLower = lower + counter * binWidth;
                int cUpper  = lower + (counter + 1) * binWidth;
                //exception for the maximum value
                
                while(fscanf(file,"%d",&num)==1){
                if(num == upper){
                        occurence[numberOfBins-1]++;
                    }
                if(num >= cLower && num < cUpper)
                {
                    occurence[counter]++;
                }
            }
        }//below this line, every child create their intermediate file.
            fclose(file);

            FILE *intermediateFileHandler = fopen(intermediateFile,"w");
            for(int i=0; i<numberOfBins; i++)
            {
                fprintf(intermediateFileHandler, "%d\n",occurence[i]);
            }
            fclose(intermediateFileHandler);
           exit(0);
        }//end of child process

    }//wait children to finish
    while ((wpid = wait(&status)) > 0);
    //parent open intermediate files
    for(int i=0;i<numProcess;i++){
        sprintf(intermediateFile,"Intermediate_%d",i);
        FILE *readIntermediate = fopen(intermediateFile, "r");
        
        for (int k = 0; k < numberOfBins && ( fscanf(readIntermediate, "%d" , &num) == 1); ++k)
        {
            result[k] = result[k] + num;
           // printf["%d",result[numberOfBins-1]];
        }
    }
    
    FILE *resultFile = fopen(argv[5+numProcess], "w");
    if(resultFile == 0) {
        perror("fopen");
        exit(1);
    }
    
    for(int p=0; p < numberOfBins; p++){
        fprintf(resultFile,"%d:%d\n",p,result[p]);
     
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

   printf("%f\n",time_spent);
    
    return 0;
}



