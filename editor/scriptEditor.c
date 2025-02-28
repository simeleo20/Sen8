#include "scriptEditor.h"
#include "../types/types.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "../core.h"
#include "editor.h"


doubleLinkedList *scriptEditorLines;
doubleLinkedList *scriptEditorCursor;
int rowCursor = 0;
int lineCursor = 0;
int savedRowCursor = 0;
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
        scriptEditorCursor = scriptEditorLines;
        rowCursor = 0;
        lineCursor = 0;
        savedRowCursor = 0;
    }
}

void detectInput()
{
    if(IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))
    {
        if(rowCursor>0)
        {
            rowCursor--;
            savedRowCursor = rowCursor;
        }
        else if(scriptEditorCursor->prev != NULL)
        {
            scriptEditorCursor = scriptEditorCursor->prev;
            lineCursor--;
            rowCursor = ((flexString*)scriptEditorCursor->data)->byteUsed;
            savedRowCursor = rowCursor;
        }
    }
    else if(IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))
    {
        if(rowCursor<((flexString*)scriptEditorCursor->data)->byteUsed)
        {
            rowCursor++;
            savedRowCursor = rowCursor;
            
        }
        else if(scriptEditorCursor->next != NULL)
        {
            scriptEditorCursor = scriptEditorCursor->next;
            lineCursor++;
            rowCursor = 0;
            savedRowCursor = rowCursor;
        }
    }
    else if(IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
    {
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
    else if(IsKeyPressed(KEY_ENTER) || IsKeyPressedRepeat(KEY_ENTER))
    {
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
        return;
    }
    else if(IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP))
    {
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
        return;
    }
    else if(IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN))
    {
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
        return;
    }
    else if(IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE))
    {
        scriptEditorCursor->data = removeFlexStringInFlexString(scriptEditorCursor->data,rowCursor,1);
        return;
    }
    else if(IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB))
    {
        scriptEditorCursor->data = insertCharsInFlexString(scriptEditorCursor->data,"\t",rowCursor,1);
        rowCursor++;
        savedRowCursor = rowCursor;
        return;
    }
    
    char c = GetCharPressed();
    if(c != 0 && c<128)
    {

        scriptEditorCursor->data = insertCharsInFlexString(scriptEditorCursor->data, &c,rowCursor,1);
        rowCursor++;
        savedRowCursor = rowCursor;
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
    if(drawSelector)
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
    if(scriptEditorCursor == NULL)
    {
        scriptEditorCursor = scriptEditorLines;
    }
    for(int i = 2; i < 29; i++)
    {
        drawHorizontaFilledLine(0, 32, i, _DARK_BLUE,5);
    }
    detectInput();
    drawText();
}
