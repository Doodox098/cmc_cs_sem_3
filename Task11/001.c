#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

int proc(void)
{
    int pid = fork();
    if (!pid) {
        write(1, "1\n", 2);
    }
    return pid;
}

int main()
{
    if ((proc()) && ((((fork() != 0) == proc())) == ((fork() != 0) && proc()))) {
        _exit(0);
    }
    _exit(0);
}
