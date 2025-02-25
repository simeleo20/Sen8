#include "console.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "core.h"
#include <unistd.h>
#include "editor/scriptEditor.h"

printMem prints;
int endTextYAbs = 0;
charNode *head;
int cursor = 0;
extern core cCore;

#ifdef _WIN32
    #define getUserFolder getenv("APPDATA")
    #define slash "\\"
#elif __linux__
    #define getUserFolder getenv("HOME")
    #define slash "/"
#elif __APPLE__
    #define getUserFolder getenv("HOME")
    #define slash "/"
#elif __ANDROID__
    #define getUserFolder getenv("HOME")
    #define slash "/"
#elif PLATFORM_WEB
    #define getUserFolder "/"
    #define slash "/"
#endif

#define appFolderName "Sen8"

char* getBaseFolder() {
    char* baseFolder = malloc(strlen(getUserFolder) + strlen(slash appFolderName) + 1);
    if (baseFolder) {
        strcpy(baseFolder, getUserFolder);
        strcat(baseFolder, slash appFolderName);
    }
    return baseFolder;
}


string baseFolder;
string localPath = "";



command commands[] = {
    {"ls", execLs},
    {"cd", execCd},
    {"mk", execMk},
    {"load", execLoad},
    {"save", execSave},
    {"run", execRun}
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

string charNodesToString(charNode *head)
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
    int start = arrIInside(prints.bottomPtr,maxConsoleLines);
    //printf("end text %d\n", endTextYAbs);
    for(int i = ((endTextYAbs < 32)? endTextYAbs : 32 ); i >= 0; i--)
    {
        printS(0,2+ i*7, 7, prints.lines[start]);
        start = arrIInside(start-1,maxConsoleLines);
    }
    int i = ((endTextYAbs < 33)? endTextYAbs : 33 );
    printC(0,2+ i*7, '>', 7, 10);
    
}
int i =0 ;

