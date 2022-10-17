#include <stdio.h>

int main(void)
{
    static const double MIN_X = 2.0;
    static const double MAX_X = 5.0;
    static const double MIN_Y = 1.0;
    static const double MAX_Y = 7.0;
    static const double MAX_X_IF_MIN_Y = 3.0;
    double x, y;
    int ans = 0;
    if(scanf("%lf%lf", &x, &y) != 2) {
        return 1;
    }
    if (y >= MIN_Y && y <= MAX_Y && x >= MIN_X && x <= MAX_X && x - MAX_X_IF_MIN_Y <= y - MIN_Y) {
        ans = 1;
    }
    printf("%d\n", ans);
    return 0;
}
