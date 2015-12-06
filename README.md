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
* [x] Add flag processing (2)
* [x] Add "analyze" flag and functionality (epic)
  * [x] Add a bill of health config file (1)
  * [x] Generate default bill if file is not found (2)
  * [x] Add reading of bouncer config(1)
  * [x] Add reading of the sshd_config (1)
  * [x] Implement comparison of bill and config file (2)
  * [x] Print results of comparison (1)
  * [x] Add "defend" flag and functionality (epic)
    * [x] read auth.log file (1)
    * [x] figure out a way to "tail" the log file (3)
    * [x] string match and extract the offending IP addresses (3)
    * [x] produce firewall rules based on offending IPs (2)
    * [x] log activity for stats! (1)
* [x] Print a general setup guide (2)
* [x] Add health bill check into argv param handling
* [x] Only block subsequent ssh requests for now
* [x] Add a demo of "stats" flag and functionality (epic)
    * [x] Setup sqlite3 database. (1)

### Stretch Goals
* [ ] Add "stats" flag and functionality (epic)
    * [ ] Ingest, store, use `country <=> ip` map. (3)
    * [ ] Add download capability to get fresh `country <=> ip` map. (2)
* [ ] Add unit testing framework (2)
* [ ] Add support for IPv6

## References
1. [Creating a Daemon Process in C Language with an Example Program](http://www.thegeekstuff.com/2012/02/c-daemon-process/) by HIMANSHU ARORA on FEBRUARY 24, 2012
2. [Check if user is root in C?](http://stackoverflow.com/questions/4159910/check-if-user-is-root-in-c) (Stackoverflow)
3. [User identifier](https://en.wikipedia.org/wiki/User_identifier) (Wikipedia)
4. [Using C/C++, how do you print to stdout in color, but only if the terminal supports it?](http://stackoverflow.com/questions/12827143/using-c-c-how-do-you-print-to-stdout-in-color-but-only-if-the-terminal-suppo) (Stackoverflow)
5. [stdlib and colored output in C](http://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c) (Stackoverflow)
6. [The GNU C Library: Host Address Functions](http://www.gnu.org/software/libc/manual/html_node/Host-Address-Functions.html) (GNU C Library)
7. [An Introduction To The SQLite C/C++ Interface](https://sqlite.org/cintro.html)
8. [The GNU C Library: Getopt](http://www.gnu.org/software/libc/manual/html_node/Getopt.html) (GNU C Library)
9. [Download file using libcurl in C/C++](http://stackoverflow.com/questions/1636333/download-file-using-libcurl-in-c-c) (Stackoverflow)
10. [Is there a C project Default Directory Layout?](http://stackoverflow.com/questions/8523078/is-there-a-c-project-default-directory-layout) (Stackoverflow)
11. [How can I add an empty directory to a Git repository?](http://stackoverflow.com/questions/115983/how-can-i-add-an-empty-directory-to-a-git-repository) (Stackoverflow)
12. [SSH Essentials: Working with SSH Servers, Clients, and Keys](https://www.digitalocean.com/community/tutorials/ssh-essentials-working-with-ssh-servers-clients-and-keys) (DigitalOcean)
13. [What's the best way to check if a file exists in C? (cross platform)](http://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform) (Stackoverflow)
14. [How to get size of file in C](http://www.linuxquestions.org/questions/programming-9/how-to-get-size-of-file-in-c-183360/) (LinuxQuestions)
15. [getline(3) - Linux man page](http://linux.die.net/man/3/getline) (Die.net)
16. [fgetpos(), fsetpos()](http://beej.us/guide/bgc/output/html/multipage/fgetpos.html) (Beej's Guide to C)
17. [cstring (string.h)](http://www.cplusplus.com/reference/cstring/) (cplupluc.com)
18. [INET_PTON(3)](http://man7.org/linux/man-pages/man3/inet_pton.3.html) (Linux Programmer's Manual)
19. [Iptables Drop IP Address](http://www.cyberciti.biz/faq/linux-iptables-drop/) (nixCraft)
20. [Initial Server Setup with Ubuntu 14.04](https://www.digitalocean.com/community/tutorials/initial-server-setup-with-ubuntu-14-04) (DigitalOcean)
21. [how to import .sql into sqlite3](http://stackoverflow.com/questions/2049109/how-to-import-sql-into-sqlite3) (Stackoverflow)
22. [Using SQLite in C programs](http://www.wassen.net/sqlite-c.html)
