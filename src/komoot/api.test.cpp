#include "catch/fakeit.hpp"
#include "catch2/catch_test_macros.hpp"

#include "nlohmann/json.hpp"

#include "api.hpp"
#include "connector.hpp"

namespace komoot_downloader::komoot {

TEST_CASE("komoot::API login")
{
    SECTION("success with good credentials")
    {
        const nlohmann::json json = {
            {"username", "1234"},
            {"password", "????"}};

        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request)).Return(RequestSuccess{200, json});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "good_password"));
    }

    SECTION("failure with bad credentials")
    {
        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request)).Return(RequestFailure{403, "Unknown user or wrong credentials."});

        API api(mock_connector.get());

        REQUIRE(not api.login("test@example.com", "bad_password"));
    }
}

TEST_CASE("komoot::API fetch tracks")
{
    const nlohmann::json json_login = {
        {"username", "1234"},
        {"password", "????"}};

    SECTION("fetch single list of tracks")
    {
        const nlohmann::json json_tracks =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1001,
                                "name": "Tour 1"
                            },
                            {
                                "date": "2022-05-22T10:38:24.001Z",
                                "id": 1002,
                                "name": "Tour 2"
                            },
                            {
                                "date": "2022-05-23T10:38:24.001Z",
                                "id": 1003,
                                "name": "Tour 3"
                            }
                        ]
                    },
                    "_links": {
                        "self": {
                            "href": "/v007/users/1234/tours/"
                        }
                    },
                    "page": {
                        "number": 0,
                        "size": 3,
                        "totalElements": 3,
                        "totalPages": 1
                    }
                }
            )"_json;

        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request))
            .Return(RequestSuccess{200, json_login})
            .Return(RequestSuccess{200, json_tracks});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        const auto tracks = api.fetch_tracks();

        REQUIRE(tracks.has_value());
        REQUIRE(tracks->size() == 3);
    }

    SECTION("fetch multiple lists of tracks")
    {
        const nlohmann::json json_tracks1 =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1001,
                                "name": "Tour 1"
                            },
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1002,
                                "name": "Tour 2"
                            }
                        ]
                    },
                    "_links": {
                        "next": {
                            "href": "/v007/users/1234/tours/?page=1&limit=2"
                        },
                        "self": {
                            "href": "/v007/users/1234/tours/"
                        }
                    },
                    "page": {
                        "number": 0,
                        "size": 2,
                        "totalElements": 6,
                        "totalPages": 3
                    }
                }
            )"_json;

        const nlohmann::json json_tracks2 =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1003,
                                "name": "Tour 3"
                            },
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1004,
                                "name": "Tour 4"
                            }
                        ]
                    },
                    "_links": {
                        "next": {
                            "href": "/v007/users/1234/tours/?page=2&limit=2"
                        },
                        "prev": {
                            "href": "/v007/users/1234/tours/"
                        },
                        "self": {
                            "href": "/v007/users/1234/tours/?page=1&limit=2"
                        }
                    },
                    "page": {
                        "number": 1,
                        "size": 2,
                        "totalElements": 6,
                        "totalPages": 3
                    }
                }
            )"_json;

        const nlohmann::json json_tracks3 =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1005,
                                "name": "Tour 5"
                            },
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1006,
                                "name": "Tour 6"
                            }
                        ]
                    },
                    "_links": {
                        "prev": {
                            "href": "/v007/users/1234/tours/?page=1&limit=2"
                        },
                        "self": {
                            "href": "/v007/users/1234/tours/?page=2&limit=2"
                        }
                    },
                    "page": {
                        "number": 2,
                        "size": 2,
                        "totalElements": 6,
                        "totalPages": 3
                    }
                }
            )"_json;

        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request))
            .Return(RequestSuccess{200, json_login})
            .Return(RequestSuccess{200, json_tracks1})
            .Return(RequestSuccess{200, json_tracks2})
            .Return(RequestSuccess{200, json_tracks3});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        const auto tracks = api.fetch_tracks();

        REQUIRE(tracks.has_value());
        REQUIRE(tracks->size() == 6);
    }

    SECTION("can handle request error")
    {
        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request))
            .Return(RequestSuccess{200, json_login})
            .Return(RequestFailure{403, "Access is denied"});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        const auto tracks = api.fetch_tracks();

        REQUIRE(!tracks);
    }

    SECTION("can handle missing track data")
    {
        const nlohmann::json json_tracks_bad_data =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "id": 1001,
                                "name": "Tour 1"
                            },
                            {
                                "date": "2022-05-22T10:38:24.001Z",
                                "name": "Tour 2"
                            },
                            {
                                "date": "2022-05-23T10:38:24.001Z",
                                "id": 1003
                            }
                        ]
                    },
                    "_links": {
                        "self": {
                            "href": "/v007/users/1234/tours/"
                        }
                    },
                    "page": {
                        "number": 0,
                        "size": 3,
                        "totalElements": 3,
                        "totalPages": 1
                    }
                }
            )"_json;

        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request))
            .Return(RequestSuccess{200, json_login})
            .Return(RequestSuccess{200, json_tracks_bad_data});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        const auto tracks = api.fetch_tracks();

        REQUIRE(tracks.has_value());
        REQUIRE(tracks->size() == 3);
    }

    SECTION("can handle missing links")
    {
        const nlohmann::json json_tracks_bad_data1 =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1001,
                                "name": "Tour 1"
                            },
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1002,
                                "name": "Tour 2"
                            }
                        ]
                    },
                    "page": {
                        "number": 0,
                        "size": 2,
                        "totalElements": 6,
                        "totalPages": 3
                    }
                }
            )"_json;

        const nlohmann::json json_tracks_bad_data2 =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1003,
                                "name": "Tour 3"
                            },
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1004,
                                "name": "Tour 4"
                            }
                        ]
                    },
                    "_links": {
                    },
                    "page": {
                        "number": 1,
                        "size": 2,
                        "totalElements": 6,
                        "totalPages": 3
                    }
                }
            )"_json;

        const nlohmann::json json_tracks_bad_data3 =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1005,
                                "name": "Tour 5"
                            },
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1006,
                                "name": "Tour 6"
                            }
                        ]
                    },
                    "_links": {
                        "next": {
                        },
                        "self": {
                            "href": "/v007/users/1234/tours/?page=1&limit=2"
                        }
                    },
                    "page": {
                        "number": 2,
                        "size": 2,
                        "totalElements": 6,
                        "totalPages": 3
                    }
                }
            )"_json;

        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request))
            .Return(RequestSuccess{200, json_login})
            .Return(RequestSuccess{200, json_tracks_bad_data1})
            .Return(RequestSuccess{200, json_tracks_bad_data2})
            .Return(RequestSuccess{200, json_tracks_bad_data3});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        REQUIRE(api.fetch_tracks()->size() == 2);
        REQUIRE(api.fetch_tracks()->size() == 2);
        REQUIRE(api.fetch_tracks()->size() == 2);
    }

    SECTION("throw exception on missing tours data")
    {
        const nlohmann::json json_tracks_bad_data1 =
            R"(
                {
                    "_links": {
                        "self": {
                            "href": "/v007/users/1234/tours/"
                        }
                    },
                    "page": {
                        "number": 0,
                        "size": 3,
                        "totalElements": 3,
                        "totalPages": 1
                    }
                }
            )"_json;

        const nlohmann::json json_tracks_bad_data2 =
            R"(
                {
                    "_embedded": {
                    },
                    "_links": {
                        "self": {
                            "href": "/v007/users/1234/tours/"
                        }
                    },
                    "page": {
                        "number": 0,
                        "size": 3,
                        "totalElements": 3,
                        "totalPages": 1
                    }
                }
            )"_json;

        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request))
            .Return(RequestSuccess{200, json_login})
            .Return(RequestSuccess{200, json_tracks_bad_data1})
            .Return(RequestSuccess{200, json_tracks_bad_data2});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        REQUIRE_THROWS(api.fetch_tracks());
        REQUIRE_THROWS(api.fetch_tracks());
    }

    SECTION("throw exception on missing page data")
    {
        const nlohmann::json json_tracks_bad_data1 =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1001,
                                "name": "Tour 1"
                            },
                            {
                                "date": "2022-05-22T10:38:24.001Z",
                                "id": 1002,
                                "name": "Tour 2"
                            },
                            {
                                "date": "2022-05-23T10:38:24.001Z",
                                "id": 1003,
                                "name": "Tour 3"
                            }
                        ]
                    },
                    "_links": {
                        "self": {
                            "href": "/v007/users/1234/tours/"
                        }
                    }
                }
            )"_json;

        const nlohmann::json json_tracks_bad_data2 =
            R"(
                {
                    "_embedded": {
                        "tours": [
                            {
                                "date": "2022-05-21T10:38:24.001Z",
                                "id": 1001,
                                "name": "Tour 1"
                            },
                            {
                                "date": "2022-05-22T10:38:24.001Z",
                                "id": 1002,
                                "name": "Tour 2"
                            },
                            {
                                "date": "2022-05-23T10:38:24.001Z",
                                "id": 1003,
                                "name": "Tour 3"
                            }
                        ]
                    },
                    "_links": {
                        "self": {
                            "href": "/v007/users/1234/tours/"
                        }
                    },
                    "page": {
                    }
                }
            )"_json;

        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request))
            .Return(RequestSuccess{200, json_login})
            .Return(RequestSuccess{200, json_tracks_bad_data1})
            .Return(RequestSuccess{200, json_tracks_bad_data2});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        REQUIRE_THROWS(!api.fetch_tracks());
        REQUIRE_THROWS(!api.fetch_tracks());
    }
}

