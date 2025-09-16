#include "notation/chord.hpp"
#include "notation/notation_globals.hpp"

#include <stdexcept>
#include <cmath>

namespace notation {
    Chord::Chord(std::vector<Pitch> pitches, double duration, int voice)
    : MusicalElement(duration, voice)
    , pitches_{std::move(pitches)} {};

    auto Chord::get_pitches() const -> std::vector<Pitch> const& {
        return pitches_;
    }

    auto Chord::print() const -> std::string {
        // TODO: For testing and debugging purposes
        return "I am a chord";
    }

    auto Chord::add_pitch(Pitch const& pitch) -> Chord& {
        pitches_.push_back(pitch);
        std::sort(pitches_.begin(), pitches_.end());

        return *this;
    }

    auto Chord::add_note(Note const& note) -> Chord& {
        if (std::fabs(note.get_duration() - this->get_duration()) > dur_tolerance) {
            throw std::invalid_argument("Chord::add_note -> Input argument has mismatching duration");
        }

        add_pitch(note.get_pitch());
        return *this;
    }

    auto Chord::add_notes(std::initializer_list<Note> const& notes) -> Chord& {
        for (auto const& note : notes) {
            add_note(note);
        }

        return *this;
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
