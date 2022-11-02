// execve
// busybox

//    #define _GNU_SOURCE
//    #include <stdio.h>
//    #include <stdlib.h>
//    #include <string.h>
//    #include <errno.h>
//    #include <sys/types.h>
//
//    int main(int argc, char *argv[]) {
//        execve(argv[1], argv + 1, envp);
//        fprintf(stderr, "execve failed: %s\n", strerror(errno));
//    }

//    #include <stdio.h>
//
//    int main(int argc, char *argv[])
//    {
//        for (int i = 0; i < argc; ++i) {
//            printf("[%d]: %s\n", i, argv[i]);
//        }
//    }

//    [cher@fedora sem09]$ cat p3.foo
//    #! /home/cher/cmc-os/2022-2023/sem09/args -a -b -c
//
//    hello

//     /bin/env args -a -b -c
//
//     hello

//  PATH=`pwd`/sem09:$PATH

// system

//    #define _GNU_SOURCE
//    #include <stdio.h>
//    #include <unistd.h>
//    #include <sys/wait.h>
//    #include <sys/types.h>
//    #include <stdlib.h>
//
//    int sys(const char *command)
//    {
//        pid_t pid = fork();
//        if (pid < 0) {
//            return -1;
//        }
//        if (!pid) {
//            execl("/bin/sh", "sh", "-c", command, NULL);
//            _exit(1);
//        }
//        int status = 0;
//        int r = waitpid(pid, &status, 0);
//        if (r >= 0) {
//            if (WIFEXITED(status)) {
//                return WEXITSTATUS(status);
//            } else if (WIFSIGNALED(status)) {
//                return 128 + WTERMSIG(status);
//            } else {
//                abort();
//            }
//        } else {
//            abort();
//        }
//    }

//    #include <stdio.h>
//    #include <unistd.h>
//    #include <sys/types.h>
//    #include <sys/wait.h>
//    #include <stdlib.h>
//    #include <fcntl.h>
//
//    int
//    main(void) {
//        int pid = fork();
//        if (pid < 0) {
//            abort();
//        }
//        if (!pid) {
//            int fd = open("/tmp/out.txt", O_WRONLY | O_TRUNC | O_CREAT | O_CLOEXEC, 0600);
//            if (fd < 0) {
//                _exit(1);
//            }
//            if (dup2(fd, STDOUT_FILENO) < 0) {
//                _exit(1);
//            }
//            if (chdir("usr/bin") < 0) {
//                _exit(1);
//            }
//            execlp("ls", "ls", "-l", NULL);
//            _exit(1);
//        }
//        wait(NULL);
//    }

//    getresuid() getresgid()