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
int endTextYAbs = 0;
charNode *head;
int cursor = 0;


command commands[] = {
    {"ls", execLs},
    {"cd", execCd}
};
#define commandsSize (sizeof(commands)/sizeof(command))

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
    return i;
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
    int start = prints.bottomPtr;
    //printf("end text %d\n", endTextYAbs);
    for(int i = ((endTextYAbs < 33)? endTextYAbs : 33 ); i >= 0; i--)
    {
        printS(0,2+ i*7, 7, prints.lines[start]);
        start = arrIInside(start-1,maxConsoleLines);
    }
    
}
int i =0 ;

void conSetup()
{
    if(i==0)
    {
        //print("0\n>1\n>2\n>3\n4\n5\n6\n7\n");
        i++;
        
    }
    
}

void drawInWriting()
{
    static int countWrite = 0;
    static int lastCursor = 0;
    if(lastCursor != cursor)
    {
        countWrite = 0;
    }
    cstring str = charNodesToString(head);
    int endTextRel = ((endTextYAbs < 33)? endTextYAbs : 33 );
    printS(0, 2+endTextRel*7, 7, ">");
    printS(6, 2+endTextRel*7, 7, str);
    if(countWrite % 40 < 20)
    {
        drawRectFilled(5+cursor*6, 1+endTextRel*7, 7, 7, 7, 10);
        printC(6+cursor*6, 2+endTextRel*7, str[cursor], 0);
    }
    free(str);
    lastCursor = cursor;
    countWrite++;
}

int execLs(cstring str)
{
    print("\n");
    FilePathList list = LoadDirectoryFiles(".");
    for(int i = 0; i < list.count; i++)
    {
        bool isFile = IsPathFile(list.paths[i]);
        print("- ");
        if(!isFile) print("[");
        
        print(GetFileName(list.paths[i]));
        if(!isFile) print("]");
        print("\n");
    }
    UnloadDirectoryFiles(list);
    print("\n");
    return 1;
}
int execCd(cstring str)
{
    int i = 3;
    while(str[i]==' ' && str[i]!='\0')
    {
        i++;
    }
    strcpy(str, str+i);
    if(ChangeDirectory(str))
    {
        return 1;
    }
    else
    {
        print("Directory not found\n");
        return -1;
    }
    
}

int tryExecCode(cstring str)
{
    u16 strSize = strlen(str);
    for(int i = 0; i < commandsSize; i++)
    {
        u8 keywordSize = strlen(commands[i].keyWord);
        for(int j = 0; j < keywordSize; j++)
        {
            if(j == strSize) break;
            if(str[j] != commands[i].keyWord[j]) break;
            if(j == keywordSize-1)
            {
                return commands[i].exec(str);
            }
        }
    }
    return 0;
}

void detectWrite()
{
    int key =  GetKeyPressed();
    if(key == 257)
    {
        cstring str = charNodesToString(head);
        print(">");
        print(str);
        print("\n");
        if(str[0] == '\0')
        {
            free(str);
            return;
        }
        head = deleteAllCharNodes(head);
        cursor = 0;
        //endTextYAbs += 1;
        if(!tryExecCode(str))
        {
            print("Command not found\n");
        }
        free(str);
        return;
    }
    else if(key == KEY_LEFT)
    {
        cursor--;
        if(cursor < 0) cursor = 0;
        return;
    }
    else if(key == KEY_RIGHT)
    {
        u16 size = countCharNodes(head);
        cursor++;
        if(cursor > size) cursor = size;
        return;
    }
    else if(key == KEY_BACKSPACE)
    {
        head = deleteCharNode(head, cursor-1);
        cursor--;
        if(cursor < 0) cursor = 0;
        return;
    }
    else if(key == KEY_DELETE)
    {
        head = deleteCharNode(head, cursor);
        return;
    }
    else if(key == KEY_HOME)
    {
        cursor = 0;
        return;
    }
    else if(key == KEY_END)
    {
        cursor = countCharNodes(head);
        return;
    }

    unsigned char c = GetCharPressed();
    if(c>127) return;
    if(c== 0) return;
    head = insertCharNode(head, cursor, c);
    cursor++;
    
}



void consoleLoop()
{
    conSetup();
    detectWrite();
    drawPrints();
    drawInWriting();
}

void print(cstring str)
{
    printf(str);
    int i = 0;
    char lastChar = '\0';

    while(str[i] != '\0')
    {
        if(prints.bottomXPtr >= maxConsoleChars) 
        {
            prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\0';
            prints.bottomPtr += 1;
            endTextYAbs += 1;
            prints.bottomXPtr = 0;
        }
        if(prints.bottomPtr >= maxConsoleLines) prints.bottomPtr = 0;
        lastChar = str[i];
        if(str[i]=='\n')
        {
            i++;
            prints.bottomXPtr = 0;
            prints.bottomPtr +=1;
            if(prints.bottomPtr >= maxConsoleLines) prints.bottomPtr = 0;
            endTextYAbs += 1;
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

