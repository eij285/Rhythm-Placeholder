#include "notation/layout/bar_content.hpp"
#include "notation/notation_globals.hpp"

#include <cmath>

namespace notation {
    BarContent::BarContent() {}

    auto BarContent::get_entries() const -> std::vector<BarEntry> const& {
        return entries_;
    }

    auto BarContent::remaining_duration(double total_duration, int voice) const -> double {
        auto const cursor = voice_cursors_.contains(voice) ? voice_cursors_.at(voice) : 0.0;
        return total_duration - cursor;
    }

    auto BarContent::empty() const -> bool {
        return entries_.empty();
    }

    auto BarContent::try_add(std::unique_ptr<MusicalElement> element, double total_duration) -> bool {
        auto const voice = element->get_voice();
        auto const elem_duration = element->get_duration();

        if (remaining_duration(total_duration, voice) + dur_tolerance >= elem_duration) {
            auto const onset = voice_cursors_[voice];
            voice_cursors_[voice] += elem_duration;
            entries_.push_back({onset, std::move(element)});
            return true;
        }

        return false;
    }

    auto BarContent::clear() -> void {
        entries_.clear();
        voice_cursors_.clear();
    }
} // namespace notation
