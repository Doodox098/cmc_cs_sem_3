#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    if (fd == -1) {
        return -1;
    }
    DIR *old_dir = opendir(".");
    if (old_dir == NULL) {
        return -1;
    }
    if (fchdir(fd) < 0) {
        return -1;
    }
    DIR *dir = fdopendir(fd);
    if (dir == NULL) {
        return -1;
    }
    unsigned long long dirs_mem = 16;
    unsigned long long dirs_amount = 0;
    char **dirs = calloc(dirs_mem, sizeof(*dirs));
    if (dirs == NULL) {
        return -1;
    }
    struct stat next_st;
    struct stat cur_st;
    struct stat read_st;
    struct dirent *cur_dir;
    do {
        DIR *next_fd = opendir("..");
        if (next_fd == NULL) {
            return -1;
        }
        DIR *cur_fd = opendir(".");
        if (cur_fd == NULL) {
            return -1;
        }
        if (fstat(dirfd(next_fd), &next_st) < 0) {
            return -1;
        }
        if (fstat(dirfd(cur_fd), &cur_st) < 0) {
            return -1;
        }
        if (next_st.st_ino == cur_st.st_ino && next_st.st_dev == cur_st.st_dev) {
            closedir(next_fd);
            closedir(cur_fd);
            break;
        }
        if (dirfd(dir) != fd) {
            close(dirfd(dir));
        }
        closedir(dir);
        dir = fdopendir(dirfd(next_fd));
        //closedir(next_fd);
        closedir(cur_fd);
        if (dir == NULL) {
            return -1;
        }
        if (fchdir(dirfd(next_fd)) < 0) {
            return -1;
        }
        do {
            if ((cur_dir = readdir(dir)) == NULL) {
                return -1;
            }
            DIR *read_fd = opendir(cur_dir->d_name);
            if (read_fd == NULL) {
                read_st.st_ino = ~cur_st.st_ino;
                continue;
            }
            if (fstat(dirfd(read_fd), &read_st) < 0) {
                return -1;
            }
            closedir(read_fd);
        } while (read_st.st_ino != cur_st.st_ino || read_st.st_dev != cur_st.st_dev);
        if (dirs_amount >= dirs_mem) {
            dirs_mem += dirs_mem;
            dirs = realloc(dirs, dirs_mem * sizeof(*dirs));
            if (dirs == NULL) {
                return -1;
            }
        }
        dirs[dirs_amount++] = strdup(cur_dir->d_name);
    } while(next_st.st_ino != cur_st.st_ino || next_st.st_dev != cur_st.st_dev);
    closedir(dir);
    if (dirs_amount == 0) {
        if (size > 0 && buf != NULL) {
            strncpy(buf, "/", size);
        }
        if (fchdir(dirfd(old_dir)) < 0) {
            return -1;
        }
        closedir(old_dir);
        free(dirs);
        return 1;
    }
    char *new_buf = calloc(PATH_MAX + 1, sizeof(*new_buf));
    if (new_buf == NULL) {
        return -1;
    }
    int new_size = 0;
    while (dirs_amount > 0) {
        int old_size = new_size;
        new_size += snprintf(new_buf + new_size, PATH_MAX - new_size, "/%s", dirs[--dirs_amount]);
        free(dirs[dirs_amount]);
        if (old_size > new_size) {
            return -1;
        }
    }
    free(dirs);
    if (size > 0 && buf != NULL) {
        strncpy(buf, new_buf, size);
    }
    if (fchdir(dirfd(old_dir)) < 0) {
        return -1;
    }
    closedir(old_dir);
    free(new_buf);
    return new_size;
}


