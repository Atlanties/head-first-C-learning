#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>

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
    
    int pid_status;
    if(waitpid(pid, &pid_status, 0) == -1) {
        error("Error when waiting for subprocess!");
    }
    return 0;

}