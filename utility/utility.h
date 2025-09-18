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

typedef struct
{
    void *data;
    size_t size;
    size_t capacity;
    size_t elem_size;
} xarray;

void xarray_init(xarray *arr, size_t elem_size);
void xarray_push_back(xarray *arr, void *elem);
void xarray_pop_back(xarray *arr, void *out);
void xarray_free(xarray *arr);
bool xarray_empty(xarray *arr);