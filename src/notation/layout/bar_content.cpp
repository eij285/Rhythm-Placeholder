#include "notation/layout/bar_content.hpp"
#include "notation/elements/rest.hpp"
#include "notation/notation_globals.hpp"

#include <algorithm>
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

    auto BarContent::remove_at(std::size_t index) -> std::unique_ptr<MusicalElement> {
        if (index >= entries_.size()) {
            throw std::out_of_range("BarContent::remove_at: index out of range");
        }

        auto const voice    = entries_[index].element->get_voice();
        auto const onset    = entries_[index].onset;
        auto const duration = entries_[index].element->get_duration();

        auto const has_subsequent = std::any_of(
            entries_.begin() + static_cast<std::ptrdiff_t>(index) + 1,
            entries_.end(),
            [voice](BarEntry const& e) { return e.element->get_voice() == voice; }
        );

        auto original = std::move(entries_[index].element);

        if (has_subsequent) {
            entries_[index].element = std::make_unique<Rest>(duration, voice);
        } else {
            voice_cursors_[voice] = onset;
            entries_.erase(entries_.begin() + static_cast<std::ptrdiff_t>(index));
        }

        return original;
    }

    auto BarContent::clear() -> void {
        entries_.clear();
        voice_cursors_.clear();
    }
} // namespace notation
