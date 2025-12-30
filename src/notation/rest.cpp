#include "notation/rest.hpp"
#include "notation/note.hpp"
#include "notation/chord.hpp"

#include <vector>

namespace notation {
    Rest::Rest(double duration, int voice)
    : MusicalElement(duration, voice) {};

    auto Rest::with_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> {
        return std::make_unique<Note>(
            pitch,
            get_duration(),
            get_voice()
        );
    }

    auto Rest::with_pitches(std::initializer_list<Pitch> const& pitches) const -> std::unique_ptr<MusicalElement> {
        return std::make_unique<Chord>(
            std::vector<Pitch>{pitches},
            get_duration(),
            get_voice()
        );
    }

    auto Rest::without_pitch([[maybe_unused]] Pitch const& pitch) const -> std::unique_ptr<MusicalElement> {
        throw std::logic_error("MusicalElement of type Rest: No pitch to remove");
    }

    auto Rest::without_pitches(std::initializer_list<Pitch> const& pitches) const -> std::unique_ptr<MusicalElement> {
        if (pitches.size() > 0) {
            throw std::invalid_argument("MusicalElement of type Rest: No pitch to remove");
        }

        return std::make_unique<Rest>(get_duration(), get_voice());
    }
} // namespace notation
