# Thread

## Thread vs procedure

- Procedure: we got resources copied in a child process
1. Create Procedure need time(tens of ms)
2. Inconvenient in sharing data
3. Create Procedure need extra coding
- Thread: we will share those resources
1. We can assign every single task to thread
2. All threads will share same file, socket and variables
3. Every thread will just do one simple thing
- Resources: in hardware they are the data in registers, for software they are everything in stack and heap, including variables, file descriptor table

## Create thread

- For Linux, we will use pthread.h to manage thread
- For functions running in thread, we need to use return void pointer
- We use pthread_create() to create thread
- We use pthread_join() to maintain the main procedure running until thread ends
- pthread_join() will return a return value of thread function, get it
- when assemble, we need to use "gcc xxx.c **-lpthread** -o xxx"
```c
void* does_not(void *a)
{
    int i = 0;
    for(i = 0; i < 5; i++){
        sleep(1);
        puts("Dose not!");
    }
    return NULL;
}

void does_too(void *a)
{
    int i = 0;
    for(i = 0; i < 5; i++){
        sleep(1);
        puts("Dose too!");
    }
    return NULL;
}

// thread function can received a void pointer as parameters, long has the same lenth as void pointer!!!
// pthread_create(&threads[t], NULL, do_stuff, (void*)t);
void* do_stuff(void* param)
{
 long thread_no = (long)param;
 printf("Thread number %ld\n", thread_no);
 return (void*)(thread_no + 1);
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

void error(char *msg)
{
    fprintf(stderr, "%s : %s", msg, strerror(errno));
    exit(1);
}

int main(int argc, char* argv[])
{
    pthread_t t0;
    pthread_t t1;
    if(pthread_create(&t0, NULL, does_not, NULL) == -1)
        error("Can't create t0");
    if(pthread_create(&t1, NULL, does_not, NULL) == -1)
        error("Can't create t1");    

    void * result;
    if(pthread_join(t0, &result) == -1)   
        error("Can't recycle t0");
    if(pthread_join(t1, &result) == -1)   
        error("Can't recycle t1");    
    return 1;
}
```
## thread security

```c
#include<stdio.h>
#include<pthread.h>

int beers = 2000000;
void* drink_lots(void *a)
{
    int i;
    for(i = 0; i < 100000; i++){
        beers -= 1;
    }
    return NULL;
}

int main()
{
    pthread_t threads[20];
    printf("%i bottles on the wall\n%i bottles of beer\n", beers, beers);

    for(int t = 0; t < 20; t++){
        pthread_create(&threads[t], NULL, drink_lots, NULL);        
    }
    void* result;
    for(int t = 0; t < 20; t++){
        pthread_join(threads[t], &result);
    }
    printf("there are now %i bottles of beer on the wall\n", beers);
    return 0;
}
```
- The result of upper program would varies as the code is not **thread secured**
- When two threads **read and write the same variables**, it is not thread secured
- So we need to set lock to prevent it from accessing the shared resources simutaneously

### mutex lock

```c
pthread_mutex_t a_lock = PTHREAD_MUTEEX_INITIALIZER;
pthread_mutex_lock(&a_lock);
pthread_mutex_unlocK(&a_lock);
```
- PTHREAD_MUTEEX_INITIALIZER is a macro, when assembler reach it would insert the initialization code of mutex lock
- remember to use lock in suitable place, as it would affect the running speed severely

