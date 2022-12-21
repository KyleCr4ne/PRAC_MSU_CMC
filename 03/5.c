#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *
insert(struct Elem *current, struct Elem *previous, struct Elem *head, long long int x)
{
    struct Elem *new = calloc(1, sizeof(*current));
    if (head == current){
        head = new;
        head->next = current;
    }
    else{
        new->next = current;
        previous->next = new;
    }
    new->str = calloc(strlen(current->str) + 1, sizeof(current->str[0]));
    sprintf(new->str, "%d", (int)x + 1);
    return head;
}

struct Elem *
dup_elem(struct Elem *head)
{
    if (head == NULL)
        return head;
    struct Elem *current = head;
    struct Elem *previous = NULL;
    while (current != NULL) {
        char *end_ptr = NULL;
        long long int x = strtol(current->str, &end_ptr, 10);
        if ((*end_ptr == 0) && (end_ptr != current->str) && (int) x == x && x != (1U << 31) - 1) {
            head = insert(current, previous, head, x);
        }
        previous = current;
        current = current->next;

    }
    return head;
}
