#include "tauler.hpp"

/**
* Tauler
* Constructor de la classe Tauler
* Inicialitza totes les posicions del tauler a null
*/
Tauler::Tauler()
{
	for (int fila = 0; fila < N_FILES; fila++)
	{
		for (int columna = 0; columna < N_COLUMNES; columna++)
		{
			m_tauler[fila][columna] = nullptr;
		}
	}
}

/**
* Tauler
* Destructor de la classe Tauler
* Allibera totes les fitxes del tauler
*/
Tauler::~Tauler()
{
	for (int fila = 0; fila < N_FILES; fila++)
	{
		for (int columna = 0; columna < N_COLUMNES; columna++)
		{
			allibera(fila, columna);
		}
	}
}

/**
* inicialitza
* Aquesta funcio inicialitza el tauler a partir de les dades d'un fitxer
* @param nomFitxer: nom del fitxer en questio, passat com a string
*/
void Tauler::inicialitza(const string& nomFitxer)
{
	ifstream fitxer;
	fitxer.open(nomFitxer);
	if (fitxer.is_open())
	{
		char tipusFitxa;
		TipusFitxa tipus;
		ColorFitxa color;
		string stringPosicio;
		Posicio posicioFitxa;
		while (!fitxer.eof()) //Mentres no s'acabi el fitxer extreu les dades
		{
			fitxer >> tipusFitxa >> stringPosicio;
			posicioFitxa.fromString(stringPosicio); //Passa la posicio de string a Posicio
			switch (tipusFitxa) //Passa el tipus de la fitxa al format enum corresponent per inicialitzar la nova Fitxa
			{
			case 'O':
				tipus = TIPUS_NORMAL;
				color = COLOR_BLANC;
				break;
			case 'X':
				tipus = TIPUS_NORMAL;
				color = COLOR_NEGRE;
				break;
			case 'D':
				tipus = TIPUS_DAMA;
				color = COLOR_BLANC;
				break;
			case 'R':
				tipus = TIPUS_DAMA;
				color = COLOR_NEGRE;
				break;
			default:
				tipus = TIPUS_NORMAL;
				color = COLOR_BLANC;
				break;
			}
			m_tauler[posicioFitxa.getFila()][posicioFitxa.getColumna()] = new Fitxa(tipus, color, posicioFitxa); //Assigna la fitxa nova a la posicio del tauler corresponent
		}
	}
	fitxer.close();
}

/**
* actualitzaMovimentsValids
* Aquesta funcio actualitza tots els moviments valids que pot fer qualsevol de les peces del tauler
*/
void Tauler::actualitzaMovimentsValids()
{
	for (int fila = 0; fila < N_FILES; fila++)
	{
		for (int columna = 0; columna < N_COLUMNES; columna++)
		{
			if ((fila + columna) % 2 != 0) //Accedim nomes a les caselles que poden contenir fitxes
			{
				if (m_tauler[fila][columna] != nullptr) //Si la posicio conte una fitxa no buida
				{
					m_tauler[fila][columna]->determinaMovimentsValids(m_tauler); //Determinem els seus moviments valids
				}
			}
		}
	}
}

/**
* getPosicionsPossibles
* Aquesta funcio retorna a l'array posicionsPossibles totes les posicions on es podria moure la peça que ocupa la posició origen
* @param origen: la posicio de la fitxa de la qual volem obtenir les posicions possibles
* @param nPosicions: el numero de posicions a les quals pot estar la fitxa
* @param posicionsPossibles: array en el que s'emmagatzemen les posicions possibles de la fitxa origen
*/
void Tauler::getPosicionsPossibles(const Posicio& origen, vector<Posicio>& posicionsPossibles)
{
	Fitxa* fitxa = m_tauler[origen.getFila()][origen.getColumna()];
	if (fitxa != nullptr)
	{
		for (int i = 0; i < fitxa->getNumMovimentsValids(); i++) //Agafa el nombre de moviments valids per iterar correctament
		{
			for (int j = 0; j < fitxa->getMovimentValid(i).getNumPosicions(); j++) //Agafa el nombre de posicions valides de cada moviment
			{
				if (!posicioDinsArray(fitxa->getPosicioValida(i, j), posicionsPossibles)) //Si l'array posicionsPossibles no conte la posicio
				{
					posicionsPossibles.push_back(fitxa->getPosicioValida(i, j));
				}
			}
		}
	}
}

