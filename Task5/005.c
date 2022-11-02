#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int
copy_file(const char *srcpath, const char *dstpath) {
    enum {BUF_SIZE = 4 * 1024 * 8};
    char buf[BUF_SIZE];
    struct stat srcinfo, dstinfo;
    int input;
    if ((input = open(srcpath, O_RDONLY)) == -1) {
        fprintf(stderr, "Cannot open source file\n");
        exit(-1);
    }
    stat(srcpath, &srcinfo);
    if (S_ISDIR(srcinfo.st_mode)) {
        fprintf(stderr, "Source file is directory\n");
        exit(-1);
    }
    stat(dstpath, &dstinfo);
    if (srcinfo.st_ino == dstinfo.st_ino) {
        return 0;
    }
    int output;
    if (S_ISDIR(dstinfo.st_mode)) {
        char *file = calloc(BUF_SIZE, sizeof(*file));
        strcpy(file, dstpath);
        file[strlen(file)] = '/';
        strcpy(file + strlen(file), strrchr(srcpath, '/')? strrchr(srcpath, '/') : srcpath);
        if ((output = open(file, O_WRONLY | O_CREAT | O_TRUNC, srcinfo.st_mode)) == -1) {
            fprintf(stderr, "Cannot open destination file\n");
            exit(-1);
        }
        free(file);
    } else {
        if ((output = open(dstpath, O_WRONLY | O_CREAT | O_TRUNC, srcinfo.st_mode)) == -1) {
            fprintf(stderr, "Cannot open destination file\n");
            exit(-1);
        }
    }
    int size = 0;
    do {
        if ((size = read(input, buf, BUF_SIZE)) == -1) {
            fprintf(stderr, "Error in reading from destination file\n");
            exit(-1);
        }
        long w_size = size;
        char *buf_cur_pos = buf;
        while (w_size > 0){
            long wrote = write(output, buf_cur_pos, w_size);
            w_size -= wrote;
            buf_cur_pos += wrote;
        }
    } while (size != 0);
    close(input);
    close(output);
    return 0;
}
