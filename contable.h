/**
  ******************************************************************************
  * @file    contable.h
 *  @brief   Console tables API.
 *
 * Usage Example:

int main()
{
    con_table tbl;
    contable_init(&tbl);

    contable_add_column(&tbl, "col a", contable_decimal,   10); // Add decimal column
    contable_add_column(&tbl, "col b", contable_string,    16); // Add string column
    contable_add_column(&tbl, "col c", contable_decimal,   12); // Add decimal column

    contable_print(&tbl, "%d%s%d", 1, "string a", 10);
    contable_print(&tbl, "%d%s%d", 2, "string b", 20);
    contable_print(&tbl, "%d%s%d", 3, "string c", 30);

    contable_destroy(&tbl,true);

    return 0;
}

Copyright (c) 2020, Eitan Michaelson
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef CON_TABLE_H_
#define CON_TABLE_H_

#ifndef NULL
#define NULL ((void *)0)
#endif

#include <stdbool.h>

  // How to represent the data in the column
typedef enum
{
    contable_decimal,
    contable_hex,
    contable_string,

}con_col_type;

// Link list node that will be allocated for each column of our table.
typedef struct con_col_t
{
    char              name[32];
    int               width;
    con_col_type      type;
    struct con_col_t* next; // Next linked element.

} con_col;


// Table object associated with each unique table we draw.
typedef struct con_table_t
{

    bool     columnsAdded;
    int      totalWidh;
    con_col* columns; // The head of our columns list.

} con_table;

/**************************************************************************/ /**
 * @brief
 *  Add a column to the table.
 * @return bool.
 *
 *****************************************************************************/

bool contable_add_column(con_table* tbl, char* name, con_col_type type, int width);

/**************************************************************************/ /**
 * @brief
 *  Print a table raw separator.
 * @return void.
 *
 *****************************************************************************/

void contable_print_separator(con_table* tbl);

/**************************************************************************/ /**
 * @brief
 *   Adds a blank line to the table.
 * @return void.
 *
 *****************************************************************************/

void contable_print_blank(con_table* tbl);

/**************************************************************************/ /**
 * @brief
 *  Print table raw, 'printf' style.
 *  Eeach '%' arg will be positioned accroding to the columns we've added.
 *  The first call to this function blocks the option to add more columns.
 * @return void.
 *
 *****************************************************************************/

int contable_print(con_table* tbl, const char* _format, ...);

/**************************************************************************/ /**
 * @brief
 *  Destroy a table object and free it's internal columns list.
 * @return void.
 *
 *****************************************************************************/

void contable_destroy(con_table* tbl, bool separator);

/**************************************************************************/ /**
 * @brief
 *  Initializes a new table object.
 * @return void.
 *
 *****************************************************************************/

void contable_init(con_table* tbl);

#endif // CON_TABLE_H_
