all:
	gcc -Wall -g src/bouncer50.c src/termcolors.c src/analyze.c src/stats.c -lsqlite3 -o bin/bouncer50
