#include "notation/layout/stave.hpp"

namespace notation {
    Stave::Stave() {}

    auto Stave::get_bar_contents() const -> std::vector<BarContent> const& {
        return bar_contents_;
    }

    auto Stave::get_bar_content(std::size_t index) const -> BarContent const& {
        return bar_contents_.at(index);
    }

    auto Stave::add_bar(double total_duration) -> void {
        bar_contents_.emplace_back(total_duration);
    }
} // namespace notation
