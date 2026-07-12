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

    auto Part::add_bar_to_staves() -> void {
        for (auto& stave : staves_) {
            stave.add_bar();
        }
    }

    auto Part::add_element_to_bar(size_t stave_index, size_t bar_index,
                                  std::unique_ptr<MusicalElement> element,
                                  double total_duration) -> bool {
        return staves_.at(stave_index).bar_contents_.at(bar_index)
                   .try_add(std::move(element), total_duration);
    }
} // namespace notation
