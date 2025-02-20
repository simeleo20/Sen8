#include "console.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "core.h"

printMem prints;
charNode *head;
int cursor = 0;



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
int arrIInside(int i, int size)
{
    while (i<0)
    {
        i += size;
    }
    while (i>=size)
    {
        i -= size;
    }
     
    return i;
}

void drawPrints()
{
    int start = arrIInside(prints.bottomPtr - 32,maxConsoleLines);
    while(prints.lines[start][0]=='\0' && prints.bottomPtr!= start)
    {
        start = arrIInside(start+1,maxConsoleLines);
    }
    for(int i = 0; i < 33; i++)
    {
        printS(0,2+ i*7, 7, prints.lines[start]);
        start = arrIInside(start+1,maxConsoleLines);
    }
}
int i =0 ;

void conSetup()
{
    if(i==0)
    {
        //print("0\n>1\n>2\n>3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34");

        i++;
        
    }
    
}

void detectWrite()
{
    int key =  GetKeyPressed();

    char c = GetCharPressed();
    if(c== 0) return;
    char str[2] = {c,'\0'};
    //print(str);
}

void consoleLoop()
{
    conSetup();
    detectWrite();
    drawPrints();
}

void print(cstring str)
{
    int i = 0;
    char lastChar = '\0';
    while(str[i] != '\0')
    {
        if(prints.bottomXPtr >= maxConsoleChars) 
        {
            prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\0';
            prints.bottomPtr += 1;
            prints.bottomXPtr = 0;
        }
        if(prints.bottomPtr >= maxConsoleLines) prints.bottomPtr = 0;
        lastChar = str[i];
        if(str[i]=='\n')
        {
            i++;
            prints.bottomXPtr = 0;
            prints.bottomPtr +=1;
            prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\n';
            continue;
        }

        prints.lines[prints.bottomPtr][prints.bottomXPtr] = str[i];
        prints.bottomXPtr +=1;
        i++;
    }
    if(lastChar == '\n')
    {
        prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\n';
        prints.lines[prints.bottomPtr][prints.bottomXPtr+1] = '\0';
    }
    else{
        prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\0';
    }
}

