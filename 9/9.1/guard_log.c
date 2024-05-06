#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

char* now(){
    time_t t;
    time(&t);
    char* time_str = asctime(localtime(&t));
    time_str[strlen(time_str) - 1] = '\0';  // 去掉换行符
    return time_str;      
}

int main(){

    char comment[80];
    char cmd[120];

    fgets(comment, 80, stdin);
    comment[strlen(comment) - 1] = '\0';  // 去掉换行符
    sprintf(cmd, "echo '%s %s' >> reports.log", comment, now());
    system(cmd);

    return 0;
}