#ifndef PANTALLAS_H
#define PANTALLAS_H

#include "raylib.h"

void textoCentrado(const char* texto, int y, int tamanio, Color color);// el texto centrado es para que se vea mejor en las pantallas de inicio , pausa y fin
void oscurecerFondo();// oscurece el fondo de la ventana para que se vea mejor el texto de las pantallas de inicio , pausa y fin

void dibujarPantallaInicio();
void dibujarPantallaPausa();
void dibujarPantallaFin(int puntaje);

#endif
