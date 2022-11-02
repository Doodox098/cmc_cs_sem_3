#include <stdio.h>
#include <stdlib.h>

int
compare(const void * val1, const void * val2)
{
    if (*(int*)val1 % 2 != *(int*)val2 % 2){
        return 1;
    } else if((*(int*)val1 & 1)) {
        return *(int*)val2 - *(int*)val1;
    } else {
        return *(int*)val1 - *(int*)val2;
    }
}

void
sort_even_odd(size_t count, int *data)
{
    if (count <= 1) {
        return;
    }
    int *start = data;
    int *end = data + count - 1;
    while (start != end) {
        if (*end % 2 == 0) {
            while (*start % 2 == 0 && start != end) {
                start++;
            }
            if (start == end) {
                break;
            }
            int swap = *start;
            *start = *end;
            *end = swap;
        }
        end--;
    }
    qsort(data, count, sizeof(int), compare);
}

int
main(void)
{
    int m[] = {7, 4, 8, 2,1, 3, 5,  3};
    sort_even_odd(8, m);
    return 0;
}