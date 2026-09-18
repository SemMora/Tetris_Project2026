#include "Bolsa.h"
#include "Pieza.h"
#include <cstdlib>

void agregarBolsa(ColaPiezas& cola) {
	int bolsa[CANT_TIPOS];
	for (int i = 0; i < CANT_TIPOS; i++) {
		bolsa[i] = i;
	}
	
	// Algoritmo de Fisher-Yates: recorre desde la ultima posición del vector y va intercambiando de forma aleatoria
	for (int i = CANT_TIPOS - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = bolsa[i];
		bolsa[i] = bolsa[j];// aquí cada posición i de la bolsa se intercambia por otro valor de forma aleatoria
		bolsa[j] = temp;
	}
	
	for (int i = 0; i < CANT_TIPOS; i++) {
		cola.encolar(bolsa[i]); // la bolsa es int y no Piezas , por que cada i de bolsa se puede usar como indice para crear una pieza gráfica más adelante
	}
}
