# Final Project for CS50

Final CS50 project. Program that:
* reviews your ssh config file and provides guidelines
* monitors ssh activity (/var/log/auth.log)
* blacklists attempts to log in with username/password
* provides statistics on login attempts

## User Stories
* [x] Create a daemon process scaffold for the program (2)
* [x] Add `root` checks when program is executed (1)
* [x] Add color for output (1)
* [ ] Add flag processing (2)
* [ ] Add "config analyze" flag and functionality (3)
* [ ] Add "activity stats" flag and functionality (3)
* [ ] Add "active defense" flag and functionality (3)

### Stretch Goals
* [ ] Add unit testing framework (2)

## References
1. [Creating a Daemon Process in C Language with an Example Program](http://www.thegeekstuff.com/2012/02/c-daemon-process/) by HIMANSHU ARORA on FEBRUARY 24, 2012
2. [Check if user is root in C?](http://stackoverflow.com/questions/4159910/check-if-user-is-root-in-c) (Stackoverflow)
3. [User identifier](https://en.wikipedia.org/wiki/User_identifier) (Wikipedia)
4. [Using C/C++, how do you print to stdout in color, but only if the terminal supports it?](http://stackoverflow.com/questions/12827143/using-c-c-how-do-you-print-to-stdout-in-color-but-only-if-the-terminal-suppo) (Stackoverflow)
5. [stdlib and colored output in C](http://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c) (Stackoverflow)
