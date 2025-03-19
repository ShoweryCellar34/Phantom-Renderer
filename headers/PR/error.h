#pragma once

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
