#include "notation/rest.hpp"

namespace notation {
    Rest::Rest(double duration, int voice)
    : MusicalElement(duration, voice) {};

    auto Rest::print() const -> std::string {
        // TODO: For testing and debugging purposes
        return "I am a rest";
    }
} // namespace notation
