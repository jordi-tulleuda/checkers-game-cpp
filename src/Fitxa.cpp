#include "Fitxa.h"

/**
* Fitxa
* Constructor per defecte de la classe Fitxa
*/
Fitxa::Fitxa()
{
	m_tipus = TIPUS_NORMAL;
	m_color = COLOR_BLANC;
}

/**
* Fitxa
* Constructor de la classe Fitxa
* @param t: el tipus de fitxa
* @param c: el color de la fitxa
* @param pos: la posicio de la fitxa
*/
Fitxa::Fitxa(TipusFitxa t, ColorFitxa c, const Posicio& pos)
{
	m_tipus = t;
	m_color = c;
	m_posicio = pos;
}

Fitxa::~Fitxa()
{

}

/**
* getMovMesCapturador
* Aquesta funcio retorna el moviment mes capturador de la fitxa
* @return retorna el moviment mes capturador
*/
Moviment Fitxa::getMovMesCapturador() const
{
	Moviment movMesCapturador;
	for (int i = 0; i < m_movimentsValids.size(); i++)
	{
		if (m_movimentsValids[i].getNumPecesCapturades() > movMesCapturador.getNumPecesCapturades()) //Si el moviment de la iteracio te mes captures que el que haviem trobat
		{
			movMesCapturador = m_movimentsValids[i]; //Assignem el nou moviment com el mes capturador
		}
	}
	return movMesCapturador;
}

/**
* posicioDinsTauler
* Aquesta funcio determina si una posicio es dins del tauler o no
* @return retorna el valor boolea de si la posicio es dins del tauler o no
*/
bool Fitxa::posicioDinsTauler(const Posicio& pos) const
{
	bool posValida = pos.getFila() >= 0 && pos.getFila() <= 7 &&
		pos.getColumna() >= 0 && pos.getColumna() <= 7;
	return posValida;
}

/**
* posicioDinsTauler
* Aquesta funcio determina si una posicio diagonal a la de una fitxa es dins del tauler o no
* @param direccioY: la coordenada y del moviment diagonal
* @param direccioX: la coordenada x del moviment diagonal
* @return retorna el valor boolea de si la posicio es dins del tauler o no
*/
bool Fitxa::posicioDinsTauler(const Posicio& pos, int direccioY, int direccioX) const
{
	Posicio nova(pos.getFila() + direccioY, pos.getColumna() + direccioX);
	return posicioDinsTauler(nova);
}

/**
* determinaMovimentsValids
* Aquesta funcio determina els moviments valids que pot fer una fitxa
* @param tauler: el tauler de joc
*/
void Fitxa::determinaMovimentsValids(Fitxa* tauler[][N_COLUMNES])
{
	m_movimentsValids.clear();
	vector<Moviment> movPendents;
	Moviment movActual;

	vector<bool> posMatadora;
	vector<Posicio> posValides;
	vector<Posicio> pecesCapturades;
	Posicio posActual = m_posicio;

	posicionsValides(posActual, tauler, posValides, posMatadora, pecesCapturades, movActual); //Obtenim les primeres posicions valides on es pot moure des de la posicio actual

	afegeixMovimentPendent(movPendents, movActual, posValides, posMatadora, pecesCapturades); //Afegim les posicions valides a moviments pendents

	while (!movPendents.empty())
	{
		Moviment mov = movPendents.back();
		movPendents.pop_back();

		posActual = mov.getPosicio(mov.getNumPosicions() - 1);

		posValides.clear();
		posMatadora.clear();
		pecesCapturades.clear();
		posicionsValides(posActual, tauler, posValides, posMatadora, pecesCapturades, mov);

		if (mov.getNumPosicions() > 0)
		{
			avaluaMoviment(mov, tauler);
			m_movimentsValids.push_back(mov);
		}

		if (!posValides.empty())
		{
			afegeixMovimentPendent(movPendents, mov, posValides, posMatadora, pecesCapturades);
		}
	}
}

