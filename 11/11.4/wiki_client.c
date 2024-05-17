#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<stdlib.h>

void error(char *msg)
{
    fprintf(stderr, "%s: %s", msg, strerror(errno));
    exit(1);
}

// client version is much more complex than server
int open_socket(char* host, char* port)
{
    struct addrinfo *res;
    struct addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = PF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(host, port, &hint, &res) == -1)
        error("Can't get addrinfo!");

    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(s == -1)
        error("Can't open socket");
    int c = connect(s, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    if(c == -1)
        error("Can't connect to server");
    return s;
}

int say(int socket, char* msg)
{   
    int c = send(socket, msg, strlen(msg), 0);
    if(c == -1)
        error("Send message error!");
    return c;
}   

int main(int argc, char* argv[])
{
    int socket;
    socket = open_socket("en.wikipedia.org", "80");
    char buf[255];

    sprintf(buf, "GET /wiki/%s http/1.1\r\n", argv[1]);
    say(socket, buf);

    say(socket, "Host: en.wikipedia.org\r\n\r\n");
    char rec[256];
    int bytesRcvd = recv(socket, rec, 255, 0);
    while(bytesRcvd)
    {
        if(bytesRcvd == -1)
            error("Can't read from server");
        rec[bytesRcvd] = '\0';
        printf("%s", rec);
        bytesRcvd = recv(socket, rec, 255, 0);
    }

    close(socket);
    return 0;
}
