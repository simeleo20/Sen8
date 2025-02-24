#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flexString.h"


#define CAPACITYTOADD 8
flexString *newFlexString(char* str)
{

    int newCapacity = 0;
    int strsize = strlen(str);
    while(newCapacity <= strsize)
    {
        newCapacity += CAPACITYTOADD;
    }
    flexString *out = malloc(sizeof(flexString)+newCapacity+1);
    out->byteUsed = strsize;
    out->capacity = newCapacity;
    strcpy(out->string,str);
    out->string[out->byteUsed] = '\0';
    return out;
}


flexString *resizeFlexString(flexString *flexStr,int capacity)
{
    flexString *out = realloc(flexStr,sizeof(flexString)+capacity+1);
    out->capacity = capacity;
    return out;
}
flexString *appendInFlexString(flexString *flexStr, char c)
{
  
    flexString *out = flexStr;
    if(flexStr->byteUsed==flexStr->capacity)
    {
        out = resizeFlexString(flexStr,flexStr->capacity+CAPACITYTOADD);
        out->capacity = flexStr->capacity+CAPACITYTOADD;
    }
    out->string[out->byteUsed] = c;
    out->byteUsed+=1;
    out->string[out->byteUsed] = '\0';
    return out;
}
flexString *popLastInFlexString(flexString *flexStr)
{
    flexString *out = flexStr;
    if(flexStr->byteUsed>0)
    {
       out->byteUsed-=1;
    }
    if(flexStr->byteUsed<flexStr->capacity-CAPACITYTOADD)
    {
        out->capacity = flexStr->capacity-CAPACITYTOADD;
        out = resizeFlexString(flexStr,out->capacity);
    }
    out->string[out->byteUsed] = '\0';
    return out;
}
flexString *removeFlexStringInFlexString(flexString *dest,int index,int length)
{
    flexString *out = dest;
    if(index>dest->byteUsed)
    {
        printf("index out of str\n");
        return out;
    }
    if(index+length>dest->byteUsed)
    {
        printf("length out of str\n");
        return out;
    }
    for(int i = index;i<dest->byteUsed-length;i++)
    {
        out->string[i] = out->string[i+length];
    }
    if(dest->byteUsed-length<dest->capacity-CAPACITYTOADD)
    {
        int newCapacity = dest->capacity;
        while(newCapacity > dest->byteUsed-length)
        {
            newCapacity -= CAPACITYTOADD;
        }
        newCapacity += CAPACITYTOADD;
        out = resizeFlexString(out,newCapacity);
    }
    out->byteUsed = out->byteUsed-length;
    out->string[out->byteUsed] = '\0';
    return out;
}
flexString *insertFlexStringInFlexString(flexString *dest,flexString*  src,int index)
{
    flexString *out = dest;
    if(index>dest->byteUsed)
    {
        printf("index out of str\n");
        return out;
    }
    if(dest->byteUsed+src->byteUsed > dest->capacity)
    {
        int newCapacity = dest->capacity;
        while(newCapacity < dest->byteUsed+src->byteUsed)
        {
            newCapacity += CAPACITYTOADD;
        }
        out = resizeFlexString(out,newCapacity);
    }
    int moveOffset = src->byteUsed;


    for(int i = out->byteUsed; i>=index;i--)
    {
        out->string[i+moveOffset] = out->string[i];
    }
    for(int i = 0;i<moveOffset;i++)
    {
        out->string[index+i] = src->string[i];
    }
    out->byteUsed = out->byteUsed+ src->byteUsed;
    out->string[out->byteUsed] = '\0';
    return out;
}
flexString *insertCharsInFlexString(flexString *dest,string chars,int index,int len)
{
    flexString *out = dest;
    if(index>dest->byteUsed)
    {
        printf("index out of str\n");
        return out;
    }
    if(dest->byteUsed+len > dest->capacity)
    {
        int newCapacity = dest->capacity;
        while(newCapacity < dest->byteUsed+len)
        {
            newCapacity += CAPACITYTOADD;
        }
        out = resizeFlexString(out,newCapacity);
    }
    int moveOffset = len;

    for(int i = out->byteUsed; i>=index;i--)
    {
        out->string[i+moveOffset] = out->string[i];
    }
    for(int i = 0;i<moveOffset;i++)
    {
        out->string[index+i] = chars[i];
    }
    out->byteUsed = out->byteUsed+ len;
    out->string[out->byteUsed] = '\0';
    return out;
}


