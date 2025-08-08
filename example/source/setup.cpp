#include <setup.hpp>

#include <globals.hpp>

#include <PR/PR.h>
#include <files.hpp>

void setupPaths() {
    userDataPath = getUserDataPath() / "Example/";
    std::filesystem::create_directories(userDataPath);
    resourcesPath = getExecutablePath().parent_path();
}

void setupLog(std::filesystem::path logFilePath) {
    prLogSetLevel(PR_LOG_TRACE);
    prLogSetStream(1, (FILE**)stdout);

    logFile = fopen(logFilePath.u8string().c_str(), "w");
    FILE* logStreams[] = {stdout, logFile};
    prLogSetStream(2, logStreams);
}

void shutdownLog() {
    fclose(logFile);
}
