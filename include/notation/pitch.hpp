#pragma once
// #include <string>
#include <compare>

namespace notation {
    enum class PitchName {
        Cb, C, Cs, Db, D, Ds, Eb, E, Es, Fb, F, Fs, Gb, G, Gs, Ab, A, As, Bb, B, Bs
    };

    class Pitch {
     public:
        PitchName name_;
        int octave_;

        Pitch(PitchName name, int octave);
        [[nodiscard]] auto operator<=>(Pitch const& other) const -> std::strong_ordering;
        [[nodiscard]] auto is_enharmonic(Pitch const& other) const -> bool;

     private:
        auto abs_semitone() const -> int;
    };
}
