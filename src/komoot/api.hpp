#pragma once

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "track.hpp"

namespace komoot_downloader::komoot {

class Connector;

class API {
public:
    explicit API(Connector* connector);

    void login(const std::string& email, const std::string& password);
    [[nodiscard]] std::vector<Track> fetch_tracks();

private:
    [[nodiscard]] nlohmann::json request(const std::string& url, const std::string& auth_user, const std::string& auth_password);

private:
    Connector* connector_;

    std::string user_id_;
    std::string access_token_;
};

}  // namespace komoot_downloader::komoot
