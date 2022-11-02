#include <dirent.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int
main(int argc, char **argv) {
    DIR *dr1 = opendir(argv[1]);
    if (dr1 == NULL) {
        exit(-1);
    }
    DIR *dr2 = opendir(argv[2]);
    if (dr1 == NULL) {
        exit(-1);
    }

    struct dirent *curr1;
    struct stat *inf1 = calloc(1, sizeof(*inf1));;
    char name1[PATH_MAX] = {};
    struct dirent *curr2;
    struct stat *inf2 = calloc(1, sizeof(*inf2));
    char name2[PATH_MAX] = {};
    long long start_pos = telldir(dr2);
    unsigned long long sum_size = 0;

    while((curr1 = readdir(dr1)) != NULL) {
        snprintf(name1, PATH_MAX, "%s/%s", argv[1], curr1->d_name);
        if (lstat(name1, inf1) < 0) {
            continue;
        }
        if (S_ISREG(inf1->st_mode) && access(name1, W_OK) == 0) {
            seekdir(dr2, start_pos);
            while((curr2 = readdir(dr2)) != NULL) {
                snprintf(name2, PATH_MAX, "%s/%s", argv[2], curr2->d_name);
                if (stat(name2, inf2) < 0) {
                    continue;
                }
                if (inf2->st_ino == inf1->st_ino && inf2->st_dev == inf1->st_dev) {
                    if (sum_size< ULLONG_MAX - inf2->st_size) {
                        sum_size += inf2->st_size;
                    }
                    break;
                }
            }
        }
    }
    closedir(dr2);
    closedir(dr1);
    free(inf2);
    free(inf1);
    printf("%llu\n", sum_size);
    return 0;
}
