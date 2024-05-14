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





