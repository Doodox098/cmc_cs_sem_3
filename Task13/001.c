#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    int fd[2];
    
    pipe(fd);
    
    int pid = fork();
    if(pid < 0) {
        exit(1);
    } else if (!pid) {
        pid = fork();
        if(pid < 0) {
            _exit(1);
        } else if (!pid) {
            pid = fork();
            if(pid < 0) {
                _exit(1);
            } else if (!pid) {
                pid = fork();
                if(pid < 0) {
                    _exit(1);
                } else if (!pid) {
                    long long t = time(NULL);
                    write(fd[1], &t, sizeof(t));
                    write(fd[1], &t, sizeof(t));
                    write(fd[1], &t, sizeof(t));
                } else {
                    wait(NULL);
                    long long t;
                    read(fd[0], &t, sizeof(t));
                    struct tm cur_t;
                    localtime_r((time_t*)(&t), &cur_t);
                    printf("D:%02d\n", cur_t.tm_mday);
                    fflush(stdout);
                    _exit(0);
                }
            } else {
                wait(NULL);
                long long t;
                read(fd[0], &t, sizeof(t));
                struct tm cur_t;
                localtime_r((time_t*)(&t), &cur_t);
                printf("M:%02d\n", cur_t.tm_mon + 1);
                fflush(stdout);
                _exit(0);
            }
        } else {
            wait(NULL);
            long long t;
            read(fd[0], &t, sizeof(t));
            struct tm cur_t;
            localtime_r((time_t*)(&t), &cur_t);
            printf("Y:%02d\n", cur_t.tm_year + 1900);
            fflush(stdout);
            _exit(0);
        }
    } else {
        wait(NULL);
        exit(0);
    }
}