/**
* totesCapturesPossiblesFetes
* Aquesta funcio permet saber si s'ha fet el moviment amb mes captures
* @param fitxaOrigen: la fitxa que ha fet el moviment
* @param index: l'index del moviment que ha fet la fitxa
* @return retorna el valor boolea de si la fitxa ha fet totes les captures possibles o no
*/
bool Tauler::totesCapturesPossiblesFetes(const Fitxa& fitxaOrigen, int index) const
{
	int i = 0;
	bool totesCapturesFetes = true;
	while (i < fitxaOrigen.getNumMovimentsValids() && totesCapturesFetes) //Itera tots els moviments valids
	{
		if (fitxaOrigen.getMovimentValid(i).getNumPecesCapturades() > fitxaOrigen.getMovimentValid(index).getNumPecesCapturades()) //Si n'hi ha algun que mati mes peces que el triat
		{
			totesCapturesFetes = false; //No s'han fet totes les captures
		}
		else
		{
			i++;
		}
	}
	return totesCapturesFetes;
}

/**
* eliminaFitxes
* Aquesta funcio elimina totes les fitxes que una fitxa ha eliminat amb un moviment
* @param fitxaOrigen: la fitxa que ha eliminat les altres
* @param index: index del moviment que ha fet la fitxaOrigen
*/
void Tauler::eliminaFitxes(const Moviment& mov)
{
	Posicio pos;
	for (int i = 0; i < mov.getNumPecesCapturades(); i++)
	{
		pos = mov.getPecaCapturada(i); //Itera totes les captures
		allibera(pos.getFila(), pos.getColumna());
	}
}

/**
* promoureLaFitxa
* Aquesta funcio permet saber si s'ha de promoure una fitxa
* @param fitxaOrigen: la fitxa que volem saber si s'ha de promoure
* @param desti: la posicio on va la fitxa
* @return retorna el valor boolea de si la fitxa s'ha de promoure o no
*/
bool Tauler::promoureLaFitxa(const Fitxa& fitxaOrigen, const Posicio& desti)
{
	bool promoure = false;
	if (fitxaOrigen.getTipus() != TIPUS_DAMA) //Si ja es dama, no cal promoure-la
	{
		if ((desti.getFila() == 0 && fitxaOrigen.getColor() == COLOR_BLANC) || (desti.getFila() == N_FILES - 1 && fitxaOrigen.getColor() == COLOR_NEGRE)) //Si una fitxa arriba a l'ultima fila del seu color
		{
			promoure = true;
		}
	}
	return promoure;
}

/**
* movimentMesMatador
* Aquesta funcio permet saber si el moviment que ha fet una fitxa es el mes matador
* @param mov: el moviment que ha fet la fitxa
* @return retorna el valor boolea de si el moviment es el mes matador o no
*/
bool Tauler::movimentMesMatador(const Moviment& mov)
{
	bool mesMatador = true;
	int pecesCapturades = mov.getNumPecesCapturades();
	int i = 0, j, k;
	while (i < N_FILES && mesMatador)
	{
		j = 0;
		while (j < N_COLUMNES && mesMatador)
		{
			k = 0;
			if ((i + j) % 2 != 0)
			{
				while (k < m_tauler[i][j]->getNumMovimentsValids() && mesMatador) // itera tots els moviments valids
				{
					if (m_tauler[i][j]->getMovimentValid(k).getNumPecesCapturades() > pecesCapturades) // si n'hi ha algun que mati mes peces que el triat
					{
						mesMatador = false;
					}
					else
					{
						k++;
					}
				}
			}
			if (mesMatador)
			{
				j++;
			}
		}
		if (mesMatador)
		{
			i++;
		}
	}
	return mesMatador;
}

