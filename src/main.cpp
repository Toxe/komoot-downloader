#include <vector>

#include "fmt/core.h"

#include "command_line/command_line.hpp"
#include "komoot_api/komoot_api.hpp"

int main(int argc, char* argv[])
{
    CommandLine cli(argc, argv);
    KomootAPI komoot_api;

    fmt::print("Logging in...\n");
    komoot_api.login(cli.email(), cli.password());

    fmt::print("Fetching available tracks...\n");
    std::vector<int> track_ids = komoot_api.fetch_track_ids();

    for (int id : track_ids)
        fmt::print("{}, ", id);

    fmt::print("\n");
}
