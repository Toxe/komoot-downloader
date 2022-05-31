#pragma once

#include <chrono>
#include <string>

namespace komoot_downloader::komoot {

class Track {
public:
    Track(int id, std::string name, const std::string& date);

    [[nodiscard]] int id() const { return id_; }
    [[nodiscard]] std::string filename() const;

private:
    int id_;
    std::string name_;
    std::chrono::year_month_day date_;
};

}  // namespace komoot_downloader::komoot
