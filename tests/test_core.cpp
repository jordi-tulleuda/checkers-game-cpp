// Added for the public portfolio: small headless tests of the original classes.
// These do not validate capture rules or graphical gameplay.
#include "posicio.hpp"
#include "Moviment.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

static int checks = 0;
static void require(bool condition, const char* message) {
    ++checks;
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        std::exit(1);
    }
}

static void testCore() {
    const Posicio a1("a1"), h8("h8"), d4("d4");
    require(a1.getFila() == 7 && a1.getColumna() == 0, "a1 maps to bottom-left");
    require(h8.getFila() == 0 && h8.getColumna() == 7, "h8 maps to top-right");
    require(d4.toString() == "d4", "square round trip");
    const Posicio pos(5, 1);
    require(pos.toString() == "b3", "numeric coordinates round trip");
    require(pos == Posicio("b3"), "position equality");
    require(pos != d4, "position inequality");
    Posicio mutablePos;
    mutablePos.setCoordenades(1, 6);
    require(mutablePos.toString() == "g7", "position setter");
    mutablePos.fromString("c6");
    require(mutablePos.getFila() == 2 && mutablePos.getColumna() == 2, "position parser");
    std::ostringstream stream;
    stream << d4;
    require(stream.str() == "d4", "position output stream");

    Moviment move;
    require(move.getNumPosicions() == 0 && move.getNumPecesCapturades() == 0, "empty move");
    require(!move.getMovimentMatador() && move.getPuntuacio() == 0, "default move attributes");
    move.afegirPosicio(Posicio("b4"));
    move.afegirPosicio(Posicio("d6"));
    move.afegirPecaCapturada(Posicio("c5"));
    require(move.getNumPosicions() == 2, "move positions recorded");
    require(move.getPosicio(1) == Posicio("d6"), "move endpoint");
    require(move.getNumPecesCapturades() == 1, "captured pieces recorded");
    require(move.getPecaCapturada(0) == Posicio("c5"), "capture square");
    require(move.contePosicio(Posicio("b4")), "contains intermediate square");
    require(!move.contePosicio(Posicio("e7")), "rejects absent square");
    move.setMovimentMatador();
    move.setDireccioMoviment(-1, 1);
    move.setPuntuacio(8);
    require(move.getMovimentMatador(), "capture flag");
    require(move.getDireccioY() == -1 && move.getDireccioX() == 1, "direction setter");
    require(move.getPuntuacio() == 8, "move score setter");
}

static void testBoard(const char* filename) {
    std::ifstream input(filename);
    require(input.good(), "initial board exists");
    char piece;
    std::string square;
    int white = 0, black = 0, total = 0;
    std::set<std::string> occupied;
    while (input >> piece >> square) {
        require(square.size() == 2, "square has two characters");
        require(square[0] >= 'a' && square[0] <= 'h' && square[1] >= '1' && square[1] <= '8', "valid square");
        const Posicio position(square);
        require((position.getFila() + position.getColumna()) % 2 == 1, "piece on playable square");
        require(occupied.insert(square).second, "no overlapping pieces");
        require(piece == 'O' || piece == 'X', "initial piece is a normal checker");
        white += piece == 'O';
        black += piece == 'X';
        ++total;
    }
    require(total == 24, "24 starting pieces");
    require(white == 12 && black == 12, "12 checkers per color");
}

int main(int argc, char** argv) {
    if (argc >= 2 && std::string(argv[1]) == "core") testCore();
    else if (argc >= 3 && std::string(argv[1]) == "board") testBoard(argv[2]);
    else { std::cerr << "Usage: checkers_core_tests core | board <board-file>\n"; return 2; }
    std::cout << "PASS (" << checks << " assertions)\n";
    return 0;
}
