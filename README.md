# Rhythm-Placeholder
A small C++20 library for musical notation primitives (notes, rests, chords, bars, staves, parts, pieces).

## Layout
- include/notation/
  - [`notation::MusicalElement`](include/notation/musical_element.hpp)
  - [`notation::Pitch`](include/notation/pitch.hpp) — implementation: [src/notation/pitch.cpp](src/notation/pitch.cpp)
  - [`notation::Note`](include/notation/note.hpp) — implementation: [src/notation/note.cpp](src/notation/note.cpp)
  - [`notation::Rest`](include/notation/rest.hpp) — implementation: [src/notation/rest.cpp](src/notation/rest.cpp)
  - [`notation::Chord`](include/notation/chord.hpp) — implementation: [src/notation/chord.cpp](src/notation/chord.cpp)
  - [`notation::Bar`](include/notation/bar.hpp)
  - [`notation::Stave`](include/notation/stave.hpp)
  - [`notation::Part`](include/notation/part.hpp)
  - [`notation::Piece`](include/notation/piece.hpp)
  - [`notation::notation_globals`](include/notation/notation_globals.hpp)

Source implementations are in `src/notation/` next to the headers listed above.

## Building

Requires CMake (>= 3.15) and a C++20 toolchain.

```sh
mkdir -p build
cd build
cmake ..
cmake --build .