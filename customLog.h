#ifndef CUSTOM_LOG_H
#define CUSTOM_LOG_H

#include <stdio.h>
#include "customLog.c"

void CustomLog(int msgType, const char *text, va_list args);

#endif // CUSTOM_LOG_H