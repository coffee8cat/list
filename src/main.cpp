#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "my_list.h"

int main ()
{
    FILE* html_stream = prepare_to_dump();
    if (html_stream == NULL)
        return -1;

    list_t lst = list_ctor();
    /*
    list_insert_after(&lst, 0, 10);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 20);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 30);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 40);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 50);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 60);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 70);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 80);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 90);
    list_dump(&lst, html_stream);
    */

    /*
    list_insert_after(&lst, 0, 100);
    list_dump(&lst, html_stream);
    list_erase(&lst, 1);
    list_dump(&lst, html_stream);
    list_insert_after(&lst, 0, 44);
    list_dump(&lst, html_stream);
    */


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

    fclose(html_stream);
    return 0;
}
