#include "komoot_api.hpp"

#include <stdexcept>
#include <vector>

#include "cpr/cpr.h"
#include "fmt/core.h"
#include "nlohmann/json.hpp"

void KomootAPI::login(const std::string& email, const std::string& password)
{
    session_.SetUrl(cpr::Url{fmt::format("https://api.komoot.de/v006/account/email/{}/", email)});
    session_.SetAuth(cpr::Authentication{email, password});
    cpr::Response res = session_.Get();

    const auto json = nlohmann::json::parse(res.text);

    if (res.status_code != 200)
        throw std::runtime_error(fmt::format("login failed (code {}: {})", res.status_code, json["message"]));

    user_id_ = json["username"];
    access_token_ = json["password"];
}

std::vector<int> KomootAPI::fetch_track_ids()
{
    session_.SetUrl(cpr::Url{fmt::format("https://api.komoot.de/v007/users/{}/tours/", user_id_)});
    session_.SetAuth(cpr::Authentication{user_id_, access_token_});
    cpr::Response res = session_.Get();

    const auto json = nlohmann::json::parse(res.text);

    if (res.status_code != 200)
        throw std::runtime_error(fmt::format("request failed (code {}: {})", res.status_code, json["message"]));

    std::vector<int> ids;

    for (auto data : json["_embedded"]["tours"]) {
        int id = data["id"];
        ids.push_back(id);
    }

    return ids;
}
