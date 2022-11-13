// pipe
// PIPE_BUF - размер буфера и максимальный размер атомарности.

// read:
// 1) Нет fd для записи буфер пуст 0
// 2) Если данные есть read возвращается немедленно
// 3) Ждать

// write:
// 1) SIGPIPE (НЕТ FD НА ЧТЕНИЕ) EPIPW - ошибка при записи
// 2) PIPE <= PIPE_BUF
//      а) если есть место, записывается атомарно
//      б) иначе ждать
// 3) > PIPE_BUF
//      ???

//        Считаем PIPE_BUF

//        #define _GNU_SOURCE
//        #include <fcntl.h>
//        #include <stdio.h>
//        #include <unistd.h>
//        #include <limits.h>
//
//        int main()
//        {
//            int fd[2];
//            int count = 0;
//
//            printf("%d\n", (int) PIPE_BUF); //4кб
//            pipe2(fd, O_NONBLOCK);
//          //pipe(fd);
//          //fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);
//
//            while (1) {
//                char c = 0;
//                if (write(fd[1], &c, sizeof(c)) < 0)
//                    break;
//                ++count;
//            }
//            printf("%d\n", count);
//            return 0;
//        }

//  ls -l /proc/21023/fd/

//        #include <stdio.h>
//        #include <stdlib.h>
//        #include <unistd.h>
//        #include <sys/wait.h>
//
//        int main(int argc, char *argv[])
//        {

//            // _________________ //
//            // ARGV[1] | ARGV[2] //
//            // _________________ //

//            int fd[2];
//            pipe(fd);
//
//            if (!fork()) {
//                dup2(fd[0], 0);
//                close(fd[0]);
//                close(fd[1]);
//                execlp(argv[2], argv[2], NULL);
//                _exit(1);
//            }
//            close(fd[0]);
//            if (!fork()) {
//                close(fd[1]);
//                dup2(fd[1], 1);
//                execlp(argv[1], argv[1], NULL);
//                _exit(1);
//            }
//            close(fd[1]);
//            while (wait(NULL) > 0) {}
//        }

// pipe2

//        #define _GNU_SOURCE
//        #include <fcntl.h>
//        #include <stdio.h>
//        #include <stdlib.h>
//        #include <unistd.h>
//        #include <sys/wait.h>
//
//        int main(int argc, char *argv[])
//        {
//
//            // _________________ //
//            // ARGV[1] | ARGV[2] //
//            // _________________ //
//
//            int fd[2];
//            pipe2(fd, O_CLOEXEC);
//
//            if (!fork()) {
//                dup2(fd[0], 0);
//                execlp(argv[2], argv[2], NULL);
//                _exit(1);
//            }
//            if (!fork()) {
//                close(fd[1]);
//                dup2(fd[1], 1);
//                execlp(argv[1], argv[1], NULL);
//                _exit(1);
//            }
//            close(fd[0]);
//            close(fd[1]);
//            while (wait(NULL) > 0) {}
//        }

// eventfd