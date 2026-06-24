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
            timeline_.emplace_back();
        } else {
            timeline_.push_back(timeline_.back());
        }

        auto const ts = timeline_.back().get_time_signature();
        auto const total_duration = ts.get_top() * (4.0 / ts.get_bot());

        for (auto& part : parts_) {
            part.add_bar_to_staves(total_duration);
        }

        return timeline_.back();
    }

    auto Score::add_part(std::string instrument, int no_staves) -> Part& {
        parts_.emplace_back(instrument, no_staves);

        for (auto const& bar_info : timeline_) {
            auto const ts = bar_info.get_time_signature();
            auto const total_duration = ts.get_top() * (4.0 / ts.get_bot());
            parts_.back().add_bar_to_staves(total_duration);
        }

        return parts_.back();
    }
}