#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char *read_file(const char *filename);

typedef struct
{
    void *data;
    size_t size;
    size_t capacity;
    size_t elem_size;
} stack;

void stack_init(stack *s, size_t elem_size);
void stack_push(stack *s, void *elem);
void stack_pop(stack *s, void *out);
void stack_top(stack *s, void *out);
void stack_free(stack *s);
bool stack_empty(stack *s);