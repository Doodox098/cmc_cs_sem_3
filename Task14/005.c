#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>

volatile int prime = 0;

void
handler(int s)
{
    static int count = 0;
    if (s == SIGTERM) {
        exit(0);
    }
    if (s == SIGINT) {
        ++count;
        if (count == 4) {
            exit(0);
        }
        printf("%d\n", prime);
        fflush(stdout);
    }
}

int
is_prime(int num)
{
    int max = sqrt(num) + 1;
    if (num == 2) {
        return 1;
    }
    for (int i = 2; i < max; ++i) {
        if (!(num % i)) {
            return 0;
        }
    }
    return 1;
}

int
main()
{
    int low, high;
    scanf("%d%d", &low, &high);
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGTERM, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    for (int i = low; i < high; ++i) {
        if (is_prime(i)) {
            prime = i;
        }
    }
    printf("-1\n");
    fflush(stdout);
    exit(0);
}