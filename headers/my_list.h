#ifndef _LIST_DATA_STRUCTURE_H__
#define _LIST_DATA_STRUCTURE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const size_t max_list_size = 8;
typedef int list_data_t;

struct list_t
{
    int    data[max_list_size];
    size_t next[max_list_size];
    int    prev[max_list_size];
    size_t free;
    size_t capacity;
};

list_t list_ctor();
int list_dtor(list_t* lst);

FILE* prepare_html();
int list_dump(list_t* lst, FILE* stream);
int make_dot_file(list_t* lst, FILE* fp);

inline int* list_next(list_t* lst, size_t i);
inline int* list_prev(list_t* lst, size_t i);
inline int* list_get_front(list_t* lst);
inline int* list_get_back(list_t* lst);

int list_push_back(list_t* lst, int elem);
int list_insert_after(list_t* lst, size_t i, int elem);
int list_erase(list_t* lst, size_t i);

#endif
