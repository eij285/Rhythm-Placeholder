#include "stave.hpp"
#include <vector>

namespace notation {
    class Part {
     public:
        class iterator {

        };

        Part();

     private:
        std::string instrument_;
        std::vector<Stave> staves_;
    };
} // namespace notation
