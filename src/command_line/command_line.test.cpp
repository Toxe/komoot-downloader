#include "catch2/catch_test_macros.hpp"

#include <array>

#include "command_line.hpp"

namespace komoot_downloader {

TEST_CASE("CommandLine")
{
    SECTION("accept email and password")
    {
        auto args = std::to_array({"command", "test@example.com", "-p", "password"});

        const CommandLine cli(static_cast<int>(args.size()), args.data());

        REQUIRE(cli.email() == args[1]);
        REQUIRE(cli.password() == args[3]);
    }

    SECTION("accept directory")
    {
        auto args = std::to_array({"command", "test@example.com", "-p", "password", "-d", "."});

        const CommandLine cli(static_cast<int>(args.size()), args.data());

        REQUIRE(cli.directory() == args[5]);
    }
}

}  // namespace komoot_downloader
