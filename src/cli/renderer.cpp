#include "cli/renderer.hpp"
#include <iostream>

namespace cli {

auto render_header(notation::Score const& score) -> void {
    std::cout << score.get_title() << "\n";

    if (score.get_author() != "") {
        std::cout << "Author: " << score.get_author() << "\n\n";
    } else {
        std::cout << "\n";
    }

}

} // namespace cli
