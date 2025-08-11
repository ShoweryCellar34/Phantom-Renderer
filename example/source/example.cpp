#include <globals.hpp>

#include <PR/PR.h>
#include <setup.hpp>

int main(int argc, char** argv) {
    setupPaths();
    setLogFilePath(g_userDataPath / "prLog.txt");
    setupLog();

    setupWindow();
    setupShaders();
    shutdownShaders();
    shutdownWindow();

    shutdownLog();
    return 0;
}
