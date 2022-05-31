#include "catch2/catch_test_macros.hpp"

#include "track.hpp"

namespace komoot_downloader::komoot {

TEST_CASE("komoot::Track")
{
    SECTION("returns filename")
    {
        REQUIRE(Track{12345678, "Tour", "2022-10-20T10:38:24.001Z"}.filename() == "2022-10-20_12345678_Tour.gpx");
        REQUIRE(Track{12345678, "Tour", "2022-01-01T10:38:24.001Z"}.filename() == "2022-01-01_12345678_Tour.gpx");
    }
}

}  // namespace komoot_downloader::komoot
