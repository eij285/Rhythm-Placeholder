#include "notation/pitch.hpp"

#include <cassert>

namespace notation {
    Pitch::Pitch(PitchName name, int octave)
    : name_{name}
    , octave_{octave} {};

    auto Pitch::get_name() const -> PitchName {
        return name_;
    }

    auto Pitch::get_octave() const -> int {
        return octave_;
    }

    auto Pitch::operator<=>(Pitch const& other) const -> std::strong_ordering {
        if (octave_ != other.octave_) {
            return octave_ <=> other.octave_;
        }

        return name_ <=> other.name_;
    }

    auto Pitch::operator==(Pitch const& other) const -> bool {
        return (octave_ == other.octave_) && (name_ == other.name_);
    }

    auto Pitch::is_enharmonic(Pitch const& other) const -> bool {
        return this->abs_semitone() == other.abs_semitone();
    }

    auto Pitch::abs_semitone() const -> int {
        auto octave_base = octave_ * 12;

        switch (name_) {
            case PitchName::Cb:
                return octave_base - 1;
            case PitchName::C:
                return octave_base;
            case PitchName::Cs:
            case PitchName::Db:
                return octave_base + 1;
            case PitchName::D:
                return octave_base + 2;
            case PitchName::Ds:
            case PitchName::Eb:
                return octave_base + 3;
            case PitchName::E: 
            case PitchName::Fb:
                return octave_base + 4;
            case PitchName::Es:
            case PitchName::F:
                return octave_base + 5;
            case PitchName::Fs:
            case PitchName::Gb:
                return octave_base + 6;
            case PitchName::G:
                return octave_base + 7;
            case PitchName::Gs:
            case PitchName::Ab:
                return octave_base + 8;
            case PitchName::A:
                return octave_base + 9;
            case PitchName::As:
            case PitchName::Bb:
                return octave_base + 10;
            case PitchName::B:
            case PitchName::Bs:
                return octave_base + 11;
        }

        assert(false && "Invalid PitchName in semitone_val");
        __builtin_unreachable();
    }
}