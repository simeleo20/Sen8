#include "scriptEditor.h"
#include "../types/types.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "../core.h"
#include "editor.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
EM_ASYNC_JS(char*, getClipboardText, (), {
    var text = await navigator.clipboard.readText();
    var lengthBytes = lengthBytesUTF8(text) + 1;
    var stringOnWasmHeap = _malloc(lengthBytes);
    stringToUTF8(text, stringOnWasmHeap, lengthBytes);
    return stringOnWasmHeap;
});


#endif

doubleLinkedList *scriptEditorLines;

int rowCursor = 0;
int lineCursor = 0;
int savedRowCursor = 0;
int startSelectionRowCursor;
int startSelectionLineCursor;
Vector2 camPos = {0,0};
extern core cCore;
string lastScript;

string luaKeywords[] = {
    "end",
    "function", 
    "local", 
    "if", 
    "else", 
    "elseif", 
    "in", 
    "do", 
    "and", 
    "not",
    "or", 
    "for", 
    "nil", 
    "break", 
    "false", 
    "repeat", 
    "return", 
    "then", 
    "true", 
    "until", 
    "while",
    ""
};

u8 luaKeywordsCount =21;


void deleteTextFromTo(int x0,int y0, int x1, int y1)
{
    int topLine, bottomLine;
    int topRow, bottomRow;
    int leftRow, rightRow;
    if(y0<y1)
    {
        topLine = y0;
        bottomLine = y1;
        topRow = x0;
        bottomRow = x1;
    }
    else
    {
        topLine = y1;
        bottomLine = y0;
        topRow=x1;
        bottomRow=x0;
    }
    if(x0<x1)
    {
        leftRow = x0;
        rightRow = x1;
    }
    else
    {
        leftRow = x1;
        rightRow = x0;
    }
    doubleLinkedList *cursor =  scriptEditorLines;
    for(int i = 0;i<topLine;i++)
    {
        cursor = cursor->next;
    }
    if(y0==y1)
    {
        cursor->data = removeFlexStringInFlexString(cursor->data,leftRow,rightRow-leftRow);
        return;
    }
    cursor->data = removeFlexStringInFlexString(cursor->data,topRow,((flexString*)cursor->data)->byteUsed - topRow);
    for(int i = 1; i<bottomLine-topLine;i++)
    {
        scriptEditorLines = removeNextDoubleLinkedList(scriptEditorLines,cursor);
    }
    cursor = cursor->next;
    cursor->data = removeFlexStringInFlexString(cursor->data,0, bottomRow);
    cursor = cursor->prev;
    cursor->data = insertFlexStringInFlexString(cursor->data,cursor->next->data,((flexString*)cursor->data)->byteUsed);
    scriptEditorLines = removeNextDoubleLinkedList(scriptEditorLines,cursor);
}
string copyToString(int x0,int y0, int x1, int y1)
{
    int topLine, bottomLine;
    int topRow, bottomRow;
    int leftRow, rightRow;
    if(y0<y1)
    {
        topLine = y0;
        bottomLine = y1;
        topRow = x0;
        bottomRow = x1;
    }
    else
    {
        topLine = y1;
        bottomLine = y0;
        topRow=x1;
        bottomRow=x0;
    }
    if(x0<x1)
    {
        leftRow = x0;
        rightRow = x1;
    }
    else
    {
        leftRow = x1;
        rightRow = x0;
    }
    int size = 0;
    doubleLinkedList *cursor =  scriptEditorLines;
    for(int i = 0;i<topLine;i++)
    {
        cursor = cursor->next;
    }
    if(y0==y1)
    {
        size = rightRow-leftRow;
        string out = malloc(size+1);
        out[size] = '\0';
        memcpy(out,((flexString*)cursor->data)->string+leftRow,size); 
        return out;
    }
    size = ((flexString*)cursor->data)->byteUsed-topRow+1;
    for(int i = 1; i<bottomLine-topLine;i++)
    {
        cursor = cursor->next;
        size += ((flexString*)cursor->data)->byteUsed+1;
    }
    cursor = cursor->next;
    size+= rightRow+1;

    string out = malloc(size);
    cursor =  getDoubleLinkedList(scriptEditorLines,topLine);
    int currentWritten=0;
    memcpy(out,((flexString*)cursor->data)->string+topRow,((flexString*)cursor->data)->byteUsed-topRow);
    currentWritten = ((flexString*)cursor->data)->byteUsed-topRow;
    out[currentWritten]='\n';
    currentWritten++;
    for(int i = 1; i<bottomLine-topLine;i++)
    {
        cursor = cursor->next;
        memcpy(out+currentWritten,((flexString*)cursor->data)->string,((flexString*)cursor->data)->byteUsed);
        currentWritten += ((flexString*)cursor->data)->byteUsed;
        out[currentWritten]='\n';
        currentWritten++;
    }
    cursor = cursor->next;
    memcpy(out+currentWritten,((flexString*)cursor->data)->string,bottomRow);
    currentWritten+= bottomRow;
    out[currentWritten]='\0';
    currentWritten++;
    return out;
}
void insertString(int x0,int y0,string str)
{
    doubleLinkedList *cursor = getDoubleLinkedList(scriptEditorLines,y0);
    u8 c = str[0];
    int firstln = 0;
    int yCount=0;
    while(c!='\0')
    {
        if(c=='\n')
        {
            break;
        }
        firstln++;
        c = str[firstln];
    }
    if(c == '\n')
    {
        int i = firstln+1;
        int lastLn = firstln;

        insertAfterDoubleLinkedList(cursor,newDoubleLinkedList(newFlexString("")));
        cursor->next->data = insertCharsInFlexString(cursor->next->data,((flexString*)cursor->data)->string+x0,0,((flexString*)cursor->data)->byteUsed-x0);
        cursor->data =  removeFlexStringInFlexString(cursor->data,x0,((flexString*)cursor->data)->byteUsed-x0);
        cursor->data = insertCharsInFlexString(cursor->data,str,x0,firstln);
        while(str[i]!='\0')
        {
            if(str[i]=='\n')
            {
                insertAfterDoubleLinkedList(cursor,newDoubleLinkedList(newFlexString("")));
                cursor = cursor->next;
                yCount++;
                cursor->data = insertCharsInFlexString(cursor->data,str+lastLn+1,0,i-lastLn-1);
                lastLn = i;
            }
            i++;
        }
        cursor = cursor->next;
        yCount++;
        cursor->data = insertCharsInFlexString(cursor->data,str+lastLn+1,0,i-lastLn-1);
        rowCursor = i-lastLn-1;
        lineCursor += yCount; 
        startSelectionRowCursor = rowCursor;
        startSelectionLineCursor = lineCursor;
    }
    else
    {
        cursor->data = insertCharsInFlexString(cursor->data, str,x0,firstln);
        rowCursor += firstln;
        startSelectionRowCursor = rowCursor;
    }
    
}   

