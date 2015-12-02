/**
 * Notifies user using green color
 */
void notify (char* message);

/**
 * Warns user using yellow color
 */
void warn (char* message);

/**
 * Alerts user using red color
 */
void alert (char* message);

/**
 * Checks bouncer50.conf and sshd_config before passing them to analyzeConfig
 */
int checkConfigs (void);

/**
 * Generates minimal bouncer50.conf
 */
void generateConfig (void);

/**
 * Compares sshd_config with bouncer50.conf
 * to ensure minimal acceptable configuration
 */
void analyzeConfig (void);

/**
 * Print ssh setup guidelines when called
 */
void printSshGuidelines (void);

/**
 * Daemonized Defend mode
 */
void defendMode (void);
