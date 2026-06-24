#pragma once
#include "notation/layout/bar_content.hpp"

namespace notation {
    class Stave {
        friend class Part;

     public:
        Stave();

        [[nodiscard]] auto get_bar_contents() const -> std::vector<BarContent> const&;
        [[nodiscard]] auto get_bar_content(std::size_t index) const -> BarContent const&;

     private:
        std::vector<BarContent> bar_contents_;

        auto add_bar(double total_duration) -> void;
    };
} // namespace notation
