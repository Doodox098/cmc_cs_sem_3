#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    enum Limits
    {
        MAX_N = 10000,
        MAX_M = 50000
    };
    int n, m;
    if (scanf("%d%d", &n, &m) != 2 || n < 1 || m < 1 || n > MAX_N || m > MAX_M) {
        return 1;
    }
    int *power_line;
    if (!(power_line = calloc(n, sizeof(*power_line)))) {
        return 1;
    }
    int type_of_command, first_pillar, last_pillar, snow_diff;
    for (int i = 0; i < m; i++) {
        scanf("%d", &type_of_command);
        if (type_of_command == 1) {
            scanf("%d%d%d", &first_pillar, &last_pillar, &snow_diff);
            for (int j = first_pillar; j < last_pillar; j++) {
                power_line[j] += snow_diff;
                if (power_line[j] < 0) {
                    power_line[j] = 0;
                }
            }
        } else {
            int snow_amount = 0;
            scanf("%d%d", &first_pillar, &last_pillar);
            for (int j = first_pillar; j < last_pillar; j++) {
                snow_amount += power_line[j];
            }
            printf("%d\n", snow_amount);
        }
    }
    free(power_line);
    return 0;
}
