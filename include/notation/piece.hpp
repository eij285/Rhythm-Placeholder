#include "part.hpp"
#include <vector>

namespace notation {
    class Piece {
     public:
        Piece();

     private:
        std::vector<Part> parts_;
    };
} // namespace notation