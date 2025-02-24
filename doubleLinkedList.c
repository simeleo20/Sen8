#include <stdlib.h>
#include <stdio.h>
#include "doubleLinkedList.h"

//alloca spazio e inizializza il nodo
doubleLinkedList *newDoubleLinkedList(void *data)
{
    doubleLinkedList *out = malloc(sizeof(doubleLinkedList));
    out->data = data;
    return out;
}

doubleLinkedList *scrollDoubleLinkedList(doubleLinkedList *node, int scroll)
{
    doubleLinkedList *current = node;
    for(int i = 0; i < scroll; i++)
    {
        if(current->next == NULL) return node;
        current = current->next;
    }
    for(int i = 0; i > scroll; i--)
    {
        if(current->prev == NULL) return node;
        current = current->prev;
    }
    return current;
}
// aggiunge un nodo prima di quello selezionato e ritorna la nuova testa
doubleLinkedList *insertBeforeDoubleLinkedList(doubleLinkedList *head,doubleLinkedList *node, doubleLinkedList *newNode)
{
    if(node->prev == NULL)
    {
        newNode->next = node;
        node->prev = newNode;
        return newNode;
    }
    newNode->prev = node->prev;
    newNode->next = node;
    node->prev->next = newNode;
    node->prev = newNode;
    return head;
}

// aggiunge un nodo dopo di quello selezionato
void insertAfterDoubleLinkedList(doubleLinkedList *node, doubleLinkedList *newNode)
{
    if(node->next != NULL)
    {
        node->next->prev = newNode;
    }
    newNode->next = node->next;

    newNode->prev = node;
    node->next = newNode;
}

doubleLinkedList *removeDoubleLinkedList(doubleLinkedList *head, doubleLinkedList *node)
{
    if(node == NULL)
    {
        printf("trying to remove inexistent node\n");
        return head;
    }
    if(head==node)
    {
        doubleLinkedList *next = node->next;
        next->prev = NULL;
        free(node->data);
        free(node);
        return next;
    }
    if(node->next!=NULL)
    {
        node->next->prev = node->prev;
    }
    
    node->prev->next = node->next;
    free(node->data);
    free(node);
    return head;
}
doubleLinkedList *removePrevDoubleLinkedList(doubleLinkedList *head, doubleLinkedList *node)
{
    doubleLinkedList *cursor = node->prev;
    return removeDoubleLinkedList(head,cursor);
}
doubleLinkedList *removeNextDoubleLinkedList(doubleLinkedList *head, doubleLinkedList *node)
{
    doubleLinkedList *cursor = node->next;
    return removeDoubleLinkedList(head,cursor);
}

void printDoubleLinkedListLine(doubleLinkedList *head)
{
    doubleLinkedList *current = head;
    while(current != NULL)
    {
        printf("l:%s\n", ((flexString*)current->data)->string);
        current = current->next;
    }
    printf("\n");
}