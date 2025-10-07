#include <files.hpp>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#endif

#include <PR/PR.h>

std::filesystem::path getUserDataPath() {
    std::filesystem::path path;

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
    std::filesystem::path path;

#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if(length == 0) {
        prLogEvent(PR_EVENT_USER, PR_LOG_FATAL, "Failed to use \"GetModuleFileNameA\"  to get \"\" get execuable path");
        exit(EXIT_FAILURE);
    }
    path = buffer;
#else
    path = std::filesystem::current_path();
#endif

    return path;
}
