#include "Medicion.h"
#include "raylib.h"
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

double ordenarMidiendo(ListaPuntajes& lista, int algoritmo) {
	double inicio = GetTime();
	lista.ordenar(algoritmo);
	double fin = GetTime();
	return (fin - inicio) * 1000000.0; //hago la conversion a microsegundos por que GetTime devuelve segundos
}

static void llenarAlAzar(ListaPuntajes& lista, int n, unsigned int semilla) {// unsigned int es para que la semilla no sea negativa
	srand(semilla); // utilizo una semilla para que ambos algoritmos ordenen la misma lista de puntajes aleatorios
	lista.vaciar();
	int i = 0;
	while (i < n) {
		lista.agregar("Prueba", rand() % 100000);
		i++;
	}
}

static double promedio(int algoritmo, int n) {// lo que hace es calcular el promedio luego de repetir un total de 3 veces el ordenamiento de puntajes al azar
	double total = 0;
	int i = 0;
	while (i < REPETICIONES) {
		ListaPuntajes lista;
		llenarAlAzar(lista, n, 1000 + i);
		total += ordenarMidiendo(lista, algoritmo);
		i++;
	}
	return total / REPETICIONES;
}

ResultadoMedicion medirTiempos(const char* archivo) {
	ResultadoMedicion resultado;
	int tamanios[4] = { 10, 100, 1000, 10000 };
	
	int i = 0;
	while (i < 4) {
		resultado.tamanios[i] = tamanios[i];
		resultado.bubble[i] = promedio(1, tamanios[i]);
		resultado.merge[i] = promedio(2, tamanios[i]);
		i++;
	}
	srand((unsigned)time(nullptr)); // de esta forma siempre que se mida el tiempo se usaran puntajes diferentes
	
	std::ofstream salida(archivo); 
	salida << "Tiempo de ordenar la tabla de puntajes (microsegundos, promedio de " << REPETICIONES << " corridas)\n";
	salida << "n\tBubble Sort O(n^2)\tMerge sort O(n log n)\n";
	salida << std::fixed << std::setprecision(2);
	i = 0;
	while (i < 4) {
		salida << resultado.tamanios[i] << "\t" << resultado.bubble[i] << "\t" << resultado.merge[i] << "\n";
		i++;
	}
	return resultado;
}
