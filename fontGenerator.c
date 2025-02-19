#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char data[127][8];

char bit8toHex(char *bit8)
{
    char hex = 0;
    for (int i = 0; i < 8; i++)
    {
        hex |= (bit8[i] & 1) << (7 - i);
    }
    return hex;
}
char * bitArrToHexArr(char *bitArr)
{
    char *hexArr = (char *)malloc(64);
    for (int i = 0; i < 64; i++)
    {
        hexArr[i] = bit8toHex(bitArr + i * 8);
    }
    return hexArr;
}

void popolate()
{
    for(int i = 0; i < 127; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            data[i][j] = 0;
        }
    }
    char a[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memcpy(data['a'], bitArrToHexArr(a), 8);
    
}

void write()
{
    FILE *file = fopen("font.inl", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < 127; i++) {
        for (int j = 0; j < 8; j++) {
            fprintf(file, "0x%02x ,", data[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main()
{
    popolate();
    write();
    return 0;
}