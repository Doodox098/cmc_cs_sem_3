#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
enum
{
    MIN_NUM_ARGS = 4,
    BASE = 10,
    SIZE_OF_WORD = 4,
    SIZE_OF_ARGS = 16,
};
struct Args
{
    int args[SIZE_OF_ARGS];
};

int
main(int argc, char *argv[])
{
    if (argc < MIN_NUM_ARGS) {
        exit(1);
    }
    void *handle = dlopen(argv[1], RTLD_LAZY);
    if (handle == NULL) {
        puts(dlerror());
        exit(1);
    }
    void *g = dlsym(handle, argv[2]);
    if (g == NULL) {
        exit(1);
    }
    char *this_sym = argv[3];
    unsigned long size = strlen(this_sym);
    if (size - 1 != argc - MIN_NUM_ARGS) {
        exit(1);
    }
    struct Args args = {};
    int indx = 0;
    for (int i = 1; i < size; i++) {
        if (this_sym[i] == 's') {
            memcpy(&args.args[indx], &argv[i + MIN_NUM_ARGS - 1], SIZE_OF_WORD);
        } else if (this_sym[i] == 'i') {
            errno = 0;
            char *eptr = NULL;
            long long num = strtoll(argv[i + MIN_NUM_ARGS - 1], &eptr, BASE);
            if (errno || *eptr || eptr == argv[i + MIN_NUM_ARGS - 1] || (int) num != num) {
                exit(1);
            }
            args.args[indx] = (int) num;
        } else if (this_sym[i] == 'd') {
            errno = 0;
            char *eptr = NULL;
            double num = strtod(argv[i + MIN_NUM_ARGS - 1], &eptr);
            if (errno || *eptr || eptr == argv[i]) {
                exit(1);
            }
            memcpy(&args.args[indx++], &num, SIZE_OF_WORD);
            memcpy(&args.args[indx], (char *) (&num) + SIZE_OF_WORD, SIZE_OF_WORD);
        } else {
            exit(1);
        }
        indx++;
    }
    if (*this_sym == 'i') {
        typedef int (*func_t) (struct Args);
        func_t f = g;
        printf("%d\n", f(args));
    } else if (*this_sym == 'd') {
        typedef double (*func_t) (struct Args);
        func_t f = g;
        printf("%.10g\n", f(args));
        
    } else if (*this_sym == 's') {
        typedef char* (*func_t) (struct Args);
        func_t f = g;
        printf("%s\n", f(args));
    } else if (*this_sym == 'v') {
        typedef double (*func_t) (struct Args);
        func_t f = g;
        f(args);
    }
    dlclose(handle);
    return 0;
}