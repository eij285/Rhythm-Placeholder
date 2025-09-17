#include <catch2/catch_test_macros.hpp>
#include "notation/note.hpp"
#include "notation/pitch.hpp"

TEST_CASE("Constructors set correct private field members") {
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

TEST_CASE("TODO: Print() prints correct thing") {
	REQUIRE(true);
}

