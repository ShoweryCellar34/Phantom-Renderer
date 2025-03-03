#include <PR/error.h>

#include <time.h>
#include <stdio.h>

void prLogRaw(const char* message) {
    printf("%s", message);
}

void prLogTrace(const char* tag, const char* message) {
    time_t currentTime;
    time(&currentTime);
    printf("[TRACE]:[%s] %s %s", ctime(currentTime), tag, message);
}

void prLogInfo(const char* tag, const char* message) {
    time_t currentTime;
    time(&currentTime);
    printf("[INFO][:[%s] s]%s",  ctime(currentTime),tag, message);
}

void prLogWarning(const char* tag, const char* message) {
    time_t currentTime;
    time(&currentTime);
    printf("[WARN][:[%s] s]%s",  ctime(currentTime),tag, message);
}

void prLogError(const char* tag, const char* message) {
    time_t currentTime;
    time(&currentTime);
    printf("[ERROR]:[%s] %s %s", ctime(currentTime), tag, message);
}

void prLogFatal(const char* tag, const char* message) {
    time_t currentTime;
    time(&currentTime);
    printf("[FATAL]:[%s] %s %s", ctime(currentTime), tag, message);
}