/**
* posicionsValides
* Aquesta funcio determina les posicions valides que pot fer una fitxa
* @param pos: la posicio actual de la fitxa
* @param tauler: el tauler de joc
* @param posValides: array de posicions valides
* @param posMatadora: array de booleans que indica si la posicio es matadora o no
* @param numPosValides: el numero de posicions valides
* @param pecesCapturades: array de peces capturades
* @param numPecesCapturades: el numero de peces capturades
* @param movActual: el moviment actual
*/
void Fitxa::posicionsValides(const Posicio& pos, Fitxa* tauler[][N_COLUMNES], vector<Posicio>& posValides, vector<bool>& posMatadora, 
	vector<Posicio>& capturesValides, const Moviment& movActual) const
{
	posValides.clear();
	posMatadora.clear();
	capturesValides.clear();
	if (m_tipus == TIPUS_NORMAL)
	{
		if (m_color == COLOR_BLANC)
		{
			for (int i = 0; i < MAX_DIRECCIONS / 2; i++)
			{
				posValidesFitxesDireccio(pos, tauler, posValides, posMatadora, capturesValides, movActual, dY[i], dX[i]);
			}
		}
		else
		{
			for (int i = 0; i < MAX_DIRECCIONS / 2; i++)
			{
				posValidesFitxesDireccio(pos, tauler, posValides, posMatadora, capturesValides, movActual, dY[i + 2], dX[i + 2]);
			}
		}
	}
	else
	{
		posValidesDames(pos, tauler, posValides, posMatadora, capturesValides, movActual);
	}
}

/**
* posValidesFitxesDireccio
* Aquesta funcio determina les posicions valides que pot fer una fitxa en una direccio concreta
* @param pos: la posicio actual de la fitxa
* @param tauler: el tauler de joc
* @param posValides: array de posicions valides
* @param posMatadora: array de booleans que indica si la posicio es matadora o no
* @param numPosValides: el numero de posicions valides
* @param pecesCapturades: array de peces capturades
* @param numPecesCapturades: el numero de peces capturades
* @param movActual: el moviment actual
* @param dY: la coordenada y del moviment
* @param dX: la coordenada x del moviment
*/
void Fitxa::posValidesFitxesDireccio(const Posicio& pos, Fitxa* tauler[][N_COLUMNES], vector<Posicio>& posValides, vector<bool>& posMatadora, 
	vector<Posicio>& capturesValides, const Moviment& movActual, int dY, int dX) const
{
	if (posicioDinsTauler(pos, dY, dX))
	{
		if (tauler[pos.getFila() + dY][pos.getColumna() + dX] == nullptr && movActual.getNumPosicions() < 1)
		{
			Posicio novaPosicio((pos.getFila() + dY), pos.getColumna() + dX);
			if (!movActual.contePosicio(novaPosicio))
			{
				posMatadora.push_back(false);
				posValides.push_back(novaPosicio);
				Posicio capturaBuida;
				capturesValides.push_back(capturaBuida);
			}
		}
		else if (movActual.getMovimentMatador() || movActual.getNumPosicions() < 1)
		{
			if (tauler[pos.getFila() + dY][pos.getColumna() + dX] != nullptr)
			{
				if (tauler[pos.getFila() + dY][pos.getColumna() + dX]->getColor() != m_color)
				{
					if (posicioDinsTauler(pos, 2 * dY, 2 * dX))
					{
						if (tauler[pos.getFila() + 2 * dY][pos.getColumna() + 2 * dX] == nullptr)
						{
							Posicio novaPosicio((pos.getFila() + 2 * dY), pos.getColumna() + 2 * dX);
							if (!movActual.contePosicio(novaPosicio))
							{
								posMatadora.push_back(true);
								posValides.push_back(novaPosicio);
								Posicio novaCaptura((pos.getFila() + dY), pos.getColumna() + dX);
								capturesValides.push_back(novaCaptura);
							}
						}
					}
				}
			}
		}
	}
}

