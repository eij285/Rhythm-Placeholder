#pragma once
#include "notation/elements/musical_element.hpp"
#include "notation/layout/score.hpp"
#include <cstddef>
#include <memory>
#include <optional>
#include <string>

namespace cli {

auto render_header(notation::Score const& score) -> void;
auto render_score_bar(notation::Score const& score, std::size_t bar_index,
                       std::optional<std::size_t> const& part_index = std::nullopt) -> void;

// Parses a token in the /a "s/m/c/q/sq" note-letter format render_score_bar() prints,
// e.g. "C4q", "(C4-E4-G4)c", "Rq". Returns nullptr if `token` isn't well-formed.
[[nodiscard]] auto parse_element_token(std::string const& token) -> std::unique_ptr<notation::MusicalElement>;

} // namespace cli
