#pragma once

namespace notation {
    enum class KeySignature {
        C = 0, G = 1, D = 2, A = 3, E = 4, B = 5, Fs = 6, Cs = 7,
        F = -1, Bb = -2, Eb = -3, Ab = -4, Db = -5, Gb = -6, Cb = -7
    };

    struct TimeSignature {
     public:
        TimeSignature(int top = 4, int bot = 4);

        [[nodiscard]] auto get_top() const -> int;
        [[nodiscard]] auto get_bot() const -> int;
        [[nodiscard]] auto total_duration() const -> double;

        auto set(int top, int bot) -> void;

     private:
        int top_;
        int bot_;
    };

    struct BarInfo {
     public:
        BarInfo(TimeSignature time_signature = TimeSignature(), KeySignature key_signature = KeySignature::C,
                int tempo = 120);

        [[nodiscard]] auto get_time_signature() const -> TimeSignature;
        [[nodiscard]] auto get_key_signature() const -> KeySignature;
        [[nodiscard]] auto get_tempo() const -> int;

        auto set_time_signature(TimeSignature time_signature) -> void;
        auto set_time_signature(int top, int bot) -> void;
        auto set_key_signature(KeySignature key_signature) -> void;
        auto set_tempo(int tempo) -> void;

     private:
        TimeSignature time_signature_;
        KeySignature key_signature_;
        int tempo_;
    };
}
