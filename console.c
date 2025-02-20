#include "console.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

printMem prints;
charNode *head;
int cursor = 0;

void drawPrints()
{
    for(int i = 0; i < maxConsoleLines; i++)
    {
        printf("%d: %s\n", i,prints.lines[i]);
    }
}

charNode* newCharNode(char c)
{
    charNode *node = malloc(sizeof(charNode));
    node->c = c;
    node->next = NULL;
    return node;
}
charNode* deleteAllCharNodes(charNode *head)
{
    charNode *current = head;
    charNode *next;
    while(current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
    return head;
}
charNode* deleteCharNode(charNode *head, int index)
{
    charNode *current = head;
    charNode *prev = NULL;
    int i = 0;
    while(current != NULL && i < index)
    {
        prev = current;
        current = current->next;
        i++;
    }
    if(current == NULL) return head;
    if(prev == NULL)
    {
        head = current->next;
    }
    else
    {
        prev->next = current->next;
    }
    free(current);
    return head;
}
charNode* insertCharNode(charNode *head, int index, char c)
{
    charNode *node = newCharNode(c);
    charNode *current = head;
    charNode *prev = NULL;
    int i = 0;
    while(current != NULL && i < index)
    {
        prev = current;
        current = current->next;
        i++;
    }
    if(prev == NULL)
    {
        node->next = head;
        head = node;
    }
    else
    {
        prev->next = node;
        node->next = current;
    }
    return head;
}
void printCharNodes(charNode *head)
{
    charNode *current = head;
    while(current != NULL)
    {
        printf("%c", current->c);
        current = current->next;
    }
    printf("\n");
}
u16 countCharNodes(charNode *head)
{
    charNode *current = head;
    int i = 0;
    while(current != NULL)
    {
        i++;
        current = current->next;
    }
}

cstring charNodesToString(charNode *head)
{
    u16 size = countCharNodes(head);
    string str = malloc(size+1);
    charNode *current = head;
    int i = 0;
    while(current != NULL)
    {
        str[i] = current->c;
        i++;
        current = current->next;
    }
    str[i] = '\0';
    return str;
}

int main()
{
    while(true)
    {
        char c = getch();
        if(c=='a')
        {
            cursor--;
            if(cursor < 0) cursor = 0;
            continue;
        }
        else if(c=='d')
        {
            cursor++;
            continue;
        }
        else if(c=='w')
        {
            if(cursor == 0) continue;
            head = deleteCharNode(head, cursor-1);
            cursor--;
            printCharNodes(head);
            continue;
        }
        else if(c==3)
        {
            break;
        }
        else if(c=='s')
        {
            head=deleteAllCharNodes(head);
            cursor = 0;
            printCharNodes(head);
            continue;
        }
        head = insertCharNode(head, cursor, c);
        cursor++;
        printCharNodes(head);
    }

    return 0;
}

void print(cstring str)
{
    int i = 0;
    while(str[i] != '\0')
    {
        if(prints.bottomXPtr >= maxConsoleChars) 
        {
            prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\0';
            prints.bottomPtr += 1;
            prints.bottomXPtr = 0;
        }
        if(prints.bottomPtr >= maxConsoleLines) prints.bottomPtr = 0;

        if(str[i]=='\n')
        {
            i++;
            prints.bottomXPtr = 0;
            prints.bottomPtr +=1;
            continue;
        }

        prints.lines[prints.bottomPtr][prints.bottomXPtr] = str[i];
        prints.bottomXPtr +=1;
        i++;
    }
    prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\0';
}

