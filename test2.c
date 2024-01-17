
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

#define BIND_CORE        1

struct {
    long a; // thread_1 access
    char data[64];
    long b; // thread 2 access
} data;


static int set_cpu(int i)  
{  
    cpu_set_t mask;  
    CPU_ZERO(&mask);  
    CPU_SET(i,&mask);  
  
    printf("thread %lu, i = %d\n", pthread_self(), i);  
    if(-1 == pthread_setaffinity_np(pthread_self() ,sizeof(mask),&mask))  
    {  
        return -1;  
    }  
    return 0;  
}  


void *thread_1()
{
#if BIND_CORE
    int cpu = 1;
    if(set_cpu(cpu))
    {  
        printf("set cpu erro\n");  
    }
#endif
    for (int i = 0; i < 1000000000; i++) {
        data.a = 0xAAAA;
    }
}

void *thread_2()
{
#if BIND_CORE
    int cpu = 2;
    if(set_cpu(cpu))
    {  
        printf("set cpu erro\n");  
    }
#endif
    for (int i = 0; i < 1000000000; i++) {
        data.b = 0xBBBB;
    }
}

int main()
{
    pthread_t id1,id2;
    struct timeval tv_start, tv_end;
    unsigned long t_us;

    int cpu_num;
    cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    printf("cpu_num = %d \r\n", cpu_num);

    if (gettimeofday(&tv_start, NULL) == -1) {
        printf("[Error] gettimeofday start failed \r\n");
        return -1;
    }
    pthread_create(&id1, NULL, (void*)thread_1, NULL);
    pthread_create(&id2, NULL, (void*)thread_2, NULL);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    if (gettimeofday(&tv_end, NULL) == -1) {
        printf("[Error] gettimeofday end failed \r\n");
        return -1;
    }
    t_us = tv_end.tv_sec * 1000000 + tv_end.tv_usec - (tv_start.tv_sec * 1000000 + tv_start.tv_usec);
    printf("[Total Consume] %ld us \r\n", t_us);
    printf("Main Thread left \r\n");
    return 0;
}