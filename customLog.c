#include <raylib.h>
#include <stdio.h>
#include "console.h"

void CustomLog(int msgType, const char *text, va_list args)
{
    // char timeStr[64] = { 0 };
    // time_t now = time(NULL);
    // struct tm *tm_info = localtime(&now);

    //strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    //printf("[%s] ", timeStr);
    switch (msgType)
    {
        case LOG_INFO: printf("[INFO] : "); break;
        case LOG_ERROR: printf("[ERROR]: "); break;
        case LOG_WARNING: printf("[WARN] : "); break;
        case LOG_DEBUG: printf("[DEBUG]: "); break;
        default: break;
    }

    vprint(text, args);
    print("\n");
}
