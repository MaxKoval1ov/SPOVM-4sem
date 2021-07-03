#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<ncurses.h>
#include <sys/wait.h>
#include <list>
int global = 1;

void
update(int i, int n)
{
	int before = global;
	global = n;
	printf("[%d] %d -> %d\n", i, before, global);
}

int main()
{
    pid_t pid=fork();
    int status;
	update(1, 10);		// [1] 1 -> 10
	switch (pid) {
	case -1:
		exit(-1);
	case 0:			// child
		update(2, 20);	// [2] 10 -> 20
		exit(0);
	default:		// parent
		update(3, 30);	// [3] 10 -> 30
		waitpid(pid,&status,0);
	}
	update(4, 40);		// [4] 30 -> 40
	return 0;
}