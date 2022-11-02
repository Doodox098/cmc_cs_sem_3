#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

// return 0 if not a root, 1 if it is root and -1 if smth went wrong
int
check_if_root(int fd)
{
    if (fchdir(fd) == -1) {
        return -1;
    }
    DIR *pred = opendir("..");
    if (pred == NULL) {
        return -1;
    }
    int pred_fd = dirfd(pred);
    if (pred_fd == -1) {
        closedir(pred);
        return -1;
    }
    struct stat stb_son;
    struct stat stb_pred;
    if (fstat(fd, &stb_son) != 0 || fstat(pred_fd, &stb_pred) != 0) {
        closedir(pred);
        return -1;
    }
    closedir(pred);
    return (stb_son.st_ino == stb_pred.st_ino && stb_son.st_dev == stb_pred.st_dev);
}

// returns -1 if smth went wrong and 1 if everything is ok
int
getway(int fd, char *res)
{
    int check_if_root_res = check_if_root(fd);
    if (check_if_root_res == -1) {
        return -1;
    } else if (check_if_root_res == 1) {
        strcat(res, "/");
        return 1;
    }
    struct stat stb_curr;
    if (fstat(fd, &stb_curr) != 0) {
        return -1;
    }
    if (!S_ISDIR(stb_curr.st_mode)) {
        return -1;
    }
    DIR *pred_dir = opendir("..");
    if (pred_dir == NULL) {
        return -1;
    }
    if (chdir("..") == -1) {
        closedir(pred_dir);
        return -1;
    }
    struct dirent *curr_d;
    while ((curr_d = readdir(pred_dir)) != NULL) {
        struct stat stb_pred;
        if (strcmp(curr_d->d_name, "..") == 0 ||
            strcmp(curr_d->d_name, ".") == 0) {
            continue;
        }
        if (lstat(curr_d->d_name, &stb_pred) != 0) {
            continue;
        }
        if (stb_curr.st_ino == stb_pred.st_ino &&
            stb_curr.st_dev == stb_pred.st_dev) {
            break;
        }
    }
    if (curr_d == NULL) {
        closedir(pred_dir);
        return -1;
    }
    DIR *dir_to_go = opendir(".");
    if (dir_to_go == NULL) {
        closedir(pred_dir);
        return -1;
    }
    int fd_to_go = dirfd(dir_to_go);
    if (fd_to_go == -1) {
        closedir(pred_dir);
        closedir(dir_to_go);
        return -1;
    }
    int st = getway(fd_to_go, res);
    if (st == -1) {
        free(curr_d);
        closedir(pred_dir);
        closedir(dir_to_go);
        return -1;
    }
    strcat(res, "/");
    strcat(res, curr_d->d_name);
    closedir(pred_dir);
    closedir(dir_to_go);
    return 1;
}

ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    DIR *pwd = opendir(".");
    if (pwd == NULL) {
        return -1;
    }
    int fdpwf = dirfd(pwd);
    if (fdpwf == -1) {
        closedir(pwd);
        return -1;
    }
    char total_path[PATH_MAX] = "";
    int st = getway(fd, total_path);
    if (fchdir(fdpwf) == -1) {
        closedir(pwd);
        return -1;
    }
    if (st == -1) {
        closedir(pwd);
        return -1;
    }
    closedir(pwd);
    return snprintf(buf, size, "%s", total_path);
}


int
main(int argc, const char *argv[])
{
    DIR *file = opendir(argv[1]);
    long n = strtol(argv[2], NULL, 10);
    char *buf = calloc(n, sizeof(*buf));
    char *path = calloc(151, sizeof(*path));
    getcwd(path, 150);
    puts(path);
    int a = getcwd2(dirfd(file), buf, n);
    if(buf!=0)puts(buf);
    printf("%d\n", a);
    getcwd(path, 150);
    puts(path);
    printf("%d\n", dirfd(file));
    free(buf);
    free(path);
    closedir(file);
    return 0;
}