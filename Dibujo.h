#ifndef DIBUJO_H
#define DIBUJO_H

#include "raylib.h"
#include "Juego.h"


Color colorPieza(int tipo);// esta funcion devuelve el color de la pieza segun su tipo (0 a 6)


Color colorCelda(int valor);// este metodo da el color de la celda segun su valor en donde 0 es para la celda vacia y 1 al 7  para las piezas

void dibujarCelda(int fila, int columna, Color color);// dibuja una celda del tablero en la fila y columna que se necesita junto con su color 

void dibujarTablero(const Tablero& tablero);
void dibujarPieza(const Pieza& pieza);


void dibujarMiniPieza(int tipo, int x, int y);

void dibujarHold(int tipo);
void dibujarSiguientes(int tipo1, int tipo2, int tipo3);
void dibujarDatos(int puntaje, int lineas, int nivel);
void dibujarEventos(int multiplicador, Evento proximo, float tiempoJuego, const char* mensaje);
void dibujarControles();

void dibujarJuego(const Juego& juego); // aqui se dibuja todo en general osea el tablero , la pieza bajando , el hold y demás

#endif
