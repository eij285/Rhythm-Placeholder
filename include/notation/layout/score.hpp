#pragma once
#include "notation/layout/bar_info.hpp"
#include "notation/layout/part.hpp"

#include <memory>
#include <vector>

namespace notation {
    class Score {
     public:
        Score(std::string title = "Untitled", std::string author = "");
        Score(std::vector<Part> parts, std::string title = "Untitled", std::string author = "");

        [[nodiscard]] auto get_title() const -> std::string;
        [[nodiscard]] auto get_author() const -> std::string;
        [[nodiscard]] auto get_parts() const -> std::vector<std::unique_ptr<Part>> const&;
        [[nodiscard]] auto get_part(size_t index) const -> Part const&;
        [[nodiscard]] auto get_part(size_t index) -> Part&;
        [[nodiscard]] auto get_timeline() const -> std::vector<std::unique_ptr<BarInfo>> const&;

        auto add_new_bar() -> BarInfo&;
        auto add_part(std::string instrument, int no_staves) -> Part&;

     private:
        std::string title_;
        std::string author_;

        // unique_ptr, not bare value: add_new_bar()/add_part() hand back live references into
        // these entries, and growing the outer vector only relocates the pointers, never the
        // pointees, so a previously returned Part&/BarInfo& stays valid.
        std::vector<std::unique_ptr<BarInfo>> timeline_;
        std::vector<std::unique_ptr<Part>> parts_;
    };
} // namespace notation
