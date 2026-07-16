#include "cli/notation_cmd.hpp"
#include <iostream>
#include <string_view>

auto main(int argc, char* argv[]) -> int {
    if (argc < 2) {
        std::cerr << "Usage: notate_cli --notation [-r] [-f <file>]\n";
        return 1;
    }

    auto const cmd = std::string_view{argv[1]};

    if (cmd == "--notation") {
        return cli::run_notation(argc - 2, argv + 2);
    }

    std::cerr << "Unknown command: " << cmd << "\n";
    std::cerr << "Usage: notate_cli --notation [-r] [-f <file>]\n";
    return 1;
}
