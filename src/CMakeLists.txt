add_executable(
        komoot_downloader
        main.cpp
        command_line/command_line.cpp
        command_line/command_line.hpp
        komoot_api/komoot_api.cpp
        komoot_api/komoot_api.hpp
        komoot_api/track.hpp
)

set_target_properties(komoot_downloader PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(komoot_downloader PUBLIC cxx_std_20)
target_compile_options(komoot_downloader PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
target_link_libraries(komoot_downloader PRIVATE ${SANITIZER_FLAGS} CLI11::CLI11 cpr::cpr fmt::fmt nlohmann_json::nlohmann_json)

# Tests
if (BUILD_TESTING)
    add_executable(
            komoot_downloader_tests
            tests.cpp
    )

    set_target_properties(komoot_downloader_tests PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_features(komoot_downloader_tests PUBLIC cxx_std_20)
    target_compile_options(komoot_downloader_tests PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
    target_link_libraries(komoot_downloader_tests PRIVATE ${SANITIZER_FLAGS} Catch2::Catch2 Catch2::Catch2WithMain)

    add_test(NAME komoot_downloader_tests COMMAND komoot_downloader_tests)
endif ()
