#include <iostream>
#include <exception>
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <csignal>

#include "Program.h"
#include "ProgramState.h"

using namespace std;

static const char* mode = nullptr;

void print_stacktrace() {
  void* buffer[100];
  int frames = backtrace(buffer, 100);
  char** symbols = backtrace_symbols(buffer, frames);

  fprintf(stderr, "Stack trace:\n");
  for (int i = 0; i < frames; i++) {
    fprintf(stderr, "%s\n", symbols[i]);
  }

  free(symbols);
}

void crash_handler(int sig) {
  std::cerr << "Mode: " << mode << std::endl;
  std::cerr << "Curr func: " << ProgramState::GetCurrFunc() << std::endl;
  // fprintf(stderr, "Error: signal %d\n", sig);
  // print_stacktrace();
  signal(sig, SIG_DFL);
  raise(sig);
}

int main(int argc, const char* argv[])
{
  if (argc != 2) {
    cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
    return 5;
  }

  mode = argv[1];

  signal(SIGSEGV, crash_handler);  // Segfault
  signal(SIGABRT, crash_handler);  // Assert/abort

  try
  {
    const std::string mode(argv[1]);
    Program program(mode);
    program.Run();
  }
  catch (const std::exception& e)
  {
    cerr << e.what() << endl;
    return 1;
  }

  return 0;
}
