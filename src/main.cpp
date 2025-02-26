#include <iostream>
#include "Program.h"

#define TestMode
#ifdef TestMode
#include "TestModule/TestModule.h"
#endif

using namespace std;

int main() {
#ifdef TestMode

    TestModule testModule;
    testModule.Run();
#else
    Program program;
    program.Run();
#endif

  return 0;
}
