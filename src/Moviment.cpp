#include "Moviment.h"

/**
* afegirPosicio
* Aquesta funció afegeix una posició a l'array de posicions del moviment si hi ha espai disponible.
* @param p: Posició que es vol afegir al moviment.
*/
void Moviment::afegirPosicio(const Posicio& pos)
{
    m_posicions.push_back(pos);
}

/**
* afegirPecaCapturada
* Aquesta funció afegeix una posició de captura a l'array de captures si hi ha espai disponible.
* @param p: Posició de la captura que es vol afegir al moviment.
*/
void Moviment::afegirPecaCapturada(const Posicio& pos)
{
    m_pecesCapturades.push_back(pos);
}

/**
* contePosicio
* Aquesta funció comprova si una posició ja està inclosa en el moviment.
* @param posicio: Posició a comprovar.
* @return true si la posició ja està inclosa, false en cas contrari.
*/
bool Moviment::contePosicio(const Posicio& posicio) const
{
    bool contePosicio = false;
    for (int i = 0; i < m_posicions.size(); i++)
    {
        if (m_posicions[i] == posicio)
        {
            contePosicio = true;
        }
    }
    return contePosicio;
}
