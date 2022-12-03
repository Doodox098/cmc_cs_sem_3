//_____________________________________________________________
//  System5 IPC
//  1) sem  =  mutex/pipe/eventfd/posix sem/condvar
//  2) msg  =  mqueue, pipe
//  3) shm - mmap
//  key_t
//_____________________________________________________________
//     #include <sys/types.h>
//     #include <sys/ipc.h>
//     #include <sys/sem.h>
//     int semget(key_t key, int nsems, int semflg)

//    #include <sys/msg.h>
//    int msgget(key_t key, int msgflg);

//    eventfd (UNIX)
//_____________________________________________________________
//    #include <fcntl.h>           /* For O_* constants */
//    #include <sys/stat.h>        /* For mode constants */
//    #include <mqueue.h>
//
//    mqd_t mq_open(const char *name, int oflag);
//    mqd_t mq_open(const char *name, int oflag, mode_t mode,
//                  struct mq_attr *attr);
//
//    Link with -lrt.

//    mq_send
//    mq_receive
//____________________________________________________________
//    Обедающие Философы (N философов)
/*
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>

int
main(int argc, char *argv[])
{
    int n = strtol(argv[1], NULL, 10);
    int *fds = calloc(n, sizeof(fds[0]));
    char *stat = mmap(NULL, n + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    memset(stat, '.', n);
    stat[n] = 0;
    
    for (int i = 0; i < n; ++i) {
        fds[i] = eventfd(1, 0);
    }
    for (int i = 0; i < n; ++i) {
        if (!fork()) {
            while (1) {
                stat[i] = 'W';
                unsigned long long v1;
                int s0 = i;
                int s1 = (i + 1) % n;
                if (s0 > s1) {
                    s1 = i;
                    s0 = (i + 1) % n;
                }
                read(fds[s0], &v1, sizeof(v1));
                read(fds[s1], &v1, sizeof(v1));
                // ...
                stat[i] = 'E';
                sched_yield();
                stat[i] = 'S';
                v1 = 1;
                write(fds[s1], &v1, sizeof(v1));
                write(fds[s0], &v1, sizeof(v1));
                // ...
                sched_yield();
                
            }
            _exit(0);
        }
    }
    
    while (1) {
        printf("%s\r", stat);
        fflush(stdout);
        sched_yield();
    }
}
 */
//____________________________________________________________
// Одновременное ожидание
// select, pselect, poll, ppoll, epoll

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int intflag = 0;

void handler(int s)
{
    intflag = 1;
}


int main(int argc, char *argv[])
{
    mkfifo("fifo", 0600);
    int fdf = open("fifo", O_RDONLY);
    sigset_t ss, ss1;
    sigemptyset(&ss);
    sigaddset(&ss, SIGINT);
    sigemptyset(&ss1);
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigprocmask(SIG_BLOCK, &ss, NULL);
    struct timespec to = {.tv_sec = 1, .tv_nsec = 500000000};
    
    while (1) {
        fd_set rfds;
        FD_ZERO(&rfds);
        int maxfd = 0;
        FD_SET(0, &rfds);
        FD_SET(fdf, &rfds);
        if (fdf > maxfd) maxfd = fdf;
        int r = pselect(maxfd + 1, &rfds, NULL, NULL, &to, &ss1);
        if (r < 0 && errno == EINTR) {
            printf("Signal!\n");
        } else if (r < 0) {
            fprintf(stderr, "error: %s\n", strerror(errno));
            exit(1);
        } else if (r == 0) {
            printf("Timeout!\n");
        } else {
            if (FD_ISSET(0, &rfds)) {
                char buf[80];
                int rr = read(0, buf, sizeof(buf) - 1);
                buf[rr] = 0;
                printf("stdin: %s\n", buf);
            }
            if (FD_ISSET(fdf, &rfds)) {
                char buf[80];
                int rr = read(fdf, buf, sizeof(buf) - 1);
                buf[rr] = 0;
                printf("pipe: %s\n", buf);
            }
        }
    }
}