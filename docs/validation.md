# Test and build scope

## Portable tests

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The portfolio-only CMake target compiles `Posicio.cpp` and `Moviment.cpp` and checks:

- Board-coordinate conversion and round trips.
- Move metadata and position handling.
- A synthetic initial 8×8 board fixture.

These tests **do not verify** complete gameplay, captures, promotion, the computer opponent, game replay or SDL2 graphics.

## Original graphical application

The original course project uses a supplied SDL2-based graphical environment on Windows/Visual Studio. The supplied graphical library and its graphics-manager implementation are deliberately omitted here, as are third-party binaries and assets. Consequently, this package **cannot build or run the complete graphical application as provided**. Use an authorized local copy of the university project's original dependencies to run the full application.
