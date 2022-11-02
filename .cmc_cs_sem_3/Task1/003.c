#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int n;
    if (scanf("%d", &n) != 1) {
        return 1;
    }
    int *line = calloc(n - 1, sizeof(*line));
    for (int i = 0; i < n - 1; i++) {
        printf("0 ");
    }
    printf("\n");
    line[0] = 1;
    line[n - 2] = n - 1;
    for (int i = 2; i < n - 1; i++) {
        int j;
        for (j = 0; (j * i) % n != 1; j++);
        line[i - 1] = j;
    }
    for (int i = 0; i < n - 1; i++) {
        printf("%d ", line[i]);
    }
    printf("\n");
    for (int i = 2; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            printf("%d ", (line[j] * i) % n);
        }
        printf("\n");
    }
    free(line);
    return 0;
}