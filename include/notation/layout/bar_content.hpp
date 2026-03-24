#pragma once
#include "notation/elements/musical_element.hpp"

#include <utility>
#include <vector>

namespace notation {
    class BarContent {
     public:
        BarContent(int total_duration);

        [[nodiscard]] auto get_elements() const -> std::vector<std::unique_ptr<MusicalElement>> const&;
        [[nodiscard]] auto remaining_duration() const -> double;
        [[nodiscard]] auto empty() const -> bool;

        auto try_add(std::unique_ptr<MusicalElement> element) -> bool;
        auto clear() -> void;

     private:
        std::vector<std::unique_ptr<MusicalElement>> elements_;
        double total_duration_;
        double used_duration_;
    };
} // namespace notation