void conSetup()
{
    if(i==0)
    {
        baseFolder = getBaseFolder();
        printf(GetWorkingDirectory());
        printf("\n");
        printf(getUserFolder);
        if(ChangeDirectory(getUserFolder))
        {
            printf("Directory changed\n");
        }
        else
        {
            printf("Directory not found\n");
        }
        if(DirectoryExists(appFolderName))
        {
            printf("Directory exists\n");
        }
        else
        {
            printf("Directory not found\n");
            printf("Creating directory\n");
            MakeDirectory(appFolderName);
            printf("base folder: %s\n", baseFolder);
            
        }
        ChangeDirectory(appFolderName);
        printf("working directory: %s\n", GetWorkingDirectory());

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
    string str = charNodesToString(head);
    int endTextRel = ((endTextYAbs < 33)? endTextYAbs : 33 );
    int localPathLen = strlen(localPath);
    printS((localPathLen*6)+6, 2+endTextRel*7, 7, str);
    if(countWrite % 40 < 20)
    {
        
        drawRectFilled((localPathLen*6)+5+cursor*6, 1+endTextRel*7, 7, 7, 7, 10);
        printC((localPathLen*6)+6+cursor*6, 2+endTextRel*7, str[cursor], 0, 20);
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

    //strcpy(str, str+i);
    
    if(ChangeDirectory(str+i))
    {

        if(memcmp(GetWorkingDirectory(), baseFolder, strlen(baseFolder)) != 0)
        {
            ChangeDirectory(baseFolder);
            return 1;
        }

        

        return 1;
    }
    else
    {
        print("Directory not found\n");
        return -1;
    }
    
}
int execMk(cstring str)
{
    int i = 3;
    while(str[i]==' ' && str[i]!='\0')
    {
        i++;
    }
    //strcpy(str, str+i);
    if(MakeDirectory(str+i))
    {
        print("Directory not created\n");
        return -1;
    }
    else
    {
        
        print("Directory created\n");
        return 1;
    }
}

int execLoad(cstring str)
{
    int i = 5;
    while(str[i]==' ' && str[i]!='\0')
    {
        i++;
    }
    //strcpy(str, str+i);
    bool found = false;
    cstring ext = GetFileExtension(str+i);
    string newStr = (string)str;
    if(ext == NULL)
    {
        FilePathList list = LoadDirectoryFiles(".");
        s32 strLen = strlen(newStr+i);
        print("Searching file without extension: %s\n", newStr+i);
        for(int j = 0; j < list.count; j++)
        {
            
            if(strncmp(GetFileName(list.paths[j]), newStr+i, strLen) == 0)
            {
                newStr = malloc(i+strlen(GetFileName(list.paths[j]))+1);
                strcpy(newStr, str);
                strcpy(newStr+i ,GetFileName(list.paths[j]));
                ext = GetFileExtension(newStr);
                found = true;
                print("File found\n");
                break;
            }
        }
        if(!found)
        {
            print("File not found\n");
            return -1;
        }
        UnloadDirectoryFiles(list);
    }
    if(strcmp(ext, ".sen") == 0)
    {

        cstring fileChars = LoadFileText(newStr+i);
        if(fileChars == NULL)
        {
            print("File not found\n");
            return -1;
        }

        print("loaded sen file\n");
        loadSenString(fileChars);
        return 1;
    }
    else if(strcmp(ext, ".bin")==0||strcmp(ext, ".cart")==0)
    {
        int size;
        unsigned char *data = LoadFileData(newStr+i, &size);

        if(data == NULL)
        {
            print("File not found\n");
            printf("file: %s\n", newStr+i);
            return -1;
        }

        cart cartridge;
        memcpy(&cartridge, data, sizeof(cart)-sizeof(cstring));
        string script = malloc(size - (sizeof(cart)-sizeof(cstring)));
        memcpy(script, data+sizeof(cart)-sizeof(cstring), size - (sizeof(cart)-sizeof(cstring)));
        cartridge.script = script;

        loadCart(&cartridge);
        free(data);
        loadScriptFromRam();
        print("loaded cartridge\n");
        return 1;
    }
    print("format not recognized\n");
    return -1;
}

int execSave(cstring str)
{
    int i = 5;
    while(str[i]==' ' && str[i]!='\0')
    {
        i++;
    }
    //strcpy(str, str+i);
    
    
    cart cartridge;
    cartridge.language = cCore.ram.language;
    
    int scriptSize=strlen(cCore.ram.script)+1;
    cartridge.script = cCore.ram.script;
    
    memcpy(cartridge.spritesTileMem, cCore.ram.spritesTileMem, sizeof(cCore.ram.spritesTileMem));
    memcpy(cartridge.bgTilesMem, cCore.ram.bgTilesMem, sizeof(cCore.ram.bgTilesMem));
    
    unsigned char *data = malloc(scriptSize + sizeof(cart)-sizeof(cstring));
    memcpy(data, &cartridge, sizeof(cart)-sizeof(cstring));
    memcpy(data+sizeof(cart)-sizeof(cstring), cartridge.script, scriptSize);
    SaveFileData(str+i, data, scriptSize + sizeof(cart)-sizeof(cstring));
    free(data);
    print("saved\n");
}
int execRun(cstring str)
{
    startRunning();
    return 1;
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
    if(IsKeyPressed(257) || IsKeyPressedRepeat(257))
    {
        string str = charNodesToString(head);
        print("\n");
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
            print("Command not found \n");
        }
        free(str);
        if(strlen(GetWorkingDirectory())> strlen(baseFolder))
        {
            localPath = malloc(strlen(GetWorkingDirectory()) - strlen(baseFolder) + 1);
            strcpy(localPath, GetWorkingDirectory() + strlen(baseFolder)+1);
            print(localPath);
        }
        else
        {
            localPath = "";
        }
        
        return;
    }
    else if(IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))
    {
        cursor--;
        if(cursor < 0) cursor = 0;
        return;
    }
    else if(IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))
    {
        u16 size = countCharNodes(head);
        cursor++;
        if(cursor > size) cursor = size;
        return;
    }
    else if(IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
    {
        head = deleteCharNode(head, cursor-1);
        cursor--;
        if(cursor < 0) cursor = 0;
        return;
    }
    else if(IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE))
    {
        head = deleteCharNode(head, cursor);
        return;
    }
    else if(IsKeyPressed(KEY_HOME) || IsKeyPressedRepeat(KEY_HOME))
    {
        cursor = 0;
        return;
    }
    else if(IsKeyPressed(KEY_END) || IsKeyPressedRepeat(KEY_END))
    {
        cursor = countCharNodes(head);
        return;
    }
    else if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        if(IsKeyPressed(KEY_C))
        {
            string str = charNodesToString(head);
            SetClipboardText(str);
            free(str);
            return;
        }
        else if(IsKeyPressed(KEY_V))
        {
            printf("v\n");
            cstring str = GetClipboardText();
            for(int i = 0; i < strlen(str); i++)
            {
                printf("%c\n", str[i]);
                head = insertCharNode(head, cursor, str[i]);
                cursor++;
            }
            return;
        }
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

void print(cstring str, ...)
{
    va_list args;
    va_start(args, str);
    int needed = vsnprintf(NULL, 0, str, args) + 1;
    va_end(args);

    char *buffer = malloc(needed);
    if (!buffer) {
        return;
    }

    va_start(args, str);
    vsnprintf(buffer, needed, str, args);
    va_end(args);

    printf("%s", buffer);
    int i = 0;
    char lastChar = '\0';

    while(buffer[i] != '\0')
    {
        if(prints.bottomXPtr >= maxConsoleChars) 
        {
            prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\0';
            prints.bottomPtr += 1;
            endTextYAbs += 1;
            prints.bottomXPtr = 0;
        }
        if(prints.bottomPtr >= maxConsoleLines) prints.bottomPtr = 0;
        lastChar = buffer[i];
        if(buffer[i]=='\n')
        {
            i++;
            prints.lines[prints.bottomPtr][prints.bottomXPtr] = '\n';
            prints.lines[prints.bottomPtr][prints.bottomXPtr+1] = '\0';
            prints.bottomXPtr = 0;
            prints.bottomPtr +=1;
            if(prints.bottomPtr >= maxConsoleLines) prints.bottomPtr = 0;
            endTextYAbs += 1;
            continue;
        }

        prints.lines[prints.bottomPtr][prints.bottomXPtr] = buffer[i];
        //printf("c%d ", buffer[i]);
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
    free(buffer);
}

void vprint(cstring str,va_list args)
{
    int needed = vsnprintf(NULL, 0, str, args) + 1;
    char *buffer = malloc(needed);
    if (!buffer) {
        return;
    }
    vsnprintf(buffer, needed, str, args);
    print("%s", buffer);
    free(buffer);
}