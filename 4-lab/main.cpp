#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include<vector>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

using namespace std;


pthread_mutex_t mInfo;


void *output(void *arg)
{
   for(;;){
        pthread_mutex_lock((pthread_mutex_t *)arg);  
        printf("Num: %lu\n", pthread_self());     
        pthread_mutex_unlock((pthread_mutex_t *)arg);
        usleep(1500000);                             
   }
}

void deleteThread(pthread_t info)
{
    pthread_cancel(info); 
}

pthread_t createThread(pthread_mutex_t *mInfo)
{
    pthread_t info;
    pthread_create(&info, NULL, output, (void*)mInfo);
    return info;
}

void deleteAll(vector< pthread_t> &threads)
{
    while (threads.size())
    {                                        
        pthread_cancel(threads[threads.size()-1]); 
        threads.pop_back();                        
    }
}

int main()
{
    vector<pthread_t> threads; 

  
    pthread_mutex_init(&mInfo,NULL);       

    printf("Enter symbol ('+', '-' or 'q'):\n");
    while (true)
    {
        switch (cin.get())
        {
        case '+':
        {
            printf("New thread\n");
            threads.push_back(createThread(&mInfo));
            break;
        }
        case '-':
        {
            if (threads.size())
            {
                printf("Delete thread\n");
                deleteThread(threads[threads.size()-1]);
                threads.pop_back();
            }
            else{
                printf("The are no any threads\n");
            }
            break;
        }
        case 'q':
        { 
            deleteAll(threads);
            return 0;
        }
        break;
    }
}
}

