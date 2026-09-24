#ifndef TABLERO_H
#define TABLERO_H

#include "Constantes.h"

//Nodo de la lista que representa una fila del tablero con 10 celdas usando un vector para las celdas.
// Para saber si una celda está vacía su valor debe ser 0, y en caso de estar ocupada debe ser (tipo de pieza + 1).
struct NodoFila {
	int celdas[COLUMNAS];
	NodoFila* siguiente;   // cada siguiente es realmente la fila de abajo del tablero
};


class Tablero {// El tablero es una lista simple de 20 filas y 10 columnas 
private:
	NodoFila* primero;
	void insertarFilaVaciaAlInicio();
	NodoFila* buscarFila(int fila) const;  
	bool nodoCompleto(NodoFila* nodo) const;
	
public:
	Tablero();
	~Tablero();
	
	int obtenerCelda(int fila, int columna) const;
	void ponerCelda(int fila, int columna, int valor);
	bool estaLibre(int fila, int columna) const;  // devuelve false si no existe por salirse del rango de los parametros o si está ocupada
	
	bool filaCompleta(int fila) const;
	bool hayFilasCompletas() const;
	int limpiarLineas();   // limpia y devuelve cuantas lineas tuvo que limpiar
	void vaciar();      
	void guardarEn(int destino[FILAS][COLUMNAS]) const;  
	void cargarDesde(const int origen[FILAS][COLUMNAS]);
	
	// esto es lo mismo que hay en PilaHold , metodos para eliminar de forma eficiente sin duplicados 
	Tablero(const Tablero&) = delete;
	Tablero& operator=(const Tablero&) = delete;
};

#endif
