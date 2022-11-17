//#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc == 1) {
        exit(0);
    }
    char file[PATH_MAX];
    char *tmp_dir;
    if (!(tmp_dir = getenv("XDG_RUNTIME_DIR"))) {
        if (!(tmp_dir = getenv("TMPDIR"))) {
            tmp_dir = "/tmp";
        }
    }
    snprintf(file, PATH_MAX, "%s/.mul%ld-%d.py", tmp_dir, time(NULL), getpid());
    int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0700);
    const char interpreter[] = "#!/usr/bin/env python3\n";
    const char template_start[] = "import os, sys\nsys.set_int_max_str_digits(";
    char *template_cont;
    const char mul[] = "*";
    const char template_end[] = ")\nos.remove(sys.argv[0])";
    unsigned long long size = 640;
    for (int i = 1; i < argc; ++i) {
        size += strlen(argv[i]);
    }
    write(fd, interpreter, sizeof(interpreter) - 1);
    write(fd, template_start, sizeof(template_start) - 1);
    asprintf(&template_cont, "%llu)\nprint(", size);
    write(fd, template_cont, strlen(template_cont));
    free(template_cont);
    for (int i = 1; i < argc - 1; ++i) {
        write(fd, argv[i], strlen(argv[i]));
        write(fd, mul, 1);
    }
    write(fd, argv[argc - 1], strlen(argv[argc - 1]));
    write(fd, template_end, sizeof(template_end) - 1);
    close(fd);
    execlp(file, file, NULL);
    remove(file);
    exit(0);
}