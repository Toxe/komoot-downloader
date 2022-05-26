#include "api.hpp"

#include <stdexcept>

#include "fmt/core.h"

namespace komoot_downloader::komoot {

API::API(Connector& connector)
    : connector_(connector)
{
}

void API::show_error_message(const std::string& text, const RequestFailure& res) const
{
    if (res.error_message.empty())
        fmt::print("Error: {} (code: {})\n", text, res.status_code);
    else
        fmt::print("Error: {} (code: {}, message: {})\n", text, res.status_code, res.error_message);
}

bool API::login(const std::string& email, const std::string& password)
{
    const auto res = connector_.request(fmt::format("https://api.komoot.de/v006/account/email/{}/", email), email, password);

    if (std::holds_alternative<RequestFailure>(res)) {
        show_error_message("Login failed", std::get<RequestFailure>(res));
        return false;
    }

    const auto json = std::get<RequestSuccess>(res).json;

    user_id_ = json["username"];
    access_token_ = json["password"];

    return true;
}

std::optional<std::vector<Track>> API::fetch_tracks()
{
    std::string url = fmt::format("https://api.komoot.de/v007/users/{}/tours/", user_id_);
    std::vector<Track> tracks;

    while (!url.empty()) {
        const auto res = connector_.request(url, user_id_, access_token_);

        if (std::holds_alternative<RequestFailure>(res)) {
            show_error_message("Request failed", std::get<RequestFailure>(res));
            return std::nullopt;
        }

        const auto json = std::get<RequestSuccess>(res).json;

        if (!json.contains("_embedded") || !json["_embedded"].contains("tours"))
            throw std::runtime_error("missing data in section: _embedded -> tours");

        for (const auto& tour : json["_embedded"]["tours"]) {
            tracks.push_back(Track{tour.value("id", 0), tour.value("name", ""), tour.value("date", "")});
        }

        if (!json.contains("page") || !json["page"].contains("totalElements"))
            throw std::runtime_error("missing data in section: page");

        fmt::print("Fetched {} of {} tracks\n", tracks.size(), json["page"]["totalElements"].get<int>());

        if (json.contains("_links") && json["_links"].contains("next") && json["_links"]["next"].contains("href")) {
            url = json["_links"]["next"]["href"];
        } else {
            url = "";
        }
    }

    return tracks;
}

}  // namespace komoot_downloader::komoot
