#include "bar.hpp"

namespace notation {
    class Stave {
     public:
        Stave();
     private:
        std::vector<Bar> bars;
    };
} // namespace notation