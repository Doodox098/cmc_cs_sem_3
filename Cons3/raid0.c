// Raid 0 "Disk1" "Disk2" "Disk3" "Disk4"
// BlockSize

#include <fcntl.h>
#include <unistd.h>

enum { BlockSize = 512 };
enum { COUNT = 4 };

int fd[COUNT];

void Ini_Raid0(void)
{
    char *names[COUNT] = {"Disk0", "Disk1", "Disk2", "Disk3"};
    for (int i = 0; i < COUNT; i++) {
        fd[i] = open(names[i], O_RDONLY);
    }
}

void Read_Raid0(int num, char *buf)
{
    int disk_n = num % COUNT;
    off_t offset = BlockSize * (num / COUNT);
    lseek(fd[disk_n], offset, SEEK_SET);
    read(fd[disk_n], buf, BlockSize);
}