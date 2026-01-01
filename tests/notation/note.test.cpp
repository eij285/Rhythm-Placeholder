#include <catch2/catch_test_macros.hpp>
#include "notation/elements/note.hpp"
#include "notation/elements/pitch.hpp"
#include "notation/elements/chord.hpp"
#include "notation/elements/rest.hpp"

TEST_CASE("NOTE: Constructors set correct private field members") {
	SECTION("Constructor takes in a Pitch object, duration and an optional voice") {
		auto C4 = notation::Pitch{notation::PitchName::C, 4};
		auto C4_crotchet = notation::Note{C4, 1.0};

		CHECK(C4_crotchet.get_pitch() == C4);
		CHECK(C4_crotchet.get_duration() == 1.0);
		CHECK(C4_crotchet.get_voice() == 0);

		auto Gb2 = notation::Pitch{notation::PitchName::Gb, 2};
		auto Gb2_minim = notation::Note{Gb2, 2.0, 1};

		CHECK(Gb2_minim.get_pitch() == Gb2);
		CHECK(Gb2_minim.get_duration() == 2.0);
		CHECK(Gb2_minim.get_voice() == 1);
	}

	SECTION("Constructor takes in a PitchName, octave, duration and an optional voice") {
		auto E6 = notation::Pitch{notation::PitchName::E, 6};
		auto E6_semibreve = notation::Note{notation::PitchName::E, 6, 4.0};

		CHECK(E6_semibreve.get_pitch() == E6);
		CHECK(E6_semibreve.get_duration() == 4.0);
		CHECK(E6_semibreve.get_voice() == 0);

		auto Fs1 = notation::Pitch{notation::PitchName::Fs, 1};
		auto Fs1_quaver = notation::Note{notation::PitchName::Fs, 1, 0.5, 3};

		CHECK(Fs1_quaver.get_pitch() == Fs1);
		CHECK(Fs1_quaver.get_duration() == 0.5);
		CHECK(Fs1_quaver.get_voice() == 3);
	}
}

TEST_CASE("NOTE: Modifiers with_pitch and with_pitches return a Chord with the correct pitches") {
	auto D5 = notation::Pitch{notation::PitchName::D, 5};
	auto D5_crotchet = notation::Note{D5, 1.0};

	SECTION("with_pitch returns a Chord with the original pitch and the new pitch") {
		auto Fs5 = notation::Pitch{notation::PitchName::Fs, 5};
		auto maj_3rd = D5_crotchet.with_pitch(Fs5);

		CHECK(maj_3rd->get_duration() == D5_crotchet.get_duration());
		CHECK(maj_3rd->get_voice() == D5_crotchet.get_voice());

		auto* chord = dynamic_cast<notation::Chord*>(maj_3rd.get());
		REQUIRE(chord != nullptr);

		auto expected = std::vector<notation::Pitch>{D5, Fs5};

		CHECK(chord->get_pitches() == expected);
	}

	SECTION("with_pitches returns a Chord with the original pitch and the new pitches") {
		auto F5 = notation::Pitch{notation::PitchName::F, 5};
		auto A5 = notation::Pitch{notation::PitchName::A, 5};
		auto Dm = D5_crotchet.with_pitches({F5, A5});

		CHECK(Dm->get_duration() == D5_crotchet.get_duration());
		CHECK(Dm->get_voice() == D5_crotchet.get_voice());

		auto* chord = dynamic_cast<notation::Chord*>(Dm.get());
		REQUIRE(chord != nullptr);

		auto expected = std::vector<notation::Pitch>{D5, F5, A5};

		CHECK(chord->get_pitches() == expected);
	}
}

TEST_CASE("NOTE: Modifiers without_pitch and without_pitches behave as expected") {
	auto D5 = notation::Pitch{notation::PitchName::D, 5};
	auto D5_crotchet = notation::Note{D5, 1.0};

	SECTION("without_pitch returns a Rest") {
		auto Fs5 = notation::Pitch{notation::PitchName::Fs, 5};

		CHECK_THROWS_AS(D5_crotchet.without_pitch(Fs5), std::invalid_argument);

		auto cr_rest = D5_crotchet.without_pitch(D5);

		CHECK(cr_rest->get_duration() == D5_crotchet.get_duration());
		CHECK(cr_rest->get_voice() == D5_crotchet.get_voice());

		auto* rest = dynamic_cast<notation::Rest*>(cr_rest.get());
		REQUIRE(rest != nullptr);
	}

	SECTION("without_pitches returns a Chord with the original pitch and the new pitches") {
		auto F5 = notation::Pitch{notation::PitchName::F, 5};

		CHECK_THROWS_AS(D5_crotchet.without_pitches({D5, F5}), std::invalid_argument);

		auto cr_rest = D5_crotchet.without_pitches({D5});

		CHECK(cr_rest->get_duration() == D5_crotchet.get_duration());
		CHECK(cr_rest->get_voice() == D5_crotchet.get_voice());

		auto* rest = dynamic_cast<notation::Rest*>(cr_rest.get());
		REQUIRE(rest != nullptr);
	}
}
