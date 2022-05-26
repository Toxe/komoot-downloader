#pragma once

#include <string>

namespace komoot_downloader {

class CommandLine {
public:
    CommandLine(int argc, const char* argv[]);

    [[nodiscard]] const std::string& email() const { return email_; }
    [[nodiscard]] const std::string& password() const { return password_; }
    [[nodiscard]] const std::string& directory() const { return directory_; }

private:
    std::string email_;
    std::string password_;
    std::string directory_;
};

}  // namespace komoot_downloader
