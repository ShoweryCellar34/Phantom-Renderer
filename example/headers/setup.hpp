#pragma once

#include <filesystem>

void setupPaths();

void setLogFilePath(std::filesystem::path logFilePath);

void setupLog();

void shutdownLog();

void setupWindow();

void shutdownWindow();

void setupShaders();

void shutdownShaders();

void setupFramebuffers();

void shutdownFramebuffers();

void setupTextures();

void shutdownTextures();
