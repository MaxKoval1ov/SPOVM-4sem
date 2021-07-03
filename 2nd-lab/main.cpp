#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <ncurses.h>
#include <curses.h>

using namespace std;

int main(int argc, char *argv[])
{
    int reciever = 0;
    initscr();
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &set, NULL);
    vector<int> pids;
    char answer;

    
    printw("Enter '+', '-' or 'q' \n");
    while (answer != 'q')
    {
        answer = wgetch(stdscr);
        switch (answer)
        {
        case '+':
        {
            int pid = fork();
            switch (pid)
            {
            case -1:
            {
                cout << "Error!" << endl;
                exit(-1);
            }
            case 0:
            {
                execv("./child", argv);

                exit(0);
            }
            default:
            {
                pids.push_back(pid);
                sleep(1);
            }
            break;
            }
        }
        break;
        case '-':
        {
            if (pids.size() != 0)
            {
                kill(pids[pids.size() - 1], SIGKILL);
                pids.pop_back();
            }
            else
            {
                cout << "The are no any processes,yet" << endl;
            }
        }
        break;
        case 'q':
        {
            for (int i = 0; i < pids.size(); i++)
            {
                kill(pids[i], SIGKILL);
            }
            pids.clear();
            endwin();
        }
        break;
        }
        char str[] = "Number of the processes:";
        for (int i = 0; i < strlen(str); i++)
        {
            usleep(20000);
            printf("%c", str[i]);
        }
        cout << pids.size() << "\n\r";
        cout<< "______________________\n\r"<<"\n\r";
        usleep(10000);
        for (int i = 0; i < pids.size(); i++)
        {
            kill(pids[i], SIGUSR1);
            sigwait(&set, &reciever);
        }
        sleep(1);
    }
    return 0;
}