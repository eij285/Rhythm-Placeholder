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


# Library
add_library(notation
    src/notation/elements/musical_element.cpp
    src/notation/elements/pitch.cpp
    src/notation/elements/note.cpp
    src/notation/elements/chord.cpp
    src/notation/elements/rest.cpp

    src/notation/layout/score.cpp
    src/notation/layout/bar_content.cpp
    src/notation/layout/part.cpp
    src/notation/layout/stave.cpp
    src/notation/layout/bar_info.cpp
)

target_include_directories(notation
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)


# Tests
add_executable(tests
    tests/notation/pitch.test.cpp
    tests/notation/note.test.cpp
    tests/notation/chord.test.cpp
    tests/notation/rest.test.cpp
    tests/layout/bar_content.test.cpp
    tests/layout/bar_info.test.cpp
)

target_link_libraries(tests
    PRIVATE
        notation
        Catch2::Catch2WithMain
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Enable Catch2's test discovery
include(CTest)
include(Catch)
catch_discover_tests(tests)
