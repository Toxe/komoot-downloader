#include "command_line/command_line.hpp"

#include "fmt/core.h"

int main(int argc, char* argv[])
{
    CommandLine cli(argc, argv);

    fmt::print("directory: {}\n", cli.directory());
    fmt::print("email: {}\n", cli.email());
    fmt::print("password: {}\n", cli.password());
}
