#pragma once

#include <string>

class CommandLine {
public:
    CommandLine(int argc, char* argv[]);

    [[nodiscard]] const std::string& email() const { return email_; }
    [[nodiscard]] const std::string& password() const { return password_; }
    [[nodiscard]] const std::string& directory() const { return directory_; }

private:
    std::string email_;
    std::string password_;
    std::string directory_;
};
