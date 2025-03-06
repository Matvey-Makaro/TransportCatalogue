#pragma once

#include <iostream>
#include <string>

class Program
{
public:
    Program(std::string mode);
    void Run(std::istream& in = std::cin, std::ostream& out = std::cout);

private:
    void MakeBase(std::istream& in);
    void ProcessRequests(std::istream& in, std::ostream& out);

private:
    std::string _mode;
};
