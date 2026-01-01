#include "notation/elements/musical_element.hpp"

namespace notation {
    MusicalElement::MusicalElement(double duration, int voice)
    : duration_{duration}
    , voice_{voice} {};

    auto MusicalElement::get_voice() const -> int {
        return voice_;
    };

    auto MusicalElement::get_duration() const -> double {
        return duration_;
    };
}
