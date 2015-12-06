all:
	gcc -Wall -g src/bouncer50.c src/termcolors.c src/analyze.c src/stats.c src/sqlite3/sqlite3.c -o bin/bouncer50
