# Checkers Game — C++ / Programming Methodology

**A checkers (draughts) game developed as a two-person university project** for *Programming Methodology* at Universitat Autònoma de Barcelona (UAB), academic year 2024–2025.

The assignment provided **C++ class headers/interfaces and a graphical library**. Our work focused on **implementing the game logic and connecting it to the supplied graphical framework**. This repository presents the source code for technical review; it does not claim the course-provided components as student-created.

## Implemented gameplay

- 8×8 checkers board with regular pieces and kings.
- Legal-move generation, mandatory captures and capture sequences.
- Promotion to king and game-state management.
- Local player-versus-player and player-versus-computer modes, with heuristic-based move choice for the computer player.
- Replay of saved games and support for text-based game data.
- Integration with the **course-provided SDL2-based graphical library** (the library is not redistributed here).

These features are present in the submitted source, but I have **not independently built and played the complete graphical game** in the portfolio environment.

## What was provided vs. what we developed

| Provided by the course | Developed by the student team |
| --- | --- |
| Class header templates and prescribed interfaces | Implementations of game logic and rules in the C++ source files |
| SDL2-based graphics library/framework | Integration of game modes and interactions with the provided framework |
| Assignment requirements and guidance | Move validation, captures, game-state transitions and computer-opponent behavior |

The header files retained under `src/` were based on the provided templates and may contain team modifications. The course's graphical implementation, including the original `GraphicManager` files credited to a third-party author, **is not included**. This project was developed **in a pair**, not individually. See [attribution](docs/attribution.md).

## Repository layout

```text
src/                       Game-logic implementation and project headers
  Fitxa.cpp                Piece behavior and movement
  Tauler.cpp               Board state and rules
  Moviment.cpp             Move sequence representation
  Posicio.cpp              Board coordinates
  joc.cpp                  Game modes, flow and replay
  main.cpp                 Original game entry point (requires the supplied graphics library)
  *.h / *.hpp              Class definitions based on course-provided headers

tests/test_core.cpp        Additional portfolio-only portable tests
data/starting-board.txt    Synthetic board fixture for the tests
docs/                      Attribution and test limitations
CMakeLists.txt             Builds the portable tests only
```

The original graphical game cannot be compiled from this package alone: it refers to headers, libraries and assets distributed with the course, which have intentionally been left out. Source references to `GraphicManager.h` and `Graphic Lib/` are therefore expected.

## Run portable tests

You need a C++ compiler and CMake. The following commands build and run a small independent test target without SDL2:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The tests check coordinate conversion, movement metadata and a sample starting board. They **do not** verify the full game, captures, AI opponent or graphics. See [validation details](docs/validation.md).

## Academic context

- **Institution:** Universitat Autònoma de Barcelona (UAB)
- **Subject:** Programming Methodology, first year (2024–2025)
- **Team:** Two students
- **Mark:** 10.5
- **Technology:** C++, Visual Studio/Windows, course-provided SDL2 graphics framework

### Attribution and distribution

The main programming work is the implementation of game behavior on top of the course's **provided header skeletons and graphical library**. Some source interfaces and project setup originated from the teaching materials. The library, binary dependencies, sprites and third-party font files are not redistributed. This repository carries no blanket open-source license; publication of collaborative coursework should be agreed with the teammate and permitted by the course.
