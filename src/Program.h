#pragma once

#include <iostream>

class Program
{
public:
    Program();
    void Run(std::istream& in = std::cin, std::ostream& out = std::cout);
};