/**
* posValidesDames
* Aquesta funcio determina les posicions valides que pot fer una dama
* @param pos: la posicio actual de la dama
* @param tauler: el tauler de joc
* @param posValides: array de posicions valides
* @param posMatadora: array de booleans que indica si la posicio es matadora o no
* @param numPosValides: el numero de posicions valides
* @param pecesCapturades: array de peces capturades
* @param numPecesCapturades: el numero de peces capturades
* @param movActual: el moviment actual
*/
void Fitxa::posValidesDames(const Posicio& pos, Fitxa* tauler[][N_COLUMNES], vector<Posicio>& posValides, vector<bool>& posMatadora, 
	vector<Posicio>& pecesCapturades, const Moviment& movActual) const
{
	for (int i = 0; i < MAX_DIRECCIONS; i++)
	{
		if (posicioDinsTauler(pos, dY[i], dX[i]))
		{
			if (tauler[pos.getFila() + dY[i]][pos.getColumna() + dX[i]] == nullptr && !movActual.getMovimentMatador())
			{
				if (movActual.getNumPosicions() < 1 || dY[i] == movActual.getDireccioY() && dX[i] == movActual.getDireccioX())
				{
					Posicio novaPosicio((pos.getFila() + dY[i]), pos.getColumna() + dX[i]);
					if (!movActual.contePosicio(novaPosicio))
					{
						posMatadora.push_back(false);
						posValides.push_back(novaPosicio);
						Posicio capturaBuida;
						pecesCapturades.push_back(capturaBuida);
					}
				}
			}
			else if (movActual.getMovimentMatador() || movActual.getNumPosicions() < 1 || movActual.getDireccioY() == dY[i] && movActual.getDireccioX() == dX[i])
			{
				if (tauler[pos.getFila() + dY[i]][pos.getColumna() + dX[i]] != nullptr)
				{
					if (tauler[pos.getFila() + dY[i]][pos.getColumna() + dX[i]]->getColor() != m_color)
					{
						if (posicioDinsTauler(pos, 2 * dY[i], 2 * dX[i]))
						{
							if (tauler[pos.getFila() + 2 * dY[i]][pos.getColumna() + 2 * dX[i]] == nullptr)
							{
								Posicio novaPosicio((pos.getFila() + 2 * dY[i]), pos.getColumna() + 2 * dX[i]);
								if (!movActual.contePosicio(novaPosicio))
								{
									posMatadora.push_back(true);
									posValides.push_back(novaPosicio);
									Posicio novaCaptura((pos.getFila() + dY[i]), pos.getColumna() + dX[i]);
									pecesCapturades.push_back(novaCaptura);
								}
							}
						}
					}
				}
			}
		}
	}
}

/**
* duplicaMoviment
* Aquesta funcio serveix per duplicar un moviment
* @param moviment: el moviment a duplicar
* @return retorna el moviment duplicat
*/
Moviment Fitxa::duplicaMoviment(const Moviment& moviment)
{
	Moviment nouMoviment;
	for (int i = 0; i < moviment.getNumPosicions(); i++)
	{
		nouMoviment.afegirPosicio(moviment.getPosicio(i));
	}
	for (int i = 0; i < moviment.getNumPecesCapturades(); i++)
	{
		nouMoviment.afegirPecaCapturada(moviment.getPecaCapturada(i));
	}
	return nouMoviment;
}

