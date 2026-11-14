#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "posicio.hpp"
#include <vector>

/**
* CLASS Moviment
* Representa un moviment vàlid d'una fitxa, incloent les posicions per les que passa i les captures que realitza durant el moviment.
*/
class Moviment
{
public:
    Moviment() : m_movimentMatador(false), m_direccioY(0), m_direccioX(0), m_puntuacio(0) {}

    void afegirPosicio(const Posicio& pos);
    void afegirPecaCapturada(const Posicio& pos);

    int getNumPosicions() const { return m_posicions.size(); }
    int getNumPecesCapturades() const { return m_pecesCapturades.size(); }
    bool getMovimentMatador() const { return m_movimentMatador; }
    int getPuntuacio() const { return m_puntuacio; }
    int getDireccioY() const { return m_direccioY; }
    int getDireccioX() const { return m_direccioX; }

    void setPuntuacio(int puntuacio) { m_puntuacio = puntuacio; }
    void setMovimentMatador() { m_movimentMatador = true; }
    void setDireccioMoviment(int dY, int dX) { m_direccioY = dY, m_direccioX = dX; }

    const Posicio& getPosicio(int index) const { return m_posicions[index]; }
    const Posicio& getPecaCapturada(int index) const { return m_pecesCapturades[index]; }

    bool contePosicio(const Posicio& posicio) const;

private:
    vector<Posicio> m_posicions;
	vector<Posicio> m_pecesCapturades;
    bool m_movimentMatador;
    int m_direccioY;
    int m_direccioX;
    int m_puntuacio;
};

#endif