#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
  int reciever = 0;
  sigset_t set;                       
  sigemptyset(&set);                  
  sigaddset(&set, SIGUSR1);  
  sigprocmask(SIG_BLOCK, &set, NULL);         
  int pid = getpid();                
  char str[] = "Process pid:";
  while (true)
  {
    sigwait(&set, &reciever); 
    cout << endl;
    for (int i = 0; i < strlen(str); i++)
    {
      usleep(20000);
      printf("%c", str[i]);
    }
    cout << getpid() << "\n\r";
    usleep(10000);
    cout<<"\n\r";
    kill(getppid(), SIGUSR2); 
    sleep(1);
  }
  return 0;
}