/**
* afegeixMovimentPendent
* Aquesta funcio serveix per afegir un moviment pendent a l'array de moviments pendents
* @param movPendents: l'array de moviments pendents
* @param numMovPendents: el numero de moviments pendents
* @param movActual: el moviment actual
* @param posValides: l'array de posicions valides
* @param posMatadora: l'array de booleans que indica si la posicio es matadora o no
* @param numPosValides: el numero de posicions valides
* @param pecesCapturades: l'array de peces capturades
* @param numPecesCapturades: el numero de peces capturades
*/
void Fitxa::afegeixMovimentPendent(vector<Moviment>& movPendents, Moviment movActual, vector<Posicio>& posValides, vector<bool>& posMatadora, vector<Posicio>& pecesCapturades)
{
	for (int i = 0; i < posValides.size(); i++)
	{
		if (!movActual.contePosicio(posValides[i]))
		{
			Moviment movPendent = duplicaMoviment(movActual);
			movPendent.afegirPosicio(posValides[i]);
			if (posMatadora[i])
			{
				movPendent.afegirPecaCapturada(pecesCapturades[i]);
				movPendent.setMovimentMatador();
			}
			else if (m_tipus == TIPUS_DAMA)
			{
				determinaDireccioMoviment(movPendent);
			}
			movPendents.push_back(movPendent);
		}
	}
}

/**
* avaluaMoviment
* Aquesta funcio serveix per avaluar un moviment i assignar-li una puntuacio
* @param mov: el moviment a avaluar
* @param tauler: el tauler de joc
*/
void Fitxa::avaluaMoviment(Moviment& mov, Fitxa* tauler[][N_COLUMNES])
{
	int puntuacio = 0;
	Posicio desti = mov.getPosicio(mov.getNumPosicions() - 1);
	for (int i = 0; i < mov.getNumPecesCapturades(); i++) //Per cada peca capturada, suma
	{
		Posicio posPecaCapturada = mov.getPecaCapturada(i);
		if (tauler[posPecaCapturada.getFila()][posPecaCapturada.getColumna()]->getTipus() == TIPUS_DAMA)
		{
			puntuacio += 9;
		}
		else
		{
			puntuacio += 6;
		}
	}
	if (m_tipus == TIPUS_NORMAL) //Donem un bonus a les fitxes normals per evitar que es moguin unicament les dames quan n'hi ha al tauler
	{
		puntuacio++;
	}
	if (desti.getColumna() != 0 && desti.getColumna() != 8) //Si no va als extrems del tauler, suma
	{
		puntuacio++;
	}
	if (m_tipus != TIPUS_DAMA && (desti.getFila() >= 2 && desti.getFila() <= 5 && desti.getColumna() >= 2 && desti.getColumna() <= 5)) //Si va a una posicio del centre, suma
	{
		puntuacio++;
	}
	if (potSerCapturada(m_posicio, tauler)) //Si esta en perill actualment, suma
	{
		puntuacio++;
	}
	if (!potSerCapturada(desti, tauler)) //Si no pot ser capturada despres de fer el moviment, suma
	{
		puntuacio += 3;
		if (m_tipus != TIPUS_DAMA && (m_color == COLOR_BLANC && desti.getFila() < 2 || m_color == COLOR_NEGRE && desti.getFila() > N_FILES - 3)) //Bonificacio per mes agressivitat i coronar
		{
			puntuacio += 2;
		}
	}
	if (m_tipus != TIPUS_DAMA && (m_color == COLOR_BLANC && desti.getFila() == 0 || m_color == COLOR_NEGRE && desti.getFila() == N_FILES - 1)) //Si es promouria a dama, suma
	{
		puntuacio++;
	}
	mov.setPuntuacio(puntuacio);
}

