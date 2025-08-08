#pragma once

#include <filesystem>

void setupPaths();

void setupLog(std::filesystem::path logFilePath);

void shutdownLog();
