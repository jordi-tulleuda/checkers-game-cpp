#ifndef TAULER_H
#define TAULER_H

#include <fstream>

#include "Fitxa.h"
#include "posicio.hpp"
#include "info_joc.hpp"

/**
* CLASS Tauler
* Representa el tauler de dames amb una matriu 8x8 de fitxes. Gestiona:
* - Inicialització des de fitxer.
* - Actualització de moviments vàlids.
* - Moviment de fitxes.
* - Conversió a format visual (string).
*/
class Tauler
{
public:
	Tauler();
	~Tauler();

	void inicialitza(const string& nomFitxer);
	void actualitzaMovimentsValids();
	void getPosicionsPossibles(const Posicio& origen, vector<Posicio>& posicionsPossibles);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string toString() const;
	Fitxa* getFitxaActual(int posY, int posX) { return m_tauler[posY][posX]; }
	Fitxa* fitxaAmbMesCaptures(ColorFitxa color);
	Fitxa* getMovAmbMillorPuntuacio(ColorFitxa color, Moviment& millorMov);
	void visualitza();
	bool comprovaFinalPartida(ColorFitxa& color);

private:
	Fitxa* m_tauler[N_FILES][N_COLUMNES]; // Passsat a memoria dinamica

	bool totesCapturesPossiblesFetes(const Fitxa& fitxaOrigen, int index) const;
	void eliminaFitxes(const Moviment& mov);
	bool promoureLaFitxa(const Fitxa& fitxaOrigen, const Posicio& desti);
	bool movimentMesMatador(const Moviment& mov);
	void efectuaMoviment(const Posicio& origen, const Posicio& desti, const Moviment& mov, Fitxa* fitxaOrigen);

	bool posicioDinsArray(const Posicio& pos, const vector<Posicio>& posicionsPossibles) const;

	char tipusFitxaToChar(TipusFitxa tipus, ColorFitxa color) const;

	void allibera(int fila, int columna);

};

#endif