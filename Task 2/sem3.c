//                                                      СЕМИНАР 3
// gcc -fsanitize=undefined -fsanitize=address -fsanitize=leak - сантиайзеры
// char беззнаковый 8 бит
// short 16 бит
// int 32 бита
// long ? бит
// long long 64 бита
// int128 не официальный стандарт
// Все отрицательные числа в ДОПОЛНИТЕЛЬНОМ КОДЕ


/*
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
    int64_t a = 100;
    uint64_t b;

    printf("%" PRIi64 "\n", a);
}
*/

// limits.h -> CHAR_BIT
// Big endian (например для быстрой обработки сетевого трафика)
// 0x12345678
// 12 34 56 78
// Little endian (основной)
// 78 56 34 12

// ОКРУЖЕНИЯ
// CONFORMING - окружение, содержащее стандартные библиотеки
// FREESTANDING - минимальный набор библиотек
// ldd - проверка динамических библиотек
// gcc -static - статическая линковка
// gcc 2.c -Wall -Werror -std=gnu18 -g -o 2 -nostdlib -nostdinc -ffreestanding
// arm-none-eabi-gcc 2.c -Wall -Werror -std=gnu18 -g -o 2 -nostdlib -nostdinc -ffreestanding
// objdump -D -j .text ./2
// СИСТЕМНЫЕ ВЫЗОВЫ
// |PROCESS| -syscalls-> |KERNEL|

/*
const char hello[] = "hello";

__attribute__((naked))
void _start(void)
{
    asm("mov    $4, %eax\n"
        "mov    $1, %ebx\n"
        "mov    $hello, %ecx\n"
        "mov    $6, %edx\n" // Вызов syscall write
        "int    $0x80\n"
        "mov    $1, %eax\n"
        "mov    $55, %ebx\n"
        "int    $0x80\n");
}
*/

// ФАЙЛОВЫЙ ДЕСКРИПТОР
// 0 - STDIN_FILE0
// 1 - STDOUT_FILE1
// 2 - STDERR

/*
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

int main()
{
    uint8_t c;

    while (1) {
        ssize_t r = read(STDIN_FILENO, &c, sizeof(c));
        if (r < 0) {
            fprintf(stderr, "read failed: %s\n", strerror(errno));
            _exit(1);
        }
        if (!r) {
            break;
        }
        r = write(STDOUT_FILENO, &c, sizeof(c));
    }

    _exit(0);
}*/
