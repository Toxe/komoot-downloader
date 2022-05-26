#pragma once

#include <optional>
#include <string>
#include <vector>

#include "connector.hpp"
#include "track.hpp"

namespace komoot_downloader::komoot {

class Connector;

class API {
public:
    explicit API(Connector& connector);

    [[nodiscard]] bool login(const std::string& email, const std::string& password);
    [[nodiscard]] std::optional<std::vector<Track>> fetch_tracks();

private:
    void show_error_message(const std::string& text, const RequestFailure& res) const;

private:
    Connector& connector_;

    std::string user_id_;
    std::string access_token_;
};

}  // namespace komoot_downloader::komoot
