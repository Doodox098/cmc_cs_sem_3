#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>

/*void work(int serial, int *p)
{
    while (1) {
        printf("%d %d\n", serial, *p);
        fflush(stdout);
        ++*p;
        usleep(10000); //не использовать
    }
}*/

void work(int serial, volatile int *p)
{
    while (1) {
        if (p[serial] != 0) { // busy wait
            int buf = p[serial];
            p[serial] = 0;
            printf("%d %d\n", serial, buf);
            p[1 - serial] = buf + 1;
        }
    }
}

int main(int argc, char *argv[])
{
//    int v = 10;
//    int *p = &v;
    int *p = mmap(NULL, 2 * sizeof(*p), PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_ANON, -1, 0);
    p[0] = 1;
//    int *p1 = calloc(1, sizeof(*p1));
//    *p1 = 0;
    for (int i = 0; i < 2; ++i) {
        int pid = fork();
        if (pid == 0) {
            work(i, p);
            _exit(0);
        }
    }
    while (wait(NULL) > 0) {
    }
    exit(0);
}
// gcc p4.c -O2 -S -m32 -fverbose-asm
