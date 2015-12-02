/**
 * defend.c
 *
 * Nazar Sharunenko
 * nazar.sh@gmail.com
 *
 * Functions enabling the daemonized process to:
 *     read auth.log
 *     blacklist offending IPs
 *     output own logging for blacklisted IPs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "bouncer50.h"

#define AUTH_LOG "/var/log/auth.log"
