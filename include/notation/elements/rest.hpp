#pragma once
#include "notation/elements/musical_element.hpp"

namespace notation {
    class Rest : public MusicalElement {
     public:
        Rest(double duration, int voice = 0);
        
        [[nodiscard]] auto with_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> override;
        [[nodiscard]] auto with_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> override;

        [[nodiscard]] auto without_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> override;
        [[nodiscard]] auto without_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> override;
    };
} // namespace notation
