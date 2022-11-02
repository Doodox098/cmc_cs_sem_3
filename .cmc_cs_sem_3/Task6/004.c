#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void
files_check(const char* path, unsigned long long max_size, int depth, char *name)
{
    enum {DEPTH_MAX = 4};
    if (depth > DEPTH_MAX) {
        return;
    }
    int dirs_mem = 16;
    int dirs_amount = 0;
    char **directories = calloc(dirs_mem, sizeof(*directories));
    if (directories == NULL) {
        return;
    }
    DIR *dir = opendir(path);
    if (!dir) {
        return;
    }
    char *cur_path = calloc(PATH_MAX, sizeof(*cur_path));
    if (cur_path == NULL) {
        return;
    }
    struct dirent *cur_dir;
    while ((cur_dir = readdir(dir)) != NULL) {
        struct stat cur_file;
        sprintf(cur_path, "%s/%s", path, cur_dir->d_name);
        if (lstat(cur_path, &cur_file) < 0) {
            continue;
        }
        if (S_ISREG(cur_file.st_mode) && cur_file.st_size <= max_size && !access(cur_path, R_OK)) {
            if (name[0] == '/') {
                printf("%s%s\n", name + 1, cur_dir->d_name);
            } else {
                printf("%s%s\n", name, cur_dir->d_name);
            }
        } else if (S_ISDIR(cur_file.st_mode)) {
            if (strcmp(cur_dir->d_name, ".") == 0
                    || strcmp(cur_dir->d_name, "..") == 0) {
                continue;
            }
            if (dirs_amount >= dirs_mem) {
                dirs_mem += dirs_mem;
                directories = realloc(directories, dirs_mem * sizeof(*directories));
                if (directories == NULL) {
                    return;
                }
            }
            directories[dirs_amount++] = strdup(cur_dir->d_name);
        }
    }
    closedir(dir);
    char cur_name[PATH_MAX];
    for (int i = 0; i < dirs_amount; i++) {
        sprintf(cur_path, "%s/%s", path, directories[i]);
        sprintf(cur_name, "%s%s/", name, directories[i]);
        files_check(cur_path, max_size, depth + 1, cur_name);
        free(directories[i]);
    }
    free(cur_path);
    free(directories);
}

int
main(int argc, const char *argv[])
{
    enum {BASE = 10};
    if (argc < 3) {
        exit(1);
    }
    char *eptr = NULL;
    errno = 0;
    unsigned long long max_size = strtol(argv[2], &eptr, BASE);
    if (errno || *eptr || eptr == argv[2]) {
        max_size = 0;
    }
    int depth = 1;
    char name[PATH_MAX] = "";
    files_check(argv[1], max_size, depth, name);
    return 0;
}