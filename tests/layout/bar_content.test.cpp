#include <catch2/catch_test_macros.hpp>
#include "notation/layout/bar_content.hpp"
#include "notation/elements/pitch.hpp"
#include "notation/elements/note.hpp"
#include "notation/elements/rest.hpp"
#include "notation/elements/chord.hpp"

TEST_CASE("BAR CONTENT: Constructor sets correct private field members") {
    auto total_duration = 4;
    notation::BarContent bar_content{total_duration};

    CHECK(bar_content.get_elements().empty());
    CHECK(bar_content.remaining_duration() == total_duration);
    CHECK(bar_content.empty() == true);
}

TEST_CASE("BAR_CONTENT: get_elements() returns elements in correct order") {
    
}

TEST_CASE("BAR_CONTENT: remaining_duration() updates correctly after adding elements") {
    auto total_duration = 4;
    notation::BarContent bar_content{total_duration};

    auto C4 = notation::Pitch{notation::PitchName::C, 4};
    auto E4 = notation::Pitch{notation::PitchName::E, 4};

    SECTION("remaining_duration() updates correctly upon adding a rest") {
        auto crotchet_rest = std::make_unique<notation::Rest>(1.0);

        REQUIRE(bar_content.try_add(crotchet_rest));
        CHECK(bar_content.remaining_duration() == 3.0);
    }

    SECTION("remaining_duration() updates correctly upon adding a note") {
        auto minim_C = std::make_unique<notation::Note>(2.0, C4);

        REQUIRE(bar_content.try_add(minim_C));
        CHECK(bar_content.remaining_duration() == 2.0);
    }

    SECTION("remaining_duration() updates correctly upon adding a chord") {
        auto semibreve_CM = std::make_unique<notation::Chord>(4.0, std::initializer_list<notation::Pitch>{C4, E4});
        
        REQUIRE(bar_content.try_add(semibreve_CM));
        CHECK(semibreve_CM.remaining_duration() == 0.0);
    }
}

TEST_CASE("BAR CONTENT: empty() works as expected") {
    auto total_duration = 3;
    notation::BarContent bar_content{total_duration};

    CHECK(bar_content.empty());

    auto crotchet_rest = std::make_unique<notation::Rest>(1.0);
    CHECK(!bar_content.empty());
}

TEST_CASE("BAR CONTENT: try_add() attempts to add elements to the end of the bar, given valid duration") {
    auto total_duration = 3;
    notation::BarContent bar_content{total_duration};


}