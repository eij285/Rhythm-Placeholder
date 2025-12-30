#include <catch2/catch_test_macros.hpp>
#include "notation/pitch.hpp"

TEST_CASE("PITCH: Test for constructor with two arguments") {
    auto const C4 = notation::Pitch(notation::PitchName::C, 4);

    CHECK(C4.get_name() == notation::PitchName::C);
    CHECK(C4.get_octave() == 4);
}

TEST_CASE("PITCH: Test for spaceship operator <=> and ==") {
    SECTION("Equality == works for two pitches with same octave and PitchName") {
        auto const Ds6_v1 = notation::Pitch(notation::PitchName::Ds, 6);
        auto const Ds6_v2 = notation::Pitch(notation::PitchName::Ds, 6);

        REQUIRE((Ds6_v1 <=> Ds6_v2) == std::strong_ordering::equal);

        CHECK(Ds6_v1 == Ds6_v2);
        CHECK(Ds6_v1 <= Ds6_v2);
        CHECK(Ds6_v1 >= Ds6_v2);     
    }

    SECTION("Octave number produces correct ordering") {
        auto const Bb2 = notation::Pitch(notation::PitchName::Bb, 2);
        auto const Bb3 = notation::Pitch(notation::PitchName::Bb, 3);

        REQUIRE((Bb2 <=> Bb3) == std::strong_ordering::less);
        REQUIRE((Bb3 <=> Bb2) == std::strong_ordering::greater);

        CHECK(Bb2 < Bb3);
        CHECK(Bb2 <= Bb3);
        CHECK(Bb3 > Bb2);
        CHECK(Bb3 >= Bb2);
    }

    SECTION("Pitchname produces correct ordering within same octave") {
        auto const A0 = notation::Pitch(notation::PitchName::A, 0);
        auto const B0 = notation::Pitch(notation::PitchName::B, 0);

        REQUIRE((A0 <=> B0) == std::strong_ordering::less);
        REQUIRE((B0 <=> A0) == std::strong_ordering::greater);

        CHECK(A0 < B0);
        CHECK(A0 <= B0);
        CHECK(B0 > A0);
        CHECK(B0 >= A0);
    }

    SECTION("Operator <=> does not follow enharmonic ordering") {
        auto const E5 = notation::Pitch(notation::PitchName::E, 5);
        auto const Es5 = notation::Pitch(notation::PitchName::Es, 5);
        auto const Fb5 = notation::Pitch(notation::PitchName::Fb, 5);
        auto const F5 = notation::Pitch(notation::PitchName::F, 5);

        REQUIRE((E5 <=> Fb5) == std::strong_ordering::less);
        CHECK(E5 < Fb5);
        CHECK_FALSE(E5 == Fb5);

        REQUIRE((Es5 <=> F5) == std::strong_ordering::less);
        CHECK(Es5 < F5);
        CHECK_FALSE(Es5 == F5);

        REQUIRE((Es5 <=> Fb5) == std::strong_ordering::less);
        CHECK(Es5 < Fb5);
        CHECK_FALSE(Fb5 < Es5);
    }
}

TEST_CASE("PITCH: Test for is_enharmonic() member function") {
    SECTION("Pitches separated by an octave are not enharmonic") {
        auto const G2 = notation::Pitch(notation::PitchName::G, 2);
        auto const G3 = notation::Pitch(notation::PitchName::G, 3);

        CHECK_FALSE(G2.is_enharmonic(G3));
    }

    SECTION("Identically notated pitches are enharmonic") {
        auto const As3_v1 = notation::Pitch(notation::PitchName::As, 3);
        auto const As3_v2 = notation::Pitch(notation::PitchName::As, 3);

        CHECK(As3_v1.is_enharmonic(As3_v1));
        CHECK(As3_v1.is_enharmonic(As3_v2));
    }

    SECTION("Unrelated pitches are not enharmonic") {
        auto const Bb2 = notation::Pitch(notation::PitchName::Bb, 2);
        auto const Fs2 = notation::Pitch(notation::PitchName::Fs, 2);
        auto const As5 = notation::Pitch(notation::PitchName::As, 5);

        CHECK_FALSE(Bb2.is_enharmonic(Fs2));
        CHECK_FALSE(Bb2.is_enharmonic(As5));
        CHECK_FALSE(Fs2.is_enharmonic(As5));
    }

    SECTION("Pitches with differing notations but same absolute semitone value still match") {
        auto const Cs3 = notation::Pitch(notation::PitchName::Cs, 3);
        auto const Db3 = notation::Pitch(notation::PitchName::Db, 3);

        CHECK(Cs3.is_enharmonic(Db3));
        CHECK(Db3.is_enharmonic(Cs3));

        auto const Es4 = notation::Pitch(notation::PitchName::Es, 4);
        auto const F4 = notation::Pitch(notation::PitchName::F, 4);

        CHECK(Es4.is_enharmonic(F4));
        CHECK(F4.is_enharmonic(Es4));

        auto const Bs5 = notation::Pitch(notation::PitchName::Bs, 5);
        auto const C6 = notation::Pitch(notation::PitchName::C, 6);

        CHECK(Bs5.is_enharmonic(C6));
        CHECK(C6.is_enharmonic(Bs5));
    }
}

TEST_CASE("PITCH: Test for is_enharmonic() across an entire octave C to C") {
    auto const B3 = notation::Pitch(notation::PitchName::B, 3);
    auto const Cb4 = notation::Pitch(notation::PitchName::Cb, 4);

    CHECK(B3.is_enharmonic(Cb4));

    auto const Bs3 = notation::Pitch(notation::PitchName::Bs, 3);
    auto const C4 = notation::Pitch(notation::PitchName::C, 4);

    CHECK(Bs3.is_enharmonic(C4));

    auto const Cs4 = notation::Pitch(notation::PitchName::Cs, 4);
    auto const Db4 = notation::Pitch(notation::PitchName::Db, 4);

    CHECK(Cs4.is_enharmonic(Db4));

    auto const Ds4 = notation::Pitch(notation::PitchName::Ds, 4);
    auto const Eb4 = notation::Pitch(notation::PitchName::Eb, 4);

    CHECK(Ds4.is_enharmonic(Eb4));

    auto const E4 = notation::Pitch(notation::PitchName::E, 4);
    auto const Fb4 = notation::Pitch(notation::PitchName::Fb, 4);

    CHECK(E4.is_enharmonic(Fb4));

    auto const Es4 = notation::Pitch(notation::PitchName::Es, 4);
    auto const F4 = notation::Pitch(notation::PitchName::F, 4);

    CHECK(Es4.is_enharmonic(F4));

    auto const Fs4 = notation::Pitch(notation::PitchName::Fs, 4);
    auto const Gb4 = notation::Pitch(notation::PitchName::Gb, 4);

    CHECK(Fs4.is_enharmonic(Gb4));

    auto const Gs4 = notation::Pitch(notation::PitchName::Gs, 4);
    auto const Ab4 = notation::Pitch(notation::PitchName::Ab, 4);

    CHECK(Gs4.is_enharmonic(Ab4));

    auto const As4 = notation::Pitch(notation::PitchName::As, 4);
    auto const Bb4 = notation::Pitch(notation::PitchName::Bb, 4);

    CHECK(As4.is_enharmonic(Bb4));

    auto const Bs4 = notation::Pitch(notation::PitchName::Bs, 4);
    auto const C5 = notation::Pitch(notation::PitchName::C, 5);

    CHECK(Bs4.is_enharmonic(C5));
}
