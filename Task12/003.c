#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    int count = 0;
    int status;
    for (int i = 1; i < argc;) {
        if(argv[i][0] == 'p') {
            while (i < argc && argv[i][0] == 'p') {
                int pid = fork();
                if (!pid) {
                    execlp(argv[i] + 1, argv[i] + 1, NULL);
                    _exit(1);
                } else {
                    i++;
                }
            }
            while(wait(&status) > 0) {
                if(status == 0) {
                    count++;
                }
            }
        } else if (argv[i][0] == 's') {
            int pid = fork();
            if (!pid) {
                execlp(&argv[i][1], &argv[i][1], NULL);
                _exit(1);
            }
            wait(&status);
            if(status == 0) {
                count++;
            }
            i++;
        }
    }
    printf("%d\n", count);
    exit(0);
}