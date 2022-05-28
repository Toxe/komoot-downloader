#include "connector.hpp"

#include "cpr/session.h"

namespace komoot_downloader::komoot {

class Connector::Impl {
public:
    [[nodiscard]] RequestResult request(const std::string& url, const std::string& auth_user, const std::string& auth_password);
    [[nodiscard]] DownloadResult download(const std::string& url, const std::string& auth_user, const std::string& auth_password);

private:
    [[nodiscard]] bool request_was_succesful(const long status_code) const;

private:
    cpr::Session session_;
};

RequestResult Connector::Impl::request(const std::string& url, const std::string& auth_user, const std::string& auth_password)
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

DownloadResult Connector::Impl::download(const std::string& url, const std::string& auth_user, const std::string& auth_password)
{
    session_.SetUrl(cpr::Url{url});
    session_.SetAuth(cpr::Authentication{auth_user, auth_password});
    const cpr::Response res = session_.Get();

    if (request_was_succesful(res.status_code))
        return DownloadSuccess{res.status_code, res.text};
    else
        return DownloadFailure{res.status_code};
}

bool Connector::Impl::request_was_succesful(const long status_code) const
{
    return status_code >= 200 && status_code <= 299;
}

Connector::Connector() : pimpl_(std::make_unique<Connector::Impl>()) { }
Connector::~Connector() = default;
RequestResult Connector::request(const std::string& url, const std::string& auth_user, const std::string& auth_password) { return pimpl_->request(url, auth_user, auth_password); }
DownloadResult Connector::download(const std::string& url, const std::string& auth_user, const std::string& auth_password) { return pimpl_->download(url, auth_user, auth_password); }

}  // namespace komoot_downloader::komoot
