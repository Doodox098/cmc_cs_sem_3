#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[]) {
    enum {
        NUM_ARGS = 4,
        BASE = 10
    };
    if (argc < NUM_ARGS) {
        exit(1);
    }
    char *eptr = NULL;
    errno = 0;
    long rows = strtol(argv[2], &eptr, BASE);
    if (errno || *eptr || eptr == argv[2] || (int) rows != rows) {
        puts("Wrong rows number");
        exit(1);
    }
    eptr = NULL;
    errno = 0;
    long cols = strtol(argv[3], &eptr, BASE);
    if (errno || *eptr || eptr == argv[2] || (int) cols != cols) {
        puts("Wrong cols number");
        exit(1);
    }
    size_t psz = sysconf(_SC_PAGESIZE);
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0600);
    unsigned long long size = rows * cols * sizeof(int);
    int *file = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (file == MAP_FAILED) {
        exit(1);
    }
    int cur_col = 0;
    int cur_row = 0;
    int row_growth = 0;
    int col_growth = 1;
    int rows_filled = 0;
    int cols_filled = 0;
    for (int i = 1; i <= rows * cols; i++) {
        file[cur_row * cols + cur_col] = i;
        if (col_growth > 0 && cur_col + col_growth >= cols - cols_filled / 2) {
            col_growth = 0;
            row_growth = 1;
            rows_filled += 1;
        } else if (row_growth > 0 && cur_row + row_growth >= rows - rows_filled / 2) {
            col_growth = -1;
            row_growth = 0;
            cols_filled += 1;
        } else if (col_growth < 0 &&  cur_col + col_growth < 0 + cols_filled / 2) {
            col_growth = 0;
            row_growth = -1;
            rows_filled += 1;
        } else if (row_growth < 0 && cur_row + row_growth < 0 + rows_filled / 2) {
            col_growth = 1;
            row_growth = 0;
            cols_filled += 1;
        }
        cur_col += col_growth;
        cur_row += row_growth;
    }
    write(fd, file, size);
    munmap(file, (size / psz + (size % psz != 0)) * psz);
    close(fd);
    return 0;
}