#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

int main(){
    # execl: list, we have path
    if(execl("/sbin/ifconfig", "/sbin/ifconfig", NULL) == -1)
        # execlp: using default environment PATH
        if(execlp("ipconfig", "ipconfig", NULL) == -1){
            # EPERM=1: no permission
            # ENOENT=2: no file or directory
            # ESRCH=3: no such procedure
            fprintf(stderr, "Cannot run ipconfig: %s", strerror(errno));
            return 1;
        }
    return 0;
}
