#include "posicio.hpp"

/**
* Posicio
* Constructor per defecte que inicialitza una posició a partir d'un string.
* @param posicioStr: String de dos caràcters, on el primer és la columna (a-h) i el segon la fila (1-8).
*/
Posicio::Posicio(const string& posicioStr)
{
    fromString(posicioStr);
}

/**
* Posicio
* Constructor que inicialitza una posició a partir de les coordenades fila i columna.
* @param fila: Índex de la fila.
* @param columna: Índex de la columna.
*/
Posicio::Posicio(int fila, int columna)
{
    m_fila = fila;
    m_columna = columna;
}

/**
* toString
* Converteix la posició actual a un string.
* @return String.
*/
string Posicio::toString() const
{
    string posicio = "a1";
    posicio = 'a' + m_columna; //Calcula la lletra de la columna
    posicio += char('1' + (N_FILES - 1) - m_fila); //Calcula el número de la fila
    return posicio;
}

/**
* fromString
* Actualitza la posició a partir d'un string.
* @param pos: String vàlid (ex: "h8").
*/
void Posicio::fromString(const string& pos)
{
    m_fila = (N_FILES - 1) - (pos[1] - '1'); //Converteix el número de fila a índex
    m_columna = pos[0] - 'a'; //Converteix la lletra de columna a índex
}

/**
* setCoordenades
* Actualitza les coordenades d'una posicio.
* @param fila: fila a actualitzar
* @param columna: columna a actualitzar
*/
void Posicio::setCoordenades(int fila, int columna)
{
    m_fila = fila;
    m_columna = columna;
}

/**
* operator==
* Compara si dues posicions són iguals.
* @param other: L'altra posició a comparar.
* @return true si coincideixen fila i columna.
*/
bool Posicio::operator==(const Posicio& other) const
{
    bool iguals = false;
    if (m_fila == other.getFila() && m_columna == other.getColumna())
    {
        iguals = true;
    }
    return iguals;
}

/**
* operator!=
* Determina si dues posicions són diferents.
* @param other: L'altra posició a comparar.
* @return true si les files i les columnes no coincideixen.
*/
bool Posicio::operator!=(const Posicio& other) const
{
    bool diferents = false;
    if (m_fila != other.getFila() || m_columna != other.getColumna())
    {
        diferents = true;
    }
    return diferents;
}

/**
* operator<<
* Sobrecàrrega de l'operador de sortida per imprimir una posició.
* @param out: L'ostream on es vol imprimir.
* @return L'ostream amb la posició convertida a string.
*/
ostream& operator<<(ostream& out, const Posicio& pos)
{
    out << pos.toString();
    return out;
}