# Chapter 11 network and socket

## knock-knock server

- Basically, most networkd communication programs consists of two parts, server and client
- **Protocal**: server and client would start a structural conversation, lower protocal contains IP, higher contains HTTP
- Our knock-knock server uses DIY protocal

## BLAB

- C program uses data stream to communiction to outside, up to now we have file, stdin and stdout, socket is data stream as well
- BLAB: Bind, Listen, Accept, Begin

```c
#include<sys/socket.h>
// socket(protocal family, socket type, protocal)
// protocal family: PF_INET(IPv4 address family), PF_UNIX(UNIX domain socket), PF_INET6(IPV6 address family)
// socket type: "SOCKET_STREAM" means data arrived at right sequence never lost or repeat, often TCP
// "SOCKET_DGRAM" often UDP, unrealiable
// protocal: default 0 means let OS choose suitable protocal
// Here we get an IPV4 TCP socket
int listener_d = socket(PF_INET, SOCK_STREAM, 0);
// listener_d is socket descriptor
if(listener_d == -1)
    error("Can't open socket");
```

### Bind

- Computer may have many server applications, in order to prevent mess, different ones will have different port
- We need to tell applications which port it will use
- To Bind port, we need socket descriptor and socket name

 ```c
 #include<arpa/inet.h>
 /* 
 about struct sockaddr_in
 sockaddr_in is used to represent socket address of IPV4 address family 
 struct sockaddr_in {
    short            sin_family;   // Protocal family
    unsigned short   sin_port;     // Port number, should be big end number in network, (use htons() to change it, 16bit)
    struct in_addr   sin_addr;     // IPv4 address (big end number in network, 32bit)
    char             sin_zero[8];  // reserved  (filled with 0)
};
 */
// construct a socket named "internet3000port" socket
 struct sockaddr_in name;
 name.sin_family = PF_INET;
 name.sin_prot = (in_port_t)htons(30000);
 // INADDR_ANY is a special Macro represent that socket can be bind to any available network interface
 // htonl() change it to network big end data
 name.sin_addr.s_addr = htonl(INADDR_ANY);
// same as : name.sin_addr.s_addr = inet_addr("127.0.0.1");
 /* sockaddr is a general struct to save socket address
 struct sockaddr {
    sa_family_t sa_family;   // protocal family
    char        sa_data[14]; // address data
};
*/
 int c = bind(listener_d, (struct sockaddr *) &name, sizeof(name));
 if(c == -1)
    error("Can't bind port!");
 ```

 ### Listen

 - listen will let OS knows how long you want your waiting list
 ```c
 // lenth is 10
 if(listen(listen_d, 10) == -1)
    error("Can't listen!");
 ```

 ### Accept

 - Systemcall accept() will always wait for clients to connect
 - It will return second socket descriptor

```c
/*
struct sockaddr_storage {
    sa_family_t ss_family;   // 地址族
    char        __ss_padding[128 - sizeof(sa_family_t)]; // reserved elements, used to fill struct
};
*/
struct sockaddr_storage client_addr;
unsigned int address_size = sizeof(client_addr);
/* accept function need 3 parameters:
listen socket: listener_d
pointer to store client address information
pointer to address size: ensure accept to deal with different size of socket, and change it when come back
*/
int connect_d = accept(listener_d, (struct sockaddr*)&client_addr, &address_size);
if(connect_d == -1)
    error("Can't open socket");
```

### socket is not traditional data stream

- Socket is a bidirectional communication
- we use send() function to communicate

```c
char *msg = "Internet Knock-Knock Protocol Server\r\nVersion 1.0\r\nKnock! Knock!\r\n> ";
if (send(connect_d, msg, strlen(msg), 0) == -1)
    error("send");
// server application is suseptible to errors, server must deal with it
```
- it would be better to use port larger than 1024, those are smaller often allow admin to use only

### Delay in binding port

- When any port is bind to socket, OS won't allow any program bind it again in **30s**.
- By setting reuse , we can let it possible to bind in a short interval
```c
/*
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
sockfd: socket descriptor
level: the level of protocal, "SOL_SOCKET"(general socket option), "IPPROTO_TCP", "IPPROTO_IP"
optname: specific option
optal: value point to option
optlen: size of option value(byte)
*/
int reuse = 1
if(setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
    error("Can't set reuse of socket!");
```

