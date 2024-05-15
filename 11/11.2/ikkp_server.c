#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

// read the data from socket
int read_in(int socket, char *buf, int len)
{
    char *s = buf;
    int slen = len;
    int c = 0;
    do{
        c = recv(socket, s, slen, 0);
        s += c;
        slen -= c;
    }while((c>0) && (s[c-1] =='\n' && s[c-2] == '\r'));
    // error
    if(c < 0)
        return c;
    // return an empty string if we get nothing
    else if(c == 0)
        buf[0] = '\0';
    else
        s[c-2] = '\0';
    return len - slen;
}

// error exit
void error(char *msg)
{
    fprintf(stderr, "%s : %s\n", msg, strerror(errno));
    exit(1);
}

// open an listener socket
int open_listener_socket()
{
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1)
        error("Can't open socket!");
    return s;
}

// bind socket to port 30000
void bind_to_socket(int socket)
{
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t)htons(30000);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse = 1;
    if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
        error("Can't set the reuse option on the socket!");
    int c = bind(socket, (struct sockaddr*)&name, sizeof(name));
    if(c == -1)
        error("Can't bind to socket!");
}

// send message to socket
int say(int socket, char *s)
{
    int result = send(socket, s, strlen(s), 0);
    if(result == -1)
        error("Can't send message!");
    return result;
}

int listener_d;
void handle_shutdown(int sig)
{
    if(listener_d)
        close(listener_d);
    fprintf(stderr, "Bye!\n");
    exit(0);
}

int catchsignal(int sig, void(*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;    
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    return sigaction(sig, &action, NULL);
}

int main(int argc, char *argv[])
{
    if(catchsignal(SIGINT, handle_shutdown)==-1)
        error("Can't catch signal!");

    char buf[255];
    int say_or_get = 1;
    int say_index = 0;
    int get_index = 0;

    char *say_msg[] = {
        "Internet Knock-Knock Protocol Server\nVersion 1.0\nKnock! Knock!\n>",
        "Oscar\n>",
        "Oscar Silly question, you get a silly answer\n"
    };
    char *get_msg[] = {
        "Who's there?",
        "Oscar who?"
    };
    char response[255] = "You should say: ";
    int server_socket = open_listener_socket();
    bind_to_socket(server_socket);
    if(listen(server_socket, 10))
        error("Can't listen!");

    puts("Waiting for connection!");
    while(1){
        struct sockaddr_storage client_addr;
        unsigned int size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)& client_addr, &size);
        do{
            if(say_or_get%2 == 0)
            {
                if(say(client_socket, say_msg[say_index++]) == -1)
                    break;                
            }else{
               
                if(read_in(client_socket, buf, sizeof(buf))>0){                        
                    // printf("Before if: buf = %s, get_msg[get_index] = %s, get_index = %d\n", buf, get_msg[get_index], get_index);
                    if(strncasecmp(get_msg[get_index], buf, strlen(get_msg[get_index])) != 0){
                        //printf("Inside if: buf = %s, get_msg[get_index - 1] = %s, get_index = %d\n", buf, get_msg[get_index - 1], get_index);
                        fprintf(stderr, "%s %s %i\n", get_msg[get_index], buf, strncasecmp(get_msg[get_index], buf, strlen(get_msg[get_index])));
                        say(client_socket, strcat(response, get_msg[get_index]));                    
                        break;
                    }
                    get_index++;
                    // fprintf(stderr, "%i %i", get_index, say_index);                    
                }                
            }            
            if(get_index == 2 && say_index == 3){
                break;
            }
            say_or_get++;
        }while(1);
        fprintf(stderr, "End client connection! say: %i read: %i\n", say_index, get_index);
        say_index = 0;
        get_index = 0;
        say_or_get = 0;
        close(client_socket);
    }
    return 0;
}

