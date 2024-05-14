#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

int read_in(int socket, char *buf, int len)
{
    char *s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    while((c>0) && (s[c-1]!='\n')){
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
    }
    // error
    if(c < 0)
        return c;
    // return an empty string if we get nothing
    else if(c == 0)
        buf[0] = '\0';
    else
        s[c-1] = '\0';
    return len - slen;
}

void error(char *msg)
{
    fprintf(stderr, "%s : %s\n", msg, strerror(errno));
    exit(1);
}

int open_listener_socket()
{
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1)
        error("Can't open socket!");
    return s;
}

void bind_to_socket(int socket)
{
    
}