#include <catch2/catch_test_macros.hpp>
#include "notation/layout/part.hpp"
#include "notation/layout/score.hpp"

TEST_CASE("PART: Constructor works as expected") {
    SECTION("Default Constructor") {
        auto const part = notation::Part();

        CHECK(part.get_instrument() == "");
        CHECK(part.get_staves().size() == 1);
    }

    SECTION("Custom Constructor") {
        auto const part = notation::Part("Piano", 2);

        CHECK(part.get_instrument() == "Piano");
        CHECK(part.get_staves().size() == 2);
    }
}

TEST_CASE("PART: add_part() stores the correct instrument name") {
    auto score = notation::Score{};
    auto& part = score.add_part("Flute", 1);

    CHECK(part.get_instrument() == "Flute");
}

TEST_CASE("PART: add_part() creates the correct number of staves") {
    auto score = notation::Score{};

    SECTION("Single stave instrument") {
        auto& part = score.add_part("Trumpet", 1);
        CHECK(part.get_staves().size() == 1);
    }

    SECTION("Grand staff") {
        auto& part = score.add_part("Piano", 2);
        CHECK(part.get_staves().size() == 2);
    }
}
