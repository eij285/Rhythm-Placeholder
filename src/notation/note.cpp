#include "notation/note.hpp"

namespace notation {
    Note::Note(Pitch const& pitch, double duration, int voice)
    : MusicalElement(duration, voice)
    , pitch_{pitch} {};

    Note::Note(PitchName const& name, int octave, double duration, int voice)
    : MusicalElement(duration, voice)
    , pitch_{name, octave} {};

    auto Note::get_pitch() const -> Pitch {
        return pitch_;
    }

    auto Note::print() const -> std::string {
        // TODO: For testing and debugging purposes
        return "I am a note";
    }
}
