#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    enum {
        NUMBER_OF_ARGS = 3,
        FILE_SIZE = 64 * 1024 * 8,
        BASE = 16,
        PAGE_OFFSET = 9,
        PPAGE_OFFSET = 8,
        PAGE_SIZE = 512,
        OFFSET_MASK = 0X1FF,
    };
    if (argc != NUMBER_OF_ARGS) {
        exit(1);
    }
    errno = 0;
    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        puts(strerror(errno));
        exit(1);
    }
    struct stat file_st;
    errno = 0;
    if (fstat(fd, &file_st) < 0) {
        puts(strerror(errno));
        exit(1);
    }
    if (file_st.st_size < FILE_SIZE) {
        puts("Wrong file size");
        exit(1);
    }
    char *eptr = NULL;
    errno = 0;
    long vmem_address = strtol(argv[2], &eptr, BASE);
    if (errno || *eptr || eptr == argv[2] || (int) vmem_address != vmem_address) {
        puts("Wrong vmem address");
        exit(1);
    }
    short *pmem = mmap(NULL, FILE_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
    if (pmem == MAP_FAILED) {
        exit(1);
    }
    unsigned int cur_address;
    while(scanf("%x", &cur_address) > 0) {
        errno = 0;
        if (fstat(fd, &file_st) < 0) {
            puts(strerror(errno));
            exit(1);
        }
        unsigned int v_page = cur_address >> PAGE_OFFSET;
        if ((vmem_address >> 1) + v_page >= FILE_SIZE || (vmem_address >> 1) + v_page >= file_st.st_size) {
            continue;
        }
        unsigned short p_page = *(pmem + (vmem_address >> 1) + v_page) >> PPAGE_OFFSET;
        unsigned int p_address = p_page * PAGE_SIZE + (cur_address & OFFSET_MASK);
        if (p_address >= (long)(pmem + FILE_SIZE) || p_address >= (long)(pmem + file_st.st_size)) {
            continue;
        }
        unsigned char * value_address = (unsigned char *)pmem + p_address;
        unsigned int value = *value_address;
        printf("%u\n", value);
    }
    munmap(pmem, FILE_SIZE);
    close(fd);
    return 0;
}
