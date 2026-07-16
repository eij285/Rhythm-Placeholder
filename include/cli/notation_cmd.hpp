#pragma once
#include <optional>
#include <string>

namespace cli {

struct NotationOptions {
    std::optional<std::string> file;
    bool read_only = false;
};

auto parse_notation_args(int argc, char* argv[]) -> NotationOptions;
auto run_notation(int argc, char* argv[]) -> int;

} // namespace cli
