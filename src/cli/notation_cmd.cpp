#include "cli/notation_cmd.hpp"
#include "cli/renderer.hpp"
#include "notation/layout/score.hpp"
#include <cctype>
#include <iostream>
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

auto print_help() -> void {
    std::cout << "/h              Show this help message\n"
                  "/p \"name\" <n>   Add a part with n staves, e.g. /p \"Piano\" 2\n"
                  "/q              Quit\n";
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

auto run_write_mode(notation::Score& score) -> void {
    std::cout << "Write Mode: /h for list of commands\n";

    auto running = true;
    while (running) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        auto const args = split_args(line);
        auto const& cmd = args[0];

        if (cmd == "/h") {
            print_help();
        } else if (cmd == "/q") {
            running = false;
        } else if (cmd == "/p") {
            handle_add_part(score, args);
        } else {
            std::cout << "Unknown command: " << cmd << " (try /h)\n";
        }
    }
}

auto run_read_mode(notation::Score const& score) -> void {
    std::cout << "Read Mode: /q to quit\n";
    // TODO: real score rendering once layout/UI exists
    render_header(score);

    auto running = true;
    while (running) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line == "/q") running = false;
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

    if (opts.read_only) {
        run_read_mode(score);
    } else {
        run_write_mode(score);
    }

    return 0;
}

} // namespace cli
