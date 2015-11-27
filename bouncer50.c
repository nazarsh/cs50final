#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

/*
TODOs
[ ] Create a normal process (Parent process)
[ ] Create a child process from within the above parent process
[ ] The process hierarchy at this stage looks like :  TERMINAL -> PARENT PROCESS -> CHILD PROCESS
[ ] Terminate the the parent process.
[ ] The child process now becomes orphan and is taken over by the init process.
[ ] Call setsid() function to run the process in new session and have a new group.
[ ] After the above step we can say that now this process becomes a daemon process without having a controlling terminal.
[ ] Change the working directory of the daemon process to root and close stdin, stdout and stderr file descriptors.
[ ] Let the main logic of daemon process run.
*/

int main (int argc, char* argv[])
{
	printf("CS50 Bouncer is starting\n");

	FILE *fp = NULL;
	pid_t process_id = 0;
	pid_t sid = 0;

	// use fork() to create child process
	process_id = fork();

	if (process_id < 0)
	{
		printf("fork failed!\n");
		exit(1);
	}
	else if (process_id > 0)
	{
		printf("process_id of child process %d \n", process_id);
		exit(0);
	}

	umask(0);

	sid = setsid();
	if (sid < 0)
	{
		exit(1);
	}

	chdir("/~");

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	fp = fopen ("bouncer.log", "w+");

	while (1)
	{
		sleep(5);
		fprintf(fp, "logging ...\n");
		fflush(fp);
	}

	fclose(fp);
	return 0;
}
