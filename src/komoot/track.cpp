#include "track.hpp"

#include <iomanip>
#include <sstream>
#include <utility>

#include "fmt/chrono.h"
#include "fmt/core.h"

namespace komoot_downloader::komoot {

Track::Track(int id, std::string name, const std::string& date)
    : id_{id}, name_(std::move(name))
{
    std::istringstream in{date};
    in >> std::get_time(&date_, "%Y-%m-%d");
}

std::string Track::filename() const
{
    return fmt::format("{:%Y-%m-%d}_{}_{}.gpx", date_, id_, name_);
}

}  // namespace komoot_downloader::komoot
