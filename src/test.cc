#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <memory.h>
#include <sys/uio.h>
#include <sys/sendfile.h>
#include <syslog.h>
#include <pthread.h>



void *tfn(void *arg)
{
    long int i;
    i = (long int)arg;
    if (i == 2) 
        pthread_exit(NULL);
    sleep(i);
    printf("I've slept for %lu sec. tfn--pid=%d, tid=%lu\n", i, getpid(), pthread_self());
    return (void*)0;
}

/**
 * @brief 
 * 
 * @return int 
 */
int main(int argc, char* argv[])
{
    printf("%d\n", sizeof(void*));
    printf("%d\n", sizeof(long long int));
    printf("%d\n", sizeof(long int));
    printf("%d\n", sizeof(int));
    long long int n = 5, i;
    pthread_t tid;
    if (argc == 2)
        n = atoi(argv[1]);
    for(int i=0; i<n; i++) {
        int ret = pthread_create(&tid, NULL, tfn, (void *)i);
    }
    sleep(n);
    printf("I've slept for %lu sec. main--pid=%d, tid=%lu\n", n, getpid(), pthread_self());
    return 0;
}