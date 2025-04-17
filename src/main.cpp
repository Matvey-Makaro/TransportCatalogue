#include <iostream>
#include <exception>
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <csignal>

#include "Program.h"

using namespace std;

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
  fprintf(stderr, "Error: signal %d\n", sig);
  print_stacktrace();
  signal(sig, SIG_DFL);
  raise(sig);
}

int main(int argc, const char* argv[])
{
  signal(SIGSEGV, crash_handler);  // Segfault
  signal(SIGABRT, crash_handler);  // Assert/abort

  if (argc != 2) {
    cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
    return 5;
  }

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
