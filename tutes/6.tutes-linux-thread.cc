/*
 * @Date: 2022-11-15 09:57:53
 * @LastEditors: ThetisEliza wxf199601@gmail.com
 * @LastEditTime: 2022-11-15 15:08:38
 * @FilePath: /gobang-epoll-server-tute.hub/tutes/6.tutes-linux-thread.cc
 */

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


int var=100;
int a = 1;

void *tfn(void *arg)
{
    long int i;
    i = (long int)arg;
    
    if (i == 2) 
        pthread_exit(NULL);
    if (i == 3) {
        pthread_cancel(pthread_self());
        a++;
    }        
    printf("tid:%lu a: %d\n", pthread_self(), a);
    sleep(i);
    var++;
    printf("I've slept for %lu sec. tfn--pid=%d, tid=%lu, check var%d\n", 
        i, getpid(), pthread_self(), var);
    
    
    return (void*)0;
}

void *tfnTestCancel(void *args) 
{
    while(1) {
        printf("child thread ... \n");
        pthread_testcancel();
        sleep(2);
    }
}


typedef struct {
    int a;
    int b;
} exit_t;


void *tfnTestJoin(void *args)
{
    exit_t *ret;
    ret = (exit_t *)malloc(sizeof(exit_t));
    ret->a = 100;
    ret->b = 300;
    pthread_exit((void*) ret);
    return NULL;
}



void *tfnTestDetach(void *args)
{
    int n=5;
    while(n--) {
        printf("pthread tfn n=%d\n", n);
        sleep(1);
    }
    return (void*) 7;
}


/**
 * @description: In this tutorial we would show you the usage of thread. This is a really important part
 * for the next part - threadpool - the nexus of the server to process work tasks.
 * 
 * @param {int} argc
 * @param {char*} argv
 * @return {*}
 */
int main(int argc, char* argv[])
{
    // long long int n = 5, i;
    // pthread_t tid;
    // if (argc == 2)
    //     n = atoi(argv[1]);
    // for(int i=0; i<n; i++) {
    //     int ret = pthread_create(&tid, NULL, tfn, (void *)i);
    // }
    // sleep(n);
    // void *tret = NULL;
    // pthread_cancel(4);
    // pthread_join(4, &tret);
    // printf("I've slept for %lu sec. main--pid=%d, tid=%lu, check var%d\n", 
    //     n, getpid(), pthread_self(), var);

    // pthread_t tid;
    // void *tret = NULL;
    // pthread_create(&tid, NULL, tfnTestCancel, NULL);
    // sleep(5);
    // pthread_cancel(tid);
    // printf("child thread cancel signal sent\n");
    // pthread_join(tid, &tret);
    // printf("child thread exit code=%ld\n", tret);

    // pthread_t tid;
    // exit_t *retval;
    // pthread_create(&tid, NULL, tfnTestJoin, NULL);
    // pthread_join(tid, (void**)&retval);
    // printf("a=%d, b=%d\n", retval->a, retval->b);

    pthread_t tid;
    void* ret;
    pthread_create(&tid, NULL, tfn, NULL);
    pthread_detach(tid);
    int retvar = pthread_join(tid, (void**)&ret);
    if (retvar != 0) {
        printf("pthread join err\n");
    }
    else{
        printf("pthread exit with %ld\n", (long int)ret);
    }


    return 0;
}