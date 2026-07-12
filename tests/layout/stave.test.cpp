#include <catch2/catch_test_macros.hpp>
#include "notation/layout/stave.hpp"
#include "notation/layout/score.hpp"

TEST_CASE("STAVE: Constructor creates empty bar_contents as expected") {
    auto stave = notation::Stave();
    CHECK(stave.get_bar_contents().empty());
}

TEST_CASE("STAVE: A stave added to an empty score has no bar contents") {
    auto score = notation::Score{};
    auto& part = score.add_part("Violin", 1);

    CHECK(part.get_staves()[0].get_bar_contents().empty());
}

TEST_CASE("STAVE: get_bar_contents() has one entry per add_new_bar() call") {
    auto score = notation::Score{};
    auto& part = score.add_part("Violin", 1);

    SECTION("One bar") {
        score.add_new_bar();
        CHECK(part.get_staves()[0].get_bar_contents().size() == 1);
    }

    SECTION("Multiple bars") {
        score.add_new_bar();
        score.add_new_bar();
        score.add_new_bar();
        CHECK(part.get_staves()[0].get_bar_contents().size() == 3);
    }
}

TEST_CASE("STAVE: get_bar_content(i) returns the correct BarContent at the given index") {
    auto score = notation::Score{};
    auto& part = score.add_part("Violin", 1);

    score.add_new_bar();               // bar 0: 4/4, total_duration = 4.0
    auto& bar1 = score.add_new_bar();  // bar 1: copies 4/4, total_duration = 4.0
    bar1.set_time_signature(3, 4);
    score.add_new_bar();               // bar 2: copies 3/4, total_duration = 3.0

    auto const& stave = part.get_staves()[0];

    CHECK(stave.get_bar_content(0).remaining_duration() == 4.0);
    CHECK(stave.get_bar_content(1).remaining_duration() == 3.0);
    CHECK(stave.get_bar_content(2).remaining_duration() == 3.0);
}

TEST_CASE("STAVE: get_bar_content(i) throws std::out_of_range for an invalid index") {
    auto score = notation::Score{};
    auto& part = score.add_part("Violin", 1);
    score.add_new_bar();

    auto const& stave = part.get_staves()[0];

    CHECK_THROWS_AS(stave.get_bar_content(1), std::out_of_range);
}
