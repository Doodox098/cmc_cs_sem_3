#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
normalize_path(const char *path)
{
    size_t path_len = strlen(path);

    char *n_path = calloc(path_len + 1, sizeof(*n_path));
    char *cur_char = n_path;

    *cur_char = '/';
    cur_char++;

    char old_char = '/';
    int points_cnt = 0;
    do {
        path++;

        if (*path == '.' && (old_char == '.' || old_char == '/')) {
            points_cnt++;
        }

        if (*path != '.' && *path != '/' && *path) {
            points_cnt = 0;
        }

        int update_path = 0;
        if (*path == '/' || !*path) {
            if (points_cnt == 1) {
                update_path = 1;

                while (*cur_char != '/') {
                    *cur_char = 0;
                    cur_char--;
                }
            } else if (points_cnt == 2) {
                update_path = 1;

                while (*cur_char != '/') {
                    *cur_char = 0;
                    cur_char--;
                }

                if (n_path != cur_char) {
                    *cur_char = 0;
                    cur_char--;
                    while (*cur_char != '/') {
                        *cur_char = 0;
                        cur_char--;
                    }
                }
            }

            points_cnt = 0;
        }

        if (old_char != '/' || *path != '/') // удаляет подряд идущие слеши
        {
            *cur_char = *path;
        }

        old_char = *cur_char;

        cur_char += (!!*path || update_path);
    } while (*path);

    *cur_char = 0;

    int n_path_len = strlen(n_path);
    if (n_path_len > 1 && n_path[n_path_len - 1] == '/') {
        n_path[n_path_len - 1] = 0;
    }

    return n_path;
}

char *
relativize_path(const char *path1, const char *path2)
{
    size_t rel_path_len = 0;
    size_t rel_path_cap = PATH_MAX;
    char *rel_path = calloc(rel_path_cap, sizeof(*rel_path));

    char *n_path1 = normalize_path(path1);
    char *n_path2 = normalize_path(path2);

    char *cur1 = n_path1;
    char *cur2 = n_path2;

    char *last_slash = 0;
    for (int i = 0; *(cur1 + i); i++) {
        if (*(cur1 + i) == '/') {
            last_slash = cur1 + i;
        }
    }

    if (last_slash) {
        *(last_slash + 1) = 0;
    }

    while (*cur1 == *cur2 && *cur1) {
        cur1++;
        cur2++;
    }

    if (*cur1 != '/' && *cur2 == '/' && *cur1) {
        cur2--;
    }

    if (*cur2 != '/' && *cur1 == '/' && *cur2) {
        cur1--;
    }

    while (*cur1 != '/' && *cur1 && cur1 != n_path1) {
        cur1--;
    }
    cur1 += !!*cur1;

    while (*cur2 != '/' && *cur2 && cur2 != n_path2) {
        cur2--;
    }
    cur2 += !!*cur2;

    while (*cur1) {
        if (*cur1 == '/') {
            rel_path_len += 3;
            if (rel_path_len >= rel_path_cap) {
                rel_path_cap <<= 1;
                rel_path = realloc(rel_path, rel_path_cap * sizeof(*rel_path));
            }

            strcat(rel_path, "../");
        }

        cur1++;
    }

    rel_path_len += strlen(cur2);
    if (rel_path_len >= rel_path_cap) {
        rel_path_cap <<= 1;
        rel_path = realloc(rel_path, rel_path_cap * sizeof(*rel_path));
    }

    strcat(rel_path, cur2);
    if (rel_path_len == 0) {
        strcat(rel_path, ".");
    } else if (rel_path[rel_path_len - 1] == '/') {
        rel_path[rel_path_len - 1] = 0;
    }

    free(n_path1);
    free(n_path2);

    return rel_path;
}

int
main(void){
    while(1){
        ;char x[100] = "";
        fgets(x, 100, stdin);
        printf("%s", normalize_path(x));
    }
    return 0;
}