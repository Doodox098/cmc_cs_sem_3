#include <stdio.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    enum { KILOBYTE = 1024 };
    unsigned long long size = 0;
    for (int i = 1; i < argc; i++) {
        struct stat cur_file;
        if (!lstat(argv[i], &cur_file) && cur_file.st_nlink == 1
                && S_ISREG(cur_file.st_mode) && !(cur_file.st_size % KILOBYTE)) {
            size += cur_file.st_size;
        }
    }
    printf("%llu\n", size);
    return 0;
}