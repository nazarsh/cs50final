/**
 * stats.c
 *
 * Nazar Sharunenko
 * nazar.sh@gmail.com
 *
 *   connects to a local sqlite3 database
 *   provides a way to read and write data about the IP addresses being clocked
 *   implementation relies on: http://www.wassen.net/sqlite-c.html
 */
#include "bouncer50.h"

#define DB_LOCATION "res/ip2nation.db"

// callback function to get results
int callback(void *arg, int argc, char **argv, char **colName) {
    int i;
    for(i = 0; i < argc; i++){
        printf("Country: %s", argv[i] ?  : "NULL");
    }
    printf("\n");
    return 0;
}

void mockStats(void)
{
    // database connection
    sqlite3 *db;
    // statement
    int rc;

    // open local imported database
    rc = sqlite3_open(DB_LOCATION, &db);
    // handle errors if any
    if (rc != SQLITE_OK) {
        printf("ERROR opening SQLite DB in memory: %s\n", sqlite3_errmsg(db));
        goto out;
    }
    notify("successfully connected to database");

    // sample query to show functionality that could be further developped
    char *errmsg;

    rc = sqlite3_exec(db, \
        "SELECT ip2nationCountries.iso_country \
        FROM ip2nation INNER JOIN ip2nationCountries \
        ON ip2nation.country = ip2nationCountries.code \
        WHERE ip2nation.ip LIKE '%1845%'", \
        callback, NULL, &errmsg);

    if (errmsg != NULL) {
        printf("Error in sqlite3_exec: %s\n", errmsg);
        sqlite3_free(errmsg);
    }

    out:
    // close database
    sqlite3_close(db);
    notify("database closed.");
}
