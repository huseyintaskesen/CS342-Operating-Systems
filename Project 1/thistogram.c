//
//  thistogram.c
//  phistogram
//
//  Created by Huseyin Taskesen on 18.10.2018.
//  Copyright © 2018 Huseyin Taskesen. All rights reserved.
//


#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int number = 0;
struct occurence_st count;

struct arg_st {
    int binWidth;
    int numberOfBins;
    int lower;
    int upper;
    char fileName[20];
};
struct occurence_st{
    int* occurence;
};

static void *computeOccurence(void *argument)
{
    struct arg_st *arg = argument;

    int binWidth = arg->binWidth;
    int numberOfBins = arg->numberOfBins;
    int lower = arg->lower;
    int upper = arg->upper;
    char *fileName = arg->fileName;
    
    FILE *file = fopen(fileName, "r");

    for (int counter = 0 ; counter < numberOfBins ; counter++)
    {
        double cLower = lower + counter * binWidth;
        double cUpper  = lower + (counter + 1) * binWidth;
        //open file
        if(file == 0) {
            perror("fopen");
            exit(1);
        }

        while(fscanf(file,"%d",&number)==1){
        //exception for the maximum value
        if(number == upper){
            count.occurence[numberOfBins-1]++;
        }
        if(number >= cLower && number < cUpper)
        {
            count.occurence[counter]++ ;
        }
    }
}
    
    return NULL;
}



int main (int argc, const char * argv[]) {
    if (argc < 6)
    {
        printf("you need to give at least 6 arguments\n");
        exit(1);
    }

    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    int numberOfBins = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    
    pthread_t tids[numThreads];
    struct arg_st args[numThreads];

   
    count.occurence = malloc(sizeof(int)*numberOfBins);
     for(int i =0; i<numberOfBins;i++){
        count.occurence[i] = 0;
    }
    
    double binWidth = (upper-lower) / numberOfBins;
    //clock begins
    clock_t begin = clock();
    for (int i = 0 ; i < numThreads ; i++){
        
        
            args[i].lower = lower;
            args[i].upper = upper;
            args[i].binWidth = binWidth;
            args[i].numberOfBins = numberOfBins;
            sprintf(args[i].fileName,"%s",argv[5+i]);

        

        int current = pthread_create(&(tids[i]),
                                     NULL,
                                     &computeOccurence,&args[i]);

        if (current != 0) {
            printf("thread create failed \n");
            exit(1);
        }

    }
for (int i = 0 ; i < numThreads ; i++){

    void *returnedValue;
    pthread_join(tids[i], (void*) &returnedValue);
}
    FILE *resultFile = fopen(argv[5+numThreads], "w");

    for(int i=0; i<numberOfBins;i++)
    {
        
        fprintf(resultFile, "%d:%d\n",i,count.occurence[i]);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("%f\n",time_spent);
    free(count.occurence);
    }






