#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int
cmd(char *cur_cmd)
{
    int pid;
    pid = fork();
    if (pid < 0) {
        return 0;
    }
    if (!pid) {
        execlp(cur_cmd, cur_cmd, NULL);
        return 1;
    } else {
        int status;
        wait(&status);
        return !WEXITSTATUS(status) && WIFEXITED(status);
    }
}

int
main(int argc, char **argv)
{
    int ans;
    ans = !((cmd(argv[1]) || cmd(argv[2])) && cmd(argv[3]));
    return ans;
}