#pragma once

#include <string>
#include <vector>

#include "cpr/session.h"

#include "track.hpp"

class KomootAPI {
public:
    void login(const std::string& email, const std::string& password);
    [[nodiscard]] std::vector<Track> fetch_tracks();

private:
    [[nodiscard]] auto request(const std::string& url, const std::string& auth_user, const std::string& auth_password);
    [[nodiscard]] auto request(const std::string& url);

private:
    std::string user_id_;
    std::string access_token_;

    cpr::Session session_;
};
