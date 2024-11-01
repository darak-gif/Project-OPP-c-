#include <iostream>
#include "CommandLine.h"

int main()
{
    CommandLine::getCommandLine().start(std::cin);

    return 0;
}