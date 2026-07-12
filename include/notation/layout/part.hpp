#pragma once
#include "notation/layout/stave.hpp"

#include <vector>

namespace notation {
    class Part {
        friend class Score;

     public:
        Part(std::string instrument = "", int no_staves = 1);

        [[nodiscard]] auto get_instrument() const -> std::string;
        [[nodiscard]] auto get_staves() const -> std::vector<Stave> const&;

     private:
        std::string instrument_;
        std::vector<Stave> staves_;

        auto add_bar_to_staves() -> void;
    };
} // namespace notation
