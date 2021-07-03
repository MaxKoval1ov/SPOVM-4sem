#include "header.h"

// typedef void (*ReadFromFile)(int, char *);
// typedef void (*WriteInFile)(int, char *);

#define key 1234

int size = 0;

sem_t semaphoreRead;
sem_t semaphoreWrite;

class Data
{
public:
    pthread_t pThread[2];
    vector<string> names;
    string buf;
};

void Search(Data &data)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("./text/");

    if (!dir)
    {
        perror("diropen");
        exit(1);
    };

    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, "Result.txt") || !strcmp(entry->d_name, "..") || !strcmp(entry->d_name, "."))
        {
            continue;
        }
        string temp_char = "./text/";
        temp_char += entry->d_name;
        data.names.push_back(temp_char);
    }

    cout << "Files found:" << "\n";
    for(int i = 0; i < data.names.size(); i++)
    {
        cout << data.names[i] << "\n";
    }
    closedir(dir);
}

void *ThreadReader(void *data)
{
    int sem_id, file_descriptor;
    char *(*ReadFromFile)(int, string &);

    sem_id = semget(key, 3, 0);

    *(void **)(&ReadFromFile) = dlsym(handle, "ReadFromFile");

    for (int i = 1; ((Data *)data)->names.size() != 0; i++)
    {
        if (((Data *)data)->names.size() == 0)
            break;
        while (semctl(sem_id, 0, GETVAL, 0) || semctl(sem_id, 1, GETVAL, 0));

        cout << "Read from file-" << i << "|";

        file_descriptor = open(((Data *)data)->names[0].c_str(), O_RDONLY);
        (*ReadFromFile)(file_descriptor, (((Data *)data)->buf));
        close(file_descriptor);
        ((Data *)data)->names.erase(((Data *)data)->names.begin());
        semctl(sem_id, 1, SETVAL, 1);
        semctl(sem_id, 0, SETVAL, 0);
    }
    semctl(sem_id, 2, SETVAL, 1);
    return 0;
}

void *ThreadWriter(void *data)
{
    int sem_id, file_descriptor;
    int (*writeToFile)(int, string &);

    sem_id = semget(key, 3, 0);

    *(void **)(&writeToFile) = dlsym(handle, "WriteInFile");

    remove("./text/Result.txt");
    file_descriptor = open("./text/Result.txt", O_WRONLY | O_CREAT, S_IRWXU);
    for (int i = 1; !semctl(sem_id, 2, GETVAL, 0); )
    {
          if (!semctl(sem_id, 0, GETVAL, 0) && semctl(sem_id, 1, GETVAL, 0)) {
         semctl(sem_id, 0, SETVAL, 1);
         cout << "Write from file-" << i << "\n";
          (*writeToFile)(file_descriptor, ((Data*)data)->buf);
          ((Data*)data)->buf.clear();
          i++;
         semctl(sem_id, 1, SETVAL, 0);
            semctl(sem_id, 0, SETVAL, 0);
          }
    }
    close(file_descriptor);
    return 0;
}

int main()
{
    Data data;
    Search(data);

    int semaphore_id = 0;

    semaphore_id = semget(key, 3, 0666 | IPC_CREAT);
    semctl(semaphore_id, 0, SETVAL, 0);
    semctl(semaphore_id, 1, SETVAL, 0);
    semctl(semaphore_id, 2, SETVAL, 0);


    handle = dlopen("./library.so", RTLD_LAZY);
    if (!handle)
    {
        cout << "Error";
        exit(1);
    }


    pthread_t readThread, writeThread;

    pthread_create(&readThread, NULL, ThreadReader, &data);
    pthread_create(&writeThread, NULL, ThreadWriter, &data);

    data.pThread[0] = readThread;
    data.pThread[1] = writeThread;

    pthread_join(data.pThread[0], NULL);
    pthread_join(data.pThread[1], NULL);

    pthread_cancel(data.pThread[0]);
    pthread_cancel(data.pThread[1]);

    dlclose(handle); 
}