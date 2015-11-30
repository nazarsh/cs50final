#include <stdio.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"

/*
 * Notifies user using green color
 */
void notify(char* message)
{
    printf(GREEN);
    printf("%s", message);
    printf(RESET "\n");
}

/*
 * Warns user using yellow color
 */
void warn(char* message)
{
    printf(YELLOW);
    printf("%s", message);
    printf(RESET "\n");
}

/*
 * Alerts user using red color
 */
void alert(char* message)
{
    printf(RED);
    printf("%s", message);
    printf(RESET "\n");
}
