#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<stdio.h>

int main(int argc, char* argv[]){

    char* my_env[] = {"FOOD=Coffee", NULL};
    if(execle("./coffee", "./coffee", "donuts", NULL, my_env) == -1){
        fprintf(stderr, "Can't create order: %s!\n", strerror(errno));
        return -1;
    }
    return 0;
}