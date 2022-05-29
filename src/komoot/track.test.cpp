#include "catch2/catch_test_macros.hpp"

#include "track.hpp"

namespace komoot_downloader::komoot {

TEST_CASE("komoot::Track")
{
    SECTION("returns filename")
    {
        Track track{12345678, "Tour", "2022-05-29"};

        REQUIRE(track.filename() == "2022-05-29_12345678_Tour.gpx");
    }
}

}  // namespace komoot_downloader::komoot
