#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

int
main(int argc, const char *argv[])
{
    if (argc < 3) {
        return -1;
    }
    unsigned long long size = 0;
    DIR *first_dir = opendir(argv[1]);
    DIR *second_dir = opendir(argv[2]);
    struct dirent *first_file;
    struct dirent *second_file;
    char *first_path = calloc(PATH_MAX, sizeof(*first_path));
    char *second_path = calloc(PATH_MAX, sizeof(*second_path));
    struct stat first_stat;
    struct stat second_stat;
    while ((first_file = readdir(first_dir)) != NULL) {
        snprintf(first_path, PATH_MAX, "%s/%s", argv[1], first_file->d_name);
        if (lstat(first_path, &first_stat) < 0) {
            continue;
        }
        if (!S_ISREG(first_stat.st_mode)) {
            continue;
        }
        long pos = telldir(second_dir);
        while ((second_file = readdir(second_dir)) != NULL) {
            snprintf(second_path, PATH_MAX, "%s/%s", argv[2], second_file->d_name);
            if (stat(second_path, &second_stat) < 0) {
                continue;
            }
            if (!S_ISREG(second_stat.st_mode) || access(second_path, W_OK)) {
                continue;
            }
            if (second_stat.st_ino == first_stat.st_ino && second_stat.st_dev == first_stat.st_dev) {
                if (size < ULLONG_MAX - first_stat.st_size) {
                    size += first_stat.st_size;
                }
                break;
            }
        }
        seekdir(second_dir, pos);
    }
    closedir(second_dir);
    closedir(first_dir);
    printf("%llu\n", size);
    free(first_path);
    free(second_path);
    return 0;
}