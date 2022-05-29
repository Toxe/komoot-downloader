#include "track.hpp"

#include <utility>

#include "fmt/core.h"

namespace komoot_downloader::komoot {

Track::Track(int id, std::string name, std::string date)
    : id_{id}, name_(std::move(name)), date_(std::move(date))
{
}

std::string Track::filename() const
{
    return fmt::format("{}_{}_{}.gpx", date_, id_, name_);
}

}  // namespace komoot_downloader::komoot
