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

#define CONFIG_FILE_NAME "/etc/bouncer50.conf"
#define CONFIG_MIN_SIZE 10
#define SSHD_CONFIG "/etc/ssh/sshd_config"
FILE* config_fp;

/**
 * Checks bouncer50.conf and sshd_config before passing them to analyzeConfig
 */
int checkConfigs (void)
{
  // check to see if bouncer config file exists
  if(access(CONFIG_FILE_NAME, F_OK) != -1 ) {
    config_fp = fopen(CONFIG_FILE_NAME, "r+");

    if (config_fp != NULL)
    {
      // ensure file is not empty
      fseek(config_fp, 0, SEEK_END);
      int fileLen = ftell(config_fp);
      // assuming minimal configuration available, so check for size
      if (fileLen < CONFIG_MIN_SIZE)
      {
        alert("bouncer50 config file failed size check.");
        exit(1);
      }
      else
        return 1;
    }
    else
    {
      alert("unable to open a config file");
    }
    // close the file as it has been read into memory
    fclose(config_fp);
  }
  else
  {
    // generate config file if it does not exist for some reason
    generateConfig();
  }

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
  config_fp = fopen(CONFIG_FILE_NAME, "w+");

  if (config_fp != NULL)
  {
    // write a minimal configuration file
    fprintf(config_fp, "PermitRootLogin no\n");
    fprintf(config_fp, "PasswordAuthentication no\n");
    fclose(config_fp);

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
    notify("happy path");
  }
  else
  {
    alert("there was an issue analyzing your configuration files.");
  }
}
