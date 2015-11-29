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
#include "bouncer50.h"

#define BOUNCER_CONFIG "/etc/bouncer50.conf"
#define BOUNCER_CONFIG_MIN_SIZE 10
#define SSHD_CONFIG "/etc/ssh/sshd_config"
FILE* bouncer_fp;
FILE* sshd_fp;

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
                alert("bouncer50 config file failed size check.");
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
    // check sshd_config and bouncer50.conf before proceeding
    int configCheckResult = checkConfigs();

    if (configCheckResult == 1)
    {
        notify("happy path...");

        char *bouncer_line = NULL;
        size_t len = 0;
        ssize_t read;

        // since we already validated both config files, no validations happen here
        while ((read = getline(&bouncer_line, &len, bouncer_fp)) != -1) {
            printf("Retrieved line of length %zu :\n", read);
            printf("%s", bouncer_line);
        }

        free(bouncer_line);

    }
    else
    {
        alert("there was an issue analyzing your configuration files.");
    }

    // close the file as it has been read into memory
    fclose(bouncer_fp);

}
