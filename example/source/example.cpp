#include <globals.hpp>

#include <PR/PR.h>
#include <setup.hpp>

int main(int argc, char** argv) {
    setupPaths();
    setupLog(userDataPath / "prLog.txt");

    shutdownLog();
    return 0;
}
