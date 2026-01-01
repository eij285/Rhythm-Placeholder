#pragma once
#include <compare>

namespace notation {
    enum class PitchName {
        Cb, C, Cs, Db, D, Ds, Eb, E, Es, Fb, F, Fs, Gb, G, Gs, Ab, A, As, Bb, B, Bs
    };

    class Pitch {
     public:
        Pitch(PitchName name, int octave);

        [[nodiscard]] auto get_name() const -> PitchName;
        [[nodiscard]] auto get_octave() const -> int;

        [[nodiscard]] auto operator<=>(Pitch const& other) const -> std::strong_ordering;
        [[nodiscard]] auto operator==(Pitch const& other) const -> bool;

        [[nodiscard]] auto is_enharmonic(Pitch const& other) const -> bool;

     private:
        PitchName name_;
        int octave_;

        [[nodiscard]] auto abs_semitone() const -> int;
    };
} // namespace notation
