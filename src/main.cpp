#include <locale>

#include "fmt/core.h"

#include "command_line/command_line.hpp"
#include "komoot_api/komoot_api.hpp"

int main(int argc, char* argv[])
{
    std::locale::global(std::locale("en_US.UTF-8"));

    CommandLine cli(argc, argv);
    KomootAPI komoot_api;

    fmt::print("Logging in...\n");
    komoot_api.login(cli.email(), cli.password());

    fmt::print("Fetching available tracks...\n");
    const auto tracks = komoot_api.fetch_tracks();

    for (const auto& track : tracks)
        fmt::print("[{}] Track {}: {}\n", track.date, track.id, track.name);
}
