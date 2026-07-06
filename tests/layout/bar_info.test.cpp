#include <catch2/catch_test_macros.hpp>
#include "notation/layout/bar_info.hpp"

TEST_CASE("BAR INFO: Time Signature constructors store values correctly") {
    SECTION("Default constructor") {
        auto ts = notation::TimeSignature();

        CHECK(ts.get_top() == 4);
        CHECK(ts.get_bot() == 4);
    }

    SECTION("Custom constructor") {
        auto ts = notation::TimeSignature(6, 8);

        CHECK(ts.get_top() == 6);
        CHECK(ts.get_bot() == 8);
    }
}

TEST_CASE("BAR INFO: Time signature can be set") {
    auto ts = notation::TimeSignature();

    REQUIRE(ts.get_top() == 4);
    REQUIRE(ts.get_bot() == 4);

    ts.set(5, 8);

    CHECK(ts.get_top() == 5);
    CHECK(ts.get_bot() == 8);
}

TEST_CASE("BAR INFO: Default constructor sets default fields correctly") {
    auto bari = notation::BarInfo();
    auto ts = bari.get_time_signature();

    CHECK(ts.get_top() == 4);
    CHECK(ts.get_bot() == 4);
    CHECK(bari.get_key_signature() == notation::KeySignature::C);
    CHECK(bari.get_tempo() == 120);
}

TEST_CASE("BAR INFO: Custom constructor sets fields correctly") {
    auto ts = notation::TimeSignature(3, 4);
    auto bari = notation::BarInfo(ts, notation::KeySignature::Bb, 80);

    CHECK(bari.get_time_signature().get_top() == ts.get_top());
    CHECK(bari.get_time_signature().get_bot() == ts.get_bot());
    CHECK(bari.get_key_signature() == notation::KeySignature::Bb);
    CHECK(bari.get_tempo() == 80);
}

TEST_CASE("BAR INFO: Setter for time signature works as expected") {
    auto bari = notation::BarInfo();

    REQUIRE(bari.get_time_signature().get_top() == 4);
    REQUIRE(bari.get_time_signature().get_bot() == 4);

    bari.set_time_signature(3, 8);

    CHECK(bari.get_time_signature().get_top() == 3);
    CHECK(bari.get_time_signature().get_bot() == 8);

    auto ts = notation::TimeSignature(3, 4);
    bari.set_time_signature(ts);

    CHECK(bari.get_time_signature().get_top() == 3);
    CHECK(bari.get_time_signature().get_bot() == 4);
}

TEST_CASE("BAR INFO: Setter for key signature works as expected") {
    auto bari = notation::BarInfo();
    REQUIRE(bari.get_key_signature() == notation::KeySignature::C);

    bari.set_key_signature(notation::KeySignature::A);
    CHECK(bari.get_key_signature() == notation::KeySignature::A);
}

TEST_CASE("BAR INFO: Setter for tempo works as expected") {
    auto bari = notation::BarInfo();
    REQUIRE(bari.get_tempo() == 120);

    bari.set_tempo(60);
    CHECK(bari.get_tempo() == 60);
}