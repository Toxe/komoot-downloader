#pragma once

#include <optional>
#include <string>
#include <vector>

#include "track.hpp"

namespace komoot_downloader::komoot {

class Connector;

class API {
public:
    explicit API(Connector& connector);

    [[nodiscard]] bool login(const std::string& email, const std::string& password);
    [[nodiscard]] std::optional<std::vector<Track>> fetch_tracks();
    [[nodiscard]] std::optional<std::string> download_track(const Track& track);

private:
    Connector& connector_;

    std::string user_id_;
    std::string access_token_;
};

}  // namespace komoot_downloader::komoot
