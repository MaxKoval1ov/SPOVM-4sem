#include <iostream>
#include <csignal>
#include <unistd.h>
#include <vector>
#include <ncurses.h>
#include <sys/wait.h>
#include <sys/types.h>
#include<string.h>

using namespace std;

int main(int argc, const char * argv[]) {

   sigset_t   set; 
   sigemptyset(&set);                                                             
   sigaddset(&set, SIGUSR1); 
   int i = 0;
   initscr();
   vector<int> pids;
   char choose;
   while (choose != 'q')
   {
      cout<<"+ or - or q"<<endl;
      choose = getch();
      switch (choose)
      {
        case '+':
              {
               int pid=fork();
               switch(pid)
               {
                  case -1:{
                   cout<<"Error with creating";
                   exit(-1);
                  }
                  break;
                  case 0:{
                     execv("./2nd-child",NULL);
                  } 
                  break;
                  default:{
                    pids.push_back(pid);
                  }
                  break;
               }
              }
              break;
         case '-':
              {
                 if(pids.size()!=0)
                 {
                    kill(pids[pids.size()-1],SIGKILL);
                    pids.pop_back();
                 }else{
                    cout<<"Oops the are no created processes,yet!"<< endl;
                 }
              }
              break;
              case 'q':
              {
                 for(int i=0;i<pids.size();i++)
                 {
                    kill(pids[i],SIGKILL);
                 }
                 pids.clear();
              }break;
     }
     kill(pids[pids.size()-1],
   }
          getch();
       return 0;
 }