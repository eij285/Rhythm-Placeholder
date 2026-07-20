#include "cli/notation_cmd.hpp"
#include "cli/renderer.hpp"
#include "notation/layout/part.hpp"
#include "notation/layout/score.hpp"
#include "notation/layout/stave.hpp"
#include "notation/notation_globals.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace cli {

namespace {

auto split_args(std::string const& line) -> std::vector<std::string> {
    std::vector<std::string> tokens;
    std::string current;
    auto in_quotes = false;

    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (std::isspace(static_cast<unsigned char>(c)) && !in_quotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) tokens.push_back(current);

    return tokens;
}

enum class Mode { Read, Write };

auto print_help(Mode mode) -> void {
    std::cout << "/h              Show this help message\n"
                  "/m \"mode\"       Switch mode: /m \"Read\" or /m \"Write\"\n";

    if (mode == Mode::Write) {
        std::cout << "/p \"name\" <n>   Add a part with n staves, e.g. /p \"Piano\" 2\n"
                      "/a \"name\" <s> <token>\n"
                      "                Add a note/chord/rest to stave s of a part at the\n"
                      "                current cursor, e.g. /a \"Piano\" 1 C4q\n";
    } else {
        std::cout << "/p \"name\"       Render every bar of one part, e.g. /p \"Piano\"\n"
                      "/s              Render every bar of the whole score\n"
                      "/b \"name\" <n>   Render bar n of one part, e.g. /b \"Piano\" 1\n";
    }

    std::cout << "/q              Quit\n";
}

auto find_part_index(notation::Score const& score, std::string const& name) -> std::optional<std::size_t> {
    auto const& parts = score.get_parts();
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (parts[i]->get_instrument() == name) return i;
    }
    return std::nullopt;
}

auto handle_render_part(notation::Score const& score, std::vector<std::string> const& args) -> void {
    auto const& name = args[1];
    if (!find_part_index(score, name)) {
        std::cout << "No part named \"" << name << "\"\n";
        return;
    }

    auto const& timeline = score.get_timeline();
    if (timeline.empty()) {
        std::cout << "(no bars yet)\n";
        return;
    }

    for (std::size_t bar = 0; bar < timeline.size(); ++bar) {
        std::cout << "Bar " << bar + 1 << ":\n";
        render_score_bar(score, bar, name);
    }
}

auto handle_render_score(notation::Score const& score) -> void {
    auto const& timeline = score.get_timeline();
    if (timeline.empty()) {
        std::cout << "(no bars yet)\n";
        return;
    }

    for (std::size_t bar = 0; bar < timeline.size(); ++bar) {
        std::cout << "Bar " << bar + 1 << ":\n";
        render_score_bar(score, bar);
    }
}

auto handle_render_bar(notation::Score const& score, std::vector<std::string> const& args) -> void {
    if (args.size() != 3) {
        std::cout << "Usage: /b \"<part name>\" <bar>\n";
        return;
    }

    auto const& name = args[1];
    if (!find_part_index(score, name)) {
        std::cout << "No part named \"" << name << "\"\n";
        return;
    }

    int bar_number = 0;
    try {
        bar_number = std::stoi(args[2]);
    } catch (std::exception const&) {
        std::cout << "Invalid bar number: " << args[2] << "\n";
        return;
    }

    auto const& timeline = score.get_timeline();
    if (bar_number < 1 || static_cast<std::size_t>(bar_number) > timeline.size()) {
        std::cout << "Bar " << bar_number << " does not exist (score has " << timeline.size() << " bar(s))\n";
        return;
    }

    render_score_bar(score, static_cast<std::size_t>(bar_number) - 1, name);
}

// `locked_to_read` is true when the CLI was started with -r: the score is read-only for the
// whole session, so switching to Write mode is never allowed, no matter the current mode.
auto handle_switch_mode(Mode& mode, bool locked_to_read, std::vector<std::string> const& args) -> void {
    if (args.size() != 2) {
        std::cout << "Usage: /m \"Read\" or /m \"Write\"\n";
        return;
    }

    auto const& target = args[1];
    if (target == "Read") {
        mode = Mode::Read;
        std::cout << "Switched to Read Mode\n";
    } else if (target == "Write") {
        if (locked_to_read) {
            std::cout << "Cannot switch to Write Mode: this score was opened read-only\n";
            return;
        }
        mode = Mode::Write;
        std::cout << "Switched to Write Mode\n";
    } else {
        std::cout << "Unknown mode \"" << target << "\" (expected \"Read\" or \"Write\")\n";
    }
}

auto handle_add_part(notation::Score& score, std::vector<std::string> const& args) -> void {
    if (args.size() != 3) {
        std::cout << "Usage: /p \"<name>\" <staves>\n";
        return;
    }

    auto const& name = args[1];

    int no_staves = 0;
    try {
        no_staves = std::stoi(args[2]);
    } catch (std::exception const&) {
        std::cout << "Invalid staff count: " << args[2] << "\n";
        return;
    }

    if (no_staves <= 0) {
        std::cout << "Staff count must be positive\n";
        return;
    }

    score.add_part(name, no_staves);
    std::cout << "Added part \"" << name << "\" with " << no_staves << " stave(s)\n";
}

