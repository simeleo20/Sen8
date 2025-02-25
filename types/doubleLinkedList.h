#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include "flexString.h"

typedef struct doubleLinkedList
{
    void *data;
    struct doubleLinkedList *prev;
    struct doubleLinkedList *next;
} doubleLinkedList;


doubleLinkedList *newDoubleLinkedList(void *data);
doubleLinkedList *scrollDoubleLinkedList(doubleLinkedList *node, int scroll);
doubleLinkedList *insertBeforeDoubleLinkedList(doubleLinkedList *head, doubleLinkedList *node, doubleLinkedList *newNode);
void insertAfterDoubleLinkedList(doubleLinkedList *node, doubleLinkedList *newNode);
doubleLinkedList *removeDoubleLinkedList(doubleLinkedList *head, doubleLinkedList *node);
doubleLinkedList *removePrevDoubleLinkedList(doubleLinkedList *head, doubleLinkedList *node);
doubleLinkedList *removeNextDoubleLinkedList(doubleLinkedList *head, doubleLinkedList *node);
void removeAllDoubleLinkedList(doubleLinkedList *head);
void printDoubleLinkedListLine(doubleLinkedList *head);

#endif // DOUBLE_LINKED_LIST_H