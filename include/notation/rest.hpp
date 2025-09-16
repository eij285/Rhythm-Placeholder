#pragma once

#include "notation/musical_element.hpp"

namespace notation {
    class Rest : public MusicalElement {
     public:
        Rest(double duration, int voice = 0);
        auto print() const -> std::string override;
    };
} // namespace notation
