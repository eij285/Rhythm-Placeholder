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
        auto add_part(std::string instrument, int no_staves) -> Part&;

        // TODO: add_element(size_t bar_index, size_t part_index, size_t stave_index,
        //                    std::unique_ptr<MusicalElement>) -> bool
        // Score looks up total_duration from timeline and delegates to BarContent::try_add,
        // so clients never need to supply or know the duration themselves.

     private:
        std::string title_;
        std::string author_;

        std::vector<BarInfo> timeline_;
        std::vector<Part> parts_;
    };
} // namespace notation
