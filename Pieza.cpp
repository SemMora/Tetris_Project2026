#include "Pieza.h"

// FORMAS[tipo][rotacion][bloque] = {fila, columna} todo representado dentro de una caja de 4x4.
// Al estar todas las direcciones precalculadas, para poder rotar es solo cambiar el indice. 
static const int FORMAS[7][4][4][2] = {
	// I
	{ {{1,0},{1,1},{1,2},{1,3}}, {{0,2},{1,2},{2,2},{3,2}},
	{{2,0},{2,1},{2,2},{2,3}}, {{0,1},{1,1},{2,1},{3,1}} },
	// O
{ {{0,1},{0,2},{1,1},{1,2}}, {{0,1},{0,2},{1,1},{1,2}},
{{0,1},{0,2},{1,1},{1,2}}, {{0,1},{0,2},{1,1},{1,2}} },
		// T
	{ {{0,1},{1,0},{1,1},{1,2}}, {{0,1},{1,1},{1,2},{2,1}},
	{{1,0},{1,1},{1,2},{2,1}}, {{0,1},{1,0},{1,1},{2,1}} },
	// S
{ {{0,1},{0,2},{1,0},{1,1}}, {{0,1},{1,1},{1,2},{2,2}},
{{1,1},{1,2},{2,0},{2,1}}, {{0,0},{1,0},{1,1},{2,1}} },
		// Z
	{ {{0,0},{0,1},{1,1},{1,2}}, {{0,2},{1,1},{1,2},{2,1}},
	{{1,0},{1,1},{2,1},{2,2}}, {{0,1},{1,0},{1,1},{2,0}} },
	// J
{ {{0,0},{1,0},{1,1},{1,2}}, {{0,1},{0,2},{1,1},{2,1}},
{{1,0},{1,1},{1,2},{2,2}}, {{0,1},{1,1},{2,0},{2,1}} },
		// L
	{ {{0,2},{1,0},{1,1},{1,2}}, {{0,1},{1,1},{2,1},{2,2}},
	{{1,0},{1,1},{1,2},{2,0}}, {{0,0},{0,1},{1,1},{2,1}} }
};

Pieza crearPieza(int tipo) {
	Pieza p;
	p.tipo = tipo;
	p.rotacion = 0;
	p.fila = 0;
	p.columna = 3;
	p.bomba = false; 
	return p;
}

void posicionBloque(const Pieza& p, int b, int& filaBloque, int& colBloque) {
	filaBloque = p.fila + FORMAS[p.tipo][p.rotacion][b][0];
	colBloque = p.columna + FORMAS[p.tipo][p.rotacion][b][1];
}

char letraPieza(int tipo) {
	const char LETRAS[] = "IOTSZJL";
	if (tipo < 0 || tipo >= CANT_TIPOS) {
		return '?';
	}
	return LETRAS[tipo];
}
