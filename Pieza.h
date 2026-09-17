#ifndef PIEZA_H
#define PIEZA_H

// Tipos de pieza: I=0, O=1, T=2, S=3, Z=4, J=5, L=6
const int CANT_TIPOS = 7;

// Pieza que el jugador controla
struct Pieza {
	int tipo;      // 0 a 6
	int rotacion;  // 0 a 3
	int fila;      // fila de la esquina superior izquierda de su caja 4x4
	int columna;   // columna de esa misma esquina
};

// Crea una pieza del tipo dado en la posicion inicial (arriba, al centro)
Pieza crearPieza(int tipo);

// Guarda en filaBloque y colBloque donde queda el bloque b (0 a 3)
// de la pieza dentro del tablero
void posicionBloque(const Pieza& p, int b, int& filaBloque, int& colBloque);

// Letra de la pieza para mostrarla en pantalla
char letraPieza(int tipo);

#endif