TEST_CASE("komoot::API download GPX files")
{
    const std::string downloaded_track_xml =
        R"(<?xml version='1.0' encoding='UTF-8'?>
<gpx version="1.1" creator="https://www.komoot.de" xmlns="http://www.topografix.com/GPX/1/1" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
  <metadata>
    <name>Tour</name>
    <author>
      <link href="https://www.komoot.de">
        <text>komoot</text>
        <type>text/html</type>
      </link>
    </author>
  </metadata>
  <trk>
    <name>Tour</name>
    <trkseg>
      <trkpt lat="51.420253" lon="9.648904">
        <ele>121.390325</ele>
        <time>2021-09-03T08:44:35.004Z</time>
      </trkpt>
      <trkpt lat="51.420397" lon="9.648587">
        <ele>121.390325</ele>
        <time>2021-09-03T08:45:10.999Z</time>
      </trkpt>
      <trkpt lat="51.420372" lon="9.648385">
        <ele>121.390325</ele>
        <time>2021-09-03T08:45:13.999Z</time>
      </trkpt>
    </trkseg>
  </trk>
</gpx>)";

    const nlohmann::json json_login = {
        {"username", "1234"},
        {"password", "????"}};

    SECTION("successful download")
    {
        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request)).Return(RequestSuccess{200, json_login});
        fakeit::When(Method(mock_connector, download)).Return(DownloadSuccess{200, downloaded_track_xml});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        const Track track{1234, "Tour", "2022-05-21T10:38:24.001Z"};

        REQUIRE(api.download_track(track).has_value());
    }

    SECTION("failed download")
    {
        fakeit::Mock<Connector> mock_connector;
        fakeit::When(Method(mock_connector, request)).Return(RequestSuccess{200, json_login});
        fakeit::When(Method(mock_connector, download)).Return(DownloadFailure{404});

        API api(mock_connector.get());

        REQUIRE(api.login("test@example.com", "password"));

        const Track track{1234, "Tour", "2022-05-21T10:38:24.001Z"};

        REQUIRE(!api.download_track(track));
    }
}

}  // namespace komoot_downloader::komoot
