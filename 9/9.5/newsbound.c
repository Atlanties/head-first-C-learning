#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

int main(int argc, char* argv[]){

    char *feeds[] = {"http://www.cnn.com/rss/celebs.xml",
                    "http://www.rollingstone.com/rock.xml",
                    "http://eonline.com/gossip.xml"};
    int times = 3;
    char * phase = argv[1];

    for(int i = 0; i < times; i++){
        char* var;
        fprintf(var, "RSS_FEED=%s", feeds[i]);
        char* vars[] = {var, NULL};
        pid_t pid = fork();
        if(pid == -1){
            fprintf(stderr, "Create subprocess failed!: %s\n", strerror(errno));
            return 1;
        }
        # pid == 0 is subprocess
        if(!pid){
            if(execle('/usr/bin/python', '/usr/bin/python', 'rssgossip.py', phase, NULL, vars) == -1){
                fprintf(stderr, "Can't run script:%s\n", strerror(errno));
                return 1;
            }
        }
    }
    return 0;
}