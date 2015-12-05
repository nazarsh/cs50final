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
#include <arpa/inet.h>

#include "bouncer50.h"

#define LOG_DIR "/var/log/"
#define AUTH_LOG "naz.log"
#define BOUNCER_LOG "bouncer.log"
#define HEALTH_BILL false
#define INVALID_USER_STR "Invalid user"

// log files for auth and bouncer
FILE *bouncer_logfp = NULL;
FILE *auth_logfp = NULL;

int main (int argc, char* argv[])
{
	// ensure user is root
	uid_t euid = geteuid();
	if (euid != 0)
	{
		alert("Insufficient priviledges. Please execute as root.");
		exit(1);
	}

	// help text that is printed when no -h flags are set
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
			{"stats",		no_argument,	0,	's'},
			{"help",		no_argument, 	0, 	'h'},
			{0,0,0,0}
		};

		// variable to store getopt_long option index
		int option_index = 0;
		// get options
		int opts = getopt_long (argc, argv, "adsh", long_options, &option_index);

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
				notify("launching defend mode. Please check logs.");
				defendMode();
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
}

/**
 * Daemonized defend mode
 */
void defendMode (void)
{
	// Location of the log file (*nix only)
	chdir(LOG_DIR);

	// error checking
	auth_logfp = fopen (AUTH_LOG, "r");
	if (auth_logfp == NULL)
	{
		alert("could not open your auth.log file.");
		exit(1);
	}
	// PROCESS ONLY NEW LOG ENTRIES. CHANCE OF LOCKOUT OTHERWISE!
	// Log could contain owner's ip from pre-configuration change state!
	fseek(auth_logfp, 0, SEEK_END);

	// // daemonize the process
	// pid_t process_id = 0;
	// pid_t sid = 0;
	//
	// // use fork() to create child process
	// process_id = fork();
	//
	// if (process_id < 0)
	// {
	// 	alert("fork failed!");
	// 	exit(1);
	// }
	// else if (process_id > 0)
	// {
	// 	printf("process_id of child process %d.\n", process_id);
	// 	exit(0);
	// }
	//
	// umask(0);
	//
	// sid = setsid();
	// if (sid < 0)
	// {
	// 	exit(1);
	// }
	//
	// close(STDIN_FILENO);
	// close(STDOUT_FILENO);
	// close(STDERR_FILENO);

	// file parsing based on getline
	char *auth_log_line = NULL;
	size_t len_auth_log = 0;
	ssize_t read_auth_log;
	fpos_t pos;

	// flag for checking if the end of auth.log was reached
	bool reached_end = false;

	// keep watching the auth.log file for changes. Sleep when EOF.
	while (1)
	{
		if (reached_end)
		{
			// reset the reached_end flag
			reached_end = false;
			// re-open the file at the last read position
			auth_logfp = fopen (AUTH_LOG, "r");
			fsetpos(auth_logfp, &pos);
		}
		// read a line from the log file
		read_auth_log = getline(&auth_log_line, &len_auth_log, auth_logfp);
		// remember a position of the line that was read
		fgetpos(auth_logfp, &pos);

		// if readling of the line is successful
		if(read_auth_log)
		{
			// do line processing to find violating IP addresses
			processLine(auth_log_line);
		}

		// reached the end of file
		if (read_auth_log == -1)
		{
			// let us sleep a second and try again
			reached_end = true;
			fclose(auth_logfp);
			sleep(1);
		}
		// tidy up the allocated memory
		auth_log_line = NULL;
	}
}

/**
 * Process a line from auth.log and extract an offending IP address
 */
void processLine(char* str)
{
	char* last_space_pos;
	char* new_line_pos;
	long int num_to_copy;
	char* match;
	char ip_addr[INET_ADDRSTRLEN];

	// check for 'Invalid user' string and do additional processing
	match = strstr(str, INVALID_USER_STR);
	// if string contain 'Invalid user', extract IP
	if (match != NULL)
	{
		// find last occurence of space
		last_space_pos = strrchr(str,' ');
		// find newline character
		new_line_pos = strrchr(str,'\n');
		// do math magic to feed to strncpy
		num_to_copy = (new_line_pos - str + 1) - (last_space_pos - str + 2);
		// set ip_addr extracted from the string
		strncpy(ip_addr, last_space_pos + 1, num_to_copy + 1);
		// make sure to terminate the string properly
		ip_addr[num_to_copy] = '\0';
		// convert and return IP
		struct sockaddr_in sa;
		// store this IP address in sa and use as validation of the ip:
		int s = inet_pton(AF_INET, ip_addr, &(sa.sin_addr));
		// only interested in valid IP addresses. Also a big security must!
		if (s > 0)
		{
			logMessage(ip_addr);
		}
	}
}

/**
 * Logs important messages to bouncer log
 */
void logMessage(char* message_to_log)
{
	// open and error check the log file
	bouncer_logfp = fopen (BOUNCER_LOG, "a");
	if (bouncer_logfp == NULL)
	{
		alert("could not open boucer50 log file.");
		exit(1);
	}
	fprintf(bouncer_logfp, "Blocked %s\n", message_to_log);
	fflush(bouncer_logfp);
	fclose(bouncer_logfp);
}
