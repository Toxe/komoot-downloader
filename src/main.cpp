#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <locale>
#include <vector>

#include "fmt/core.h"

#include "command_line/command_line.hpp"
#include "komoot/api.hpp"
#include "komoot/connector.hpp"

[[noreturn]] void die(int error_code, const std::string_view& error_message)
{
    fmt::print("{}\n", error_message);
    std::exit(error_code);
}

void login(komoot_downloader::komoot::API& komoot_api, const komoot_downloader::CommandLine& cli)
{
    fmt::print("Logging in...\n");

    if (!komoot_api.login(cli.email(), cli.password()))
        die(1, "login failed");
}

std::vector<komoot_downloader::komoot::Track> fetch_available_tracks(komoot_downloader::komoot::API& komoot_api)
{
    fmt::print("Fetching available tracks...\n");

    const auto tracks = komoot_api.fetch_tracks();

    if (!tracks)
        die(2, "unable to fetch available tracks");

    fmt::print("Fetched {} tracks\n", tracks->size());

    return tracks.value();
}

void download_tracks(komoot_downloader::komoot::API& komoot_api, const komoot_downloader::CommandLine& cli, const std::vector<komoot_downloader::komoot::Track>& tracks)
{
    fmt::print("saving files in: {}\n", cli.directory());

    int num_downloaded = 0;
    int num_already_exist = 0;

    for (const auto& track : tracks) {
        const auto file_path = std::filesystem::path(cli.directory()) / track.filename();

        fmt::print("[{}/{}] downloading track {}\n", num_downloaded + num_already_exist + 1, tracks.size(), track.filename());

        if (std::filesystem::exists(file_path)) {
            ++num_already_exist;
            continue;
        }

        const auto file_content = komoot_api.download_track(track);

        if (!file_content)
            die(3, "download error");

        std::ofstream out{file_path, std::ofstream::binary};

        if (!out.is_open())
            die(4, fmt::format("unable to save file: {}", file_path.string()));

        out << file_content.value();

        ++num_downloaded;

        break;
    }

    fmt::print("total files: {}\nfiles downloaded: {}\nalready downloaded: {}\n", num_downloaded + num_already_exist, num_downloaded, num_already_exist);
}

int main(int argc, const char* argv[])
{
    std::locale::global(std::locale("en_US.UTF-8"));

    komoot_downloader::CommandLine cli(argc, argv);

    komoot_downloader::komoot::Connector connector;
    komoot_downloader::komoot::API komoot_api(connector);

    login(komoot_api, cli);
    const auto tracks = fetch_available_tracks(komoot_api);
    download_tracks(komoot_api, cli, tracks);
}
