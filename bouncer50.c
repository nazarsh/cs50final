#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main (int argc, char* argv[])
{
	FILE *logfp = NULL;
	pid_t process_id = 0;
	pid_t sid = 0;
	uid_t euid = geteuid();

	// ensure user is root
	if (euid != 0)
	{
		printf("Insufficient priviledges. Please execute as root.\n");
		exit(1);
	}
	else
		printf("CS50 Bouncer is starting\n");

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

	logfp = fopen ("bouncer.log", "w+");

	while (1)
	{
		sleep(5);
		fprintf(logfp, "logging ...\n");
		fflush(logfp);
	}

	fclose(logfp);
	return 0;
}
