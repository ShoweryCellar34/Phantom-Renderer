#pragma once

#define PR_MEMORY_ERROR 0
#define PR_INVALID_DATA_ERROR 1
#define PR_GL_ERROR 2

#define PR_LOG_LEVEL_FATAL 0
#define PR_LOG_LEVEL_ERROR 1
#define PR_LOG_LEVEL_WARNING 2
#define PR_LOG_LEVEL_INFO 3
#define PR_LOG_LEVEL_TRACE 4

void prSetLogLevel(int level);

void prStartFileLogging();

void prEndFileLogging();

void prLogRaw(const char* message);

void prLogTrace(const char* tag, const char* message);

void prLogInfo(const char* tag, const char* message);

void prLogWarning(const char* tag, const char* message);

void prLogError(const char* tag, const char* message);

void prLogFatal(const char* tag, const char* message);

void prError(unsigned int errorType, const char* message);
