#include "joc.hpp"
#include <iostream>
#include <fstream>
#include <Windows.h>

/**
* Joc
* Constructor per defecte de la classe Joc.
* Inicialitza els atributs del joc a valors per defecte.
*/
Joc::Joc()
{
	m_modeJoc = MODE_JOC_NONE;
	m_tornJugador = COLOR_BLANC;
	m_colorJugador = COLOR_BLANC;
	m_fitxaActual = nullptr;
	m_fitxaSeleccionada = false;
	m_mouseActiu = false;
}

/**
* inicialitza
* Inicialitza el joc amb el mode de joc especificat i els fitxers de tauler i moviments.
* @param mode: El mode de joc a inicialitzar.
* @param nomFitxerTauler: El nom del fitxer que conté la configuració del tauler.
* @param nomFitxerMoviments: El nom del fitxer que conté els moviments del joc.
*/
void Joc::inicialitza(ModeJoc mode, ColorFitxa color, const string& nomFitxerTauler, const string& nomFitxerMoviments)
{
	m_tauler.inicialitza(nomFitxerTauler);
	m_tauler.actualitzaMovimentsValids();
	GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
	GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);
	m_modeJoc = mode;
	if (m_modeJoc == MODE_JOC_NORMAL || m_modeJoc == MODE_JOC_MAQUINA)
	{
		m_nomFitxerMoviments = nomFitxerMoviments;
		m_colorJugador = color;
	}
	else if (mode == MODE_JOC_REPLAY)
	{
		ifstream fitxer;
		fitxer.open(nomFitxerMoviments);
		if (fitxer.is_open())
		{
			string origen, desti;
			fitxer >> origen >> desti;
			while (!fitxer.eof())
			{
				string moviment = origen + " " + desti;
				m_cuaMoviments.push(moviment);
				fitxer >> origen >> desti;
			}
		}
		fitxer.close();
	}
	ensenyaText(false);
	m_tauler.visualitza();
}

/**
* actualitza
* Actualitza l'estat del joc en funció de la posició del ratolí i l'estat del botó del ratolí.
* @param mousePosX: La posició X del ratolí.
* @param mousePosY: La posició Y del ratolí.
* @param mouseStatus: L'estat del botó del ratolí (apretat o no apretat).
* @return Retorna true si la partida ha finalitzat, false en cas contrari.
*/
bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus)
{
	GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0); //Dibuixem fons
	GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER); //Dibuixem tauler
	bool finalPartida = false;
	if (m_modeJoc == MODE_JOC_NORMAL)
	{
		finalPartida = accioJugador(mousePosX, mousePosY, mouseStatus);
	}
	else if (m_modeJoc == MODE_JOC_MAQUINA)
	{
		if (m_tornJugador == m_colorJugador)
		{
			finalPartida = accioJugador(mousePosX, mousePosY, mouseStatus);
		}
		else
		{
			finalPartida = accioMaquina();
		}
	}
	else if (m_modeJoc == MODE_JOC_REPLAY)
	{
		if (mouseStatus && ratoliDinsTauler(mousePosX, mousePosY)) //Si s'ha fet un click dins el tauler
		{
			if (!m_mouseActiu && !m_cuaMoviments.empty())
			{
				string moviment = m_cuaMoviments.front();
				string origen, desti;
				origen = moviment[0];
				origen += moviment[1];
				desti = moviment[3];
				desti += moviment[4];
				Posicio posOrigen(origen), posDesti(desti);
				m_cuaMoviments.pop();
				intentaFerMoviment(posOrigen, posDesti, finalPartida);
				m_mouseActiu = true;
			}
		}
		else if (!mouseStatus)
		{
			m_mouseActiu = false;
		}
	}
	ensenyaText(finalPartida);
	m_tauler.visualitza(); //Visualitzem les fitxes del tauler
	return finalPartida;
}

/**
* finalitza
* Finalitza el joc, escrivint els moviments al fitxer si el mode de joc és normal o màquina.
*/
void Joc::finalitza()
{
	if (m_modeJoc == MODE_JOC_NORMAL || m_modeJoc == MODE_JOC_MAQUINA)
	{
		ofstream fitxer;
		fitxer.open(m_nomFitxerMoviments);
		if (fitxer.is_open())
		{
			string moviment, origen, desti;
			while (!m_cuaMoviments.empty())
			{
				string moviment = m_cuaMoviments.front();
				m_cuaMoviments.pop();
				fitxer << moviment[0] << moviment[1] << moviment[2] << moviment[3] << moviment[4] << endl;
			}
		}
		fitxer.close();
	}
}