string getScriptEditorText()
{
    flexString *out = newFlexString("");
    doubleLinkedList *current = scriptEditorLines;
    while(current != NULL)
    {
        out = insertFlexStringInFlexString(out, current->data, out->byteUsed);
        out = insertCharsInFlexString(out, "\n", out->byteUsed, 1);
        current = current->next;
    }
    string outStr = malloc(out->byteUsed+1);
    memcpy(outStr, out->string, out->byteUsed);
    outStr[out->byteUsed] = '\0';
    free(out);
    return outStr;
}
void loadScriptToRam()
{
    string script = getScriptEditorText();
    cCore.ram.script = script;
}
doubleLinkedList *scriptToDoubleLinkedList(string script)
{
    doubleLinkedList *out = NULL;
    int i = 0;
    int j = 0;
    doubleLinkedList *current = NULL;
    while(script[i] != '\0')
    {
        if(script[i] == '\n')
        {
            flexString *line = newFlexString("");
            line = insertCharsInFlexString(line, script+j, 0, i-j);
            if(out == NULL)
            {
                out = newDoubleLinkedList(line);
                current = out;
            }
            else
            {
                insertAfterDoubleLinkedList(current, newDoubleLinkedList(line));
                current = current->next;
            }
            j = i+1;
        }
        i++;
    }
    return out;
}
void loadScriptFromRam()
{
    if(cCore.ram.script != NULL)
    {
        removeAllDoubleLinkedList(scriptEditorLines);
        scriptEditorLines = scriptToDoubleLinkedList(cCore.ram.script);
        rowCursor = 0;
        lineCursor = 0;
        savedRowCursor = 0;
        startSelectionLineCursor =0;
        startSelectionRowCursor =0;
    }
}
void chooseSmallerCursor()
{
    if(lineCursor>startSelectionLineCursor)
    {
        lineCursor = startSelectionLineCursor;
        rowCursor = startSelectionRowCursor;
    }
    else if(lineCursor==startSelectionLineCursor)
    {
        rowCursor = rowCursor<startSelectionRowCursor ? rowCursor : startSelectionRowCursor;
    }
}
void detectInput()
{
    if(IsKeyDown(KEY_LEFT_CONTROL))
    {
        if(IsKeyPressed(KEY_C))
        {
            int x0=rowCursor;
            int y0=lineCursor;
            int x1=startSelectionRowCursor;
            int y1=startSelectionLineCursor;
            if(x0==x1 && y0==y1)
            {
                x0 = 0;
                x1 = ((flexString*)getDoubleLinkedList(scriptEditorLines,y0)->data)->byteUsed;
            }
            string str = copyToString(x0,y0,x1,y1);
            SetClipboardText(str);
            free(str);
        }
        else if(IsKeyPressed(KEY_V))
        {
            #if defined(PLATFORM_WEB)
                cstring str = getClipboardText();
            #else
                cstring str = GetClipboardText();
            #endif
            
            insertString(rowCursor,lineCursor,(string)str);
        }
        else if(IsKeyPressed(KEY_X))
        {
            int x0=rowCursor;
            int y0=lineCursor;
            int x1=startSelectionRowCursor;
            int y1=startSelectionLineCursor;
            bool cuttingLine = false;
            if(x0==x1 && y0==y1)
            {
                x0 = 0;
                x1 = ((flexString*)getDoubleLinkedList(scriptEditorLines,y0)->data)->byteUsed;
                rowCursor = x0;
                cuttingLine = true;
            }
            string str = copyToString(x0,y0,x1,y1);
            if(cuttingLine)
            {
                int len = strlen(str);
                str = realloc(str,len+2);
                str[len] = '\n';
                str[len+1] = '\0';
            }
            SetClipboardText(str);
            deleteTextFromTo(x0,y0,x1,y1);
            chooseSmallerCursor();
            doubleLinkedList *line = getDoubleLinkedList(scriptEditorLines,lineCursor);
            if(cuttingLine)
            {
                if(lineCursor>0 || line->next!=NULL)
                {
                    scriptEditorLines = removeDoubleLinkedList(scriptEditorLines,line);
                    if(getDoubleLinkedList(scriptEditorLines,lineCursor)==NULL) lineCursor--;
                }
            }
            startSelectionRowCursor = rowCursor;
            startSelectionLineCursor = lineCursor;
        }
    }
    else if(IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))
    {
        if(rowCursor>0)
        {
            rowCursor--;
            savedRowCursor = rowCursor;
        }
        else if(lineCursor>0)
        {
            lineCursor--;
            rowCursor = ((flexString*)getDoubleLinkedList(scriptEditorLines,lineCursor)->data)->byteUsed;
            savedRowCursor = rowCursor;
        }
        if(!IsKeyDown(KEY_LEFT_SHIFT))
        {
            startSelectionRowCursor = rowCursor;
            startSelectionLineCursor = lineCursor;
        }
    }
    else if(IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))
    {
        if(rowCursor<((flexString*)getDoubleLinkedList(scriptEditorLines,lineCursor)->data)->byteUsed)
        {
            rowCursor++;
            savedRowCursor = rowCursor;
            
        }
        else if(getDoubleLinkedList(scriptEditorLines,lineCursor)->next != NULL)
        {
            lineCursor++;
            rowCursor = 0;
            savedRowCursor = rowCursor;
        }
        if(!IsKeyDown(KEY_LEFT_SHIFT))
        {
            startSelectionRowCursor = rowCursor;
            startSelectionLineCursor = lineCursor;
        }
    }
    else if(IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
    {
        if(lineCursor!=startSelectionLineCursor||rowCursor!=startSelectionRowCursor)
        {
            deleteTextFromTo(rowCursor,lineCursor,startSelectionRowCursor,startSelectionLineCursor);
            chooseSmallerCursor();
        }
        else{
            doubleLinkedList *scriptEditorCursor = getDoubleLinkedList(scriptEditorLines,lineCursor);
            if(rowCursor>0)
            {
                
                scriptEditorCursor->data = removeFlexStringInFlexString(scriptEditorCursor->data,rowCursor-1,1);
                rowCursor--;
                savedRowCursor = rowCursor;
            }
            else if(scriptEditorCursor->prev != NULL)
            {
                scriptEditorCursor = scriptEditorCursor->prev;
                lineCursor--;
                rowCursor = ((flexString*)scriptEditorCursor->data)->byteUsed;
                savedRowCursor = rowCursor;
                scriptEditorCursor->data = insertFlexStringInFlexString(scriptEditorCursor->data,((flexString*)scriptEditorCursor->next->data),rowCursor);
                scriptEditorLines = removeDoubleLinkedList(scriptEditorLines,scriptEditorCursor->next);
            }
        }
        startSelectionRowCursor = rowCursor;
        startSelectionLineCursor = lineCursor;
    }
    else if(IsKeyPressed(KEY_ENTER) || IsKeyPressedRepeat(KEY_ENTER))
    {
        if(lineCursor!=startSelectionLineCursor||rowCursor!=startSelectionRowCursor)
        {
            deleteTextFromTo(rowCursor,lineCursor,startSelectionRowCursor,startSelectionLineCursor);
            chooseSmallerCursor();
            startSelectionRowCursor = rowCursor;
            startSelectionLineCursor = lineCursor;
        }
        doubleLinkedList *scriptEditorCursor = getDoubleLinkedList(scriptEditorLines,lineCursor);
        if (scriptEditorCursor != NULL && scriptEditorCursor->data != NULL) {
            int tabCounter = 0;
            int i = 0;
            while(i < ((flexString*)scriptEditorCursor->data)->byteUsed && ((flexString*)scriptEditorCursor->data)->string[i] == '\t')
            {
                tabCounter++;
                i++;
            }
            flexString *newLine = newFlexString("");
            newLine = insertCharsInFlexString(newLine,((flexString*)scriptEditorCursor->data)->string+rowCursor,0,strlen(((flexString*)scriptEditorCursor->data)->string)-rowCursor);
            scriptEditorCursor->data = removeFlexStringInFlexString(scriptEditorCursor->data,rowCursor,strlen(((flexString*)scriptEditorCursor->data)->string)-rowCursor);

            doubleLinkedList *newLineNode = newDoubleLinkedList(newLine);
            insertAfterDoubleLinkedList( scriptEditorCursor, newLineNode);
            scriptEditorCursor = newLineNode;
            rowCursor = 0;
            for(int i = 0; i < tabCounter; i++)
            {
                scriptEditorCursor->data = insertCharsInFlexString(scriptEditorCursor->data,"\t",0,1);
                rowCursor++;
            }
            savedRowCursor =rowCursor;
            lineCursor++;
        }
        startSelectionRowCursor = rowCursor;
        startSelectionLineCursor = lineCursor;
        return;
    }
    else if(IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP))
    {
        doubleLinkedList *scriptEditorCursor = getDoubleLinkedList(scriptEditorLines,lineCursor);
        if(scriptEditorCursor->prev != NULL)
        {
            scriptEditorCursor = scriptEditorCursor->prev;
            lineCursor--;
            if(lineCursor < 0)
            {
                lineCursor = 0;
            }
            else if(rowCursor > ((flexString*)scriptEditorCursor->data)->byteUsed)
            {
                rowCursor = ((flexString*)scriptEditorCursor->data)->byteUsed;
            }
            else if(savedRowCursor <= ((flexString*)scriptEditorCursor->data)->byteUsed)
            {
                rowCursor = savedRowCursor;
            }
            else
            {
                rowCursor = ((flexString*)scriptEditorCursor->data)->byteUsed;
            }
        }
        else
        {
            rowCursor = 0;
            savedRowCursor = 0;
        }
        if(!IsKeyDown(KEY_LEFT_SHIFT))
        {
            startSelectionRowCursor = rowCursor;
            startSelectionLineCursor = lineCursor;
        }
        return;
    }
    else if(IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN))
    {
        doubleLinkedList *scriptEditorCursor = getDoubleLinkedList(scriptEditorLines,lineCursor);
        if(scriptEditorCursor->next != NULL)
        {
            scriptEditorCursor = scriptEditorCursor->next;
            lineCursor++;
            if(rowCursor > ((flexString*)scriptEditorCursor->data)->byteUsed)
            {
                rowCursor = ((flexString*)scriptEditorCursor->data)->byteUsed;
            }
            else if(savedRowCursor <= ((flexString*)scriptEditorCursor->data)->byteUsed)
            {
                rowCursor = savedRowCursor;
            }
            else
            {
                rowCursor = ((flexString*)scriptEditorCursor->data)->byteUsed;
            }
        }
        else
        {
            rowCursor = ((flexString*)scriptEditorCursor->data)->byteUsed;
            savedRowCursor = rowCursor;
        }
        if(!IsKeyDown(KEY_LEFT_SHIFT))
        {
            startSelectionRowCursor = rowCursor;
            startSelectionLineCursor = lineCursor;
        }
        return;
    }
    else if(IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE))
    {
        if(lineCursor!=startSelectionLineCursor||rowCursor!=startSelectionRowCursor)
        {
            deleteTextFromTo(rowCursor,lineCursor,startSelectionRowCursor,startSelectionLineCursor);
            chooseSmallerCursor();
        }
        else
        {
            doubleLinkedList *scriptEditorCursor = getDoubleLinkedList(scriptEditorLines,lineCursor);
            scriptEditorCursor->data = removeFlexStringInFlexString(scriptEditorCursor->data,rowCursor,1);
        }
        startSelectionRowCursor = rowCursor;
        startSelectionLineCursor = lineCursor;
        return;
    }
    else if(IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB))
    {
        if(lineCursor!=startSelectionLineCursor||rowCursor!=startSelectionRowCursor)
        {
            deleteTextFromTo(rowCursor,lineCursor,startSelectionRowCursor,startSelectionLineCursor);
            chooseSmallerCursor();
        }
        doubleLinkedList *scriptEditorCursor = getDoubleLinkedList(scriptEditorLines,lineCursor);
        scriptEditorCursor->data = insertCharsInFlexString(scriptEditorCursor->data,"\t",rowCursor,1);
        rowCursor++;
        savedRowCursor = rowCursor;
        startSelectionRowCursor = rowCursor;
        startSelectionLineCursor = lineCursor;
        return;
    }
    
    char c = GetCharPressed();
    if(c != 0 && c<128)
    {
        if(lineCursor!=startSelectionLineCursor||rowCursor!=startSelectionRowCursor)
        {
            deleteTextFromTo(rowCursor,lineCursor,startSelectionRowCursor,startSelectionLineCursor);
            chooseSmallerCursor();
        }
        doubleLinkedList *scriptEditorCursor = getDoubleLinkedList(scriptEditorLines,lineCursor);
        scriptEditorCursor->data = insertCharsInFlexString(scriptEditorCursor->data, &c,rowCursor,1);
        rowCursor++;
        savedRowCursor = rowCursor;
        startSelectionRowCursor = rowCursor;
        startSelectionLineCursor = lineCursor;
    }
}
bool isUsableWordsChar(char c)
{
    return (c >= 'a' && c <= 'z') 
    || 
    (c>='A' && c <='Z')
    ||
    (c>='0' && c <='9')
    ||
    c=='_';
}

