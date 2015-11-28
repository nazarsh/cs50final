#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "bouncer50.h"

int main (int argc, char* argv[])
{
	FILE *logfp = NULL;
	pid_t process_id = 0;
	pid_t sid = 0;
	uid_t euid = geteuid();

	// ensure user is root
	if (euid != 0)
	{
		alert("Insufficient priviledges. Please execute as root.");
		exit(1);
	}
	else
		notify("CS50 Bouncer is starting.");

	// use fork() to create child process
	process_id = fork();

	if (process_id < 0)
	{
		alert("fork failed!");
		exit(1);
	}
	else if (process_id > 0)
	{
		printf("process_id of child process %d.\n", process_id);
		exit(0);
	}

	umask(0);

	sid = setsid();
	if (sid < 0)
	{
		exit(1);
	}

	chdir("/var/log/");

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
