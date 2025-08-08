#include <files.hpp>

#include <PR/PR.h>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#endif

std::filesystem::path getUserDataPath() {
    std::filesystem::path path;

#ifdef _WIN32
    PWSTR tempPath;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &tempPath);
    if(SUCCEEDED(hr)) {
        path = tempPath;
        CoTaskMemFree(tempPath);
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to get user data path");
        return std::filesystem::path();
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
        prLogEvent(PR_EVENT_USER, PR_LOG_FATAL, "Failed to get execuable path");
        exit(EXIT_FAILURE);
    }
    path = buffer;
#endif

    return path;
}
