#include <stdio.h>
#include <stdlib.h>

typedef struct RandomGenerator RandomGenerator;
typedef struct RandomOperations
{
    int (*next)(RandomGenerator*);
    void (*destroy)(RandomGenerator*);
} RandomOperations;

struct RandomGenerator
{
    int value;
    RandomOperations *ops;
};

int next(RandomGenerator* rr) {
    enum {
        MUL = 1103515245,
        INC = 12345,
        MOD = 1u << 31,
    };
    unsigned int temp = rr->value;
    unsigned long long new_val = (MUL * temp + INC) % MOD;
    rr->value = (int) new_val;
    return (int) new_val;
}

void destroy(RandomGenerator* rr) {
    free(rr->ops);
    free(rr);
}

RandomGenerator *random_create(int seed) {
    RandomGenerator *new_rr = calloc(1, sizeof(*new_rr));
    if (new_rr == NULL) {
        return NULL;
    }
    new_rr->value = seed;
    new_rr->ops = calloc(1, sizeof(*new_rr->ops));
    if (new_rr->ops == NULL) {
        free(new_rr);
        return NULL;
    }
    new_rr->ops->next = next;
    new_rr->ops->destroy = destroy;
    return new_rr;
}
