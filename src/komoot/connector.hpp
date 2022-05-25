#pragma once

#include <string>
#include <variant>

#include "cpr/session.h"
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
    [[nodiscard]] RequestResult request(const std::string& url, const std::string& auth_user, const std::string& auth_password);

private:
    cpr::Session session_;
};

}  // namespace komoot_downloader::komoot
