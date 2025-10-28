#ifndef PR_LOGGER_INTERNAL_H
#define PR_LOGGER_INTERNAL_H

#include <PR/defines.h>

const char* i_logGetLevelString(prLogLevel_t level);

const char* i_logGetEventString(prEvent_t event);

void i_logGetTimeString(char* destination, size_t writeCount);

#endif // PR_LOGGER_INTERNAL_H
