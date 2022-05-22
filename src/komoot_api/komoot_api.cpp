#include "komoot_api.hpp"

#include <stdexcept>
#include <vector>

#include "cpr/cpr.h"
#include "fmt/core.h"
#include "nlohmann/json.hpp"

auto KomootAPI::request(const std::string& url, const std::string& auth_user, const std::string& auth_password)
{
    session_.SetUrl(cpr::Url{url});
    session_.SetAuth(cpr::Authentication{auth_user, auth_password});
    cpr::Response res = session_.Get();

    auto json = nlohmann::json::parse(res.text);

    if (res.status_code != 200)
        throw std::runtime_error(fmt::format("request failed (code {}: {})", res.status_code, json["message"]));

    return json;
}

auto KomootAPI::request(const std::string& url)
{
    return request(url, user_id_, access_token_);
}

void KomootAPI::login(const std::string& email, const std::string& password)
{
    const auto json = request(fmt::format("https://api.komoot.de/v006/account/email/{}/", email), email, password);

    user_id_ = json["username"];
    access_token_ = json["password"];
}

std::vector<int> KomootAPI::fetch_track_ids()
{
    const auto json = request(fmt::format("https://api.komoot.de/v007/users/{}/tours/", user_id_));

    std::vector<int> ids;

    for (auto data : json["_embedded"]["tours"]) {
        int id = data["id"];
        ids.push_back(id);
    }

    return ids;
}
