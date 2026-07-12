#include <catch2/catch_test_macros.hpp>
#include "notation/layout/score.hpp"
#include "notation/layout/part.hpp"
#include "notation/layout/bar_info.hpp"

TEST_CASE("SCORE: Constructors work as expected") {
    SECTION("Default constructor") {
        auto const score = notation::Score();

        CHECK(score.get_title() == "Untitled");
        CHECK(score.get_author() == "");
        CHECK(score.get_parts().empty());
        CHECK(score.get_timeline().empty());
    }

    SECTION("Custom constructor") {
        auto const score = notation::Score("Symphony", "Mozart");

        CHECK(score.get_title() == "Symphony");
        CHECK(score.get_author() == "Mozart");
        CHECK(score.get_parts().empty());
        CHECK(score.get_timeline().empty());
    }

    SECTION("Parts constructor") {
        auto parts = std::vector<notation::Part>{};
        parts.emplace_back("Violin", 1);
        parts.emplace_back("Cello", 1);
        parts.emplace_back("Piano", 2);

        auto const score = notation::Score(std::move(parts), "String Trio", "Beethoven");

        CHECK(score.get_title() == "String Trio");
        CHECK(score.get_author() == "Beethoven");
        CHECK(score.get_timeline().empty());

        REQUIRE(!score.get_parts().empty());
        CHECK(score.get_parts()[0].get_instrument() == "Violin");
        CHECK(score.get_parts()[1].get_instrument() == "Cello");
        CHECK(score.get_parts()[2].get_instrument() == "Piano");
    }
}

// TODO: Mutable BarInfo&? Should I just enable adding custom bari's, and enforce client code
// to do the previous copy? Overload/default values?
TEST_CASE("SCORE: Adding bar to blank score constructs a bar_info with default values in beginning of timeline") {
    auto score = notation::Score();
    auto bari = score.add_new_bar();

    CHECK(bari.get_time_signature().get_top() == 4);
    CHECK(bari.get_time_signature().get_bot() == 4);
    CHECK(bari.get_key_signature() == notation::KeySignature::C);
    CHECK(bari.get_tempo() == 120);
}

TEST_CASE("SCORE: Mutable BarInfo& returned by add_new_bar() is a live reference") {
    auto score = notation::Score();
    auto& bari = score.add_new_bar();
    
    REQUIRE(bari.get_time_signature().get_top() == 4);
    REQUIRE(bari.get_time_signature().get_bot() == 4);
    REQUIRE(bari.get_key_signature() == notation::KeySignature::C);
    REQUIRE(bari.get_tempo() == 120);

    bari.set_time_signature(6, 8);
    bari.set_key_signature(notation::KeySignature::G);
    bari.set_tempo(80);

    auto const timeline = score.get_timeline();
    CHECK(bari.get_time_signature().get_top() == timeline[0].get_time_signature().get_top());
    CHECK(bari.get_time_signature().get_bot() == timeline[0].get_time_signature().get_bot());
    CHECK(bari.get_key_signature() == timeline[0].get_key_signature());
    CHECK(bari.get_tempo() == timeline[0].get_tempo());
}

TEST_CASE("SCORE: Subsequent bars from add_new_bar() copies settings from previous one by default") {
    auto score = notation::Score();
    score.add_new_bar();
    auto& bar2 = score.add_new_bar();

    bar2.set_time_signature(12, 8);
    bar2.set_key_signature(notation::KeySignature::F);
    bar2.set_tempo(200);

    auto const bar3 = score.add_new_bar();

    CHECK(bar3.get_time_signature().get_top() == 12);
    CHECK(bar3.get_time_signature().get_bot() == 8);
    CHECK(bar3.get_key_signature() == notation::KeySignature::F);
    CHECK(bar3.get_tempo() == 200);
}

TEST_CASE("SCORE: add_new_bar() pushes a BarContent to every stave of every existing part") {
    auto score = notation::Score{};
    score.add_part("Violin", 1);
    score.add_part("Piano", 2);

    score.add_new_bar();

    for (auto const& part : score.get_parts()) {
        for (auto const& stave : part.get_staves()) {
            CHECK(stave.get_bar_contents().size() == 1);
        }
    }
}

