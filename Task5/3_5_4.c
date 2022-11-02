#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int
cmp(const void *s1, const void *s2)
{
    if (strcmp(*(char **) s1, *(char **) s2) >= 0) {
        return 1;
    } else {
        return -1;
    }
}

int main(int argc, char *argv[])
{
    char **files = calloc(argc, sizeof(files));
    int *node_number = calloc(argc, sizeof(node_number));

    int index = 0;
    for (int i = 1; i < argc; i++) {
        struct stat sb;

        if (stat(argv[i], &sb) < 0) {
            continue;
        }
        int node = (int) sb.st_ino;
        int flag = 0;
        for (int j = 0; j < index; j++) {
            if (node_number[j] == node) {
                if (strcmp(argv[i], files[j]) > 0) {
                    files[j] = argv[i];
                }
                flag = 1;
            }
        }
        if (!flag) {
            node_number[index] = node;
            files[index++] = argv[i];
        }
    }
    qsort(files,index, sizeof(*files), cmp);
    for (int i = 0; i < index; ++i) {
        printf("%s\n", files[i]);
    }
    free(node_number);
    free(files);
    return 0;
}
