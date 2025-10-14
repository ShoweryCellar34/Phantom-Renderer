#include <files.hpp>

#if defined(_WIN32)
#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h> 
#endif

#include <PR/PR.h>

std::filesystem::path getUserDataPath() {
    std::filesystem::path path = std::filesystem::current_path();

#if defined(_WIN32)
    PWSTR tempPath;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &tempPath);
    if(SUCCEEDED(hr)) {
        path = tempPath;
        CoTaskMemFree(tempPath);
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to use \"SHGetKnownFolderPath\" to get \"FOLDERID_RoamingAppData\" for user data path");
        return std::filesystem::path();
    }
#elif defined(__linux__)
    const char* xdgCacheHome = std::getenv("XDG_CACHE_HOME");
    if(xdgCacheHome != nullptr && std::string(xdgCacheHome).length() > 0) {
        path = xdgCacheHome;
    } else {
        const char* homeDir = std::getenv("HOME");
        if(homeDir != nullptr) {
            path = std::string(homeDir) + "/.cache";
        } else {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to use \"std::getenv\" to get \"XDG_CACHE_HOME\" or \"HOME\" for user data path");
        }
    }
#endif

    return path;
}

std::filesystem::path getExecutablePath() {
    std::filesystem::path path = std::filesystem::current_path();

#if defined(_WIN32)
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if(length == 0) {
        prLogEvent(PR_EVENT_USER, PR_LOG_FATAL, "Failed to use \"GetModuleFileNameA\"  to get \"\" get execuable path");
        exit(EXIT_FAILURE);
    }
    path = buffer;
#elif defined(__linux__)
    char executablePath[PATH_MAX];
    ssize_t length;
    length = readlink("/proc/self/exe", executablePath, sizeof(executablePath) - 1);

    if(length != -1) {
        executablePath[length] = '\0'; // Null-terminate the string
        path = executablePath;
    } else {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to use \"readlink\" to get \"/proc/self/exe\" for executable path");
    }
#endif

    return path;
}
