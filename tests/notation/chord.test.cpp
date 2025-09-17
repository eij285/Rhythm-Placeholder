#include <catch2/catch_test_macros.hpp>
#include "notation/chord.hpp"
#include "notation/pitch.hpp"

TEST_CASE("Constructor sets correct private field members") {
    auto pitches = std::vector<notation::Pitch>{
        notation::Pitch{notation::PitchName::C, 4},
        notation::Pitch{notation::PitchName::E, 4},
        notation::Pitch{notation::PitchName::G, 4}
    };

    auto CMaj_minim = notation::Chord{pitches, 2.0};

    CHECK(CMaj_minim.get_pitches() == pitches);
    CHECK(CMaj_minim.get_duration() == 2.0);
    CHECK(CMaj_minim.get_voice() == 0);

    pitches.push_back(notation::Pitch{notation::PitchName::C, 5});
    auto CMaj_semiq = notation::Chord{pitches, 0.25, 1};

    CHECK(CMaj_semiq.get_pitches() == pitches);
    CHECK(CMaj_semiq.get_duration() == 0.25);
    CHECK(CMaj_semiq.get_voice() == 1);
}

TEST_CASE("Non-member function make_chord() constructs sorted chord given valid notes") {
    auto D3 = notation::Pitch{notation::PitchName::D, 3};
    auto F3 = notation::Pitch{notation::PitchName::F, 3};
    auto Ab3 = notation::Pitch{notation::PitchName::Ab, 3};
    auto Cb4 = notation::Pitch{notation::PitchName::Cb, 4};
    auto D4 = notation::Pitch{notation::PitchName::D, 4};

    SECTION("Trying to construct chord using notes of different voices produces argument_error") {
        auto D3_voice0 = notation::Note{D3, 1.0, 0};
        auto F3_voice1 = notation::Note{F3, 1.0, 1};

        auto notes = std::initializer_list<notation::Note>{D3_voice0, F3_voice1};
        CHECK_THROWS_AS(notation::make_chord(notes), std::invalid_argument);
    }

    SECTION("Trying to construct chord using notes of different duration produces argument_error") {
        auto D3_crotchet = notation::Note{D3, 1.0};
        auto F3_minim = notation::Note{F3, 2.0};

        auto notes = std::initializer_list<notation::Note>{D3_crotchet, F3_minim};
        CHECK_THROWS_AS(notation::make_chord(notes), std::invalid_argument);
    }

    SECTION("Successfully constructs The D diminished chord") {
        auto expected_pitches = std::vector<notation::Pitch>{D3, F3, Ab3, Cb4, D4};
        
        auto Ddim_notes = std::initializer_list<notation::Note>{
            notation::Note{D3, 2.0},
            notation::Note{F3, 2.0},
            notation::Note{Ab3, 2.0},
            notation::Note{Cb4, 2.0},
            notation::Note{D4, 2.0}
        };

        auto Ddim = notation::make_chord(Ddim_notes);

        CHECK(Ddim.get_pitches() == expected_pitches);
        CHECK(Ddim.get_duration() == 2.0);
        CHECK(Ddim.get_voice() == 0);
    }

    SECTION("Successfully constructs the D diminished chord even if unsorted") {
        auto expected_pitches = std::vector<notation::Pitch>{D3, F3, Ab3, Cb4, D4};

        auto Ddim_notes_unsorted = std::initializer_list<notation::Note>{
            notation::Note{D4, 2.0},
            notation::Note{Cb4, 2.0},
            notation::Note{F3, 2.0},
            notation::Note{D3, 2.0},
            notation::Note{Ab3, 2.0}
        };

        auto Ddim = notation::make_chord(Ddim_notes_unsorted);

        CHECK(Ddim.get_pitches() == expected_pitches);
        CHECK(Ddim.get_duration() == 2.0);
        CHECK(Ddim.get_voice() == 0);
    }
}

TEST_CASE("TODO: Chord's print() prints correct thing") {
    REQUIRE(true);
}

TEST_CASE("Chord's add_pitch() successfully is able to add pitches in a sorted manner") {

}