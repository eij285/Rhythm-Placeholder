#include "notation/layout/score.hpp"
#include "notation/elements/musical_element.hpp"

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

        for (auto& part : parts_) {
            part.add_bar_to_staves();
        }

        return timeline_.back();
    }

    auto Score::add_element(size_t bar_index, size_t part_index, size_t stave_index,
                            std::unique_ptr<MusicalElement> element) -> bool {
        auto const total_dur = timeline_.at(bar_index).get_time_signature().total_duration();
        return parts_.at(part_index).add_element_to_bar(stave_index, bar_index,
                                                        std::move(element), total_dur);
    }

    auto Score::add_part(std::string instrument, int no_staves) -> Part& {
        parts_.emplace_back(instrument, no_staves);

        for (size_t i = 0; i < timeline_.size(); ++i) {
            parts_.back().add_bar_to_staves();
        }

        return parts_.back();
    }
}