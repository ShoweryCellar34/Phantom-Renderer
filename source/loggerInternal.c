#include <PR/loggerInternal.hpp>

#include <time.h>

const char* i_logGetLevelString(prLogLevel_t level) {
    switch(level) {
        case PR_LOG_TRACE:
            return "[TRCE]";
            break;

        case PR_LOG_INFO:
            return "[INFO]";
            break;

        case PR_LOG_WARNING:
            return "[WARN]";
            break;
    
        case PR_LOG_ERROR:
            return "[EROR]";
            break;

        case PR_LOG_FATAL:
            return "[FTAL]";
            break;

        default:
            return "[UNKW]";
            break;
    }
}

const char* i_logGetEventString(prEvent_t event) {
    switch(event) {
        case PR_EVENT_MEMORY:
            return "[MMRY]";
            break;

        case PR_EVENT_DATA:
            return "[DATA]";
            break;

        case PR_EVENT_OPENGL:
            return "[OPGL]";
            break;

        case PR_EVENT_USER:
            return "[USER]";
            break;

        default:
            return "UNKW";
            break;
    }
}

void i_logGetTimeString(char* destination, size_t writeCount) {
    time_t timer = time(NULL);
    struct tm* timeInfo = localtime(&timer);
    strftime(destination, writeCount, "[%Y-%m-%d %H:%M:%S]", timeInfo);
}
