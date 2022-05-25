#include "api.hpp"

#include <stdexcept>

#include "fmt/core.h"

#include "connector.hpp"

namespace komoot_downloader::komoot {

API::API(Connector* connector)
    : connector_(connector)
{
}

nlohmann::json API::request(const std::string& url, const std::string& auth_user, const std::string& auth_password)
{
    auto res = connector_->request(url, auth_user, auth_password);

    if (std::holds_alternative<RequestFailure>(res))
        throw std::runtime_error(fmt::format("request failed (code {}: {})", std::get<RequestFailure>(res).status_code, std::get<RequestFailure>(res).error_message));

    return std::get<RequestSuccess>(res).json;
}

void API::login(const std::string& email, const std::string& password)
{
    const auto json = request(fmt::format("https://api.komoot.de/v006/account/email/{}/", email), email, password);

    user_id_ = json["username"];
    access_token_ = json["password"];
}

std::vector<Track> API::fetch_tracks()
{
    std::string url = fmt::format("https://api.komoot.de/v007/users/{}/tours/", user_id_);
    std::vector<Track> tracks;

    while (true) {
        const auto json = request(url, user_id_, access_token_);

        for (const auto& tour : json["_embedded"]["tours"]) {
            tracks.push_back(Track{tour["id"].get<int>(), tour["name"].get<std::string>(), tour["date"].get<std::string>()});
        }

        fmt::print("Fetched {} of {} tracks\n", tracks.size(), json["page"]["totalElements"].get<int>());

        if (!json["_links"].contains("next"))
            break;

        url = json["_links"]["next"]["href"];
    }

    return tracks;
}

}  // namespace komoot_downloader::komoot
