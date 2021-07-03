#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <pthread.h>
#include <vector>
#include <semaphore.h>
//#include <signal.h>
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
using namespace std;

void* ThreadReader(void* fdata);
void* ThreadWriter(void* fdata);


#define buffer_SIZE 512
#define MESSAGE_SIZE 100

char buffer[buffer_SIZE];


class Data;
void *handle;
struct Data *createInfoStruct();
void CreateSem(struct Data *data);
void CreateThreads(struct Data *data);
void Search(struct Data *data);
void CloseApp(struct Data *data);
