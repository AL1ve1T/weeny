#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/////////////////////////
#include "run_tests.c"
#include "server.c"

typedef enum { TEST, RUN, TEST_AND_RUN } RUN_MODE;

char* Print() {
    return "Hello, World!\n";
}

RUN_MODE StrToMode(char* mode) {
    if (strcmp(mode, "test") == 0)
        return TEST;
    else if (strcmp(mode, "test_and_run") == 0)
        return TEST_AND_RUN;
    else
        return RUN;
}

RUN_MODE CheckRunMode(int argc, char *argv[]) {
    bool foundFlag = false;
    int flagPos;
    for (int i = 0; i < argc; i++)
        if (strcmp(argv[i], "--mode") == 0) {
            foundFlag = true;
            flagPos = i;
        }
    if (foundFlag && flagPos < argc - 1)
        return StrToMode(argv[flagPos - 1]);
    else
        return RUN;
}

int main(int argc, char *argv[]) {
    RUN_MODE runMode = CheckRunMode(argc, argv);
    if (runMode == TEST) {
        RunTests();
        return 0;
    } else if (runMode == TEST_AND_RUN) {
        RunTests();
        RunServer();
        return 0;
    } else
        RunServer();
    return 0;
}
