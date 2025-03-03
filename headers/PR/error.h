#pragma once

#define PR_MEMORY_CHECK(x, y) 

typedef enum prErrorType {
    PR_MEMORY_ERROR,
    PR_
} prErrorType;

void prLogRaw(const char* message);

void prLogTrace(const char* tag, const char* message);

void prLogInfo(const char* tag, const char* message);

void prLogWarning(const char* tag, const char* message);

void prLogError(const char* tag, const char* message);

void prLogFatal(const char* tag, const char* message);

void prError(prErrorType errorType, const char* message);