TEST_CASE("SCORE: add_new_bar() derives BarContent total_duration from the bar's time signature") {
    auto score = notation::Score{};
    score.add_part("Oboe", 1);

    SECTION("4/4 produces total_duration of 4.0") {
        score.add_new_bar();
        auto const ts = score.get_timeline()[0].get_time_signature();
        CHECK(score.get_parts()[0].get_staves()[0].get_bar_content(0).remaining_duration(ts.total_duration()) == 4.0);
    }

    SECTION("3/4 produces total_duration of 3.0") {
        auto& bar = score.add_new_bar();
        bar.set_time_signature(3, 4);
        auto const ts = score.get_timeline()[0].get_time_signature();
        CHECK(score.get_parts()[0].get_staves()[0].get_bar_content(0).remaining_duration(ts.total_duration()) == 3.0);
    }

    SECTION("6/8 produces total_duration of 3.0") {
        auto& bar = score.add_new_bar();
        bar.set_time_signature(6, 8);
        auto const ts = score.get_timeline()[0].get_time_signature();
        CHECK(score.get_parts()[0].get_staves()[0].get_bar_content(0).remaining_duration(ts.total_duration()) == 3.0);
    }

    SECTION("2/2 produces total_duration of 4.0") {
        auto& bar = score.add_new_bar();
        bar.set_time_signature(2, 2);
        auto const ts = score.get_timeline()[0].get_time_signature();
        CHECK(score.get_parts()[0].get_staves()[0].get_bar_content(0).remaining_duration(ts.total_duration()) == 4.0);
    }
}

TEST_CASE("SCORE: add_part() returns a Part with the correct instrument name and stave count") {
    auto score = notation::Score{};
    auto& part = score.add_part("Xylophone", 2);

    CHECK(part.get_instrument() == "Xylophone");
    CHECK(part.get_staves().size() == 2);
}

TEST_CASE("SCORE: add_part() on an empty score produces staves with no bar contents") {
    auto score = notation::Score{};
    auto& part = score.add_part("Violin", 1);

    CHECK(part.get_staves()[0].get_bar_contents().empty());
}

TEST_CASE("SCORE: add_part() backfills one BarContent per existing bar into each stave") {
    auto score = notation::Score{};
    score.add_new_bar();
    score.add_new_bar();
    score.add_new_bar();

    auto& part = score.add_part("Cello", 1);

    CHECK(part.get_staves()[0].get_bar_contents().size() == 3);
}

TEST_CASE("SCORE: add_part() backfills BarContent with durations matching each bar's current time signature") {
    auto score = notation::Score{};

    score.add_new_bar();              // bar 0: 4/4, kept as-is
    auto& bar1 = score.add_new_bar(); // bar 1: copied as 4/4, then changed to 3/4
    bar1.set_time_signature(3, 4);

    auto& part = score.add_part("Violin", 1);
    auto const& stave = part.get_staves()[0];

    REQUIRE(stave.get_bar_contents().size() == 2);

    auto const ts0 = score.get_timeline()[0].get_time_signature();
    auto const ts1 = score.get_timeline()[1].get_time_signature();

    CHECK(stave.get_bar_content(0).remaining_duration(ts0.total_duration()) == 4.0);
    CHECK(stave.get_bar_content(1).remaining_duration(ts1.total_duration()) == 3.0);
}

TEST_CASE("SCORE: Every stave has the same number of bar content entries as the timeline after any sequence of add_new_bar() and add_part() calls") {
    auto score = notation::Score{};

    score.add_part("Violin", 1);
    score.add_new_bar();
    score.add_new_bar();
    score.add_part("Piano", 2);
    score.add_new_bar();
    score.add_part("Cello", 1);

    auto const expected = score.get_timeline().size();

    for (auto const& part : score.get_parts()) {
        for (auto const& stave : part.get_staves()) {
            CHECK(stave.get_bar_contents().size() == expected);
        }
    }
}
