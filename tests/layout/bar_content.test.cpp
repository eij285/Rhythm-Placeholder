#include <catch2/catch_test_macros.hpp>
#include "notation/layout/bar_content.hpp"
#include "notation/elements/pitch.hpp"
#include "notation/elements/note.hpp"
#include "notation/elements/rest.hpp"
#include "notation/elements/chord.hpp"

TEST_CASE("BAR CONTENT: Constructor sets correct private field members") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};

    CHECK(bar_content.get_entries().empty());
    CHECK(bar_content.remaining_duration(total_duration) == total_duration);
    CHECK(bar_content.empty() == true);
}

TEST_CASE("BAR CONTENT: get_entries() returns entries in insertion order with correct onsets") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};

    auto F4 = notation::Pitch{notation::PitchName::F, 4};
    auto E4 = notation::Pitch{notation::PitchName::E, 4};
    auto D4 = notation::Pitch{notation::PitchName::D, 4};

    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(F4, 2.0), total_duration));
    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(E4, 1.0), total_duration));
    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(D4, 1.0), total_duration));

    auto const& entries = bar_content.get_entries();
    REQUIRE(entries.size() == 3);

    auto* first  = dynamic_cast<notation::Note*>(entries[0].element.get());
    auto* second = dynamic_cast<notation::Note*>(entries[1].element.get());
    auto* third  = dynamic_cast<notation::Note*>(entries[2].element.get());

    REQUIRE(first  != nullptr);
    REQUIRE(second != nullptr);
    REQUIRE(third  != nullptr);

    CHECK(first->get_pitch()  == F4);
    CHECK(second->get_pitch() == E4);
    CHECK(third->get_pitch()  == D4);

    CHECK(entries[0].onset == 0.0);
    CHECK(entries[1].onset == 2.0);
    CHECK(entries[2].onset == 3.0);
}

TEST_CASE("BAR CONTENT: remaining_duration() updates correctly after adding elements") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};

    auto C4 = notation::Pitch{notation::PitchName::C, 4};
    auto E4 = notation::Pitch{notation::PitchName::E, 4};

    SECTION("remaining_duration() updates correctly upon adding a rest") {
        REQUIRE(bar_content.try_add(std::make_unique<notation::Rest>(1.0), total_duration));
        CHECK(bar_content.remaining_duration(total_duration) == 3.0);
    }

    SECTION("remaining_duration() updates correctly upon adding a note") {
        REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 2.0), total_duration));
        CHECK(bar_content.remaining_duration(total_duration) == 2.0);
    }

    SECTION("remaining_duration() updates correctly upon adding a chord") {
        REQUIRE(bar_content.try_add(std::make_unique<notation::Chord>(std::vector<notation::Pitch>{C4, E4}, 4.0), total_duration));
        CHECK(bar_content.remaining_duration(total_duration) == 0.0);
    }
}

TEST_CASE("BAR CONTENT: empty() works as expected") {
    auto const total_duration = 3.0;
    notation::BarContent bar_content{};

    CHECK(bar_content.empty());

    bar_content.try_add(std::make_unique<notation::Rest>(1.0), total_duration);

    CHECK(!bar_content.empty());
}

TEST_CASE("BAR CONTENT: try_add() attempts to add elements to the end of the bar, given valid duration") {
    auto const total_duration = 3.0;
    notation::BarContent bar_content{};

    auto C1 = notation::Pitch{notation::PitchName::C, 1};
    auto D1 = notation::Pitch{notation::PitchName::D, 1};

    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C1, 2.0), total_duration));

    CHECK(!bar_content.try_add(std::make_unique<notation::Note>(D1, 2.0), total_duration));
    CHECK(bar_content.try_add(std::make_unique<notation::Note>(D1, 1.0), total_duration));
}

TEST_CASE("BAR CONTENT: clear() successfully empties the bar") {
    auto const total_duration = 3.0;
    notation::BarContent bar_content{};

    auto C1 = notation::Pitch{notation::PitchName::C, 1};
    auto D1 = notation::Pitch{notation::PitchName::D, 1};

    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C1, 2.0), total_duration));
    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(D1, 1.0), total_duration));

    bar_content.clear();

    CHECK(bar_content.get_entries().empty());
    CHECK(bar_content.remaining_duration(total_duration) == total_duration);
    CHECK(bar_content.empty() == true);
}

TEST_CASE("BAR CONTENT: remove_at() returns the original element with correct type and duration") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};
    auto C4 = notation::Pitch{notation::PitchName::C, 4};

    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 2.0), total_duration));

    auto original = bar_content.remove_at(0);

    REQUIRE(original != nullptr);
    auto* note = dynamic_cast<notation::Note*>(original.get());
    REQUIRE(note != nullptr);
    CHECK(note->get_pitch() == C4);
    CHECK(note->get_duration() == 2.0);
}

TEST_CASE("BAR CONTENT: remove_at() on last element in voice erases entry and shrinks cursor") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};
    auto C4 = notation::Pitch{notation::PitchName::C, 4};

    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 2.0), total_duration));
    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 1.0), total_duration));

    bar_content.remove_at(1); // last in voice 0

    CHECK(bar_content.get_entries().size() == 1);
    CHECK(bar_content.remaining_duration(total_duration) == 2.0);
}

TEST_CASE("BAR CONTENT: remove_at() on non-last element replaces with Rest, cursor and onset unchanged") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};
    auto C4 = notation::Pitch{notation::PitchName::C, 4};

    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 1.0), total_duration));
    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 1.0), total_duration));
    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 2.0), total_duration));

    bar_content.remove_at(0); // not last in voice 0

    REQUIRE(bar_content.get_entries().size() == 3);
    CHECK(bar_content.remaining_duration(total_duration) == 0.0);

    auto* rest = dynamic_cast<notation::Rest*>(bar_content.get_entries()[0].element.get());
    REQUIRE(rest != nullptr);
    CHECK(rest->get_duration() == 1.0);
    CHECK(bar_content.get_entries()[0].onset == 0.0);
}

TEST_CASE("BAR CONTENT: remove_at() on only element leaves bar empty with full remaining duration") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};
    auto C4 = notation::Pitch{notation::PitchName::C, 4};

    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 4.0), total_duration));
    bar_content.remove_at(0);

    CHECK(bar_content.empty());
    CHECK(bar_content.remaining_duration(total_duration) == total_duration);
}

TEST_CASE("BAR CONTENT: remove_at() in multi-voice only affects the target voice cursor") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};
    auto C4 = notation::Pitch{notation::PitchName::C, 4};

    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 2.0, 0), total_duration));
    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 4.0, 1), total_duration));

    bar_content.remove_at(1); // last in voice 1

    CHECK(bar_content.get_entries().size() == 1);
    CHECK(bar_content.remaining_duration(total_duration, 0) == 2.0); // voice 0 unchanged
    CHECK(bar_content.remaining_duration(total_duration, 1) == 4.0); // voice 1 reset
}

TEST_CASE("BAR CONTENT: remove_at() throws std::out_of_range for invalid index") {
    auto const total_duration = 4.0;
    notation::BarContent bar_content{};

    CHECK_THROWS_AS(bar_content.remove_at(0), std::out_of_range); // empty bar

    auto C4 = notation::Pitch{notation::PitchName::C, 4};
    REQUIRE(bar_content.try_add(std::make_unique<notation::Note>(C4, 1.0), total_duration));
    CHECK_THROWS_AS(bar_content.remove_at(1), std::out_of_range); // one past end
}