/**
* fitxaAmbMesCaptures
* Aquesta funcio retorna la fitxa que ha capturat mes peces
* @return retorna la fitxa que ha capturat mes peces
*/
Fitxa* Tauler::fitxaAmbMesCaptures(ColorFitxa color)
{
	Fitxa* fitxaMesMatadora = nullptr;
	int pecesCapturades = 0;
	for (int fila = 0; fila < N_FILES; fila++)
	{
		for (int columna = 0; columna < N_COLUMNES; columna++)
		{
			if ((fila + columna) % 2 != 0 && m_tauler[fila][columna] != nullptr) // nomes mira les posicions on hi poden haver fitxes del nostre color
			{
				if (m_tauler[fila][columna]->getColor() == color)
				{
					for (int indexMov = 0; indexMov < m_tauler[fila][columna]->getNumMovimentsValids(); indexMov++) // itera tots els moviments valids
					{
						if (m_tauler[fila][columna]->getMovimentValid(indexMov).getNumPecesCapturades() > pecesCapturades) // si n'hi ha algun que mati mes peces que el triat
						{
							pecesCapturades = m_tauler[fila][columna]->getMovimentValid(indexMov).getNumPecesCapturades();
							fitxaMesMatadora = m_tauler[fila][columna];
						}
					}
				}
			}
		}
	}
	return fitxaMesMatadora;
}

/**
* getMovAmbMillorPuntuacio
* Aquesta funcio retorna la fitxa que pot fer el moviment amb millor puntuacio
* @param color: el color de la fitxa que volem moure
* @param millorMov: el moviment amb millor puntuacio que pot fer la fitxa
* @return retorna la fitxa que pot fer el moviment amb millor puntuacio
*/
Fitxa* Tauler::getMovAmbMillorPuntuacio(ColorFitxa color, Moviment& millorMov)
{
	Fitxa* fitxaAMoure = nullptr;
	int millorPuntuacio = -1;
	for (int fila = 0; fila < N_FILES; fila++)
	{
		for (int columna = 0; columna < N_COLUMNES; columna++)
		{
			if ((fila + columna) % 2 != 0)
			{
				if (m_tauler[fila][columna] != nullptr)
				{
					if (m_tauler[fila][columna]->getColor() == color)
					{
						for (int k = 0; k < m_tauler[fila][columna]->getNumMovimentsValids(); k++)
						{
							if (m_tauler[fila][columna]->getMovimentValid(k).getPuntuacio() > millorPuntuacio)
							{
								fitxaAMoure = m_tauler[fila][columna];
								millorMov = m_tauler[fila][columna]->getMovimentValid(k);
								millorPuntuacio = m_tauler[fila][columna]->getMovimentValid(k).getPuntuacio();
							}
						}
					}
				}
			}
		}
	}
	return fitxaAMoure;
}

/**
* posicioDinsArray
* Aquesta funcio determina si una posicio es dins d'un array de posicions
* @param pos: la posicio que volem saber si es dins de l'array
* @param posicionsPossibles: l'array de posicions on volem saber si es troba la posicio
* @param nPosicions: el numero de posicions que hi ha a l'array
* @return retorna el valor boolea de si la posicio es dins de l'array o no
*/
bool Tauler::posicioDinsArray(const Posicio& pos, const vector<Posicio>& posicionsPossibles) const
{
	bool trobat = false;
	int i = 0;
	while (i < posicionsPossibles.size() && !trobat)
	{
		if (posicionsPossibles[i] == pos)
		{
			trobat = true;
		}
		else
		{
			i++;
		}
	}
	return trobat;
}

