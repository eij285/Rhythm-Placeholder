#pragma once
#include "notation/layout/score.hpp"
#include "rust/cxx.h"

#include <memory>

// Tauri/cxx-only glue. Not part of the notation core.
//
// cxx's `String` bridges to `rust::String` on the C++ side, not `std::string` - the two
// aren't interchangeable at the ABI boundary, so anything touching Score's real
// std::string-based API needs a small conversion here rather than a direct pass-through.
//
// make_score also stands in for Score's constructor, since cxx can't call a C++
// constructor directly for an opaque type.
namespace notation {
    auto make_score(rust::String title, rust::String author) -> std::unique_ptr<Score>;
    auto score_title(Score const& score) -> rust::String;
    auto score_author(Score const& score) -> rust::String;
} // namespace notation
