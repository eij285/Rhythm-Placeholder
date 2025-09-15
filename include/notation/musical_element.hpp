#pragma once
#include <string>

namespace notation {
    class MusicalElement {
     public:
        MusicalElement(double duration, int voice = 0);
        virtual ~MusicalElement() = default;

        [[nodiscard]] auto get_voice() const -> int;
        [[nodiscard]] auto get_duration() const -> double;
        virtual auto print() const -> std::string = 0;

     private:
        double duration_;
        int voice_;
    };
} // namespace notation
