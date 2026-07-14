#pragma once
#include "notation/elements/musical_element.hpp"

#include <map>
#include <utility>
#include <vector>

namespace notation {
    struct BarEntry {
        double onset;
        std::unique_ptr<MusicalElement> element;
    };

    class BarContent {
     public:
        BarContent();

        [[nodiscard]] auto get_entries() const -> std::vector<BarEntry> const&;
        [[nodiscard]] auto remaining_duration(double total_duration, int voice = 0) const -> double;
        [[nodiscard]] auto empty() const -> bool;

        auto try_add(std::unique_ptr<MusicalElement> element, double total_duration) -> bool;
        auto remove_at(std::size_t index) -> std::unique_ptr<MusicalElement>;
        auto clear() -> void;

     private:
        std::vector<BarEntry> entries_;
        std::map<int, double> voice_cursors_;
    };
} // namespace notation
