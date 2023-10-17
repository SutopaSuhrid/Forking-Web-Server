[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-f4981d0f882b2a3f0472912d15f9806d57e124e0fc890972558857b51b24a6f9.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=9547315)
# A3: Forking Webserver

Learning Outcomes
-----------------

1. **Write** a Makefile to compile a C program
2. **Use** sockets to respond to web browser requests (GET and POST)
3. **Use** `fork()` system call to create a process for each request
4. **Create** a simple forking webserver

Specifications
--------------

Write a forking webserver. You want to call these functions in this order:

1. `socket()`
2. `bind()`
3. `listen()`
4. `accept()`
5. `fork()`

Then you have a file descriptor in a child process that you can read the HTTP request from and write a response to. The server needs to handle GET requests and support POSTing a file to the relative path given in the request. This is about as secure as an Apache server.

I've included a `.gdbinit` file that will make GDB follow the child process when it is created, otherwise it defaults to staying with the parent process (and thus you may not reach breakpoints you've set inside the code for the child process). You will get a warning from GDB when you first run it in this directory, to get rid of the warning I added the following line the file `~/.gdb.init`:

	set auto-load safe-path /

Even though the `index.html` displays a `music.mp3` file, it is not included in the repository to reduce size. Put your own file in for testing, but don't commit it to the repository. If you commit any files used to test POSTing a file, you will receive a zero for the assignment.

### Restrictions

*   You must compile your program using the flags `std=gnu11`, `-Werror` and `-Wall`.
*	You must create a child process for each request using `fork()`

### Submission

Submit only your source code and associated `Makefile`. When `make` is executed in the directory, a binary named `server` should be created. There is currently no autograder for this assignment.

### Functions

Here is a list of functions that are used by the solution program (probably). This might help in completing the assignment:

*	`abort()`
*	`accept()`
*	`atoi()`
*	`bind()`
*	`close()`
*	`exit()`
*	`fclose()`
*	`fdopen()`
*	`fopen()`
*	`fork()`
*	`fprintf()`
*	`fread()`
*	`free()`
*	`fseek()`
*	`ftello()`
*	`fwrite()`
*	`getline()`
*	`htonl()`
*	`htons()`
*	`listen()`
*	`main()`
*	`malloc()`
*	`printf()`
*	`puts()`
*	`rewind()`
*	`setsockopt()`
*	`socket()`
*	`strdup()`
*	`strsep()`
*	`strstr()`

### Example

You can run this for the GET functionality, but it will not handle the POST request to upload a file:

    $ python3 -m http.server

