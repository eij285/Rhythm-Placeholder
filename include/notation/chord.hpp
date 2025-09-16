#pragma once
#include "notation/musical_element.hpp"
#include "notation/pitch.hpp"
#include "notation/note.hpp"

#include <vector>

namespace notation {
    class Chord : public MusicalElement {
     public:
        Chord(std::vector<Pitch> pitches, double duration, int voice = 0);

        [[nodiscard]] auto get_pitches() const -> std::vector<Pitch> const&;
        auto print() const -> std::string override;

        auto add_pitch(Pitch const& pitch) -> Chord&;
        auto add_note(Note const& note) -> Chord&;
        auto add_notes(std::initializer_list<Note> const& notes) -> Chord&;

     private:
        std::vector<Pitch> pitches_;
    };

    /* Non-Member Functions*/
    auto make_chord(std::initializer_list<Note> const& notes) -> Chord;

} // namespace notation
