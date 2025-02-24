#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include "flexString.h"

typedef struct doubleLinkedList
{
    void *data;
    struct doubleLinkedList *prev;
    struct doubleLinkedList *next;
} doubleLinkedList;

#endif // DOUBLE_LINKED_LIST_H