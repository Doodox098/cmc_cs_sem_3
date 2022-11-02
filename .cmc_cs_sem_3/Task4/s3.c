#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>

struct Node {
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
tree_to_array(int32_t *key, int idx, int file, int *i, size_t endpos) {
    if ((idx + 1) * sizeof(struct Node) > endpos){
        return;
    }
    struct Node node;
    lseek(file, idx * sizeof(struct Node), SEEK_SET);
    if (read(file, &node, sizeof(struct Node)) <= 0) {
        return;
    }
    key[(*i)++] = node.key;
    if (node.left_idx) {
        tree_to_array(key, node.left_idx, file, i, endpos);
    }
    if (node.right_idx) {
        tree_to_array(key, node.right_idx, file, i, endpos);;
    }Ы
}

int
cmp(int32_t a, int32_t b) {
    return a - b;
}

void
sort_rec(int32_t *arr, int32_t low, size_t high) {
    int32_t i = low;
    size_t j = high;
    int32_t sup = arr[(low + (high - low) / 2)];

    do {
        for (; cmp(arr[i], sup) < 0; i++);

        for (; cmp(arr[j], sup) > 0; j--);

        if (i <= j) {
            if (cmp(arr[i], arr[j]) > 0) {
                int32_t tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }

            i++;

            if (j > 0) {
                j--;
            }
        }
    } while (i <= j);

    if (i < high) {
        sort_rec(arr, i, high);
    }

    if (j > low) {
        sort_rec(arr, low, j);
    }
}

void
quick_sort(int32_t *arr, size_t size) {
    sort_rec(arr, 0, size - 1);
}

int
main(int argc, char *argv[]) {
    int discr = open(argv[1], O_RDONLY);
    printf("%d", discr);
    if (discr < 0) {
        return 0;
    }
    printf("****");
    size_t endpos = lseek(discr, 0, SEEK_END);
    size_t n = endpos / sizeof(struct Node);

    int32_t *key = calloc(n, sizeof(int32_t));

    tree_to_array(key,0, discr, 0, endpos);
    quick_sort(key, n);

    for(int i = 0; i < n; i++) {
        printf("%d ", key[i]);
        printf("****");
    }
    printf("\n");
    close(discr);
    free(key);
    return 0;
}