/**
* intentaMoureFitxa
* Intenta moure una fitxa des d'una posició d'origen a una posició de destí.
* @param origen: La posició d'origen de la fitxa.
* @param desti: La posició de destí on es vol moure la fitxa.
* @return Retorna true si el moviment s'ha efectuat correctament, false en cas contrari.
*/
bool Joc::intentaMoureFitxa(const Posicio& origen, const Posicio& desti)
{
	bool movimentCorrecte = m_tauler.mouFitxa(origen, desti);
	if (movimentCorrecte && (m_modeJoc == MODE_JOC_NORMAL || m_modeJoc == MODE_JOC_MAQUINA))
	{
		string moviment = origen.toString() + " " + desti.toString();
		m_cuaMoviments.push(moviment);
		m_fitxaSeleccionada = false;
	}
	return movimentCorrecte;
}

/**
* intentaFerMoviment
* Intenta fer un moviment des d'una posició d'origen a una posició de destí, actualitzant l'estat del joc.
* @param posOrigen: La posició d'origen de la fitxa.
* @param posDesti: La posició de destí on es vol moure la fitxa.
* @param finalPartida: Referència a una variable booleana que indica si la partida ha finalitzat.
*/
void Joc::intentaFerMoviment(const Posicio& posOrigen, const Posicio& posDesti, bool& finalPartida)
{
	bool movimentEfectuat = intentaMoureFitxa(posOrigen, posDesti);
	if (movimentEfectuat) //Si hem fet algun moviment
	{
		m_tauler.actualitzaMovimentsValids(); //Actualitzem els moviments valids de totes les fitxes del tauler
		finalPartida = m_tauler.comprovaFinalPartida(m_tornJugador);
		if (!finalPartida)
		{
			canviaTorn();
		}
	}
}

/**
* dibuixaMovimentsValids
* Dibuixa els moviments vàlids de la fitxa seleccionada al tauler.
*/
void Joc::dibuixaMovimentsValids()
{
	for (int i = 0; i < m_posicionsPossibles.size(); i++)
	{
		int posX = POS_X_TAULER + CASELLA_INICIAL_X + (m_posicionsPossibles[i].getColumna() * AMPLADA_CASELLA);
		int posY = POS_Y_TAULER + CASELLA_INICIAL_Y + (m_posicionsPossibles[i].getFila() * ALCADA_CASELLA);
		GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, posX, posY); //Dibuixem requadre verd indicador de posicio valida
	}
}

/**
* ratoliDinsTauler
* Comprova si el ratolí està dins del tauler de joc.
* @param mousePosX: La posició X del ratolí.
* @param mousePosY: La posició Y del ratolí.
* @return Retorna true si el ratolí està dins del tauler, false en cas contrari.
*/
bool Joc::ratoliDinsTauler(int mousePosX, int mousePosY) const
{
	return ((mousePosX >= (POS_X_TAULER + CASELLA_INICIAL_X)) && (mousePosY >= POS_Y_TAULER + CASELLA_INICIAL_Y) &&
		(mousePosX <= (POS_X_TAULER + CASELLA_INICIAL_X + AMPLADA_CASELLA * NUM_COLS_TAULER)) &&
		(mousePosY <= (POS_Y_TAULER + CASELLA_INICIAL_Y + ALCADA_CASELLA * NUM_FILES_TAULER)));
}

/**
* canviaTorn
* Canvia el torn del jugador actual.
* Si el torn actual és de les blanques, canvia a les negres i viceversa.
*/
void Joc::canviaTorn()
{
	if (m_tornJugador == COLOR_BLANC)
	{
		m_tornJugador = COLOR_NEGRE;
	}
	else
	{
		m_tornJugador = COLOR_BLANC;
	}
}

