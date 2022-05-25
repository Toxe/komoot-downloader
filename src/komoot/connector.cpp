#include "connector.hpp"

#include "cpr/cpr.h"

namespace komoot_downloader::komoot {

RequestResult Connector::request(const std::string& url, const std::string& auth_user, const std::string& auth_password)
{
    session_.SetUrl(cpr::Url{url});
    session_.SetAuth(cpr::Authentication{auth_user, auth_password});
    cpr::Response res = session_.Get();

    auto json = nlohmann::json::parse(res.text);

    if (res.status_code >= 200 && res.status_code <= 299)
        return RequestSuccess{res.status_code, json};
    else
        return RequestFailure{res.status_code, json.contains("message") ? json["message"] : ""};
}

}  // namespace komoot_downloader::komoot
