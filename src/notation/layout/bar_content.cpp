#include "notation/layout/bar_content.hpp"
#include "notation/notation_globals.hpp"

#include <cmath>

namespace notation {
    BarContent::BarContent(double total_duration)
    : total_duration_{total_duration}, used_duration_{0} {};

    auto BarContent::get_elements() const -> std::vector<std::unique_ptr<MusicalElement>> const& {
        return elements_;
    }

    auto BarContent::remaining_duration() const -> double {
        return total_duration_ - used_duration_;
    }

    auto BarContent::empty() const -> bool {
        return elements_.empty();
    }

    auto BarContent::try_add(std::unique_ptr<MusicalElement> element) -> bool {
        auto const elem_duration = element->get_duration();

        if (remaining_duration() + dur_tolerance >= elem_duration) {
            used_duration_ += elem_duration;
            elements_.push_back(std::move(element));
            return true;
        }

        return false;
    }

    auto BarContent::clear() -> void {
        elements_.clear();
        used_duration_ = 0;
    }
} // namespace notation
