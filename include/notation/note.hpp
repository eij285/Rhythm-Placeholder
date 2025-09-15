#pragma once
#include "notation/musical_element.hpp"
#include "notation/pitch.hpp"

#include <string>
#include <vector>

namespace notation {
    class Note : public MusicalElement {
     public:
        Note(Pitch const& pitch, double duration, int voice = 0);
        Note(PitchName const& pitch_name, int octave, double duration, int voice = 0);

        [[nodiscard]] auto get_pitch() const -> Pitch;
        auto print() const -> std::string override;

     private:
        Pitch pitch_;
    };
} // namespace notation
