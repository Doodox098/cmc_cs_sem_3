#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
tree_search(int input, int *keys, unsigned *n, int index, off_t file_size)
{
    struct Node node;
    if ((index + 1) * sizeof(struct Node) > file_size) {
        return;
    }
    lseek(input, index * sizeof(struct Node), SEEK_SET);
    if (read(input, &node, sizeof(struct Node)) <= 0){
        return;
    }
    keys[(*n)++] = node.key;
    if (node.right_idx) {
        tree_search(input, keys, n, node.right_idx, file_size);
    }
    if (node.left_idx) {
        tree_search(input, keys, n, node.left_idx, file_size);
    }
}

void quicksort(int* left, int* right) {
    if (right - left <= 1) return;
    int mid = *(left + (right - left) / 2);
    int *cur_left = left;
    int *cur_right = right - 1;
    while (cur_left <= cur_right) {
        while (*cur_left > mid) cur_left++;
        while (*cur_right < mid) cur_right--;
        if (cur_left <= cur_right) {
            int swap = *cur_left;
            *cur_left = *cur_right;
            *cur_right = swap;
            cur_left++;
            cur_right--;
        }
    }
    if (left < cur_right) quicksort(left, cur_right + 1);
    if (cur_left < right) quicksort(cur_left, right);
}

int
main(int argc, char *argv[])
{
    int input;
    if ((input = open(argv[1], O_RDONLY)) == -1) {
        return 0;
    }
    unsigned n = 0;
    off_t file_size = lseek(input, 0, SEEK_END);
    int *keys = calloc(file_size, sizeof(struct Node));
    tree_search(input, keys, &n, 0, file_size);
    quicksort(keys, &keys[n]);
    for (int i = 0; i < n; i++) {
        printf("%d ", keys[i]);
    }
    printf("\n");
    free(keys);
    return 0;
}
