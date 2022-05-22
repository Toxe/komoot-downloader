#pragma once

#include <string>
#include <vector>

#include "cpr/session.h"

class KomootAPI {
public:
    void login(const std::string& email, const std::string& password);
    std::vector<int> fetch_track_ids();

private:
    std::string user_id_;
    std::string access_token_;

    cpr::Session session_;
};