// Whether `stave`'s bar at `bar_index` has room for `duration` beats in `voice`, per the
// timeline's time signature at that bar.
auto bar_has_room(notation::Score const& score, notation::Stave& stave, std::size_t bar_index, int voice,
                   double duration) -> bool {
    auto const total_duration = score.get_timeline()[bar_index]->get_time_signature().total_duration();
    return stave.get_bar_content(bar_index).remaining_duration(total_duration, voice) + notation::dur_tolerance
           >= duration;
}

auto handle_add_note(notation::Score& score, std::vector<std::string> const& args) -> void {
    if (args.size() != 4) {
        std::cout << "Usage: /a \"<part name>\" <stave> <token>, e.g. /a \"Piano\" 1 C4q\n";
        return;
    }

    auto const& name = args[1];
    auto const part_index = find_part_index(score, name);
    if (!part_index) {
        std::cout << "No part named \"" << name << "\"\n";
        return;
    }
    auto& part = score.get_part(*part_index);

    int stave_number = 0;
    try {
        stave_number = std::stoi(args[2]);
    } catch (std::exception const&) {
        std::cout << "Invalid stave number: " << args[2] << "\n";
        return;
    }
    if (stave_number < 1 || static_cast<std::size_t>(stave_number) > part.get_staves().size()) {
        std::cout << "Part \"" << name << "\" has no stave " << stave_number << "\n";
        return;
    }
    auto& stave = part.get_stave(static_cast<std::size_t>(stave_number) - 1);

    auto element = parse_element_token(args[3]);
    if (!element) {
        std::cout << "Invalid note/chord/rest: " << args[3] << "\n";
        return;
    }
    auto const duration = element->get_duration();
    auto const voice = element->get_voice();

    if (score.get_timeline().empty()) {
        score.add_new_bar();
    }

    auto bar_index = score.get_timeline().size() - 1;
    if (!bar_has_room(score, stave, bar_index, voice, duration)) {
        score.add_new_bar();
        bar_index = score.get_timeline().size() - 1;
    }
    if (!bar_has_room(score, stave, bar_index, voice, duration)) {
        std::cout << "Note duration (" << duration << " beats) exceeds an empty bar's capacity\n";
        return;
    }

    auto const total_duration = score.get_timeline()[bar_index]->get_time_signature().total_duration();
    stave.get_bar_content(bar_index).try_add(std::move(element), total_duration);
    std::cout << "Added " << args[3] << " to \"" << name << "\" stave " << stave_number << ", bar " << bar_index + 1
               << "\n";

    if (std::abs(stave.get_bar_content(bar_index).remaining_duration(total_duration, voice)) < notation::dur_tolerance) {
        std::cout << "Bar " << bar_index + 1 << " complete:\n";
        render_score_bar(score, bar_index, name);
    }
}

auto run_session(notation::Score& score, Mode mode, bool locked_to_read) -> void {
    std::cout << (mode == Mode::Write ? "Write Mode" : "Read Mode") << ": /h for list of commands\n";

    auto running = true;
    while (running) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;

        auto const args = split_args(line);
        if (args.empty()) continue;
        auto const& cmd = args[0];

        if (cmd == "/q") {
            running = false;
        } else if (cmd == "/h") {
            print_help(mode);
        } else if (cmd == "/m") {
            handle_switch_mode(mode, locked_to_read, args);
        } else if (mode == Mode::Write) {
            if (cmd == "/p") {
                handle_add_part(score, args);
            } else if (cmd == "/a") {
                handle_add_note(score, args);
            } else {
                std::cout << "Unknown command: " << cmd << " (try /h)\n";
            }
        } else { // Mode::Read
            if (cmd == "/s") {
                handle_render_score(score);
            } else if (cmd == "/b") {
                handle_render_bar(score, args);
            } else if (cmd == "/p") {
                handle_render_part(score, args);
            } else {
                std::cout << "Unknown command: " << cmd << " (try /h)\n";
            }
        }
    }
}

} // namespace

auto parse_notation_args(int argc, char* argv[]) -> NotationOptions {
    NotationOptions opts;
    for (int i = 0; i < argc; ++i) {
        auto const arg = std::string_view{argv[i]};
        if (arg == "-r") {
            opts.read_only = true;
        }
        // else if (arg == "-f" && i + 1 < argc) {
        //     opts.file = argv[++i];
        // }
    }
    return opts;
}

auto run_notation(int argc, char* argv[]) -> int {
    auto const opts = parse_notation_args(argc, argv);

    // TODO: load score from opts.file when serialization is implemented
    std::cout << "Notation CLI\n\n";

    std::string title, author;
    std::cout << "Enter score title: ";
    std::getline(std::cin, title);
    std::cout << "Enter score author: ";
    std::getline(std::cin, author);
    std::cout << "\n";

    auto score = notation::Score(title, author);
    render_header(score);

    auto const initial_mode = opts.read_only ? Mode::Read : Mode::Write;
    run_session(score, initial_mode, opts.read_only);

    return 0;
}

} // namespace cli
