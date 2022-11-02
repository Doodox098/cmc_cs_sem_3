#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, const char *argv[])
{
    if (argc < 2) {
        return 1;
    }
    const char pattern[] = ".exe";
    DIR *dir = opendir(argv[1]);
    if (!dir) {
        return 1;
    }
    unsigned long long count = 0;
    struct dirent *cur_dir;
    while ((cur_dir = readdir(dir)) != 0) {
        char buf[PATH_MAX + PATH_MAX];
        strncpy(buf, argv[1], PATH_MAX);
        buf[strlen(buf)] = '/';
        strncpy(buf + strlen(buf), cur_dir->d_name, PATH_MAX - strlen(buf));
        if (strlen(buf) > PATH_MAX) {
            continue;
        }
        struct stat cur_file;
        if (!stat(buf, &cur_file) && S_ISREG(cur_file.st_mode) && !access(buf, X_OK)
                && strlen(cur_dir->d_name) >= strlen(pattern) && !strcmp(buf + strlen(buf) - strlen(pattern), pattern)) {
            count++;
        }
    }
    closedir(dir);
    printf("%llu\n", count);
    return 0;
}