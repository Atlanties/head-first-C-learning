#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

void error(char *msg)
{
    fprintf(stderr, "%s : %s", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
    char *advice[] = {
    "Take smaller bites\r\n",
    "Go for the tight jeans. No they do NOT make you look fat.\r\n", 
    "One word: inappropriate\r\n",
    "Just for today, be honest. Tell your boss what you *really* think\r\n", 
    "You might want to rethink that haircut\r\n"
    };


    int listener_d = socket(PF_INET, SOCK_STREAM, 0);
    if(listener_d == -1)
        error("Can't open socket!");
    
    int reuse = 1;
    if(setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
        error("Can't set socket reuse option!");
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t)htons(30000);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener_d, (struct sockaddr *) &name, sizeof(name))==-1)
        error("Can't bind port!");

    if(listen(listener_d, 10) == -1)
        error("Can't listen");
    puts("waiting for connection!");

    while(1){
        struct sockaddr_storage client_addr;
        unsigned int address_size = sizeof(client_addr);
        int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        if(connect_d == -1)
            error("Can't open client socket!");
        char *msg = advice[rand()%5];

        if(send(connect_d, msg, strlen(msg), 0) == -1)
            error("Can't send!");
        close(connect_d);
    }

    return 0;
}