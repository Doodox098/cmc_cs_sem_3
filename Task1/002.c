#include <math.h>
#include <stdio.h>

int main(void)
{
    enum Defects
    {
        DEFECT_3 = 1 << 3,
        DEFECT_2 = ~(1 << 2)
    };
    enum SYM_INDEXES
    {
        MIN_NUM = 1,
        MAX_NUM = ('9' - '0' + 1),
        MIN_LOW_LETTER = 1 + ('9' - '0' + 1),
        MAX_LOW_LETTER = ('z' - 'a' + 1 + '9' - '0' + 1),
        MIN_UP_LETTER = 1 + ('z' - 'a' + 1 + '9' - '0' + 1),
        MAX_UP_LETTER = ('Z' - 'A' + 1 + 'z' - 'a' + 1 + '9' - '0' + 1),
        MAX_INDEX = 1 + ('Z' - 'A' + 1 + 'z' - 'a' + 1 + '9' - '0' + 1)
    };
    int cur_sym;
    cur_sym = getchar();
    while(cur_sym != EOF){
        char new_sym;
        if (cur_sym >= '0' && cur_sym <= '9') {
            cur_sym = cur_sym - '0' + MIN_NUM;
        } else if (cur_sym >= 'a' && cur_sym <= 'z') {
            cur_sym = cur_sym - 'a' + MIN_LOW_LETTER;
        } else if (cur_sym >= 'A' && cur_sym <= 'Z') {
            cur_sym = cur_sym - 'A' + MIN_UP_LETTER;
        } else {
            cur_sym = getchar();
            continue;
        }
        new_sym = (cur_sym ^ DEFECT_3) & DEFECT_2;
        if (new_sym >= MIN_NUM && new_sym <= MAX_NUM) {
            new_sym = new_sym + '0' - MIN_NUM;
        } else if (new_sym >= MIN_LOW_LETTER && new_sym <= MAX_LOW_LETTER) {
            new_sym = new_sym + 'a' - MIN_LOW_LETTER;
        } else if (new_sym >= MIN_UP_LETTER && new_sym <= MAX_UP_LETTER ) {
            new_sym = new_sym + 'A' - MIN_UP_LETTER;
        } else if (new_sym == 0) {
            new_sym = '@';
        } else if (new_sym == MAX_INDEX) {
            new_sym = '#';
        }
        putchar(new_sym);
        cur_sym = getchar();
    }
    return 0;
}
