#include "bridge.hpp"

namespace notation {
    auto make_score(rust::String title, rust::String author) -> std::unique_ptr<Score> {
        return std::make_unique<Score>(std::string(title), std::string(author));
    }

    auto score_title(Score const& score) -> rust::String {
        return rust::String(score.get_title());
    }

    auto score_author(Score const& score) -> rust::String {
        return rust::String(score.get_author());
    }
} // namespace notation
