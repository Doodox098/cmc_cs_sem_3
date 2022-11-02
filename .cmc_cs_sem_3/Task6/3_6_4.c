#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void
all_files(const char *path, int curr_depth, unsigned long long Z, char *rel_path)
{
    enum{
        MAX_DEPTH = 4,
        START_MEMORY = 16
    };
    if (curr_depth == 4) {
        return;
    }
    int now_dir = 0;
    int vec_mem = START_MEMORY;
    char **in_dirs = calloc(vec_mem, sizeof(*in_dirs));
    DIR *curr = opendir(path);
    char *curr_path = NULL;
    if (curr == NULL) {
        return;
    }
    struct dirent *curr_d;
    while ((curr_d = readdir(curr)) != NULL) {
        struct stat stb;
        free(curr_path);
        asprintf(&curr_path, "%s/%s", path, curr_d->d_name);
        if (lstat(curr_path, &stb) != 0) {
            continue;
        }
        if (S_ISDIR(stb.st_mode)) {
            if (strcmp(curr_d->d_name, ".") == 0 ||
                strcmp(curr_d->d_name, "..") == 0) {
                continue;
            }
            if (now_dir >= vec_mem) {
                vec_mem *= 2;
                in_dirs = realloc(in_dirs, vec_mem * sizeof(*in_dirs));
            }
            in_dirs[now_dir++] = strdup(curr_d->d_name);
        } else if (S_ISREG(stb.st_mode) && stb.st_size <= Z && !access(curr_d->d_name, R_OK)) {
                printf("%s/%s\n", rel_path, curr_d->d_name);
        }
    }
    free(curr_path);
    closedir(curr);

    char *new_rel_path = NULL;
    for (int i = 0; i < now_dir; ++i) {
        asprintf(&curr_path, "%s/%s", path, in_dirs[i]);
        asprintf(&new_rel_path, "%s%s/", rel_path, in_dirs[i]);
        all_files(curr_path, curr_depth++, Z, new_rel_path);
        free(in_dirs[i]);
        free(new_rel_path);
    }
    free(in_dirs);
}

int
main(int argc, char *argv[])
{
    enum{
        BASE = 10
    };
    char *eptr = NULL;
    errno = 0;
    long long Z = strtol(argv[2], &eptr, BASE);
    if (errno || *eptr || eptr == argv[2]) {
        exit(-1);
    }
    char *relative_path = calloc(PATH_MAX, sizeof(relative_path));
    all_files(argv[1],1,Z, relative_path);
    free(relative_path);
    return 0;
}

