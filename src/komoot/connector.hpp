#pragma once

#include <memory>
#include <string>
#include <variant>

#include "nlohmann/json.hpp"

namespace komoot_downloader::komoot {

struct RequestSuccess {
    int status_code;
    nlohmann::json json;
};

struct RequestFailure {
    int status_code;
    std::string error_message;
};

using RequestResult = std::variant<RequestSuccess, RequestFailure>;

class Connector {
public:
    Connector();
    virtual ~Connector();

    [[nodiscard]] virtual RequestResult request(const std::string& url, const std::string& auth_user, const std::string& auth_password);

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

}  // namespace komoot_downloader::komoot
