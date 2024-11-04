#include "my_list.h"

#define NEXT(i) lst -> next[i]
#define PREV(i) lst -> prev[i]

list_t list_ctor()
{
    list_t lst = {};
    lst.data[0] = 0;

    lst.free  = 1;
    lst.capacity = 0;
    lst.next[0] = 0;
    lst.prev[0] = 1;

    for (size_t i = 1; i < max_list_size - 1; i++)
    {
        lst.next[i] = (int)i + 1;
    }
    lst.next[max_list_size - 1] = 0;

    return lst;
}

int list_dtor(list_t* lst)
{
    assert(lst);
    memset(lst, 0, sizeof(*lst));
    lst = NULL;

    return 0;
}

FILE* prepare_html()
{
    FILE* fp = fopen("list_dump.html", "w");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open html file fot dump\n");
        return NULL;
    }
    fprintf(fp, "<pre>\n");
    return fp;
}

int list_dump(list_t* lst, FILE* html_stream)
{
    static size_t dump_counter = 0;
    FILE* fp = fopen("dump1.dot", "w");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open file for dump\n");
        return -1;
    }

    make_dot_file(lst, fp);
    system("dot " "dump1" ".dot" " -Tpng -o " "dump1" ".png");
    printf("dot " "dump1" ".dot" " -Tpng -o " "dump1" ".png" "\n");

    size_t i = 0;

    fprintf(html_stream, "MEMORY PRINT\n");
    fprintf(html_stream, "curr data next prev\n");
    while (i != lst -> prev[0])
    {
        fprintf(html_stream, "%4d %4d %4d %4d\n", i, lst -> data[i], lst -> next[i], lst -> prev[i]);
        i = NEXT(i);
    }
    fprintf(html_stream, "%4d %4d %4d %4d\n", i, lst -> data[i], lst -> next[i], lst -> prev[i]);
    fprintf(html_stream, "<img src=\"dump1.png\">\n");

    fprintf(stdout, "MEMORY PRINT\n");
    fprintf(stdout, "curr data next prev\n");
    for (size_t i = 0; i < max_list_size; i++)
    {
        printf("%4d %4d %4d %4d\n", i, lst -> data[i], lst -> next[i], lst -> prev[i]);
    }
    printf("%4d %4d %4d %4d\n", i, lst -> data[i], lst -> next[i], lst -> prev[i]);

    i = 0;
    fprintf(stdout, "curr data next prev\n");
    while (i != lst -> prev[0])
    {
        fprintf(stdout, "%4d %4d %4d %4d\n", i, lst -> data[i], lst -> next[i], lst -> prev[i]);
        i = NEXT(i);
    }

    printf("%4d %4d %4d %4d\n", i, lst -> data[i], lst -> next[i], lst -> prev[i]);

    dump_counter++;
    return 0;
};

int make_dot_file(list_t* lst, FILE* fp)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "digraph\n{\n   rankdir=LR\n    ");

    size_t i = 0;
    size_t j = 0;
    while (i != lst -> prev[0])
    {
        fprintf(fp, "\"%d\" -> ", j);
        i = NEXT(i);
        j++;
    }
    fprintf(fp, "\"%d\";\n", j);

    i = 0;
    j = 0;
    while (i != lst -> prev[0])
    {
        fprintf(fp, "    node%d[shape=record,label=\"index: %d | data: %d | next: %d | prev: %d\"];\n"
                    "    {rank = same; \"%d\"; node%d}\n",
                    j, i, lst -> data[i], lst -> next[i], lst -> prev[i], j, j);
        i = NEXT(i);
        j++;
    }
    fprintf(fp, "    node%d[shape=record,label=\"index: %d | data: %d | next: %d | prev: %d\"];\n"
                "    {rank = same; \"node%d\"; node%d}\n",
                j, i, lst -> data[i], lst -> next[i], lst -> prev[i], j, j);

    i = 0;
    j = 0;
    fprintf(fp, "    ");
    while (i != lst -> prev[0])
    {
        fprintf(fp, "node%d -> ", j);
        j++;
        i = NEXT(i);
    }
    fprintf(fp, "node%d;\n", j);

    fprintf(fp, "}");

    return 0;
}

inline int* list_next(list_t* lst, size_t i)
{
    assert(lst);
    return &lst -> data[lst -> next[i]];
}

inline int* list_prev(list_t* lst, size_t i)
{
    assert(lst);
    return &lst -> data[lst -> prev[i]];
}

inline int* list_get_front(list_t* lst)
{
    assert(lst);
    return &lst -> data[lst -> next[0]];
}

inline int* list_get_back(list_t* lst)
{
    assert(lst);
    return &lst -> data[lst -> prev[0]];
}

int list_push_back(list_t* lst, int elem)
{
    assert(lst);

    size_t curr = lst -> free;
    lst -> free = NEXT(curr);

    lst -> data[curr] = elem;
    lst -> next[curr] = 0;

    lst -> next[lst -> prev[0]] = curr;
    lst -> prev[curr] = lst -> prev[0];
    lst -> prev[0] = curr;

    return 0;
}

int list_insert_after(list_t* lst, size_t i, int elem)
{
    assert(lst);

    size_t curr = lst -> free;
    lst -> free = NEXT(curr);
    lst -> data[curr] = elem;

    lst -> prev[NEXT(i)] = curr;
    lst -> next[curr] = NEXT(i);

    lst -> prev[curr] = i;
    lst -> next[i] = curr;

    return 0;
}

int list_erase(list_t* lst, size_t i)
{
    assert(lst);

    lst -> prev[NEXT(i)] = lst -> prev[i];
    lst -> next[PREV(i)] = lst -> next[i];

    lst -> prev[i] = -1;
    lst -> next[i] = lst -> free;
    lst -> free = i;

    return 0;
}

#undef NEXT
#undef PREV
