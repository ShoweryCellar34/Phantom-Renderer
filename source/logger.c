#include <PR/logger.h>

#include <PR/defines.h>

#include <PR/loggerInternal.h>

#include <stdarg.h>
#include <string.h>

int i_logStreamCount = 0;
FILE** i_logStream = NULL;
prLogLevel_t i_logLevel = PR_LOG_TRACE;

void prLogSetLevel(prLogLevel_t level) {
    i_logLevel = level;
}

void prLogSetStream(int streamCount, FILE** stream) {
    i_logStreamCount = streamCount;
    i_logStream = stream;
}

void _prLogRaw(const char* format, ...) {
    va_list arg;

    for(int i = 0; i < i_logStreamCount; i++) {
        va_start(arg, format);
        vfprintf(i_logStream[i], format, arg);
        va_end(arg);
    }
}

void _prLog(prLogLevel_t level, const char* format, ...) {
    va_list arg;

    if(level > i_logLevel) {
        return;
    }

    char levelString[PR_MAXSTR_LEN];
    strncpy(levelString, i_logGetLevelString(level), PR_MAXSTR_LEN);

    char timeString[PR_MAXSTR_LEN];
    i_logGetTimeString(timeString, PR_MAXSTR_LEN);

    char computedFormat[PR_MAXSTR_LEN];
    snprintf(computedFormat, PR_MAXSTR_LEN, "%s%s%s", levelString, timeString, format);

    for(int i = 0; i < i_logStreamCount; i++) {
        va_start(arg, format);
        vfprintf(i_logStream[i], computedFormat, arg);
        va_end(arg);
    }
}

void _prLogEvent(prEvent_t event, prLogLevel_t level, const char* format, ...) {
    va_list arg;

    if(level > i_logLevel) {
        return;
    }

    char levelString[PR_MAXSTR_LEN];
    strncpy(levelString, i_logGetLevelString(level), PR_MAXSTR_LEN);

    char timeString[PR_MAXSTR_LEN];
    i_logGetTimeString(timeString, PR_MAXSTR_LEN);

    char computedFormat[PR_MAXSTR_LEN];
    snprintf(computedFormat, PR_MAXSTR_LEN, "%s%s%s %s\n", levelString, i_logGetEventString(event), timeString, format);

    for(int i = 0; i < i_logStreamCount; i++) {
        va_start(arg, format);
        vfprintf(i_logStream[i], computedFormat, arg);
        va_end(arg);
    }
}