/**
* potSerCapturada
* Aquesta funcio determina si una fitxa pot ser capturada o no
* @param pos: la posicio de la fitxa
* @param tauler: el tauler de joc
* @return retorna el valor boolea de si la fitxa pot ser capturada o no
*/
bool Fitxa::potSerCapturada(const Posicio& pos, Fitxa* tauler[][N_COLUMNES])
{
	int i = 0, direccions = MAX_DIRECCIONS;
	bool capturable = false;
	if (m_tipus == TIPUS_NORMAL)
	{
		if (m_color == COLOR_NEGRE)
		{
			i = 2;
			direccions = MAX_DIRECCIONS;
		}
		else
		{
			direccions = MAX_DIRECCIONS / 2;
		}
	}
	while (i < direccions && !capturable)
	{
		if (posicioDinsTauler(pos, dY[i], dX[i]) && posicioDinsTauler(pos, -dY[i], -dX[i]))
		{
			if (tauler[pos.getFila() + dY[i]][pos.getColumna() + dX[i]] != nullptr)
			{
				if (tauler[pos.getFila() + dY[i]][pos.getColumna() + dX[i]]->getColor() != m_color)
				{
					if (tauler[pos.getFila() - dY[i]][pos.getColumna() - dX[i]] == nullptr || tauler[pos.getFila() - dY[i]][pos.getColumna() - dX[i]]->getPosicio() == m_posicio)
					{
						capturable = true;
					}
				}
			}
		}
		if (!capturable)
		{
			i++;
		}
	}
	return capturable;
}

/**
* determinaDireccioMoviment
* Aquesta funcio serveix per determinar la direccio del moviment
* @param movPendent: el moviment pendent
*/
void Fitxa::determinaDireccioMoviment(Moviment& movPendent)
{
	int direccioY = (movPendent.getPosicio(0).getFila() - m_posicio.getFila());
	int direccioX = (movPendent.getPosicio(0).getColumna() - m_posicio.getColumna());
	movPendent.setDireccioMoviment(direccioY, direccioX);
}

/**
* posicioDinsMovimentsValids
* Aquesta funcio determina si un moviment que vol fer una fitxa es valid o no
* @param desti: la posicio a la qual vol anar una fitxa
* @param indexMoviment: l'index del moviment valid trobat en la cerca
* @return retorna el valor boolea de si la fitxa es dins de l'array m_movimentsValids o no
*/
bool Fitxa::posicioDinsMovimentsValids(const Posicio& desti, int& indexMoviment) const
{
	int i = 0, j;
	bool trobat = false;
	while (i < m_movimentsValids.size() && !trobat)
	{
		j = 0;
		while (j < m_movimentsValids[i].getNumPosicions() && !trobat)
		{
			if (m_movimentsValids[i].getPosicio(m_movimentsValids[i].getNumPosicions() - 1) == desti)
			{
				trobat = true;
				indexMoviment = i;
			}
			else
			{
				j++;
			}
		}
		if (!trobat)
		{
			i++;
		}
	}
	return trobat;
}

/**
* visualitza
* Aquesta funcio visualitza la fitxa en una posicio concreta del tauler
* @param posX: la coordenada x de la posicio on es vol visualitzar la fitxa
* @param posY: la coordenada y de la posicio on es vol visualitzar la fitxa
*/
void Fitxa::visualitza(int posX, int posY)
{
	IMAGE_NAME imatgeFitxa;
	imatgeFitxa = determinaImatge();
	GraphicManager::getInstance()->drawSprite(imatgeFitxa, posX, posY);
}

/**
* determinaImatge
* Aquesta funcio determina quina imatge s'ha de mostrar per una fitxa concreta
* @return retorna la imatge que s'ha de mostrar per la fitxa
*/
IMAGE_NAME Fitxa::determinaImatge() const
{
	IMAGE_NAME imatgeFitxa = GRAFIC_FITXA_BLANCA;
	if (m_tipus == TIPUS_NORMAL)
	{
		if (m_color == COLOR_NEGRE)
		{
			imatgeFitxa = GRAFIC_FITXA_NEGRA;
		}
	}
	else
	{
		if (m_color == COLOR_BLANC)
		{
			imatgeFitxa = GRAFIC_DAMA_BLANCA;
		}
		else
		{
			imatgeFitxa = GRAFIC_DAMA_NEGRA;
		}
	}
	return imatgeFitxa;
}