//
//  sym_phistogram.c
//  sym_phistogram
//
//  Created by Huseyin Taskesen, Asaf Kağan Bezgin on 9.11.2018.
//  Copyright © 2018 Huseyin Taskesen, Asaf Kağan Bezgin. All rights reserved.
//


#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>

int num=0;
int status=0;
sem_t* written;
sem_t* parent_read;
int fd;

//open semaphores



int main (int argc, const char * argv[]) {

    if (argc < 6)
    {
        printf("you need to give at least 6 arguments\n");
        exit(1);
    }
    
   written = sem_open("written",O_CREAT | O_EXCL, 0660, 0);
   parent_read = sem_open("parent_read",O_CREAT | O_EXCL, 0660, 1);


    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    int numberOfBins = atoi(argv[3]);
    int numProcess = atoi(argv[4]);
    int binWidth = (upper-lower) / numberOfBins;
	int occurence[numberOfBins];

    for(int i =0; i<numberOfBins;i++){
        occurence[i] = 0;
    }
	

    int parentResult[numberOfBins];
    void* shm_start;

    for(int i =0; i<numberOfBins;i++){
        parentResult[i] = 0;
    }


    
    //clean up a shm with same name
    shm_unlink("myMemory");
    
    fd = shm_open("myMemory", O_CREAT | O_RDWR | O_EXCL,0660);
    if (fd < 0)
    /* Handle error */
        perror("error");
    
    
    if (ftruncate(fd, sizeof(occurence)) == -1)
    /* Handle error */
        perror("error");
    
    shm_start =  mmap(NULL, sizeof(occurence), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
    
    if (shm_start < 0) {
        exit (1);
        perror("can not map shm\n");
    }
    
    pid_t pid, wpid;
    //clock begins
    clock_t begin = clock();
    
    for (int i = 0 ; i < numProcess; i++){
        pid = fork();
        if(pid==0)
        {
            //open file
            FILE *file = fopen(argv[5+i], "r");
            if(file == 0) {
                perror("fopen");
                exit(1);
            }
            sem_wait(parent_read);
            //finding the interval for the current child
            for (int counter = 0 ; counter < numberOfBins ; counter++)
            {
            //open file
                FILE *file = fopen(argv[5+i], "r");
                if(file == 0) {
                    perror("fopen");
                    exit(1);
                }
                
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
                fclose(file);
            }
            //below this line, every child write to SHM
      

            memcpy(shm_start, occurence, sizeof(occurence));
            sem_post(written);
            exit(0);
            
        }


            sem_unlink("written");
            sem_unlink("parent_read");
            
            
        //end of child process
        //parent reads from SHM
        sem_wait(written);
        memcpy(occurence, shm_start, sizeof(occurence));
        for(int i =0;i<numberOfBins;i++){
            parentResult[i] = parentResult[i] + occurence[i];
        }
        sem_post(parent_read);

}
    FILE *outFile = fopen(argv[5+numProcess], "w");
    if(outFile == 0) {
        perror("fopen");
        exit(1);
    }

    for(int p=0; p < numberOfBins; p++){
        fprintf(outFile,"%d:%d\n",p,parentResult[p]);
    }
    
    
    shm_unlink("myMemory");
    munmap(shm_start, sizeof(occurence));
    
    sem_close(written);
    sem_close(parent_read);

    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("%f\n",time_spent);


    
    return 0;
}


