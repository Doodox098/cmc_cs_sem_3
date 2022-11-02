#include <stdio.h>
#include <dlfcn.h>

int
main(int argc, char *argv[])
{
    char *func_name = argv[1];
    void *handle = dlopen("libm.so.6", RTLD_LAZY);
    void *g = dlsym(handle, func_name);

    typedef double (*func_t) (double);
    func_t f = g;
    double sum = 0;
    double x;
    while(scanf("%lf", &x) == 1) {
        sum += f(x);
    }
    printf("%.10g\n", sum);
    return 0;
}
// ./func SOME_FUNCTION NUM1 NUM2 ... NUMN