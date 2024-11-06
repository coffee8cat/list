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
    lst.prev[0] = lst.free;

    for (size_t i = 1; i < max_list_size - 1; i++)
    {
        lst.next[i] = (int)i + 1;
        lst.prev[i] = -1;
    }
    lst.prev[max_list_size - 1] = -1;
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

int list_verify(list_t* lst)
{
    assert(lst);

    int i = 0;
    while (i != PREV(0))
    {
        if (PREV(NEXT(i)) != i || NEXT(PREV(i)) != (size_t)i)
        {
            fprintf(stderr, "VERIFICATION FAILED ON NODE %d\n", i);
            return i;
        }
        i = NEXT(i);
    }

    /*i = NEXT(lst -> free);
    while (NEXT(i) != 0)
    {
        if (PREV(NEXT(i)) != i || NEXT(PREV(i)) != (size_t)i)
        {
            fprintf(stderr, "VERIFICATION FAILED ON NODE %d\n", i);
            return i;
        }
        i = NEXT(i);
    }*/

    return 0;
}

FILE* prepare_to_dump()
{
    system("make clean_data\n");
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
    static size_t dump_counter = 1;
    char dot_file_name[BUFSIZ] = "";
    char png_file_name[BUFSIZ] = "";
    sprintf(dot_file_name, "data\\dump%d.dot", dump_counter);
    sprintf(png_file_name, "data\\dump%d.png", dump_counter);

    FILE* fp = fopen(dot_file_name, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open file for dump\n");
        return -1;
    }

    make_dot_file(lst, fp);
    fclose(fp);

    char command[BUFSIZ] = "";
    sprintf(command, "dot %s -Tpng -o %s\n", dot_file_name, png_file_name);
    system(command);

    dump_to_html(lst, png_file_name, html_stream);

    dump_counter++;
    return 0;
};

int make_dot_file(list_t* lst, FILE* fp)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "digraph\n{\n"
                "    rankdir=LR\n\n"
                "    ");

    for (size_t k = 0; k < max_list_size - 1; k++)
    {
        fprintf(fp, "\"%d\" -> ", k);
    }
    fprintf(fp, "\"%d\";\n\n", max_list_size - 1);

    int i = lst -> free;
    while (i != 0)
    {
        fprintf(fp, "    node%d[shape=record,style=\"rounded,filled\",fillcolor=\"#39CCCC\","
                    "label=\"index: %d | data: %d | next: %d | prev: %d\"];\n",
                    i, i, lst -> data[i], NEXT(i), PREV(i));
        i = NEXT(i);
    }


    fprintf(fp, "    node%d[shape=record,style=\"rounded,filled\",fillcolor=\"#BE08F0\","
                "label=\"index: %d | data: %d | next: %d | prev: %d\"];\n",
                0, 0, lst -> data[0], NEXT(0), PREV(0));
    i = NEXT(0);
    while (i != 0)
    {
        fprintf(fp, "    node%d[shape=record,style=\"rounded,filled\",fillcolor=\"#2ECC40\","
                    "label=\"index: %d | data: %d | next: %d | prev: %d\"];\n",
                    i, i, lst -> data[i], NEXT(i), PREV(i));
        i = NEXT(i);
    }

    fprintf(fp, "\n");
    for (size_t k = 0; k < max_list_size - 1; k++)
    {
        fprintf(fp, "    node%d -> node%d[color=\"none\",penwidth=100000000]\n"
                    "    {rank = same; \"%d\"; node%d}\n", k, k + 1, k, k);
    }
    fprintf(fp, "    {rank = same; \"%d\"; node%d}\n\n", max_list_size - 1, max_list_size - 1);

    i = 0;

    while (i != PREV(0) && NEXT(0) != 0) //last case differ
    {
        fprintf(fp, "    node%d -> node%d[color=\"#0855F0\",constraint=false]\n", i, NEXT(i));
        fprintf(fp, "    node%d -> node%d[color=\"#F00822\",constraint=false]\n", NEXT(i), i);
        i = NEXT(i);
    }
    fprintf(fp, "    node%d -> node0[color=\"#0855F0\",constraint=false]\n\n", i);
    fprintf(fp, "    node%d -> node%d[color=\"#F00822\",constraint=false]\n", NEXT(i), i);

    i = lst -> free;
    while (i != 0)
    {
        fprintf(fp, "    node%d -> node%d[color=\"#0855F0\",constraint=false]\n", i, NEXT(i));
        i = NEXT(i);
    }

    fprintf(fp, "}");

    printf("dot file completed\n");
    return 0;
}

int dump_to_html(list_t* lst, char* png_file_name, FILE* html_stream)
{
    assert(lst);
    assert(png_file_name);

    fprintf(html_stream, "MEMORY PRINT\n");
    fprintf(html_stream, "curr data next prev\n");
    for (size_t i = 0; i < max_list_size; i++)
    {
        fprintf(html_stream, "%4d %4d %4d %4d\n", i, lst -> data[i], lst -> next[i], lst -> prev[i]);
    }
    fprintf(html_stream, "<img src=%s>\n", png_file_name);

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

    if (lst -> prev[i] == -1)
    {
        fprintf(stderr, "ERROR: Cannot insert after free element\n");
        return -1;
    }
    if (lst -> free == 0)
    {
        fprintf(stderr, "ERROR: List is full, unable to insert element\n");
        return -2;
    }

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
    if (lst -> prev[i] == -1)
    {
        fprintf(stderr, "ERROR: Cannot erase free element\n");
        return -1;
    }

    lst -> prev[NEXT(i)] = lst -> prev[i];
    lst -> next[PREV(i)] = lst -> next[i];

    lst -> prev[i] = -1;
    lst -> next[i] = lst -> free;
    //lst -> prev[lst -> free] = i;
    lst -> free = i;

    if (lst -> prev[0] == 0)
    {
        lst -> prev[0] = lst -> free;
    }
    return 0;
}

#undef NEXT
#undef PREV
