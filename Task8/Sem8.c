// У процесса есть 3 стандатртных fd
// posix_spawn
// fork/exec
//    #include <stdio.h>
//    #include <unistd.h>
//    #include <stdlib.h>
//
//    int main()
//    {
//        pid_t pid = fork();
//        if (pid < 0) {
//            exit(1);
//        }
//        printf("%d\n", getpid());
//    }
// Race condition
// Data race
/*
int main()
{
    pid_t pid = fork();
    printf("hello\n");
    if (pid < 0) {
        exit(1);
    }
    if (pid > 0) _exit(0);
    printf("%d,%d,%d\n", pid, getpid(),getppid());
}
output:
3636,3635,3023
[cher@fedora sem08]$ 0,3636,1
 */
//#include <stdio.h>
//#include <unistd.h>
//#include <stdlib.h>

//int func()
//{
//    pid_t pid = fork();
//    if (pid < 0) {
//        exit(1);
//    } else if (!pid) {
//        int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
//        if (fd < 0) {
//            fprintf(stderr, "open failed\n");
//            _exit(1);
//        }
//        // exec...
//    }
//}

// wait waitpid wait3 wait4
//#include <stdio.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/wait.h>
//
//int main(void)
//{
//    for (int i = 0; i < 20; ++i) {
//        if (!fork()) {
//            printf("child %d\n", i);
//            fflush(stdout);
//            _exit(0);
//        }
//    }
//
//     while (wait(NULL) > 0) {
//     }
//}
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pid = fork();
    if (!pid) {
        abort();
        //_exit(111);
    }


    int status;
    waitpid(pid, &status, 0);
    if (!status) {}- NO! BAD!

    if (WIFEXITED(status)) {
        printf("exited with code: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("terminated with signal: %d\n", WTERMSIG(status));
    }
}