// return keyword len
u64 therIsKeyword(string startingPoint)
{
    string *keywords;
    u8 keywordsCount;
    if(cCore.ram.language == LUA) 
    {
        keywords = luaKeywords;
        keywordsCount = luaKeywordsCount;
    }
    bool found = false;
    u64 i;
    for(u8 j = 0; j< keywordsCount; j++)
    {
        i=0;
        bool isThisKeyword = true;
        while(isUsableWordsChar(startingPoint[i]))
        {
            if(keywords[j][i] == '\0') 
            {
                isThisKeyword = false;
                break;
            }
            else if(startingPoint[i]!= keywords[j][i])
            {
                isThisKeyword = false;
                break;
            }
            i++;
        }
        if(i>0 && isThisKeyword == true && keywords[j][i]=='\0' )
        {
            return i;
        }
    }
    return 0;
}
u64 therIsNumber(string startingPoint)
{
    u64 i = 0;
    while(startingPoint[i] >= '0' && startingPoint[i] <='9')
    {
        i++;
    }
    return i;
}


void drawText()
{
    static int frameCounter = 0;
    static int lastRowCursor = 0;
    static int lastLineCursor = 0;
    if(lastRowCursor != rowCursor || lastLineCursor != lineCursor)
    {
        frameCounter = 0;
    }
    bool drawSelector = frameCounter%40 < 20;
    int xOff = 0;
    int yOff = 17;
    int yLimit = 225;
    int cursorX = xOff+(rowCursor*6);
    int cursorY = yOff+(lineCursor*6);
    if(cursorX > 248+camPos.x)
    {
        camPos.x = cursorX-248;
    }
    if(cursorX < camPos.x)
    {
        camPos.x = cursorX;
    }
    if(cursorY > yLimit+camPos.y)
    {
        camPos.y = cursorY-yLimit;
    }
    if(cursorY-yOff < camPos.y)
    {
        camPos.y = cursorY-yOff;
    }
    // camPos.x = (cursorX-248)>0 ? cursorX-248 : 0;
    // camPos.y = (cursorY-232)>0 ? cursorY-232 : 0;
    doubleLinkedList *current = scriptEditorLines;
    int i = 0;
    u8 color = _WHITE;
    int topLineSelected = lineCursor < startSelectionLineCursor ? lineCursor : startSelectionLineCursor;
    int bottomLineSelected = lineCursor < startSelectionLineCursor ? startSelectionLineCursor : lineCursor;

    int topRowSelected = lineCursor < startSelectionLineCursor ? rowCursor : startSelectionRowCursor;
    int bottomRowSelected = lineCursor < startSelectionLineCursor ? startSelectionRowCursor : rowCursor;


    int leftRowSelected = rowCursor < startSelectionRowCursor ? rowCursor : startSelectionRowCursor;
    int rightRowSelected = rowCursor < startSelectionRowCursor ? startSelectionRowCursor : rowCursor;

    while(current != NULL)
    {
        flexString *line = current->data;
        u64 remainingColoredChar = 0;
        bool isNumber=false;
        bool isString=false;
        bool wasString = false;
        bool isNotUsable;
        for(int j = 0; j < line->byteUsed; j++)
        {
            int cX = xOff+(j*6)-camPos.x;
            int cY = yOff+(i*6)-camPos.y;
            
            if(remainingColoredChar == 0)
            {
                isNumber = false;
                isNotUsable = false;
            }

            if(line->string[j] == '"')
            {
                if(j==0 || (line->string[j-1]!='\\'))
                {
                    isString = !isString; 
                    if(isString == false) wasString = true;
                }
            }
            if(!isString)
            {
                if(remainingColoredChar<= 0  )
                {
                    if(!isUsableWordsChar(line->string[j]))
                    {
                        remainingColoredChar = 1;
                        isNotUsable=true;
                    }
                    else if(  j>=0 && ( (j==0) ? true : (!isUsableWordsChar(line->string[j-1]))))
                    {
                        remainingColoredChar = therIsKeyword(line->string+j);
                        if(remainingColoredChar == 0)
                        {
                            remainingColoredChar = therIsNumber(line->string+j);
                            if(remainingColoredChar != 0)
                            {
                                isNumber = true;
                            }
                            
                        }
                    }
                    

                }
            }

            if(cX > -6 && cX < 256 && cY-yOff > -6 && cY-yOff < yLimit)
            { 
                char c = line->string[j];
                
                
                if(isString)
                {
                    color = _GREEN;
                }
                else if(wasString)
                {
                    color = _GREEN;
                    wasString = false;
                }
                else if(remainingColoredChar > 0)
                {
                    if(isNumber)
                    {
                        color = _YELLOW;
                    }
                    else if(isNotUsable)
                    {
                        color = _LIGHT_GREY;
                    }
                    else
                    {
                        color = _RED;
                    }
                }
                else
                {
                    color = _WHITE;
                }

                if(
                    (
                        topLineSelected<i
                        &&
                        i<bottomLineSelected
                    )
                    ||
                    (
                        startSelectionLineCursor == lineCursor
                        &&
                        lineCursor ==i
                        &&
                        leftRowSelected <=j
                        &&
                        j< rightRowSelected
                    )
                    ||
                    (
                        startSelectionLineCursor != lineCursor
                        &&
                        i == topLineSelected
                        &&
                        j>= topRowSelected
                    )
                    ||
                    (
                        startSelectionLineCursor != lineCursor
                        &&
                        i == bottomLineSelected
                        &&
                        j < bottomRowSelected
                    )
                )
                {
                    drawRectFilled(cX,cY,6,6,_BLUE,6);
                }
                printC(cX,cY,c,color,10);
            }
            if(remainingColoredChar>0)
            {
                remainingColoredChar--;
            }
        }

        i++;
        current = current->next;
    }
    if(drawSelector && lineCursor==startSelectionLineCursor && rowCursor ==startSelectionRowCursor)
    drawRectFilled(xOff+(rowCursor*6)-camPos.x,yOff+(lineCursor*6)-camPos.y,6,6,_BLUE,6);


    frameCounter++;
    lastRowCursor = rowCursor;
    lastLineCursor = lineCursor;
}


void scriptEditorLoop()
{
    if(scriptEditorLines == NULL)
    {
        scriptEditorLines = newDoubleLinkedList(newFlexString(""));
    }

    for(int i = 2; i < 29; i++)
    {
        drawHorizontaFilledLine(0, 32, i, _DARK_BLUE,5);
    }
    detectInput();
    drawText();
}
