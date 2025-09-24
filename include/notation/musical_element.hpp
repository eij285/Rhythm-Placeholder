#pragma once
#include <memory>
#include <initializer_list>

namespace notation {
    class Pitch;

    class MusicalElement {
     public:
        MusicalElement(double duration, int voice = 0);
        virtual ~MusicalElement() = default;

        [[nodiscard]] auto get_voice() const -> int;
        [[nodiscard]] auto get_duration() const -> double;

        [[nodiscard]] virtual auto with_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> = 0;
        [[nodiscard]] virtual auto with_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> = 0;

        [[nodiscard]] virtual auto without_pitch(Pitch const& pitch) const -> std::unique_ptr<MusicalElement> = 0;
        [[nodiscard]] virtual auto without_pitches(std::initializer_list<Pitch> const& pitches) const
            -> std::unique_ptr<MusicalElement> = 0;

     private:
        double duration_;
        int voice_;
    };
} // namespace notation
