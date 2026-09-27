#ifndef DIBUJO_H
#define DIBUJO_H

#include "raylib.h"
#include "Juego.h"


Color colorPieza(int tipo);// esta funcion devuelve el color de la pieza segun su tipo (0 a 6)


Color colorCelda(int valor);// este metodo da el color de la celda segun su valor en donde 0 es para la celda vacia y 1 al 7  para las piezas

void dibujarBloque(int x, int y, int tam, Color color);//crea un bloque con un pequeño relieve
void dibujarCaja(int x, int y, int ancho, int alto, Color borde);// un cuadro oscuro con las esquinas redondeadas , lo uso para todos los paneles y menus
int dibujarTecla(const char* texto, int x, int y);// dibuja una tecla y devuelve el ancho que tuvo para saber en donde dibujar el texto

void dibujarCelda(int fila, int columna, Color color, int desplazamientoY);// dibuja una celda del tablero en la fila y columna que se necesita junto con su color , el desplazamientoY son pixeles extra hacia abajo para la animacion de caida

void dibujarTablero(const Tablero& tablero, bool resaltarCompletas);// dibuja todo el tablero y si resaltarCompletas es true , las filas completas se dibujan en blanco
void dibujarPieza(const Pieza& pieza, int desplazamientoY);
void dibujarFantasma(const Tablero& tablero, const Pieza& pieza);// fantasma de la pieza


void dibujarMiniPieza(int tipo, int centroX, int centroY);// esto dibuja una mini pieza centrada en ese punto para el hold y las siguientes piezas

void dibujarHold(int tipo);
void dibujarSiguientes(int tipo1, int tipo2, int tipo3);
void dibujarDatos(int puntaje, int lineas, int nivel);
void dibujarEventos(int multiplicador, Evento proximo, float tiempoJuego, const char* mensaje);// muestra si hay puntos dobles , cual es el proximo evento y el aviso cuando se dispara uno
void dibujarControles();

void dibujarJuego(const Juego& juego); // aqui se dibuja todo en general osea el tablero , la pieza bajando , el hold y demás
void dibujarCeldasGuardadas(const int celdas[FILAS][COLUMNAS]);// esto es para la repetición , dibuja el tablero tal como estaba en un nodo del historial
void dibujarEstado(const Estado& e);// dibuja todo el estado de un nodo del historial , osea el tablero , la pieza , el hold y las siguientes piezas

#endif
