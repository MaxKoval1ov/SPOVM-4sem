#include <iostream>
#include <stdio.h>
#include <csignal>
#include <unistd.h>
#include <vector>
#include <ncurses.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

using namespace std;

int main(int argc, const char *argv[])
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR2);
    int reciever = 0;
    int pid = getpid();
    char str[] = "This is process - ";
    while (true)
    {
        waitp(getppit(),set);
        for (int i = 0; i < strlen(str); i++)
        {
            usleep(20000);
            printf("%c", str[i]);
        }
        cout << getpid() << "\n\r";
        usleep(10000);
        kill(getppid(),SIGUSR2);
    }
}