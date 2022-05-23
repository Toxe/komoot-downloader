#include "api.hpp"

#include <stdexcept>

#include "cpr/cpr.h"
#include "fmt/core.h"
#include "nlohmann/json.hpp"

namespace komoot_downloader::komoot {

auto API::request(const std::string& url, const std::string& auth_user, const std::string& auth_password)
{
    session_.SetUrl(cpr::Url{url});
    session_.SetAuth(cpr::Authentication{auth_user, auth_password});
    cpr::Response res = session_.Get();

    auto json = nlohmann::json::parse(res.text);

    if (res.status_code != 200)
        throw std::runtime_error(fmt::format("request failed (code {}: {})", res.status_code, json["message"]));

    return json;
}

auto API::request(const std::string& url)
{
    return request(url, user_id_, access_token_);
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
        const auto json = request(url);

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
