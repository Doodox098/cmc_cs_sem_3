#include <string.h>
#include <stdio.h>

void
normalize_path(char *buf) {
    enum {
        SIZE_OF_TWO_POINTS = 3,
        SIZE_OF_ONE_POINT = 2
    };
    if (buf == (char *)0) {
        return;
    }
    char *buf_write = buf;
    char *buf_read = buf + 1;
    while (*buf_read != '\0' && *buf_read != '\n') {
        if (*buf_read == '.' && *(buf_read - 1) == '/') {
            if (*(buf_read + 1) == '/' || *(buf_read + 1) == '\n' || *(buf_read + 1) == '\0') {
                if (*(buf_read + 1) == '/') {
                    buf_read += SIZE_OF_ONE_POINT;
                } else {
                    buf_read++;
                }
                continue;
            } else if (*(buf_read + 1) == '.'
                    && (*(buf_read + 2) == '/' || *(buf_read + 2) == '\n' || *(buf_read + 2) == '\0')) {
                int f = 0;
                while ((*buf_write != '/' || f == 0) && buf_write > buf) {
                    if (*buf_write == '/') {
                        f = 1;
                    }
                    *buf_write = '\0';
                    buf_write--;
                }
                if (*(buf_read + 2) == '/'){
                    buf_read += SIZE_OF_TWO_POINTS;
                } else {
                    buf_read += SIZE_OF_ONE_POINT;
                }
                continue;
            }
        }
        buf_write++;
        *buf_write = *buf_read;
        buf_read++;
    }
    buf_write++;
    *buf_write = *buf_read;
    buf_write--;
    if (*buf_write == '/' && buf_write != buf) {
        *buf_write = *buf_read;
        buf_write--;
    }
    if(buf_write + 1 != buf_read) {
        buf_write += 2;
        while (*buf_write != '\0') {
            *buf_write = '\0';
            buf_write++;
        }
    }
}


