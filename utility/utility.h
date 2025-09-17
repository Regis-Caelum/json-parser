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
} Stack;

void stack_init(Stack *s, size_t elem_size);
void stack_push(Stack *s, void *elem);
void stack_pop(Stack *s, void *out);
void stack_top(Stack *s, void *out);
void stack_free(Stack *s);
bool stack_empty(Stack *s);