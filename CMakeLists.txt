cmake_minimum_required(VERSION 3.12)
project(checkers_portfolio_tests LANGUAGES CXX)

enable_testing()
add_executable(checkers_tests tests/test_core.cpp src/Posicio.cpp src/Moviment.cpp)
target_include_directories(checkers_tests PRIVATE src)
target_compile_features(checkers_tests PRIVATE cxx_std_11)
add_test(NAME CoordinatesAndMoves COMMAND checkers_tests core)
add_test(NAME SampleBoardData COMMAND checkers_tests board "${CMAKE_CURRENT_SOURCE_DIR}/data/starting-board.txt")
