#include <iostream>
#include "Program.h"

#ifdef RunTests
#include "TestModule/TestModule.h"
#endif

using namespace std;

int main()
{
#ifdef RunTests
  TestModule testModule;
  testModule.Run();
#else
  Program program;
  program.Run();
#endif

  return 0;
}
