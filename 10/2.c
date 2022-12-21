#include <stdlib.h>

enum
{
    INC = 12345,
    MUL = 1103515245,
    MOD = 1 << 31
};

typedef struct RandomGenerator
{
    int seed;
    struct RandomOperations *ops;

} RandomGenerator;

typedef struct RandomOperations
{
    int
    (*next)(RandomGenerator *);

    void
    (*destroy)(RandomGenerator *);
} RandomOperations;


int
next(RandomGenerator *a)
{
    a->seed = (((long long) MUL * a->seed) % MOD + INC % MOD) % MOD;
    return a->seed;
}

void
destroy(RandomGenerator *a)
{
    free(a->ops);
    free(a);
}

RandomGenerator
*random_create(int seed)
{
    RandomGenerator *cur = calloc(1, sizeof(*cur));
    cur->ops = calloc(1, sizeof(RandomOperations));
    cur->ops->next = next;
    cur->seed = seed;
    cur->ops->destroy = destroy;
    return cur;
}
