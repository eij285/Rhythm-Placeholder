#include "notation/note.hpp"
#include "notation/chord.hpp"
#include "notation/rest.hpp"

#include <stdexcept>

namespace notation {
    Note::Note(Pitch const& pitch, double duration, int voice)
    : MusicalElement(duration, voice)
    , pitch_{pitch} {};

    Note::Note(PitchName const& name, int octave, double duration, int voice)
    : MusicalElement(duration, voice)
    , pitch_{name, octave} {};

    auto Note::get_pitch() const -> Pitch const& {
        return pitch_;
    }

    auto Note::with_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> {
        return std::make_unique<Chord>(std::vector<Pitch>{pitch_, pitch}, get_duration(), get_voice());
    }

    auto Note::with_pitches(std::initializer_list<Pitch> const& pitches) const -> std::unique_ptr<MusicalElement> {
        auto new_pitches = std::vector<Pitch>{pitch_};
        new_pitches.insert(new_pitches.begin(), pitches);

        return std::make_unique<Chord>(new_pitches, get_duration(), get_voice());
    }

    auto Note::without_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> {
        if (pitch_ != pitch) {
            throw std::invalid_argument("Note's pitch does not match with given pitch");
        }

        return std::make_unique<Rest>(get_duration(), get_voice());
    }

    auto Note::without_pitches(std::initializer_list<Pitch> const& pitches) const -> std::unique_ptr<MusicalElement> {
        if (pitches.size() > 1) {
            throw std::invalid_argument("More than one pitch given for MusicalElement of type Note");
        }
        
        return without_pitch(*pitches.begin());
    }
}
