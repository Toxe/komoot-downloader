#pragma once

#include <memory>
#include <string>
#include <variant>

#include "nlohmann/json.hpp"

namespace komoot_downloader::komoot {

// RequestResult
struct RequestSuccess {
    long status_code;
    nlohmann::json json;
};

struct RequestFailure {
    long status_code;
    std::string error_message;
};

using RequestResult = std::variant<RequestSuccess, RequestFailure>;

// DownloadResult
struct DownloadSuccess {
    long status_code;
    std::string text;
};

struct DownloadFailure {
    long status_code;
};

using DownloadResult = std::variant<DownloadSuccess, DownloadFailure>;

class Connector {
public:
    Connector();
    virtual ~Connector();

    [[nodiscard]] virtual RequestResult request(const std::string& url, const std::string& auth_user, const std::string& auth_password);
    [[nodiscard]] virtual DownloadResult download(const std::string& url, const std::string& auth_user, const std::string& auth_password);

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

}  // namespace komoot_downloader::komoot