## Read data from client

- We will use **recv()** to read data from client
```c
/*
clent_d: client socket descriptor
buf: buffer to store the receive message
n: bytes we want to read
0: flags
return: numbers of bytes which is read
*/
int n = recv(client_d, buf, n, 0);
```
- **warning**:
1. string won't end with '\0'
2. using telnet input text, string will end up with '\r\n'
3. recv() will return numberes of bytes we read, error for -1, if client is end, return 0
4. recv() may need to read several times for full information

## Serves for several clients

- Though we implement the ikkp server, it can only serve one client per time
- Other client should wait for the accept() function of server to serve it
- use multi-process to deal with it
```c
// the parent process only accept the socket
// but the child processs would deal with it
if(!fork()){
    close(listener_d);
    // do something
    exit(0);
}
close(client_socket);
```
- remember to avoid unlimited processes, so it is necessary to limit the upper boundary of process number


## How to write a client end

- When a client need the following things:
1. connect to server
2. GET or POST command
3. We need the host name
4. empty line

- remind that the server need BLAB four steps, the client acctually need two: connect and communication

### remote port and IP

- Client need to know **Both IP and Port** to communicate with server, IP can be replaced by DNS
- first, client need to create the socket for remote server
```c
int s = socket(PF_INET, SOCK_STREAM, 0);
// client need to bind the socket to remote port not local port
// this is like accept() bind client_socket to remote client address information
struct sockaddr_in si;
// create a socket for 202.201.239.100 port 80
memset(&si, 0, sizeof(si));
si.sin_family = PF_INET;
si.sin_addr.s_addr = inet_addr("202.201.239.100");
si.sin_port = htons(80);
// connect socket to remote port
connect(s, (struct sock*)&si, sizeof(si));
```
- We can use DNS(Domain name system) to replace the IP as well, DNS is just like a contact which can translate domain name into IP
```c
#include<netdb.h>
/*
struct addrinfo {
    int              ai_flags;      // options for adress information
    int              ai_family;     // address family, AF_INET`（IPv4）or `AF_INET6`（IPv6）
    int              ai_socktype;   // socket_type, SOCK_STREAM`（TCP) or `SOCK_DGRAM`（UDP）
    int              ai_protocol;   // protocal type, 0 refers to choose automatically
    socklen_t        ai_addrlen;    // socket address struct length
    struct sockaddr *ai_addr;       // pointer to sockaddr struct
    char            *ai_canonname;  // when set, it contains the official name of host
    struct addrinfo *ai_next;       // next addrinfo pointer, forms a link tabel
};
*/
struct addrinfo *res;
struct addrinfo hints;
memeset(&hints, 0, sizeoff(hints));
// PF_UNSPEC allows getaddrinfo() return both (AF_INET) and (AF_INET6) addrinfo
// users can choose one to use, as it returns a link table
hints.ai_family = PF_UNSPEC;
hints.ai_sockettype = SOCK_STREAM;
/*
int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res);
node: donamin name, string
services: port number or service name("80" or "http"), string
hints: pointer to addrinfo, use to specify the details of addrinfo we need
res: pointer to pointer of addrinfo, pointer to the result link table of addrinfo
*/
getaddrinfo("www.oreilly.com", "80", &hints, &res);
// create socket
int s = socket(res->ai_family, res->ai_socktype, res->ai_protocal);
connect(s, res->ai_addr, res->ai_addr_len);
freeaddrinfo(res);
```

### About HTTP

- Protocol is something that we need to do some standard talk before establish the communication
- When using HTTP to do socket communication, we need to do the followings:
1. send a format-right request to host
2. start receive information
3. format-right means a request include request line head and an empty line

```c
int main(int argc, char* argv[])
{
    int socket;
    socket = open_socket("en.wikipedia.org", "80");
    char buf[255];
    // request line
    sprintf(buf, "GET /wiki/%s http/1.1\r\n", argv[1]);
    say(socket, buf);
    // request head and empty line '\r\n'
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
```





