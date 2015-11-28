#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>

#include "bouncer50.h"

int main (int argc, char* argv[])
{

	int opts;
	char* help_text = "Example usage:\n\n \
	bouncer50 --help    | -h\t print this help message\n \
	bouncer50 --analyze | -a\t analyze ssh configuration\n \
	bouncer50 --stats   | -s\t print remote login attemps statistics\n \
	bouncer50 --defend  | -d\t start blocking malicious remote ssh attempts\n";

	// handle no argument launch of the program
	if (argc < 2)
		printf("%s",help_text);

	// options processing loop
	while(1)
	{
		static struct option long_options[] =
		{
			{"analyze",		no_argument,	0,	'a'},
			{"defend",		no_argument,	0,	'd'},
			{"stats",			no_argument,	0,	's'},
			{"help",			no_argument, 	0, 	'h'},
			{0,0,0,0}
		};

		// variable to store getopt_long option index
		int option_index = 0;

		opts = getopt_long (argc, argv, "adsh", long_options, &option_index);

		// check for end of the options
		if (opts == -1)
			break;

		switch (opts)
		{
			case 'a':
				puts("option -a\n");
				break;
			case 'd':
				puts("option -d\n");
				break;
			case 's':
				puts("option -s\n");
				break;
			case 'h':
				printf("%s",help_text);
				break;
			case '?':
				break;
			default:
				abort();
		}
	}

	exit (0);

// ===========

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
