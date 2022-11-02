//код с консультации
#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void
all_files(const char *path, long long max_size, int depth, char *name)
{
    enum {DEPTH_MAX = 4};
    if (depth > DEPTH_MAX) {
        return;
    }
    printf("%s\n", path);
    int now_dir = 0;
    int vec_mem = 16;
    char **in_dirs = calloc(vec_mem, sizeof(*in_dirs));
    DIR *curr = opendir(path);
    //char curr_path[PATH_MAX];
    char *curr_path = NULL;
    char *cur_name = NULL;
    if (curr == NULL) {
        return;
    }
    struct dirent *curr_d;
    while ((curr_d = readdir(curr)) != NULL) {
        struct stat stb;
        free(curr_path);
        free(cur_name);
        asprintf(&curr_path, "%s/%s", path, curr_d->d_name);
        asprintf(&cur_name, "%s/%s", name, curr_d->d_name);
        if (lstat(curr_path, &stb) != 0) {
            continue;
        }
        if (S_ISREG(stb.st_mode) && stb.st_size <= max_size && S_IREAD & stb.st_mode) {
            if (cur_name[0] == '/') {
                printf("%s\n", cur_name + 1);
            } else {
                printf("%s\n", cur_name);
            }
        } else if (S_ISDIR(stb.st_mode)) {
            if (strcmp(curr_d->d_name, ".") == 0 || 
                    strcmp(curr_d->d_name, "..") == 0) {
                continue;
            }
            if (now_dir >= vec_mem) {
                vec_mem *= 2;
                in_dirs = realloc(in_dirs, vec_mem * sizeof(*in_dirs));
            }
            in_dirs[now_dir++] = strdup(curr_d->d_name); 
        }
    }
    free(curr_path);
    free(cur_name);
    closedir(curr);
    for (int i = 0; i < now_dir; ++i) {
        asprintf(&curr_path, "%s/%s", path, in_dirs[i]);
        asprintf(&cur_name, "%s/%s", path, in_dirs[i]);
        all_files(curr_path, max_size, depth + 1, cur_name);
        free(in_dirs[i]);
        
    }
    free(in_dirs);
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
    long long max_size = strtol(argv[2], &eptr, BASE);
    if (errno || *eptr || eptr == argv[2]) {
        exit(-1);
    }
    int depth = 1;
    char name[PATH_MAX] = "";
    all_files(argv[1], max_size, depth, name);
    return 0;
}
