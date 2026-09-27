#ifndef PANTALLAS_H
#define PANTALLAS_H

#include "raylib.h"

const Rectangle BtnRetroceder = { 150, 652, 120, 36 }; // botones que pueden ser usados con el mouse
const Rectangle BtnReproducir = { 290, 652, 120, 36 };
const Rectangle BtnAvanzar = { 430, 652, 120, 36 };

void textoCentrado(const char* texto, int y, int tamanio, Color color);// escribe un texto centrado a lo ancho de la ventana
void oscurecerFondo();// permite oscurecer el fondo de la ventana para que se vea que hay un menu encima

void dibujarPantallaInicio();
void dibujarPantallaPausa();
void dibujarPantallaFin(int puntaje);

void dibujarBoton(Rectangle btn, const char* texto);
bool botonPresionado(Rectangle btn);
void dibujarControlesRepeticion(int paso, int total, const char* movimiento, bool reproduciendo); // recordatorio: usa varias veces dibujarBoton
#endif
