/**
 * analyze.c
 *
 * Nazar Sharunenko
 * nazar.sh@gmail.com
 *
 * Program to:
 *   analyze sshd_config for best practices
 *   provide statistics about unauthorized ssh login attemps
 *   help block unauthorized ssh login attemps by writing firewall rules
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "bouncer50.h"

#define LOG_DIR "/var/log/"
#define AUTH_LOG "auth.log"
#define BOUNCER_LOG "bouncer.log"
#define HEALTH_BILL true

int main (int argc, char* argv[])
{
	// ensure user is root
	uid_t euid = geteuid();
	if (euid != 0)
	{
		alert("Insufficient priviledges. Please execute as root.");
		exit(1);
	}

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
				// print ssh guidelines
				printSshGuidelines();
				// run sshd_config analyzer
				analyzeConfig();
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

	pid_t process_id = 0;
	pid_t sid = 0;

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

	// Location of the log file (*nix only)
	chdir(LOG_DIR);

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	FILE *bouncer_logfp = NULL;
	FILE *auth_logfp = NULL;

	bouncer_logfp = fopen (BOUNCER_LOG, "w+");
	if (bouncer_logfp == NULL)
	{
		alert("could not open your %s file.", BOUNCER_LOG);
		exit(1);
	}

	auth_logfp = fopen (AUTH_LOG, "r+");
	if (auth_logfp == NULL)
	{
		alert("could not open your %s file.", AUTH_LOG);
		exit(1);
	}


	char *auth_log_line = NULL;
	size_t len_auth_log = 0;
	ssize_t read_auth_log;

	while (1)
	{
		read_auth_log = getline(&auth_log_line, &len_auth_log, auth_logfp)

		// reached the end of file, let us sleep a sec and try again
		if (read_auth_log == -1)
		{
			sleep(1);
		}
	}
		fprintf(bouncer_logfp, "logging ...\n");
		fflush(bouncer_logfp);
	}

	fclose(bouncer_logfp);
	return 0;
}
