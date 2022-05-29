#pragma once

#include <string>

namespace komoot_downloader::komoot {

class Track {
public:
    Track(int id, std::string name, std::string date);

    [[nodiscard]] std::string filename() const;

private:
    int id_;
    std::string name_;
    std::string date_;
};

}  // namespace komoot_downloader::komoot
