#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if (argc < 1) {
        return 1;
    }
    int output = open(argv[2], O_RDWR | O_CREAT, 0666);
    if (!strcmp(argv[1], "-p")) {
        int n = (int)strtol(argv[3], NULL, 10);
        double *in = calloc(n, sizeof(*in));
        read(output, in, n * sizeof(double));
        for (int i = 0; i < n; i++) printf("%lf\n", in[i]);
    } else if (!strcmp(argv[1], "-l")) {
        ftruncate(output, 0);
        int n = (int)strtol(argv[4], NULL, 10);
        FILE *input = fopen(argv[3], "r");
        double *in = calloc(n, sizeof(*in));
        for (int i = 0; i < n; i++) fscanf(input, "%lf", &in[i]);
        write(output, in, n * sizeof(double));
        fclose(input);
    } else if (!strcmp(argv[1], "-i")) {
        ftruncate(output, 0);
        int n = (int)strtol(argv[3], NULL, 10);
        double *in = calloc(n, sizeof(*in));
        for (int i = 0; i < n; i++) fscanf(stdin, "%lf", &in[i]);
        write(output, in, n * sizeof(double));
        fclose(stdin);
    } else if (!strcmp(argv[1], "-c")) {
        ftruncate(output, 0);
    } else if (!strcmp(argv[1], "-h")) {
        printf("*** -p  -  current nums in file\n"
               "***     ./Test -p TEST_BIN NUM\n"
               "*** -l  -  load nums from text file\n"
               "***     ./Test -l TEST_BIN TEST_TXT NUM\n"
               "*** -i  -  write nums using stdin\n"
               "***     ./Test -i TEST_BIN NUM\n"
               "*** -c  -  clean file\n"
               "***     ./Test -c TEST_BIN\n");
    }
    close(output);
    return 0;
}
