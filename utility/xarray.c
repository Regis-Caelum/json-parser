#include "utility.h"

void xarray_init(xarray *arr, size_t elem_size)
{
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    arr->elem_size = elem_size;
}

void xarray_push_back(xarray *arr, void *elem)
{
    if (arr->size == arr->capacity)
    {
        size_t new_capacity = arr->capacity == 0 ? 4 : arr->capacity * 2;
        void *new_data = realloc(arr->data, new_capacity * arr->elem_size);
        if (!new_data)
        {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
        arr->data = new_data;
        arr->capacity = new_capacity;
    }

    memcpy((char *)arr->data + arr->size * arr->elem_size, elem, arr->elem_size);
    arr->size++;
}

void xarray_pop_back(xarray *arr, void *out)
{
    if (arr->size == 0)
    {
        fprintf(stderr, "xarray_pop_back: array is empty\n");
        return;
    }

    arr->size--;
    if (out)
    {
        memcpy(out, (char *)arr->data + arr->size * arr->elem_size, arr->elem_size);
    }
}

void xarray_free(xarray *arr)
{
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    arr->elem_size = 0;
}

bool xarray_empty(xarray *arr)
{
    return arr->size == 0;
}