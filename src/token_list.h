#ifndef TOKEN_LIST_H_
#define TOKEN_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct list_node {
    struct list_node *next;
    const char* data;
    int length;
} token_list_node;

typedef token_list_node token_list;

token_list_node* token_list_append(token_list_node* head, const char* data, int length);
token_list_node* token_list_get(token_list* head, int index);
int token_list_count(token_list* head);
void token_list_destroy(token_list* head);
void token_list_print(token_list* head);


#endif