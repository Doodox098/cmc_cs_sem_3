#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    static const double ROUND_RATIO = 10000.0;//константа для округления до 4 знаков
    static const double ROUND_RATIO_PERCENT = 100.0;//константа для округления с учетом деления на 100 процентов
    double price = strtod(argv[1], NULL);
    for (int i = 2; i < argc; i++) {
        char *eptr = NULL;
        errno = 0;
        double change = strtod(argv[i], &eptr);
        if (errno || *eptr || eptr == argv[i] || (double) change != change) {
            continue;
        }
        price = round(price * (ROUND_RATIO + change * ROUND_RATIO_PERCENT)) / ROUND_RATIO;
    }
    printf("%.4lf\n", price);
    return 0;
}
