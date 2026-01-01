#include "note.hpp"
#include <utility>
#include <vector>

namespace notation {
    class Bar {
     public:
        class iterator {

        };

        Bar(std::pair<int, int> const time_sig = {4, 4});
        Bar(std::vector<Note> const& notes, std::pair<int, int> time_sig = {4, 4});

     private:
        std::pair<int, int> time_sig_;
        std::vector<Note> notes_;

        bool is_repeated_; // TODO
    };
} // namespace notation
