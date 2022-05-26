#include "connector.hpp"

#include "cpr/cpr.h"

namespace komoot_downloader::komoot {

RequestResult Connector::request(const std::string& url, const std::string& auth_user, const std::string& auth_password)
{
    session_.SetUrl(cpr::Url{url});
    session_.SetAuth(cpr::Authentication{auth_user, auth_password});
    const cpr::Response res = session_.Get();

    auto json = nlohmann::json::parse(res.text);

    if (request_was_succesful(res.status_code))
        return RequestSuccess{res.status_code, json};
    else
        return RequestFailure{res.status_code, json.value("message", "")};
}

bool Connector::request_was_succesful(const int status_code) const
{
    return status_code >= 200 && status_code <= 299;
}

}  // namespace komoot_downloader::komoot
