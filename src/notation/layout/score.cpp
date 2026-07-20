#include "notation/layout/score.hpp"

namespace notation {
    Score::Score(std::vector<Part> parts, std::string title, std::string author)
    : title_{title}, author_{author} {
        parts_.reserve(parts.size());
        for (auto& part : parts) {
            parts_.push_back(std::make_unique<Part>(std::move(part)));
        }
    };

    Score::Score(std::string title, std::string author)
    : Score::Score({}, title, author) {};

    auto Score::get_title() const -> std::string {
        return title_;
    }

    auto Score::get_author() const -> std::string {
        return author_;
    }

    auto Score::get_parts() const -> std::vector<std::unique_ptr<Part>> const& {
        return parts_;
    }

    auto Score::get_part(size_t index) const -> Part const& {
        return *parts_.at(index);
    }

    auto Score::get_part(size_t index) -> Part& {
        return *parts_.at(index);
    }

    auto Score::get_timeline() const -> std::vector<std::unique_ptr<BarInfo>> const& {
        return timeline_;
    }

    auto Score::add_new_bar() -> BarInfo& {
        if (timeline_.empty()) {
            timeline_.push_back(std::make_unique<BarInfo>());
        } else {
            timeline_.push_back(std::make_unique<BarInfo>(*timeline_.back()));
        }

        for (auto& part : parts_) {
            part->add_bar_to_staves();
        }

        return *timeline_.back();
    }

    auto Score::add_part(std::string instrument, int no_staves) -> Part& {
        parts_.push_back(std::make_unique<Part>(instrument, no_staves));

        for (size_t i = 0; i < timeline_.size(); ++i) {
            parts_.back()->add_bar_to_staves();
        }

        return *parts_.back();
    }
}
