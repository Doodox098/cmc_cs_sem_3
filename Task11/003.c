#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
    enum {
        BASE = 10,
    };
    int pid;
    for (int id = 1; id <= 3; id++) {
        pid = fork();
        if (pid < 0) {
            exit(-1);
        }
        if (!pid) {
            char line[8];
            long val;
            read(0, &line, sizeof(line));
            val = strtol(line, NULL, BASE);
            val *= val;
            printf("%d %d\n", id, (int)val);
            fflush(stdout);
            _exit(0);
        }
    }
    while (wait(NULL) > 0) {}
    return 0;
}
