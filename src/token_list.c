#include "token_list.h"

token_list_node* token_list_append(token_list_node* head, const char* data, int length)
{
    if(head == NULL)
    {
        head = (token_list_node *) malloc(sizeof(token_list_node));
        head->data = data;
        head->length = length;
        head->next = NULL;
        return head;
    }

    token_list_node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;

    token_list_node* n = (token_list_node *) malloc(sizeof(token_list_node));
    n->data = data;
    n->length = length;
    n->next = NULL;
    cursor->next = n;
    return n;
}

token_list_node* token_list_get(token_list* head, int index)
{
    token_list_node *cursor = head;
    int i = 0;
    while(cursor != NULL)
    {
        if(i == index)
            return cursor;
        cursor = cursor->next;
        i++;
    }
    return NULL;
}

int token_list_count(token_list* head)
{
    token_list_node *cursor = head;
    int i = 0;
    while(cursor != NULL)
    {
        i++;
        cursor = cursor->next;
    }
    return i;
}

void token_list_destroy(token_list* head)
{
    if(head != NULL)
    {
        token_list_node* cursor = head->next;
        token_list_node* tmp = NULL;
        while(cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
        free(head);
    }
}

void token_list_print(token_list* head)
{
    token_list_node* cursor = head;
    while(cursor != NULL)
    {
        printf("- %s (len=%d)\tself = %p\tnext = %p\n", cursor->data, cursor->length, cursor, cursor->next);
        cursor = cursor->next;
    }
}