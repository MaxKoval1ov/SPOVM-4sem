#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#define SIZE 20
#define NUM 1234

int main(int argc, char *argv[])
{

    char *shm;
    key_t key;
    int pid;
    key = shmget(NUM, SIZE, 0666 | IPC_CREAT);
   if ((shm = (char *)shmat(key, (void *)0, 0)) == (char *)(-1))
    { 
        printf("Can't attach shared memory\n");
        exit(-1); 
    }
    char *s = shm; 
    while (true)
    {
        fflush(stdin);
        string str;
        printf("Enter string [PID: %d]: ", getpid());
        getline(cin, str);      
        strcpy(s, str.c_str()); 
        pid = fork();           
        switch (pid)
        {
         case 0:
        {
            key = shmget(NUM, SIZE, 0);   
            shm = (char *)shmat(key, (void *)0, 0);     
            printf("Child wrote [PID: %d][PPID: %d]: %s\n", getpid(), getppid(), shm); 
            shmdt(shm);  
            kill(pid, SIGTERM); 
            exit(0);           
            break;
        }
        case -1:
        { 
            exit(0);
        }
        default:
        {
            wait(NULL); 
            break;
        }
        }
    }
    return 0;
}
