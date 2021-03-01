//Author: Caleb Riese
//Date: 2/19/2021

#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <math.h>
//void countNonBlankLines(FILE * inputFile, char *myError)
//fork, exec, wait, exit, shmget, shmctl, shmat, shmdt

int countNonBlankLines(FILE * inputFile) //counts lines in file that arent blank
{
    int lineCount = 0;
    char line[5];
    while (fgets(line, 5, inputFile))
    {
        if (line[0] != '\n') //if the line is not blank, then line count goes up
        {
            lineCount++;
        }
    }
    return lineCount;
}

void checkArgument(char * input, char * myError) //checks if parameter is a digit
{
    for (int i = 0; i < strlen(input); i++)
    {
        if (!isdigit(input[i]))
        {
            errno = EINVAL;
            perror(myError);
            exit(0);
        }
    }
}

int main(int argc, char * argv[])
{
    int opt;
    char myError[256] = "";
    strcat(myError, argv[0]);
    strcat(myError, ": Error");
    int maxTime = 100;
    int maxChildren = 20;

    if (argc == 1)
    {
        printf("No Arguments\n");
    }
    while ((opt = getopt (argc, argv, ":hs:t:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf("Usage: master [-h] [-s i] [-t time] datafile\n");
                return 0;
            case 's':
                checkArgument(optarg,myError);
                maxChildren = atoi(optarg);
                printf("children allowed: %d\n",maxChildren);
                break;
            case 't':
                checkArgument(optarg,myError);
                maxTime = atoi(optarg);
                printf("time allowed: %d\n",maxTime);
                break;
            case '?':
                printf("Unknown Option\n");
                errno = EINVAL;
                perror(myError);
                return 1;
        }
    }

    FILE * inputFile;
    char filename[128];
    strcpy(filename, argv[argc - 1]);
    inputFile = fopen(filename, "r");
    if (inputFile == NULL)
    {
        errno = ENOENT;
        perror(myError);
        return 1;
    }

    int lineCount = countNonBlankLines(inputFile);
    rewind(inputFile);
    int index = 0;
    int integerArray[lineCount];
    char holder[5] = {};
    while (fgets(holder, 5, inputFile)) //5 bytes 256\n\0 null and terminating
    {
        integerArray[index] = atoi(holder);
        index++;
    }
    fclose(inputFile);


    key_t key = ftok("shmfile",65);
    int shmid = shmget(key,512,0666|IPC_CREAT);
    int * sharedMemory = (int*) shmat(shmid, (void*)0, 0);
    for (int i = 0; i < lineCount; i++)
    {
        sharedMemory[i] = integerArray[i];
    }



    int depth = 1;
    while (lineCount > 1)
    {
        int numberOfProcesses = lineCount/2;
        for(int i = 0; i < numberOfProcesses; i++)
        {
            if (fork() == 0)
            {
                int sum = 0;
                int firstIndex = i * pow(2, depth);
                int secondIndex = firstIndex + pow(2,(depth - 1));
                sum += sharedMemory[firstIndex];
                sum += sharedMemory[secondIndex];
                sharedMemory[firstIndex] = sum;
                printf("depth:%d , i:%d , index:%d , Sum:%d\n", depth, i, firstIndex, sum);
                printf("depth:%d , i:%d , index:%d , Sum:%d\n", depth, i, secondIndex, sum);
                exit(0);
            }
        }
        int status;
        for (int i = 0; i < numberOfProcesses ; ++i) //Main Parent is waiting until this many
        {
            wait(&status);
        }
        lineCount = lineCount/2;
        depth++;
    }








    printf("End Sum: %d", sharedMemory[0]);
    shmdt(sharedMemory);
    sleep(2);
    shmctl(shmid,IPC_RMID,NULL); //should only be done by ONE process after all are done
    return 0;
}