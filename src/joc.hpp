#ifndef JOC_H
#define JOC_H

#include <stdio.h>
#include "info_joc.hpp"
#include "tauler.hpp"
#include <queue>
#include <forward_list>

using namespace std;

/**
* CLASS Joc
* Representa el joc de dames, gestionant la lògica del joc, els moviments i la interacció amb el tauler.
* - Inicialització del joc.
* - Actualització de l'estat del joc en resposta a les accions del jugador o de la màquina.
* - Gestió del torn dels jugadors.
* - Dibuix dels moviments vàlids i de l'estat del joc.
* - Finalització del joc i visualització de resultats.
*/
class Joc
{
public:
    Joc();

    void inicialitza(ModeJoc mode, ColorFitxa color, const string& nomFitxerTauler, const string& nomFitxerMoviments);
    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus);
    void finalitza();

private:
    Tauler m_tauler;
    ModeJoc m_modeJoc;
    ColorFitxa m_tornJugador;
    ColorFitxa m_colorJugador;
    string m_nomFitxerMoviments;
    Fitxa* m_fitxaActual;
    bool m_fitxaSeleccionada;
    vector<Posicio> m_posicionsPossibles;
    bool m_mouseActiu;
    queue<string> m_cuaMoviments;

    bool intentaMoureFitxa(const Posicio& origen, const Posicio& desti);
    void intentaFerMoviment(const Posicio& posOrigen, const Posicio& posDesti, bool& finalPartida);
    void dibuixaMovimentsValids();
    bool ratoliDinsTauler(int mousePosX, int mousePosY) const;
    void canviaTorn();
    void ensenyaText(bool finalPartida);
    bool accioJugador(int mousePosX, int mousePosY, bool mouseStatus);
    bool accioMaquina();
};

#endif 