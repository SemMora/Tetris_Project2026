#ifndef MEDICION_H
#define MEDICION_H

#include "Puntajes.h"

const int REPETICIONES = 3;   // esto lo hago para saber cuanto tarda en promedio cada algoritmo en ordenar 

struct ResultadoMedicion {// son vectores de tamaño 4 por que  se van a medir en pruebas de 10 , 100 , 1000 y 10000 puntajes
	int tamanios[4];
	double bubble[4];
	double merge[4];
};

double ordenarMidiendo(ListaPuntajes& lista, int algoritmo);
ResultadoMedicion medirTiempos(const char* archivo);// mide cuanto tarda cada algoritmo en ordenar y guarda el resultado en el txt

#endif
