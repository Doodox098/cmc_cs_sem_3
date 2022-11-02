#include <stdio.h>

int
main(int argc, char *argv[])
{
    unsigned long long sum = 0;
    long long n;
    while (scanf("%lld", &n) > 0){
        sum += n;
    }
    printf("%lld", n);
    return 0;
}
