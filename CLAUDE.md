# Project Overview

Music notation software written in C++20.

Current focus is representing sheet music using OOP.

The goal is to ship a minimal MVP by August.

## Current Features

- Note representation
- Staff representation
- Measure representation

## Not Yet Implemented

- Full notation editing
- UI
- Production deployment

## Future Implementations (Post-MVP)

### Ranged staves per Part

Parts currently have a fixed number of staves for the full duration of the score. Some scores require temporary extra staves for a bar range (e.g. a piano adding a 3rd stave, or an orchestral cue stave showing another part's entry).

The agreed approach is a `StaveSlot` wrapper held by `Part` instead of bare `Stave` objects:

```cpp
struct StaveSlot {
    Stave stave;
    size_t start_bar = 0;
    std::optional<size_t> end_bar = std::nullopt; // nullopt = active until end of score
};
```

`Part` changes from `vector<Stave>` to `vector<StaveSlot>`. Range checks go in `Part::add_bar_to_staves`. The range is a `Part`-level scheduling concern; `Stave` stays a pure bar content container with no layout awareness. `Score` gains a new entry point for adding a ranged stave to an existing part.

### Passkey pattern to replace friend chain

`Score`, `Part`, and `Stave` currently use a friend chain for privileged access. As `Score`'s API grows, `Part` risks accumulating boilerplate pass-through methods that only exist to relay `Score`'s calls to `Stave`.

The passkey pattern removes the need for `friend` and the intermediary chain. A private token type is owned by `Score`; methods that only `Score` should call are made public but require a `ScoreKey` argument that only `Score` can construct:

```cpp
class ScoreKey {
    ScoreKey() = default;
    friend class Score;
};

// In Part and Stave — public, but gated by ScoreKey
auto add_element_to_bar(size_t stave_index, size_t bar_index,
                        std::unique_ptr<MusicalElement>, double total_duration,
                        ScoreKey) -> bool;
```

`Score` passes `ScoreKey{}` at the call site; no other caller can construct it. This lets `Score` call directly into `Stave` without `Part` acting as an intermediary, and removes all `friend` declarations.

Revisit if `Part` accumulates too many pass-through methods.

## Current Priority

1. Clean music data model
2. OOP design for musical structures
3. Basic rendering/UI
4. Playback (optional, may be removed)

## Coding Standards

- Modern C++20
- No raw owning pointers
- Use std::unique_ptr
- Prefer STL algorithms
- Unit tests required

## Expectations

Before making major changes:
- Explain proposed changes
- Explain architecture concerns
- Ask before deleting major functionality