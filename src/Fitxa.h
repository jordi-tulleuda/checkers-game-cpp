#ifndef FITXA_H
#define FITXA_H

#include "Moviment.h"
#include "GraphicManager.h"

const int MAX_MOVIMENTS = 20;
const int MAX_DIRECCIONS = 4;
const int dY[MAX_DIRECCIONS] = { -1,-1,1,1 };
const int dX[MAX_DIRECCIONS] = { -1,1,-1,1 };

enum TipusFitxa // TIPUS_EMPTY eliminat, ja que no es necessari per a la logica del joc amb memoria dinamica
{
    TIPUS_NORMAL,
    TIPUS_DAMA,
};
enum ColorFitxa
{
    COLOR_NEGRE,
    COLOR_BLANC
};

/**
* CLASS Fitxa
* Representa una peça al tauler de dames. Gestiona el seu tipus, color, posició actual i la llista de moviments vàlids que pot realitzar.
*/
class Fitxa
{
public:
    Fitxa();
    Fitxa(TipusFitxa t, ColorFitxa c, const Posicio& pos);
    ~Fitxa();

    TipusFitxa getTipus() const { return m_tipus; }
    ColorFitxa getColor() const { return m_color; }

    void convertirEnDama() { m_tipus = TIPUS_DAMA; }
    
    int getNumMovimentsValids() const { return m_movimentsValids.size(); }
    const Moviment& getMovimentValid(int index) const { return m_movimentsValids[index]; }
    const Posicio& getPosicio() const { return m_posicio; }
    Posicio getPosicioValida(int numeroMoviment, int numeroPosicio) const { return m_movimentsValids[numeroMoviment].getPosicio(numeroPosicio); }
    Moviment getMovMesCapturador() const;

    void setPosicio(int fila, int columna) { m_posicio = Posicio(fila, columna); }
    void setPosicio(const Posicio& pos) { m_posicio = pos; }

    void determinaMovimentsValids(Fitxa* tauler[][N_COLUMNES]);
    bool posicioDinsMovimentsValids(const Posicio& desti, int& indexMoviment) const;

    void visualitza(int posX, int posY);

private:
    TipusFitxa m_tipus;
    ColorFitxa m_color;
    vector<Moviment> m_movimentsValids;
    Posicio m_posicio;

    Moviment duplicaMoviment(const Moviment& mov);

    void afegeixMovimentPendent(vector<Moviment>& movPendents, Moviment movActual, vector<Posicio>& posValides, vector<bool>& posMatadora, vector<Posicio>& pecesCapturades);
    void posicionsValides(const Posicio& pos, Fitxa* tauler[][N_COLUMNES], vector<Posicio>& posValides, vector<bool>& posMatadora, vector<Posicio>& capturesValides, const Moviment& movActual) const;
    void posValidesFitxesDireccio(const Posicio& pos, Fitxa* tauler[][N_COLUMNES], vector<Posicio>& posValides, vector<bool>& posMatadora, vector<Posicio>& capturesValides, const Moviment& movActual, int dY, int dX) const;
    void posValidesDames(const Posicio& pos, Fitxa* tauler[][N_COLUMNES], vector<Posicio>& posValides, vector<bool>& posMatadora, vector<Posicio>& pecesCapturades, const Moviment& movActual) const;

    void avaluaMoviment(Moviment& mov, Fitxa* m_tauler[][N_COLUMNES]);
    bool potSerCapturada(const Posicio& posDesti, Fitxa* tauler[][N_COLUMNES]);

    void determinaDireccioMoviment(Moviment& movPendent);
    bool posicioDinsTauler(const Posicio& pos) const; //Mira si una posicio a la que es vol accedir es dins del tauler
    bool posicioDinsTauler(const Posicio& pos, const int direccioY, const int direccioX) const; //Mira si una posicio diagonal a la que es vol accedir es dins del tauler

    IMAGE_NAME determinaImatge() const;
};

#endif
