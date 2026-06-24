#include "notation/layout/bar_info.hpp"

namespace notation {
    TimeSignature::TimeSignature(int top, int bot)
    : top_{top}, bot_{bot} {}

    auto TimeSignature::get_top() const -> int {
        return top_;
    }

    auto TimeSignature::get_bot() const -> int {
        return bot_;
    }

    auto TimeSignature::set(int top, int bot) -> void {
        top_ = top;
        bot_ = bot;
    }

    BarInfo::BarInfo(TimeSignature time_signature, KeySignature key_signature, int tempo)
    : time_signature_{time_signature}, key_signature_{key_signature}, tempo_{tempo} {}

    auto BarInfo::get_time_signature() const -> TimeSignature {
        return time_signature_;
    }

    auto BarInfo::get_key_signature() const -> KeySignature {
        return key_signature_;
    }

    auto BarInfo::get_tempo() const -> int {
        return tempo_;
    }

    auto BarInfo::set_time_signature(TimeSignature time_signature) -> void {
        time_signature_ = time_signature;
    }

    auto BarInfo::set_time_signature(int top, int bot) -> void {
        time_signature_.set(top, bot);
    }

    auto BarInfo::set_key_signature(KeySignature key_signature) -> void {
        key_signature_ = key_signature;
    }

    auto BarInfo::set_tempo(int tempo) -> void {
        tempo_ = tempo;
    }
} // namespace notation