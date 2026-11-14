#ifndef POSICIO_HPP
#define POSICIO_HPP

#include <string>
#include <iostream>

using namespace std;

const int N_FILES = 8;
const int N_COLUMNES = 8;

/**
* CLASS Posicio
* Representa una posició al tauler de dames, amb mètodes per convertir entre:
* El format "a1", "h8"... i coordenades (fila: 0-7, columna: 0-7).
*/
class Posicio
{
public:
    Posicio() : m_fila(0), m_columna(0) {}
    Posicio(const string& posicioStr);
    Posicio(int fila, int columna);

    int getFila() const { return m_fila; }
    int getColumna() const { return m_columna; }

    string toString() const;
    void fromString(const string& pos);

    void setCoordenades(int fila, int columna);

    bool operator==(const Posicio& other) const;
    bool operator!=(const Posicio& other) const;

private:
    int m_fila;
    int m_columna;
};

ostream& operator<<(ostream& out, const Posicio& pos);

#endif