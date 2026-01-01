#pragma once
#include "notation/elements/musical_element.hpp"
#include "notation/elements/pitch.hpp"
#include "notation/elements/note.hpp"

#include <vector>

namespace notation {
    class Chord : public MusicalElement {
     public:
        Chord(std::vector<Pitch> pitches, double duration, int voice = 0);

        [[nodiscard]] auto get_pitches() const -> std::vector<Pitch> const&;

        [[nodiscard]] auto with_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> override;
        [[nodiscard]] auto with_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> override;

        [[nodiscard]] auto without_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> override;
        [[nodiscard]] auto without_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> override;

     private:
        std::vector<Pitch> pitches_;
    };

    /* Non-Member Functions*/
    auto make_chord(std::initializer_list<Note> const& notes) -> Chord;

} // namespace notation
