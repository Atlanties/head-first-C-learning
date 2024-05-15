# Chapter 10

## What's in a process?

- Procedure has code, stack, heap 
- File descriptor is a integer which represents a data stream
| # | data stream |
|:---:|:---:|
| 0 | keyboard |
| 1 | screen |
| 2 | screen |
| 3 | sth.else |
| ...| ...|
- The nature of redirect is to replace the file descriptor
```shell
# 2> means redirect stderr, &1 means redirect to stand output
./myprog > output.txt 2> error.log
```
- Everytime we open a new file, we add one row to the descriptor table
```c
FILE *my_file = fopen("guitar.mp3", "r");
// fileno() function returns the file descriptor
int descriptor = fileno(my_file);
// dup2(sour, dest) function will duplicate the former file to be the later file descriptor data stream
dup2(4, 3);
```
## newsbound2

### First version

- We implement a function called error, which would exit the process(kill all including main/parent)
- A child process would start when fork, in subprocess, it replace standard output with file descriptor
- However, results seem to be too small, as main process soon return(it goes parallelly with subprocess)

```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

void error(char* msg){

    fprintf(stderr, "%s: %s", msg, strerror(errno));
    exit(1);
}

int main(int argc, char* argv[]){

    char *phrase = argv[1];
    char *var[] = {"RSS_FEED=http://rss.cnn.com/rss/edition.rss", NULL};
    FILE *f = fopen("stories.txt", "w");

    if(!f){
        error("No such file!");
    }

    pid_t pid = fork();
    if(pid == -1){
        error("Can't fork procesd");
    }

    if(!pid){
        if(dup2(fileno(f), 1) == -1){
            error("Can't redirect Standard Output");
        }
        if(execle("/usr/bin/python", "/usr/bin/python", "./rssgossip.py", phrase, NULL, var) == -1){
            error("Can't run script!");
        }
    }
    return 0;

}
```

### second version


```c
// add before return
int pid_status;
if(waitpid(pid, &pid_status, 0)){
    error("Waiting error!");
}
```
- **waitpid(pid, pid_status, options)**: it request main process remains going until subprocess ends
- pid: subprocess id(returns to parent process when fork())
- pid_status: we use this one to store exit information for process, waitpid() need to change it so it is a pointer
```c
// using macro to get it
if(WEXITSTATUS(pid_status))
    puts("Error status non-zero");

```
- options: when 0, function would wait for the end of process
- file descriptor table have 255 file descriptors at most

## pipe

- We used pipe to connect something before
```shell
python regression.py -u | grep 'http'
```

### pipe()

- pipe opens a channel to connect two process
```c
// fd[0] is read end, fd[1] is write end
int fd[2];
if(pipe(fd) == -1){
    error("Can't open the pipe");
}
```
- in subprocess, we need to close the read end, replace standard output to read end
```c
close(fd[0]);
dup2(fd[0], 1);
```
- in parent process, we need to do just reverse
```c
dup2(fd[1], 0)
close(fd[1]);
```
- pipe is **not** file, but you can use mkfifo() to create pipe based on file.
- the read end would wait for sth. appears in pipe
- pipe can only read and write from one end

## kill process

- when use ctrl+c, programs end
- OS using **signals** to control it
- signals are small message, when it comes, process must stop everything, looks up signal map table and execute corresponded function

### sigaction

- signal action is a struct
```c
/*
struct sigaction {
    void     (*sa_handler)(int); // pointer towards signal handle function
    sigset_t sa_mask;            // the masked signals when executing handle function
    int      sa_flags;           // flags for adjust signal processing 
};
*/
// create new action
struct sigaction action;
// handler
action.sa_handler = diediedie;
// use mask to filter signals
sigemptyset(&action.sa_mask);
// additional signs
action.sa_flags = 0;
```
- the handler function received signals parameters 
```c
void diediedie(int sig){

    puts("END!");
    exit(1);
}
```
- When signals come, there always something bad happend, maybe stdout, so watch out
- use **sigaction()** to register
```c
// signal_no: siganal number
// new_action: address of new register sigcation
// old_action: store the sigaction which is replaced, set NULL if don't need it
// return: return -1 if error
sigaction(signal_no, &new_action, &old_action);

// example:
int catch_signal(int sig, void (*handler)(int)){

    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.flags = 0;
    reteurn sigaction(sig, action, NULL);
}
catch_action(SIGINT, diediedie);
```
- signals and reasons:
| signals | reasons |
|:---:|:---:|
|SIGINT| process interrupt|
|SIGQUIT|stop process and store registers to core file|
|SIGFPE|float error|
|SIGTRAP|debugger asking for progress of process|
|SIGSEGV|process intend to visit illegal address|
|SIGWINCH| terminal window size change|
|SIGTERM| process is reuqired to terminate|
|SIGPIPE| process is writing to pipe no one reads|

- Kill: kill is a command used to send signal to process
```shell
# send SIGINT
kill -INT xxxx
# send SIGSEGV
kill -SEGV xxxx
# forced to kill, SGIKILL can't be ignored
kill -KILL xxxx
```
- raise: raise is used to send signals to process itself, often used in signalaction handler funciton, called signal udpate
```c
raise(SIGTERM);
```

## alarm program

### SIGALRM

- signal alarm is used to generate signals it need to be handle later
- **Warning**: both sleep and alarm are using the same counter, do not use them at the same time
```c
catch_signal(SIGALRM, pour_coffee);
// normally we using alram to do something else
alarm(120);
// repeatly alarm would reset the time
do_something_busy();
do_more_busy();
catch_signal(SIGALRM, pour_coffee);
// change to default set
catch_signal(SIGALRM, SIG_DFT);
// ignore some signal
catch_signal(SIGINT, SIG_IGN);
```




