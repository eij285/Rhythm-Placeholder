#include "notation/layout/part.hpp"

namespace notation {
    Part::Part(std::string instrument, int no_staves)
    : instrument_{instrument}, staves_(no_staves) {};

    auto Part::get_instrument() const -> std::string {
        return instrument_;
    }

    auto Part::get_staves() const -> std::vector<Stave> const& {
        return staves_;
    }

    auto Part::add_bar_to_staves(double total_duration) -> void {
        for (auto& stave : staves_) {
            stave.add_bar(total_duration);
        }
    }
} // namespace notation
