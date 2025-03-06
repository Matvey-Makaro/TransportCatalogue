#include <iostream>
#include "Program.h"

#ifdef RunTests
#include "TestModule/TestModule.h"
#endif

using namespace std;

int main(int argc, const char* argv[])
{
#ifdef RunTests
  TestModule testModule;
  testModule.Run();
#else
  if (argc != 2) {
    cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
    return 5;
  }

  const std::string mode(argv[1]);
  Program program(mode);
  program.Run();
#endif

  return 0;
}
