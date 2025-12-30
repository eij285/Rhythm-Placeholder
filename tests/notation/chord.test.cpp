#include <catch2/catch_test_macros.hpp>
#include "notation/chord.hpp"
#include "notation/pitch.hpp"
#include "notation/note.hpp"
#include "notation/rest.hpp"

TEST_CASE("CHORD: Constructor sets correct private field members") {
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

TEST_CASE("CHORD: Non-member function make_chord() constructs sorted chord given valid notes") {
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

TEST_CASE("CHORD: Modifiers with_pitch and with_pitches return a new Chord with the correct pitches") {
	auto D5 = notation::Pitch{notation::PitchName::D, 5};
    auto F5 = notation::Pitch{notation::PitchName::F, 5};
	auto D5_third = notation::Chord{{D5, F5}, 1.0};

    SECTION("with_pitch returns a Chord with the original pitches and the new pitch") {
        auto A5 = notation::Pitch{notation::PitchName::A, 5};
        auto D5_minor = D5_third.with_pitch(A5);

        CHECK(D5_minor->get_duration() == D5_third.get_duration());
        CHECK(D5_minor->get_voice() == D5_third.get_voice());

        auto* chord = dynamic_cast<notation::Chord*>(D5_minor.get());
        REQUIRE(chord != nullptr);

        auto expected = std::vector<notation::Pitch>{D5, F5, A5};

        CHECK(chord->get_pitches() == expected);
    }

    SECTION("with_pitches returns a Chord with the original pitches and the new pitches") {
        auto A5 = notation::Pitch{notation::PitchName::A, 5};
        auto C6 = notation::Pitch{notation::PitchName::C, 6};
        auto D5_seventh = D5_third.with_pitches({A5, C6});

        CHECK(D5_seventh->get_duration() == D5_third.get_duration());
        CHECK(D5_seventh->get_voice() == D5_third.get_voice());

        auto* chord = dynamic_cast<notation::Chord*>(D5_seventh.get());
        REQUIRE(chord != nullptr);

        auto expected = std::vector<notation::Pitch>{D5, F5, A5, C6};

        CHECK(chord->get_pitches() == expected);
    }
}

TEST_CASE("CHORD: make_chord constructs a Chord from Notes correctly") {
    auto C2 = notation::Pitch{notation::PitchName::C, 2};
    auto E2 = notation::Pitch{notation::PitchName::E, 2};
    auto G2 = notation::Pitch{notation::PitchName::G, 2};

    auto C2_quav = notation::Note{C2, 0.5};
    auto E2_quav = notation::Note{E2, 0.5};
    auto G2_quav = notation::Note{G2, 0.5};

    auto CM = notation::make_chord({C2_quav, E2_quav, G2_quav});

    auto expected_pitches = std::vector<notation::Pitch>{C2, E2, G2};

    CHECK(CM.get_pitches() == expected_pitches);
    CHECK(CM.get_duration() == 0.5);
    CHECK(CM.get_voice() == 0);
}

TEST_CASE("CHORD: All chord constructors sort pitches in ascending order") {
    auto Bb1 = notation::Pitch{notation::PitchName::Bb, 1};
    auto Bb2 = notation::Pitch{notation::PitchName::Bb, 2};
    auto D2 = notation::Pitch{notation::PitchName::D, 2};
    auto F2 = notation::Pitch{notation::PitchName::F, 2};

    auto unsorted_pitches = std::vector<notation::Pitch>{F2, Bb2, Bb1, D2};
    auto expected_order = std::vector<notation::Pitch>{Bb1, D2, F2, Bb2};
    auto chord_constructor = notation::Chord{unsorted_pitches, 1.0};

    CHECK(chord_constructor.get_pitches() == expected_order);

    auto rest = notation::Rest{1.0};
    auto chord_from_rest = rest.with_pitches({F2, Bb2, Bb1, D2});
    auto* chord_ptr = dynamic_cast<notation::Chord*>(chord_from_rest.get());
    REQUIRE(chord_ptr != nullptr);

    CHECK(chord_ptr->get_pitches() == expected_order);

    auto note = notation::Note{F2, 1.0};
    auto chord_from_note = note.with_pitches({Bb2, Bb1, D2});
    chord_ptr = dynamic_cast<notation::Chord*>(chord_from_note.get());
    REQUIRE(chord_ptr != nullptr);

    auto interval_from_note = note.with_pitch(Bb1);
    chord_ptr = dynamic_cast<notation::Chord*>(interval_from_note.get());
    REQUIRE(chord_ptr != nullptr);

    CHECK(chord_ptr->get_pitches() == std::vector<notation::Pitch>{Bb1, F2});

    auto chord_from_chord1 = notation::Chord{{F2, Bb2, Bb1}, 1.0}.with_pitch(D2);
    chord_ptr = dynamic_cast<notation::Chord*>(chord_from_chord1.get());
    REQUIRE(chord_ptr != nullptr);

    CHECK(chord_ptr->get_pitches() == expected_order);

    auto chord_from_chord2 = notation::Chord{{F2, Bb2}, 1.0}.with_pitches({Bb1, D2});
    chord_ptr = dynamic_cast<notation::Chord*>(chord_from_chord2.get());
    REQUIRE(chord_ptr != nullptr);

    CHECK(chord_ptr->get_pitches() == expected_order);

    auto chord_from_make_chord = notation::make_chord({
        notation::Note{F2, 1.0},
        notation::Note{Bb2, 1.0},
        notation::Note{Bb1, 1.0},
        notation::Note{D2, 1.0}
    });

    CHECK(chord_from_make_chord.get_pitches() == expected_order);
}
