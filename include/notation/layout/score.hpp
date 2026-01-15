#pragma once
#include "notation/layout/bar_info.hpp"
#include "notation/layout/part.hpp"

#include <vector>

namespace notation {
    class Score {
     public:
        Score(std::string title = "Untitled", std::string author = "");
        Score(std::vector<Part> parts, std::string title = "Untitled", std::string author = "");

        [[nodiscard]] auto get_title() const -> std::string;
        [[nodiscard]] auto get_author() const -> std::string;
        [[nodiscard]] auto get_parts() const -> std::vector<Part> const&;
        [[nodiscard]] auto get_timeline() const -> std::vector<BarInfo> const&;

        auto add_new_bar() -> BarInfo&;
        auto add_part(Part part) -> Part&;

     private:
        std::string title_;
        std::string author_;

        std::vector<BarInfo> timeline_;
        std::vector<Part> parts_;
    };
} // namespace notation
