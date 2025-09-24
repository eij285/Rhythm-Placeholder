#pragma once
#include "notation/musical_element.hpp"
#include "notation/pitch.hpp"

#include <initializer_list>

namespace notation {
    class Note : public MusicalElement {
     public:
        Note(Pitch const& pitch, double duration, int voice = 0);
        Note(PitchName const& pitch_name, int octave, double duration, int voice = 0);

        [[nodiscard]] auto get_pitch() const -> Pitch const&;
        
        [[nodiscard]] auto with_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> override;
        [[nodiscard]] auto with_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> override;

        [[nodiscard]] auto without_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> override;
        [[nodiscard]] auto without_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> override;

     private:
        Pitch pitch_;
    };
} // namespace notation
