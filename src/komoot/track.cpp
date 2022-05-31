#include "track.hpp"

#include <chrono>
#include <sstream>
#include <utility>

#include "fmt/core.h"

namespace komoot_downloader::komoot {

Track::Track(int id, std::string name, const std::string& date)
    : id_{id}, name_(std::move(name))
{
    std::istringstream in{date};
    std::chrono::from_stream(in, "%F", date_);
}

std::string Track::filename() const
{
    return fmt::format("{}-{:02}-{:02}_{}_{}.gpx", (int) date_.year(), (unsigned) date_.month(), (unsigned) date_.day(), id_, name_);
}

}  // namespace komoot_downloader::komoot
