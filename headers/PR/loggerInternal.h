#include <PR/defines.h>

const char* i_logGetLevelString(prLogLevel_t level);

const char* i_logGetEventString(prEvent_t event);

void i_logGetTimeString(char* destination, size_t writeCount);
