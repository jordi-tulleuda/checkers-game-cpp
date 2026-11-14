# Source origin and contribution boundaries

This repository contains a **source-only portfolio adaptation** of a pair project submitted for the 2024–2025 Programming Methodology course at UAB.

## Supplied by the instructors

According to the student, the instructors supplied **C++ class header templates** (`.h` / `.hpp`) and the **graphics library**. The coursework also specified the required behavior and the main class structure. The final headers in `src/` reflect that starting point and may have been modified while implementing the assignment; their initial interfaces are not presented as newly invented by the team.

The original submitted ZIP also contained graphical support files, SDL2/NFont dependencies and sprites. In particular, the original `GraphicManager.cpp` and `GraphicManager.h` contain a copyright attribution to **Enric Vergara (2021)**. Those two files, the `Graphic Lib/` folder, graphics assets and runtime binaries are **excluded from this package**, rather than being claimed as student-authored or redistributed.

## Implemented by the student team

The pair implemented the functional **game logic** around the supplied class interfaces, including board and piece behavior, valid moves, capture handling, promotion, game modes, saved-game replay and computer-opponent choices. The submission integrates that logic with the provided graphics framework, but the framework itself is not the students' work.

The exact allocation of tasks between the two students is not documented here; no individual claim of exclusive authorship is made.

## Added specifically for the portfolio

The standalone CMake testing configuration, `tests/test_core.cpp`, the synthetic `data/starting-board.txt` fixture and these explanatory documents were created after the original coursework for this portfolio version. They are not presented as university-submitted deliverables.

## Build and sharing limitations

The graphical application is **not self-contained** in this repository: the original authorized course environment is necessary to rebuild it. The small portable tests compile independently. Do not infer that passing them validates the graphical game.

Before publication, confirm permission from the other team member and that sharing adapted course-provided headers complies with the instructor's rules. No open-source license is attached to this combined coursework.
