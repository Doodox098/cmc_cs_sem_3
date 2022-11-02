#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

int
copy_file(const char *srcpath, const char *dstpath) {
    enum {
        BUF_SIZE = 4 * 8 * 1024
    };
    char buf[BUF_SIZE];

    struct stat src, dst;
    int discr_input;
    if ((discr_input = open(srcpath, O_RDONLY)) < 0) {
        exit(-1);
    }

    stat(srcpath, &src);
    if (S_ISDIR(src.st_mode)) {
        exit(-1);
    }
    stat(dstpath, &dst);
    if (src.st_ino == dst.st_ino) {
        return 0;
    }

    int discr_out;
    if (S_ISDIR(dst.st_mode)) {
        char file_name[PATH_MAX];
        strcpy(file_name, dstpath);
        file_name[strlen(file_name)] = '/';

        unsigned long pos_slash = strlen(srcpath);
        unsigned long i = strlen(srcpath);
        while (i > 0) {
            if (srcpath[i] == '/') {
                pos_slash = i;
                break;
            }
            i--;
        }
        strcpy(file_name + strlen(file_name), &srcpath[pos_slash]);

        if ((discr_out = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, src.st_mode)) < 0) {
            exit(-1);
        }
    } else {
        if ((discr_out = open(dstpath, O_WRONLY | O_CREAT | O_TRUNC, src.st_mode)) < 0) {
            exit(-1);
        }
    }

    size_t size_read;
    do {
        if ((size_read = read(discr_input, buf, BUF_SIZE)) < 0) {
            exit(-1);
        }
        size_t size = size_read;
        char *curr = buf;
        while (size > 0) {
            size_t succes_write = write(discr_out, curr, size);
            size -= succes_write;
            curr += succes_write;
        }
    } while (size_read != 0);

    close(discr_out);
    close(discr_input);

    return 0;
}
int main(int argc, char *argv[]){
    copy_file(argv[1], argv[2]);
}