#include "notation/chord.hpp"
#include "notation/note.hpp"
#include "notation/rest.hpp"
#include "notation/notation_globals.hpp"

#include <stdexcept>
#include <cmath>
#include <algorithm>

namespace notation {
    Chord::Chord(std::vector<Pitch> pitches, double duration, int voice)
    : MusicalElement(duration, voice)
    , pitches_{std::move(pitches)} {
        std::sort(pitches_.begin(), pitches_.end());
    };

    auto Chord::get_pitches() const -> std::vector<Pitch> const& {
        return pitches_;
    }

    [[nodiscard]] auto Chord::with_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> {
        auto new_pitches = pitches_;
        new_pitches.push_back(pitch);

        return std::make_unique<Chord>(new_pitches, get_duration(), get_voice());
    }

    [[nodiscard]] auto Chord::with_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> {
        auto new_pitches = pitches_;
        new_pitches.insert(new_pitches.begin(), pitches);

        return std::make_unique<Chord>(new_pitches, get_duration(), get_voice());
    }

    [[nodiscard]] auto Chord::without_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> {
        auto new_pitches = pitches_;
        auto const it = std::find(new_pitches.begin(), new_pitches.end(), pitch);

        if (it == new_pitches.end()) {
            throw std::invalid_argument("Given pitch not present in chord's pitches");
        } else {
            new_pitches.erase(it);
        }

        if (new_pitches.size() == 1) {
            return std::make_unique<Note>(new_pitches.at(0), get_duration(), get_voice());
        } else {
            return std::make_unique<Chord>(new_pitches, get_duration(), get_voice());
        }
    }

    [[nodiscard]] auto Chord::without_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> {        
        if (pitches.size() > pitches_.size()) {
            throw std::invalid_argument("More pitches than number of existing pitches given for MusicalElement of type Chord");
        }

        auto new_pitches = pitches_;
        
        for (auto const pitch : pitches) {
            auto const it = std::find(new_pitches.begin(), new_pitches.end(), pitch);

            if (it == new_pitches.end()) {
                throw std::invalid_argument("Given pitch not present in chord's pitches");
            } else {
                new_pitches.erase(it);
            }
        }

        if (new_pitches.size() == 0) {
            return std::make_unique<Rest>(get_duration(), get_voice());
        } else if (new_pitches.size() == 1) {
            return std::make_unique<Note>(new_pitches.at(0), get_duration(), get_voice());
        } else {
            return std::make_unique<Chord>(new_pitches, get_duration(), get_voice());
        }
    }

    auto make_chord(std::initializer_list<Note> const& notes) -> Chord {
        auto pitches = std::vector<Pitch>{};

        if (!notes.size()) {
            throw std::invalid_argument("Chord::make_chord -> Input il is empty");
        }

        auto const dur = notes.begin()->get_duration();
        auto const voice = notes.begin()->get_voice();

        for (auto const& note : notes) {
            if (std::fabs(note.get_duration() - dur) > dur_tolerance) {
                throw std::invalid_argument("Chord::make_chord -> Input arguments have mismatching duration");
            }
            else if (note.get_voice() != voice) {
                throw std::invalid_argument("Chord::make_chord -> Input arguments have mismatching voices");
            }

            pitches.push_back(note.get_pitch());
        }

        return Chord(pitches, dur, voice);
    }

} // namespace notation
