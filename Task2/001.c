#include <stdio.h>

int
main(void)
{
    char line[150];
    scanf("%s", line);
    printf("[Host:%s", line);
    scanf("%s", line);
    printf(",Login:%s", line);
    scanf("%s", line);
    printf(",Password:%s]\n", line);
    return 0;
}
