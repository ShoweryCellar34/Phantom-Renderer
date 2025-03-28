#pragma once

#include <PR/defines.h>

#include <stdio.h>

void prLogSetLevel(prLogLevel_t level);

void prLogSetStream(int streamCount, FILE** stream);

void _prLog(prLogLevel_t level, const char* format, ...);

#define prLog(level, format, ...) _prLog(level, format"\n", ##__VA_ARGS__)

void _prLogEvent(prError_t errorType, prLogLevel_t level, const char* format, ...);

#define prLogEvent(errorType, level, format, ...) _prLogEvent(errorType, level, format"\n", ##__VA_ARGS__)
