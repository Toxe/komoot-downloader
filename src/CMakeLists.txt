add_executable(
        komoot_downloader
        main.cpp
        command_line/command_line.cpp
        command_line/command_line.hpp
        komoot/api.cpp
        komoot/api.hpp
        komoot/connector.cpp
        komoot/connector.hpp
        komoot/track.hpp
)

set_target_properties(komoot_downloader PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(komoot_downloader PUBLIC cxx_std_20)
target_compile_options(komoot_downloader PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
target_link_libraries(komoot_downloader PRIVATE ${SANITIZER_FLAGS} CLI11::CLI11 cpr::cpr fmt::fmt nlohmann_json::nlohmann_json)

# Tests
if (BUILD_TESTING)
    add_executable(
            komoot_downloader_tests
            komoot/api.cpp
            komoot/api.hpp
            komoot/api.test.cpp
            komoot/connector.cpp
            komoot/connector.hpp
            komoot/track.hpp
    )

    set_target_properties(komoot_downloader_tests PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_features(komoot_downloader_tests PUBLIC cxx_std_20)
    target_compile_options(komoot_downloader_tests PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
    target_include_directories(komoot_downloader_tests PRIVATE ${FAKEIT_INCLUDE_DIRS})
    target_link_libraries(komoot_downloader_tests PRIVATE ${SANITIZER_FLAGS} cpr::cpr fmt::fmt nlohmann_json::nlohmann_json Catch2::Catch2WithMain)

    catch_discover_tests(komoot_downloader_tests)
endif ()
