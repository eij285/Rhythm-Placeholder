#include "cli/renderer.hpp"
#include "notation/elements/chord.hpp"
#include "notation/elements/note.hpp"
#include "notation/elements/pitch.hpp"
#include "notation/elements/rest.hpp"
#include "notation/layout/bar_content.hpp"
#include "notation/layout/part.hpp"
#include "notation/layout/stave.hpp"
#include "notation/notation_globals.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace cli {

namespace {

auto near(double a, double b) -> bool {
    return std::abs(a - b) < notation::dur_tolerance;
}

auto pitch_name_str(notation::PitchName name) -> std::string_view {
    switch (name) {
        case notation::PitchName::Cb: return "Cb";
        case notation::PitchName::C:  return "C";
        case notation::PitchName::Cs: return "C#";
        case notation::PitchName::Db: return "Db";
        case notation::PitchName::D:  return "D";
        case notation::PitchName::Ds: return "D#";
        case notation::PitchName::Eb: return "Eb";
        case notation::PitchName::E:  return "E";
        case notation::PitchName::Es: return "E#";
        case notation::PitchName::Fb: return "Fb";
        case notation::PitchName::F:  return "F";
        case notation::PitchName::Fs: return "F#";
        case notation::PitchName::Gb: return "Gb";
        case notation::PitchName::G:  return "G";
        case notation::PitchName::Gs: return "G#";
        case notation::PitchName::Ab: return "Ab";
        case notation::PitchName::A:  return "A";
        case notation::PitchName::As: return "A#";
        case notation::PitchName::Bb: return "Bb";
        case notation::PitchName::B:  return "B";
        case notation::PitchName::Bs: return "B#";
    }
    return "?";
}

auto pitch_str(notation::Pitch const& pitch) -> std::string {
    return std::string(pitch_name_str(pitch.get_name())) + std::to_string(pitch.get_octave());
}

// s = semibreve, m = minim, c = crotchet, q = quaver, sq = semiquaver; '.' per dot.
// duration_ is in quarter-note (crotchet) units, so a crotchet is 1.0, not a semibreve.
constexpr std::pair<double, std::string_view> duration_bases[] = {
    {4.0, "s"}, {2.0, "m"}, {1.0, "c"}, {0.5, "q"}, {0.25, "sq"},
};
constexpr double dot_multipliers[] = {1.0, 1.5, 1.75};

auto duration_letters(double duration) -> std::string {
    for (auto const& [base, letter] : duration_bases) {
        for (auto const multiplier : dot_multipliers) {
            if (near(duration, base * multiplier)) {
                auto const dots = static_cast<int>(std::lround((multiplier - 1.0) / 0.5));
                return std::string(letter) + std::string(dots, '.');
            }
        }
    }

    return "?";
}

// Inverse of duration_letters(): "c" -> 1.0, "c." -> 1.5, "sq" -> 0.25, etc.
auto parse_duration_letters(std::string_view text) -> std::optional<double> {
    std::size_t dots = 0;
    while (!text.empty() && text.back() == '.') {
        text.remove_suffix(1);
        ++dots;
    }
    if (dots >= 3) return std::nullopt; // dot_multipliers only covers 0/1/2 dots

    for (auto const& [base, letter] : duration_bases) {
        if (text == letter) {
            return base * dot_multipliers[dots];
        }
    }

    return std::nullopt;
}

// Reverse of pitch_name_str(): the two-character (sharp/flat) spellings must be checked
// before the bare-letter one for the same letter, or e.g. "C#4" would match "C" first.
constexpr std::pair<std::string_view, notation::PitchName> pitch_name_tokens[] = {
    {"C#", notation::PitchName::Cs}, {"Cb", notation::PitchName::Cb}, {"C", notation::PitchName::C},
    {"D#", notation::PitchName::Ds}, {"Db", notation::PitchName::Db}, {"D", notation::PitchName::D},
    {"E#", notation::PitchName::Es}, {"Eb", notation::PitchName::Eb}, {"E", notation::PitchName::E},
    {"F#", notation::PitchName::Fs}, {"Fb", notation::PitchName::Fb}, {"F", notation::PitchName::F},
    {"G#", notation::PitchName::Gs}, {"Gb", notation::PitchName::Gb}, {"G", notation::PitchName::G},
    {"A#", notation::PitchName::As}, {"Ab", notation::PitchName::Ab}, {"A", notation::PitchName::A},
    {"B#", notation::PitchName::Bs}, {"Bb", notation::PitchName::Bb}, {"B", notation::PitchName::B},
};

struct ParsedPitch {
    notation::Pitch pitch;
    std::size_t length; // characters consumed from the start of the input
};

// Parses a leading "<letter>[#|b]<octave>" prefix, e.g. "C#4" out of "C#4q".
auto parse_pitch_prefix(std::string const& text) -> std::optional<ParsedPitch> {
    for (auto const& [token, name] : pitch_name_tokens) {
        if (text.compare(0, token.size(), token) != 0) continue;

        auto pos = token.size();
        auto const octave_start = pos;
        if (pos < text.size() && text[pos] == '-') ++pos;
        while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) ++pos;
        if (pos == octave_start || (pos == octave_start + 1 && text[octave_start] == '-')) return std::nullopt;

        try {
            auto const octave = std::stoi(text.substr(octave_start, pos - octave_start));
            return ParsedPitch{notation::Pitch(name, octave), pos};
        } catch (std::exception const&) {
            return std::nullopt;
        }
    }

