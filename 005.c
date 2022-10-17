#include <stdio.h>
#include <stdlib.h>

void
print_num(int k, int n, char *number)
{
    if (k == n) {
        printf("%s\n", number);
        return;
    }
    for (int i = 1; i <= n; i++) {
        int f = 0;
        for (int j = 0; j < k; j++) {
            if (number[j] == i + '0') {
                f = 1;
                break;
            }
        }
        if (f == 1) {
            continue;
        }
        number[k] = i + '0';
        print_num(k + 1, n, &number[0]);
    }
    return;
}

int
main(void)
{
    int n;
    if (scanf("%d", &n) != 1) {
        return 1;
    }
    if (n < 0 || n > 10) {
        return 1;
    }
    char *number = calloc(n + 1, sizeof(*number));
    for (int i = 1; i <= n; i++) {
        number[0] = i + '0';
        print_num(1, n, &number[0]);
    }
    free(number);
    return 0;
}