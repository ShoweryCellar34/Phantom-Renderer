#include <PR/error.h>

#include <time.h>
#include <stdio.h>

short firstLog = 0;
FILE* file = NULL;

void i_prLog(const char* level, const char* tag, const char* message) {
    time_t timer;
    char timeString[26];
    struct tm* timeInfo;

    timer = time(NULL);
    timeInfo = localtime(&timer);

    strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", timeInfo);
    printf("[PR]%s%s[%s] %s\n", level, tag, timeString, message);

    // I don't usualy add comments (I know it's a bad idea not to) but this was an execption, I am aware about how bad of code this file opening is.
    if(!firstLog) {
        firstLog = 1;
        file = fopen("PR.log", "w");
    } else {
        file = fopen("PR.log", "a");
    }

    fprintf(file, "[PR]%s%s[%s] %s\n", level, tag, timeString, message);

    fclose(file);
}

void prLogRaw(const char* message) {
    i_prLog("", "", message);
}

void prLogTrace(const char* tag, const char* message) {
    i_prLog("[TRACE]", tag, message);
}

void prLogInfo(const char* tag, const char* message) {
    i_prLog("[INFO]", tag, message);
}

void prLogWarning(const char* tag, const char* message) {
    i_prLog("[WARNING]", tag, message);
}

void prLogError(const char* tag, const char* message) {
    i_prLog("[ERROR]", tag, message);
}

void prLogFatal(const char* tag, const char* message) {
    i_prLog("[FATAL]", tag, message);
}
