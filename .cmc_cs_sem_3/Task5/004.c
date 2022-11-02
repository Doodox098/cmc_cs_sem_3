#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

int
compare(const void *v1, const void *v2) {
    if (strcmp(*(const char**)v2, *(const char**)v1) > 0) {
        return -1;
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    ino_t *inodes = calloc(argc - 1, sizeof(*inodes));
    char **output = calloc(argc - 1, sizeof(*output));
    int num = 0;
    for (int i = 1; i < argc; i++) {
        struct stat input;
        if (access(argv[i], F_OK)) {
            continue;
        }
        stat(argv[i], &input);
        int flag = 0;
        for (int j = 0; j < num; j++) {
            if (num && inodes[j] == input.st_ino) {
                if (strcmp(output[j], argv[i]) < 0) {
                    output[j] = argv[i];
                }
                flag = 1;
                break;
            }
        }
        if (flag) {
            continue;
        }
        inodes[num] = input.st_ino;
        output[num++] = argv[i];
    }
    qsort(output, num, sizeof(*output), compare);
    for (int i = 0; i < num; i++) {
        puts(output[i]);
    }
    return 0;
}