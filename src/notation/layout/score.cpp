#include "notation/layout/score.hpp"

namespace notation {
    Score::Score(std::vector<Part> parts, std::string title, std::string author)
    : title_{title}, author_{author}, parts_{std::move(parts)} {};

    Score::Score(std::string title, std::string author)
    : Score::Score({}, title, author) {};

    auto Score::get_title() const -> std::string {
        return title_;
    }

    auto Score::get_author() const -> std::string {
        return author_;
    }

    auto Score::get_parts() const -> std::vector<Part> const& {
        return parts_;
    }  

    auto Score::get_timeline() const -> std::vector<BarInfo> const& {
        return timeline_;
    } 

    auto Score::add_new_bar() -> BarInfo& {
        if (timeline_.empty()) {
            // If there are no bars yet, add a default one
            timeline_.emplace_back();

        } else {
            // Otherwise, copy the last bar's settings
            auto last_bar = timeline_.back();
            timeline_.push_back(last_bar);
        }

        return timeline_.back();
    }

    auto Score::add_part(std::string instrument, int no_staves) -> Part& {
        auto part = Part{instrument, no_staves};

        parts_.push_back(std::move(part));
        return parts_.back();
    }
}