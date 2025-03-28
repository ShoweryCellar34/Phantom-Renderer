#include <PR/error.h>

#include <PR/defines.h>

#include <stdarg.h>
#include <string.h>
#include <time.h>

#define MAX_LEVELSTR_LENGTH 16
#define MAX_TIMESTR_LENGTH 32
#define MAX_LOGSTR_LENGTH 1024

int i_logStreamCount = 0;
FILE** i_logStream = NULL;
prLogLevel_t i_logLevel = PR_LOG_TRCE;

void prLogSetLevel(prLogLevel_t level) {
    i_logLevel = level;
}

void prLogSetStream(int streamCount, FILE** stream) {
    i_logStreamCount = streamCount;
    i_logStream = stream;
}

void _prLog(prLogLevel_t level, const char* format, ...) {
    va_list arg;

    if(level > i_logLevel) {
        return;
    }

    char levelString[MAX_LEVELSTR_LENGTH];
    switch(level) {
        case PR_LOG_TRCE:
            strncpy(levelString, "[TRCE]", MAX_LEVELSTR_LENGTH);
            break;

        case PR_LOG_INFO:
            strncpy(levelString, "[INFO]", MAX_LEVELSTR_LENGTH);
            break;

        case PR_LOG_WARN:
            strncpy(levelString, "[WARN]", MAX_LEVELSTR_LENGTH);
            break;
    
        case PR_LOG_EROR:
            strncpy(levelString, "[EROR]", MAX_LEVELSTR_LENGTH);
            break;

        case PR_LOG_FTAL:
            strncpy(levelString, "[FTAL]", MAX_LEVELSTR_LENGTH);
            break;

        default:
            strncpy(levelString, "[UNKW]", MAX_LEVELSTR_LENGTH);
            break;
    }

    time_t timer = time(NULL);
    struct tm* timeInfo = localtime(&timer);
    char timeString[MAX_TIMESTR_LENGTH];
    int bytesWritten = strftime(timeString, MAX_TIMESTR_LENGTH, "%Y-%m-%d %H:%M:%S", timeInfo);

    char computedFormat[MAX_LOGSTR_LENGTH];
    snprintf(computedFormat, MAX_LOGSTR_LENGTH, "%s%s%s", levelString, timeString, format);

    for(int i = 0; i < i_logStreamCount; i++) {
        va_start(arg, format);
        vfprintf(i_logStream[i], computedFormat, arg);
        va_end(arg);
    }
}

void _prLogEvent(prLogLevel_t errorType, prLogLevel_t level, const char* format, ...) {
    va_list arg;

    switch(errorType) {
        case PR_MMRY_EVENT:
            _prLog(level, "[MMRY]%s", format, arg);
            break;

        case PR_DATA_EVENT:
            _prLog(level, "[DATA]%s", format, arg);
            break;

        case PR_OPGL_EVENT:
            _prLog(level, "[OPGL]%s", format, arg);
            break;

        case PR_USER_EVENT:
            _prLog(level, "[USER]%s", format, arg);
            break;
    }
}