/**
* mouFitxa
* Aquesta funcio serveix per moure una fitxa, o intentar-ho
* @param origen: la posicio actual de la fitxa
* @param desti: la posicio on va la fitxa
* @return retorna el valor boolea de si s'ha fet el moviment de la fitxa o no
*/
bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
	bool posDinsMovimentsValids = false;
	if (origen != desti && desti.getFila() >= 0 && desti.getFila() <= 7 && desti.getColumna() >= 0 && desti.getColumna() <= 7) //Si la fitxa fa un moviment i el desti esta dins el tauler
	{
		Fitxa* fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
		int indexMoviment = -1;
		posDinsMovimentsValids = fitxaOrigen->posicioDinsMovimentsValids(desti, indexMoviment); //Mira si el desti es un moviment valid, i si ho es obte el seu index per referencia
		bool bufarFitxa = false;
		if (posDinsMovimentsValids)
		{
			const Moviment& movOrigen = fitxaOrigen->getMovimentValid(indexMoviment); //Obtenim el moviment fet
			Fitxa* fitxaMesCapturadora = fitxaAmbMesCaptures(fitxaOrigen->getColor());
			int numCapturesMovMesCapturador = 0;
			if (fitxaMesCapturadora != nullptr)
			{
				eliminaFitxes(movOrigen); //Elimina les fitxes capturades pel moviment
				numCapturesMovMesCapturador = fitxaMesCapturadora->getMovMesCapturador().getNumPecesCapturades();
				if (movOrigen.getNumPecesCapturades() < numCapturesMovMesCapturador) //Si el moviment d origen mata menys que el moviment amb mes captures
				{
					bufarFitxa = true;
				}
			}
			efectuaMoviment(origen, desti, movOrigen, fitxaOrigen);
			if (fitxaMesCapturadora != nullptr && bufarFitxa)
			{
				int fila = 0, columna = 0;
				if (fitxaMesCapturadora == fitxaOrigen)
				{
					fila = desti.getFila();
					columna = desti.getColumna();
				}
				else
				{
					fila = fitxaMesCapturadora->getPosicio().getFila();
					columna = fitxaMesCapturadora->getPosicio().getColumna();
				}
				allibera(fila, columna); //Bufa la fitxa mes capturadora
			}
		}
	}
	return posDinsMovimentsValids;
}

/**
* efectuaMoviment
* Aquesta funcio efectua el moviment d'una fitxa
* @param origen: la posicio d'origen de la fitxa
* @param desti: la posicio de desti de la fitxa
* @param mov: el moviment que ha fet la fitxa
* @param fitxaOrigen: la fitxa que ha efectuat el moviment
*/
void Tauler::efectuaMoviment(const Posicio& origen, const Posicio& desti, const Moviment& mov, Fitxa* fitxaOrigen)
{
	TipusFitxa tipusFinal = fitxaOrigen->getTipus();
	if (promoureLaFitxa(*(fitxaOrigen), desti)) //Si una fitxa arriba a l'ultima fila, promoure-la
	{
		tipusFinal = TIPUS_DAMA; //Converteix la peca en dama
	}
	m_tauler[desti.getFila()][desti.getColumna()] = new Fitxa(tipusFinal, fitxaOrigen->getColor(), desti); //Assigna la fitxa d'origen al desti
	allibera(origen.getFila(), origen.getColumna());
}

/**
* tipusFitxaToChar
* Aquesta funcio passa una fitxa al seu valor char corresponent
* @param tipus: el tipus de la fitxa
* @param color: el color de la fitxa
* @return retorna el caracter char de la fitxa en funcio del tipus i el color seguint les condicions de l'enunciat
*/
char Tauler::tipusFitxaToChar(TipusFitxa tipus, ColorFitxa color) const
{
	char tipusFitxa = '_';
	if (tipus == TIPUS_NORMAL)
	{
		if (color == COLOR_BLANC)
		{
			tipusFitxa = 'O';
		}
		else
		{
			tipusFitxa = 'X';
		}
	}
	else if (tipus == TIPUS_DAMA)
	{
		if (color == COLOR_BLANC)
		{
			tipusFitxa = 'D';
		}
		else
		{
			tipusFitxa = 'R';
		}
	}
	return tipusFitxa;
}

