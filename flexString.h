#ifndef FLEXSTRING_H
#define FLEXSTRING_H

#include "types.h"

typedef struct
{
    int capacity;
    int byteUsed;
    char string[];
} flexString;

flexString *newFlexString(char* str);
flexString *resizeFlexString(flexString *flexStr, int capacity);
flexString *appendInFlexString(flexString *flexStr, char c);
flexString *popLastInFlexString(flexString *flexStr);
flexString *removeFlexStringInFlexString(flexString *dest, int index, int length);
flexString *insertFlexStringInFlexString(flexString *dest, flexString* src, int index);
flexString *insertCharsInFlexString(flexString *dest, char* chars, int index, int len);


#endif // FLEXSTRING_H