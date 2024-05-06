#include<stdio.h>
#include<unistd.h>

int main(){

    # format of environment is variable=value
    char *my_env[] =  {"JUICE=peach and apple", NULL};
    # program_name, argv[], argv[0] = program_name
    # Once start doing exec, everything later will cease
    execle("diner_info", "diner_info", "4", NULL, my_env);
    puts("Sth. error happened to diner info!!!\n");
    return 0;
}