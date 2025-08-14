cmake_minimum_required(VERSION 3.15)
project(RhythmPlaceholder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Global warning settings
if(MSVC)
    add_compile_options(/W4 /WX) # Full warnings + treat warnings as errors
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()


# Dependencies: Catch2
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(catch2)


# Tests
add_executable(tests
    tests/test_example.cpp
)

target_link_libraries(tests
    PRIVATE
        Catch2::Catch2WithMain
)

# Mark Catch2 headers as SYSTEM to suppress warnings from external code
target_include_directories(tests SYSTEM PRIVATE
    ${catch2_SOURCE_DIR}/src
)

# Enable Catch2's test discovery
include(CTest)
include(Catch)
catch_discover_tests(tests)
