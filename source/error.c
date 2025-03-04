#include <PR/error.h>

#include <time.h>
#include <stdio.h>

FILE* logFile = NULL;
int logLevel = 4;

void prSetLogLevel(int level) {
    logLevel = level;
}

void prStartFileLogging() {
    if(!logFile) {
        logFile = fopen("PR.log", "w");
    }
}

void prEndFileLogging() {
    if(logFile) {
        fclose(logFile);
    }
    logFile = NULL;
}

void i_prLog(const char* level, const char* tag, const char* message) {
    time_t timer;
    char timeString[26];
    struct tm* timeInfo;

    timer = time(NULL);
    timeInfo = localtime(&timer);

    strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", timeInfo);
    printf("[PR]%s%s[%s] %s\n", level, tag, timeString, message);

    if(logFile) {
        fprintf(logFile, "[PR]%s%s[%s] %s\n", level, tag, timeString, message);
    }
}

void prLogRaw(const char* message) {
    i_prLog("", "", message);
}

void prLogTrace(const char* tag, const char* message) {
    if(logLevel >= 4) {
        i_prLog("[TRACE]", tag, message);
    }
}

void prLogInfo(const char* tag, const char* message) {
    if(logLevel >= 3) {
        i_prLog("[INFO]", tag, message);
    }
}

void prLogWarning(const char* tag, const char* message) {
    if(logLevel >= 2) {
        i_prLog("[WARNING]", tag, message);
    }
}

void prLogError(const char* tag, const char* message) {
    if(logLevel >= 1) {
        i_prLog("[ERROR]", tag, message);
    }
}

void prLogFatal(const char* tag, const char* message) {
    if(logLevel >= 0) {
        i_prLog("[FATAL]", tag, message);
    }
}

void prError(unsigned int errorType, const char* message) {
    switch(errorType) {
        case PR_MEMORY_ERROR:
            prLogFatal("[MEMORY]", message);
            break;

        case PR_INVALID_DATA_ERROR:
            prLogWarning("[DATA]", message);
            break;

        case PR_GL_ERROR:
            prLogError("[GL]", message);
            break;

    default:
        break;
    }
}
