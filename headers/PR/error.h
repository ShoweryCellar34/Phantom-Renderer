#pragma once

#include <PR/defines.h>

#include <stdio.h>

void prLogSetLevel(prLogLevel_t level);

void prLogSetStream(FILE* stream);

void _prLog(prLogLevel_t level, const char* format, ...);

#define prLog(level, fmt, ...) _prLog(level, fmt"\n", ##__VA_ARGS__)

void prLogEvent(prError_t errorType, prLogLevel_t level, const char* message);
