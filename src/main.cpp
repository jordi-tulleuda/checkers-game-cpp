#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined  (_WIN64)

#include <iostream>
//Definicio necesaria per poder incloure la llibreria i que trobi el main
#define SDL_MAIN_HANDLED
#include <windows.h>
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#include <conio.h>      /* getch */ 

#elif __APPLE__
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop

#endif

#include "./joc.hpp"
#include "./info_joc.hpp"

/**
* menuModeJoc
* Mostra el menú principal del joc i permet a l'usuari triar el mode de joc.
* @return Retorna el mode de joc triat per l'usuari.
*/
ModeJoc menuModeJoc()
{
    cout << "Iniciant Joc..." << endl;
    cout << "---------------------------" << endl;
    cout << "Tria una accio: " << endl;
    cout << "---------------------------" << endl;
    cout << "1 - PARTIDA JUGADOR VS JUGADOR (MODE NORMAL)" << endl;
    cout << "2 - PARTIDA JUGADOR VS MAQUINA (MODE MAQUINA)" << endl;
    cout << "3 - MIRAR REPETICIO (MODE REPLAY)" << endl;
    cout << "4 - SORTIR" << endl;
    cout << "---------------------------" << endl;
    cout << "Introdueix un numero: ";
    //Bucle triar mode de joc
    int numeroMode, colorJugador;
    do
    {
        cin >> numeroMode;
        if (numeroMode < 1 || numeroMode > 4)
        {
            cout << endl << "Accio invalida, introdueix un altre valor: ";
        }
    } while (numeroMode < 1 || numeroMode > 4);
    ModeJoc mode;
    switch (numeroMode)
    {
    case 1:
        mode = MODE_JOC_NORMAL;
        break;
    case 2:
        mode = MODE_JOC_MAQUINA;
        break;
    case 3:
        mode = MODE_JOC_REPLAY;
        break;
    default:
        mode = MODE_JOC_NONE;
        break;
    }
    return mode;
}

/**
* menuColorJugador
* Mostra el menú secundari del mode maquina del joc i permet a l'usuari triar el color amb el que vol jugar la partida.
* @return Retorna el color triat per l'usuari.
*/
ColorFitxa menuColorJugador()
{
    //Triar color en mode vs maquina
    int colorJugador;
    cout << endl << "---------------------------" << endl;
    cout << "Amb quin color vols jugar?" << endl;
    cout << "---------------------------" << endl;
    cout << "1 - BLANQUES" << endl;
    cout << "2 - NEGRES" << endl;
    cout << "---------------------------" << endl;
    cout << "Introdueix un numero: ";
    do
    {
        cin >> colorJugador;
        if (colorJugador < 1 || colorJugador > 2)
        {
            cout << endl << "Accio invalida, introdueix un altre valor: ";
        }
    } while (colorJugador < 1 || colorJugador > 2);
    ColorFitxa color = COLOR_BLANC;
    if (colorJugador == 2)
    {
        color = COLOR_NEGRE;
    }
    return color;
}

/**
* main
* Funció principal del programa que inicialitza el joc i gestiona el bucle principal de l'aplicació.
* @param argc: Nombre d'arguments de la línia de comandes.
* @param argv: Array d'arguments de la línia de comandes.
* @return Retorna 0 si l'execució ha estat correcta.
*/
int main(int argc, const char* argv[])
{
    ModeJoc mode = menuModeJoc();
    ColorFitxa color = COLOR_BLANC;
    if (mode == MODE_JOC_MAQUINA)
    {
        color = menuColorJugador();
    }
    if (mode != MODE_JOC_NONE)
    {
        //Instruccions necesaries per poder incloure la llibreria i que trobi el main
        SDL_SetMainReady();
        SDL_Init(SDL_INIT_VIDEO);

        Screen pantalla(TAMANY_PANTALLA_X, TAMANY_PANTALLA_Y); //Inicialitza un objecte de la classe Screen que s'utilitza per gestionar la finestra grafica
        pantalla.show(); //Mostrem la finestra grafica

        //Establim paths dels fitxers
        const string& nomFitxerTauler = "data/Games/tauler_inicial.txt", nomFitxerMoviments = "data/Games/moviments.txt";

        //Creem un joc nou i l'inicialitzem
        Joc joc;
        joc.inicialitza(mode, color, nomFitxerTauler, nomFitxerMoviments);
        pantalla.update();

        bool finalJoc = false;
        do
        {
            pantalla.processEvents(); // Captura tots els events de ratolí i teclat de l'ultim cicle
            if (!finalJoc)
            {
                bool mouseStatus = Mouse_getBtnLeft();
                int mousePosX = Mouse_getX();
                int mousePosY = Mouse_getY();

                finalJoc = joc.actualitza(mousePosX, mousePosY, mouseStatus);
                if (finalJoc)
                {
                    joc.finalitza(); //Guardem els moviments que hem fet a una partida normal
                }
                pantalla.update(); // Actualitza la pantalla
            }
        } while (!Keyboard_GetKeyTrg(KEYBOARD_ESCAPE)); // Sortim del bucle si pressionem ESC
        if (!finalJoc)
        {
            joc.finalitza(); //Guardem els moviments que hem fet a una partida normal
        }
        SDL_Quit(); //Instruccio necesaria per alliberar els recursos de la llibreria 
    }
    return 0;
}