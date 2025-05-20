#pragma once

#include <PR/defines.h>

#include <stdio.h>

void prLogSetLevel(prLogLevel_t level);

void prLogSetStream(int streamCount, FILE** stream);

void _prLogRaw(const char* format, ...);

#define prLogRaw(format, ...) _prLogRaw(format, ##__VA_ARGS__)

void _prLog(prLogLevel_t level, const char* format, ...);

#define prLog(level, format, ...) _prLog(level, format, ##__VA_ARGS__)

void _prLogEvent(prEvent_t event, prLogLevel_t level, const char* format, ...);

#define prLogEvent(event, level, format, ...) _prLogEvent(event, level, format, ##__VA_ARGS__)
