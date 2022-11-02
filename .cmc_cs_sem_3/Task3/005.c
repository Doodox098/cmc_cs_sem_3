#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *
dup_elem(struct Elem *head)
{
    struct Elem *old_head = head;
    while (head != NULL) {
        enum { BASIS = 10 };
        char *eptr = NULL;
        errno = 0;
        long int num = strtol(head->str, &eptr, BASIS);
        if (errno || *eptr || eptr == head->str || (int) num != num || num == INT_MAX) {
            head = head->next;
        } else {
            struct Elem *new = calloc(1, sizeof(*new));
            new->next = head->next;
            new->str = head->str;
            head->next = new;
            enum { INT_MAX_LENGTH = 12 };
            char *new_str = calloc(INT_MAX_LENGTH, sizeof(*new_str));
            sprintf(new_str, "%d", (int) (num + 1));
            head->str = new_str;
            head = new->next;
        }
    }
    return old_head;
}