/**
* toString
* Aquesta funcio mostra el tauler en format string
* @return retorna la matriu m_tauler en format string
*/
string Tauler::toString() const
{
	string output;
	for (int fila = 0; fila < N_FILES; fila++)
	{
		output += to_string(N_FILES - fila); //Afegeix el numero de columna al output
		output += ": ";
		for (int columna = 0; columna < N_COLUMNES; columna++)
		{
			if ((fila + columna) % 2 != 0) //Accedim nomes a les caselles que poden contenir fitxes
			{
				if (m_tauler[fila][columna] != nullptr)
				{
					output += tipusFitxaToChar(m_tauler[fila][columna]->getTipus(), m_tauler[fila][columna]->getColor()); //Extreu les dades de la fitxa i les afegeix
				}
				else
				{
					output += '_';
				}
			}
			else
			{
				output += '_';
			}
			output += ' ';
		}
		output += '\n'; //Salt de linia per guardar correctament la proxima fila
	}
	output += "   a b c d e f g h";
	return output;
}

/**
* visualitza
* Aquesta funcio visualitza el tauler a la pantalla, mostrant les fitxes en les seves posicions
*/
void Tauler::visualitza()
{
	int posX, posY;
	for (int fila = 0; fila < N_FILES; fila++)
	{
		for (int columna = 0; columna < N_COLUMNES; columna++)
		{
			if ((fila + columna) % 2 != 0)
			{
				if (m_tauler[fila][columna] != nullptr)
				{
					posX = POS_X_TAULER + CASELLA_INICIAL_X + (columna * AMPLADA_CASELLA);
					posY = POS_Y_TAULER + CASELLA_INICIAL_Y + (fila * ALCADA_CASELLA);
					m_tauler[fila][columna]->visualitza(posX, posY);
				}
			}
		}
	}
}

/**
* comprovaFinalPartida
* Aquesta funcio comprova si s'ha arribat al final de la partida, i si es aixi, retorna el color de la fitxa que ha guanyat
* @param color: el color de la fitxa que ha guanyat
* @return retorna el valor boolea de si s'ha arribat al final de la partida o no
*/
bool Tauler::comprovaFinalPartida(ColorFitxa& color)
{
	int nValidsBlanques = -1, nValidsNegres = -1;
	bool finalPartida = true, algunaBlanca = false, algunaNegra = false;
	int fila = 0;
	while (fila < N_FILES && finalPartida)
	{
		int columna = 0;
		while (columna < N_COLUMNES && finalPartida)
		{
			if ((fila + columna) % 2 != 0 && m_tauler[fila][columna] != nullptr)
			{
				int nValidsActual = m_tauler[fila][columna]->getNumMovimentsValids();
				if (m_tauler[fila][columna]->getColor() == COLOR_BLANC && nValidsActual > nValidsBlanques)
				{
					nValidsBlanques = nValidsActual;
					algunaBlanca = true;
				}
				else if (m_tauler[fila][columna]->getColor() == COLOR_NEGRE && nValidsActual > nValidsNegres)
				{
					nValidsNegres = nValidsActual;
					algunaNegra = true;
				}
				if (nValidsBlanques > 0 && nValidsNegres > 0)
				{
					finalPartida = false;
				}
			}
			if (finalPartida)
			{
				columna++;
			}
		}
		if (finalPartida)
		{
			fila++;
		}
	}
	if (finalPartida)
	{
		if (nValidsBlanques > 0 && nValidsNegres <= 0 && (color == COLOR_BLANC || !algunaNegra))
		{
			color = COLOR_BLANC;
		}
		else if (nValidsBlanques <= 0 && nValidsNegres > 0 && (color == COLOR_NEGRE || !algunaBlanca))
		{
			color = COLOR_NEGRE;
		}
		else
		{
			finalPartida = false;
		}
	}
	return finalPartida;
}

/**
* allibera
* Allibera la posicio al tauler a partir d'una posicio
* @param fila: la fila de la posicio que volem alliberar
* @param columna: la columna de la posicio que volem alliberar
*/
void Tauler::allibera(int fila, int columna) //Allibera la posicio al tauler a partir d'una posicio
{
	if (m_tauler[fila][columna] != nullptr)
	{
		delete m_tauler[fila][columna];
		m_tauler[fila][columna] = nullptr;
	}
}