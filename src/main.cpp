#include <locale>

#include "fmt/core.h"

#include "command_line/command_line.hpp"
#include "komoot/api.hpp"
#include "komoot/connector.hpp"

int main(int argc, const char* argv[])
{
    std::locale::global(std::locale("en_US.UTF-8"));

    komoot_downloader::CommandLine cli(argc, argv);

    komoot_downloader::komoot::Connector connector;
    komoot_downloader::komoot::API komoot_api(connector);

    fmt::print("Logging in...\n");

    if (!komoot_api.login(cli.email(), cli.password()))
        return 1;

    fmt::print("Fetching available tracks...\n");

    const auto tracks = komoot_api.fetch_tracks();

    if (!tracks)
        return 2;

    for (const auto& track : tracks.value())
        fmt::print("[{}] Track {}: {}\n", track.date, track.id, track.name);
}