/**
* ensenyaText
* Mostra text al tauler (mode de joc, jugador actual i el guanyador si la partida ha finalitzat).
* @param finalPartida: Indica si la partida ha finalitzat.
*/
void Joc::ensenyaText(bool finalPartida)
{
	int posTextX = POS_X_TAULER;
	int posTextY = POS_Y_TAULER + (ALCADA_CASELLA * NUM_FILES_TAULER) + 120;

	string modeJoc = "";
	if (m_modeJoc == MODE_JOC_NORMAL)
	{
		modeJoc = "Normal";
	}
	else if (m_modeJoc == MODE_JOC_MAQUINA)
	{
		modeJoc = "Maquina";
	}
	else if (m_modeJoc == MODE_JOC_REPLAY)
	{
		modeJoc = "Replay";
	}
	string msgModeJoc = "Mode de joc: " + modeJoc;
	GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msgModeJoc); //Ensenyem per pantalla el text amb el mode de joc
	posTextY += 35;

	string jugadorActual = "";
	if (m_tornJugador == COLOR_BLANC)
	{
		jugadorActual = "Blanques";
	}
	else
	{
		jugadorActual = "Negres";
	}

	if (finalPartida)
	{
		string msgFi = "Fi de la partida";
		GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msgFi); //Ensenyem per pantalla el text amb el guanyador
		posTextY += 35;
		string msgFinal = "Guanyador: " + jugadorActual;
		GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msgFinal); //Ensenyem per pantalla el text amb el guanyador
	}
	else if (m_modeJoc == MODE_JOC_REPLAY && m_cuaMoviments.empty())
	{
		string msgFi = "No hi ha mes moviments per reproduir";
		GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msgFi); //Ensenyem per pantalla el text amb el guanyador
	}
	else
	{
		string msgTornJugador = "Jugador actual: " + jugadorActual;
		GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msgTornJugador); //Ensenyem per pantalla el text amb les coordenades del cursor
	}
}

/**
* accioJugador
* Realitza l'acció del jugador en funció de la posició del ratolí i l'estat del botó del ratolí.
* @param mousePosX: La posició X del ratolí.
* @param mousePosY: La posició Y del ratolí.
* @param mouseStatus: L'estat del botó del ratolí (apretat o no apretat).
* @return Retorna true si la partida ha finalitzat, false en cas contrari.
*/
bool Joc::accioJugador(int mousePosX, int mousePosY, bool mouseStatus)
{
	bool finalPartida = false;
	if (mouseStatus && ratoliDinsTauler(mousePosX, mousePosY)) //Si s'ha fet un click dins el tauler
	{
		int columnaActual = (mousePosX - (POS_X_TAULER + CASELLA_INICIAL_X)) / AMPLADA_CASELLA; //Extreiem columna clicada
		int filaActual = (mousePosY - (POS_Y_TAULER + CASELLA_INICIAL_Y)) / ALCADA_CASELLA; //Extreiem fila clicada

		if (m_tauler.getFitxaActual(filaActual, columnaActual) != nullptr)
		{
			Fitxa* fitxaExtreta = (m_tauler.getFitxaActual(filaActual, columnaActual)); //Extreiem una còpia de la fitxa clicada

			if (fitxaExtreta->getColor() == m_tornJugador) //Si la fitxa es del mateix torn i no es buida
			{
				m_fitxaActual = fitxaExtreta;
				Posicio posOrigen = m_fitxaActual->getPosicio();
				m_fitxaSeleccionada = true;
				m_posicionsPossibles.clear();
				m_tauler.getPosicionsPossibles(posOrigen, m_posicionsPossibles);
			}
		}
		else if (m_fitxaSeleccionada) //Si tenim alguna fitxa seleccionada, intentem fer el moviment
		{
			Posicio posOrigen = m_fitxaActual->getPosicio();
			Posicio posDesti(filaActual, columnaActual);
			intentaFerMoviment(posOrigen, posDesti, finalPartida);
		}
	}
	if (m_fitxaSeleccionada) //Si tenim alguna fitxa seleccionada, mostrem els seus moviments valids
	{
		dibuixaMovimentsValids();
	}
	return finalPartida;
}

/**
* accioMaquina
* Realitza l'acció de la màquina jugadora, seleccionant el moviment amb millor puntuació.
* @return Retorna true si la partida ha finalitzat, false en cas contrari.
*/
bool Joc::accioMaquina()
{
	Sleep(1000); //Esperem un segon per millor jugabilitat
	bool finalPartida = false;
	Moviment movAFer;
	ColorFitxa colorMaquina = COLOR_BLANC;
	if (m_colorJugador == COLOR_BLANC)
	{
		colorMaquina = COLOR_NEGRE;
	}
	Fitxa* fitxaAMoure = m_tauler.getMovAmbMillorPuntuacio(colorMaquina, movAFer);
	if (fitxaAMoure != nullptr)
	{
		Posicio posOrigen = fitxaAMoure->getPosicio();
		Posicio posDesti = movAFer.getPosicio(movAFer.getNumPosicions() - 1);
		intentaFerMoviment(posOrigen, posDesti, finalPartida);
	}
	else
	{
		finalPartida = true;
	}
	return finalPartida;
}