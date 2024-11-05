#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "my_list.h"

int main ()
{
    FILE* html_stream = prepare_html();
    if (html_stream == NULL)
        return -1;

    list_t lst = list_ctor();
    list_insert_after(&lst, 0, 100);
    list_insert_after(&lst, 0, 200);
    list_dump(&lst, html_stream);

    list_insert_after(&lst, 1, 300);
    list_insert_after(&lst, 3, 600);
    list_erase(&lst, 3);
    list_dump(&lst, html_stream);

    list_insert_after(&lst, 4, 7);
    list_insert_after(&lst, 4, 9);
    list_erase(&lst, 1);

    list_dump(&lst, html_stream);
    list_dtor(&lst);
    WinExec("dot data\\dump1.dot -Tpng -o data\\dump1.png", 1);

    fclose(html_stream);
    return 0;
}
