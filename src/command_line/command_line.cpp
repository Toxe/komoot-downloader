#include "command_line.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>

#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "fmt/core.h"

namespace komoot_downloader {

[[noreturn]] void show_usage_and_exit(const CLI::App& app, const char* error_message = nullptr, const std::optional<CLI::ParseError>& error = {})
{
    if (error_message)
        fmt::print("\n{}\n", error_message);

    std::exit(error ? app.exit(error.value()) : 0);
}

CommandLine::CommandLine(int argc, const char* argv[])
{
    const char* const description = "Komoot Downloader";

    directory_ = std::filesystem::current_path().string();

    CLI::App app{description};
    app.add_option("email", email_, "user email")->required();
    app.add_option("-d,--directory", directory_, fmt::format("download directory (default: {})", directory_));
    app.add_option("-p,--password", password_, "user password");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& error) {
        show_usage_and_exit(app, nullptr, error);
    }

    // check download directory
    if (!std::filesystem::exists(directory_))
        show_usage_and_exit(app, fmt::format("directory not found: {}", directory_).c_str(), {});

    if (!std::filesystem::is_directory(directory_))
        show_usage_and_exit(app, fmt::format("not a directory: {}", directory_).c_str(), {});

    // need password
    if (password_.empty()) {
        fmt::print("Password: ");
        std::getline(std::cin, password_);

        if (password_.empty())
            show_usage_and_exit(app, "please enter a password", {});
    }
}

}  // namespace komoot_downloader