    return std::nullopt;
}

auto element_token(notation::MusicalElement const& element) -> std::string {
    auto const dur = duration_letters(element.get_duration());

    if (dynamic_cast<notation::Rest const*>(&element) != nullptr) {
        return "R" + dur;
    }
    if (auto const* note = dynamic_cast<notation::Note const*>(&element)) {
        return pitch_str(note->get_pitch()) + dur;
    }
    if (auto const* chord = dynamic_cast<notation::Chord const*>(&element)) {
        std::string inner;
        for (auto const& pitch : chord->get_pitches()) {
            if (!inner.empty()) inner += "-";
            inner += pitch_str(pitch);
        }
        return "(" + inner + ")" + dur;
    }

    return "?";
}

struct RenderRow {
    std::string label;
    std::vector<notation::BarEntry const*> entries; // sorted by onset
};

auto find_column(std::vector<double> const& onsets, double onset) -> std::size_t {
    for (std::size_t i = 0; i < onsets.size(); ++i) {
        if (near(onsets[i], onset)) return i;
    }
    return onsets.size();
}

} // namespace

auto render_header(notation::Score const& score) -> void {
    std::cout << score.get_title() << "\n";

    if (score.get_author() != "") {
        std::cout << "Author: " << score.get_author() << "\n\n";
    } else {
        std::cout << "\n";
    }
}

auto render_score_bar(notation::Score const& score, std::size_t bar_index,
                       std::optional<std::string> const& part_name) -> void {
    std::vector<RenderRow> rows;

    for (auto const& part : score.get_parts()) {
        if (part_name && part->get_instrument() != *part_name) continue;

        auto const& staves = part->get_staves();
        for (std::size_t s = 0; s < staves.size(); ++s) {
            RenderRow row;
            row.label = part->get_instrument();
            if (staves.size() > 1) {
                row.label += " (" + std::to_string(s + 1) + ")";
            }

            for (auto const& entry : staves[s].get_bar_content(bar_index).get_entries()) {
                row.entries.push_back(&entry);
            }
            std::sort(row.entries.begin(), row.entries.end(),
                      [](notation::BarEntry const* a, notation::BarEntry const* b) {
                          return a->onset < b->onset;
                      });

            rows.push_back(std::move(row));
        }
    }

    // Union of onset times across all parts/staves: each becomes one aligned column.
    std::vector<double> onsets;
    for (auto const& row : rows) {
        for (auto const* entry : row.entries) {
            onsets.push_back(entry->onset);
        }
    }
    std::sort(onsets.begin(), onsets.end());
    onsets.erase(std::unique(onsets.begin(), onsets.end(), near), onsets.end());

    std::vector<std::vector<std::string>> cells(rows.size(), std::vector<std::string>(onsets.size()));
    for (std::size_t r = 0; r < rows.size(); ++r) {
        for (auto const* entry : rows[r].entries) {
            auto const col = find_column(onsets, entry->onset);
            if (col < onsets.size()) {
                cells[r][col] = element_token(*entry->element);
            }
        }
    }

    std::vector<std::size_t> col_width(onsets.size(), 0);
    for (std::size_t c = 0; c < onsets.size(); ++c) {
        for (auto const& row_cells : cells) {
            col_width[c] = std::max(col_width[c], row_cells[c].size());
        }
    }

    std::size_t label_width = 0;
    for (auto const& row : rows) {
        label_width = std::max(label_width, row.label.size());
    }

    for (std::size_t r = 0; r < rows.size(); ++r) {
        std::cout << rows[r].label << std::string(label_width - rows[r].label.size() + 1, ' ') << "| ";
        for (std::size_t c = 0; c < onsets.size(); ++c) {
            auto const& token = cells[r][c];
            std::cout << token << std::string(col_width[c] - token.size(), ' ');
            if (c + 1 < onsets.size()) std::cout << ' ';
        }
        std::cout << "\n";
    }
}

auto parse_element_token(std::string const& token) -> std::unique_ptr<notation::MusicalElement> {
    if (token.empty()) return nullptr;

    if (token[0] == 'R') {
        auto const dur = parse_duration_letters(std::string_view(token).substr(1));
        if (!dur) return nullptr;
        return std::make_unique<notation::Rest>(*dur);
    }

    if (token[0] == '(') {
        auto const close = token.find(')');
        if (close == std::string::npos) return nullptr;

        auto const dur = parse_duration_letters(std::string_view(token).substr(close + 1));
        if (!dur) return nullptr;

        std::vector<notation::Pitch> pitches;
        auto const inner = token.substr(1, close - 1);
        std::size_t start = 0;
        while (true) {
            auto const dash = inner.find('-', start);
            auto const piece = inner.substr(start, dash == std::string::npos ? std::string::npos : dash - start);

            auto const parsed = parse_pitch_prefix(piece);
            if (!parsed || parsed->length != piece.size()) return nullptr;
            pitches.push_back(parsed->pitch);

            if (dash == std::string::npos) break;
            start = dash + 1;
        }
        if (pitches.empty()) return nullptr;

        return std::make_unique<notation::Chord>(pitches, *dur);
    }

    auto const parsed = parse_pitch_prefix(token);
    if (!parsed) return nullptr;

    auto const dur = parse_duration_letters(std::string_view(token).substr(parsed->length));
    if (!dur) return nullptr;

    return std::make_unique<notation::Note>(parsed->pitch, *dur);
}

} // namespace cli
