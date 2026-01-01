#include <catch2/catch_test_macros.hpp>
#include "notation/elements/rest.hpp"
#include "notation/elements/pitch.hpp"
#include "notation/elements/note.hpp"
#include "notation/elements/chord.hpp"

TEST_CASE("REST: Constructor sets correct private field members") {
    auto rest_semibreve = notation::Rest{4.0};

    CHECK(rest_semibreve.get_duration() == 4.0);
    CHECK(rest_semibreve.get_voice() == 0);

    auto rest_minim = notation::Rest{2.0, 1};

    CHECK(rest_minim.get_duration() == 2.0);
    CHECK(rest_minim.get_voice() == 1);
}

TEST_CASE("REST: Modifiers with_pitch and with_pitches return correct elements with the correct pitches") {
    auto rest_crotchet = notation::Rest{1.0};

    SECTION("with_pitch returns a Note with the new pitch") {
        auto G4 = notation::Pitch{notation::PitchName::G, 4};
        auto G4_semibreve = rest_crotchet.with_pitch(G4);

        CHECK(G4_semibreve->get_duration() == rest_crotchet.get_duration());
        CHECK(G4_semibreve->get_voice() == rest_crotchet.get_voice());

        auto* note_ptr = dynamic_cast<notation::Note*>(G4_semibreve.get());
        REQUIRE(note_ptr != nullptr);

        CHECK(note_ptr->get_pitch() == G4);
    }

    SECTION("with_pitches returns a Chord with the new pitches") {
        auto A4 = notation::Pitch{notation::PitchName::A, 4};
        auto C5 = notation::Pitch{notation::PitchName::C, 5};
        auto min_3rd = rest_crotchet.with_pitches({A4, C5});

        CHECK(min_3rd->get_duration() == rest_crotchet.get_duration());
        CHECK(min_3rd->get_voice() == rest_crotchet.get_voice());

        auto* chord_ptr = dynamic_cast<notation::Chord*>(min_3rd.get());
        REQUIRE(chord_ptr != nullptr);

        auto expected_pitches = std::vector<notation::Pitch>{A4, C5};
        CHECK(chord_ptr->get_pitches() == expected_pitches);
    }
}

TEST_CASE("REST: Modifiers without_pitch and without_pitches throw exceptions") {
    auto rest_quaver = notation::Rest{0.5};

    SECTION("without_pitch throws error") {
        auto D5 = notation::Pitch{notation::PitchName::D, 5};
        CHECK_THROWS_AS(rest_quaver.without_pitch(D5), std::logic_error);
    }

    SECTION("without_pitches returns identical Rest") {
        auto F4 = notation::Pitch{notation::PitchName::F, 4};
        auto Bb3 = notation::Pitch{notation::PitchName::Bb, 3};
        CHECK_THROWS_AS(rest_quaver.without_pitches({F4, Bb3}), std::logic_error);
    }
}
