/**
 * analyze.c
 *
 * Nazar Sharunenko
 * nazar.sh@gmail.com
 *
 * Analyzes sshd_config and provides suggestions.
 * Generates and checks sshd_config against a .conf file.
 * Gives a global "pass" to bouncer50 to enter defend mode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "bouncer50.h"

#define BOUNCER_CONFIG "/etc/bouncer50.conf"
#define BOUNCER_CONFIG_MIN_SIZE 5
#define SSHD_CONFIG "/etc/ssh/sshd_config"
FILE* bouncer_fp;
FILE* ssh_fp;

/**
 * Checks bouncer50.conf and sshd_config before passing them to analyzeConfig
 */
int checkConfigs (void)
{
    // check to see if bouncer config file exists
    if(access(BOUNCER_CONFIG, F_OK) != -1 )
    {
        bouncer_fp = fopen(BOUNCER_CONFIG, "r+");
        if (bouncer_fp != NULL)
        {
            // ensure file is not empty
            fseek(bouncer_fp, 0, SEEK_END);
            int fileLen = ftell(bouncer_fp);
            fseek(bouncer_fp, 0, SEEK_SET);

            // assuming minimal configuration available, so check for size
            if (fileLen < BOUNCER_CONFIG_MIN_SIZE)
            {
                printf("bouncer50 config file failed size check.");
                exit(1);
            }
        }
        else
        {
            alert("unable to open a config file");
        }

    }
    else
    {
        // generate config file if it does not exist for some reason
        generateConfig();
    }

    // ensure sshd_config is present. No need to verify size at this point.
    if(access(SSHD_CONFIG, F_OK) != -1 )
        return 1;
    else
    {
        alert("sshd_config file is not available.");
        return 0;
    }
}

/**
 * Generates a config file if none is found
 */
void generateConfig (void)
{
    warn("no config file available.");
    bouncer_fp = fopen(BOUNCER_CONFIG, "w+");

    if (bouncer_fp != NULL)
    {
        // write a minimal configuration file
        fprintf(bouncer_fp, "PermitRootLogin no\n");
        fprintf(bouncer_fp, "PasswordAuthentication no\n");

        notify("config file successfully generated.");
        // after writing to file, set it to beginning and re-run analyzeConfig()
        fseek(bouncer_fp, 0, SEEK_SET);
        analyzeConfig();
    }
    else
    {
        alert("unable to create a config file");
        exit(1);
    }
}

/**
 * Generates a config file if none is found
 */
void analyzeConfig (void)
{
    // track configuration settings compliance
    bool healthBill = true;

    // check sshd_config and bouncer50.conf before proceeding
    int configCheckResult = checkConfigs();

    if (configCheckResult == 1)
    {
        char *bouncer_line = NULL;
        char *ssh_line = NULL;
        size_t len_bouncer = 0;
        size_t len_ssh = 0;
        ssize_t read_bouncer;
        ssize_t read_ssh;

        // open sshd_config
        ssh_fp = fopen(SSHD_CONFIG, "r+");
        if (ssh_fp == NULL)
        {
            alert("could not open your sshd_config.");
            exit(1);
        }
        // keep track of config comparison matches
        bool match;
        // since we already validated both config files, no validations happen here
        while ((read_bouncer = getline(&bouncer_line, &len_bouncer, bouncer_fp)) != -1)
        {
            // use strtok to print the "token" without \n
            printf("Verifying \"%s\" ", strtok(bouncer_line, "\n"));

            while ((read_ssh = getline(&ssh_line, &len_ssh, ssh_fp)) != -1)
            {
                // loop through and do string matching
                strtok(ssh_line, "\n");
                int matches = strcmp(bouncer_line, ssh_line);

                // keep match at false unless a match
                if (matches != 0)
                    match = false;
                // if match, print OK, set match to true and stop
                else
                {
                    match = true;
                    break;
                }
            }
            // check a summary vs every line for != true; also adjust healthBill
            if (match != true)
            {
                warn("NO");
                healthBill = false;
            }
            else
                notify("OK");
            // "rewind" the sshd_config after every line
            fseek(ssh_fp, 0, SEEK_SET);
        }
        // cleanup
        free(bouncer_line);
        free(ssh_line);

        if (healthBill)
            notify("system is ready for --defend mode");
        else
        {
            alert("please correct your sshd_config before proceeding.");
            exit(1);
        }
    }
    else
    {
        alert("there was an issue analyzing your configuration files.");
    }
    // close the file as it has been read into memory
    fclose(bouncer_fp);
}

/**
 * Print ssh setup guidelines when called
 */
void printSshGuidelines (void)
{
    notify("guidelines to come here");
}
