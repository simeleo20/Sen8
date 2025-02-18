#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str1[] = "char os[] =\"";

    FILE *file;
    char ch;
    long fileSize;
    file = fopen("os.lua", "rb");
    if (file == NULL) {
        printf("Error: Could not open file os.lua\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);
    printf("File size: %ld bytes\n", fileSize);

    char * str2 = malloc(fileSize + 1);

    file = fopen("os.lua", "r");
    if (file == NULL) {
        printf("Error: Could not open file os.lua\n");
        return 1;
    }
    int i = 0;
    int currentStringSize = fileSize+1;
    while ((ch = fgetc(file)) != EOF) {
        if(ch=='\n')
        {
            currentStringSize++;
            str2 = realloc(str2, currentStringSize);
            str2[i++] = '\\';
            str2[i++] = 'n';
        }
        else if (ch=='"')
        {
            currentStringSize++;
            str2 = realloc(str2, currentStringSize);
            str2[i++] = '\\';
            str2[i++] = '"';
        }
        else
        {
            str2[i++] = ch;
        };
    }
    str2[i] = '\0';

    fclose(file);
    char str3[] = "\";\n";

    char *strtot = malloc(strlen(str1) + strlen(str2) + strlen(str3) + 1);
    strcpy(strtot, str1);
    strcat(strtot, str2);
    strcat(strtot, str3);

    printf("%s\n", strtot);

    //write to file
    file = fopen("oslua.c", "w");
    if (file == NULL) {
        printf("Error: Could not open file oslua.c\n");
        return 1;
    }
    fprintf(file, "%s", strtot);
    fclose(file);


    return 0;
}