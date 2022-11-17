#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc == 1) {
        exit(0);
    }
    int success = 1;
    char script[PATH_MAX];
    char executable[PATH_MAX];
    char *tmp_dir;
    if (!(tmp_dir = getenv("XDG_RUNTIME_DIR"))) {
        if (!(tmp_dir = getenv("TMPDIR"))) {
            tmp_dir = "/tmp";
        }
    }
    long tm = time(NULL);
    snprintf(script, PATH_MAX, "%s/.script%ld-%d.sh", tmp_dir, tm, getpid());
    snprintf(executable, PATH_MAX, "%s/.program%ld-%d", tmp_dir, tm, getpid());
    FILE *sh = fopen(script, "w");
    errno = 0;
    if (fprintf(sh, "#!/bin/bash\n"
            "gcc -O2 -std=gnu18 -Wall -Werror -Wno-pointer-sign -Werror=vla %s.c -o%s.exe\n"
            "%s.exe\n"
            "rm %s.c %s %s.exe\n", executable, executable, executable, executable, script, executable) < 0) {
        puts(strerror(errno));
        success = 0;
    }
    snprintf(executable, PATH_MAX, "%s/.program%ld-%d.c", tmp_dir, tm, getpid());
    chmod(script, 0500);
    fclose(sh);
    FILE *exe = fopen(executable, "w");
    errno = 0;
    if (fprintf(exe, "#include <stdio.h>\n"
            "#define summon puts(\"summon\")\n"
            "#define disqualify puts(\"disqualify\")\n"
            "#define reject puts(\"reject\")\n"
            "int main(void){\n"
            "int x;\n"
            "while(scanf(\"%%d\", &x) > 0){%s;}\n"
            "return 0;}\n", argv[1]) < 0) {
        puts(strerror(errno));
        success = 0;
    }
    chmod(executable, 0400);
    fclose(exe);
    if (success) {
        execlp(script, script, NULL);
    }
    remove(script);
    remove(executable);
    exit(0);
}