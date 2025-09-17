#include "utility.h"

void stack_init(stack *s, size_t elem_size)
{
    s->size = 0;
    s->capacity = 4;
    s->elem_size = elem_size;
    s->data = malloc(s->capacity * elem_size);
    if (!s->data)
    {
        perror("malloc failed");
        exit(1);
    }
}

void stack_push(stack *s, void *elem)
{
    if (s->size >= s->capacity)
    {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * s->elem_size);
        if (!s->data)
        {
            perror("realloc failed");
            exit(1);
        }
    }
    memcpy(s->data + s->size * s->elem_size, elem, s->elem_size);
    s->size++;
}

void stack_pop(stack *s, void *out)
{
    if (s->size == 0)
    {
        fprintf(stderr, "stack underflow\n");
        exit(1);
    }
    s->size--;
    memcpy(out, s->data + s->size * s->elem_size, s->elem_size);
}

void stack_top(stack *s, void *out)
{
    if (s->size == 0)
    {
        fprintf(stderr, "stack is empty\n");
        exit(1);
    }
    memcpy(out, s->data + (s->size - 1) * s->elem_size, s->elem_size);
}

bool stack_empty(stack *s)
{
    return s->size == 0;
}

void stack_free(stack *s)
{
    free(s->data);
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
    s->elem_size = 0;
}