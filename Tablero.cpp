#include "Tablero.h"

Tablero::Tablero() {
	primero = nullptr;
	int i = 0;
	while (i < FILAS) {
		insertarFilaVaciaAlInicio();
		i++;
	}
}

Tablero::~Tablero() {
	while (primero != nullptr) {
		NodoFila* borrar = primero;
		primero = primero->siguiente;
		delete borrar;
	}
}

void Tablero::insertarFilaVaciaAlInicio() {
	NodoFila* nueva = new NodoFila;
	int i = 0;
	while (i < COLUMNAS) {
		nueva->celdas[i] = 0;
		i++;
	}
	nueva->siguiente = primero;
	primero = nueva;
}

NodoFila* Tablero::buscarFila(int fila) const {
	NodoFila* actual = primero;
	int i = 0;
	while (i < fila && actual != nullptr) {
		actual = actual->siguiente;
		i++;
	}
	return actual;
}

bool Tablero::nodoCompleto(NodoFila* nodo) const {
	int i = 0;
	while (i < COLUMNAS) {
		if (nodo->celdas[i] == 0) {
			return false;
		}
		i++;
	}
	return true;
}

int Tablero::obtenerCelda(int fila, int columna) const {
	if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
		return 0;
	}
	return buscarFila(fila)->celdas[columna];
}

void Tablero::ponerCelda(int fila, int columna, int valor) {
	if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
		return;
	}
	buscarFila(fila)->celdas[columna] = valor;
}

bool Tablero::estaLibre(int fila, int columna) const {
	if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
		return false;
	}
	return buscarFila(fila)->celdas[columna] == 0;
}

bool Tablero::filaCompleta(int fila) const {
	if (fila < 0 || fila >= FILAS) {
		return false;
	}
	return nodoCompleto(buscarFila(fila));
}

bool Tablero::hayFilasCompletas() const {
	NodoFila* actual = primero;
	while (actual != nullptr) {
		if (nodoCompleto(actual)) {
			return true;
		}
		actual = actual->siguiente;
	}
	return false;
}

int Tablero::limpiarLineas() {
	int eliminadas = 0;
	NodoFila* anterior = nullptr;
	NodoFila* actual = primero;
	
	// 1) Recorrer la lista y solo limpiar las filas completas 
	while (actual != nullptr) {
		if (nodoCompleto(actual)) {
			NodoFila* borrar = actual;
			if (anterior == nullptr) {
				primero = actual->siguiente;
			} else {
				anterior->siguiente = actual->siguiente;
			}
			actual = actual->siguiente;
			delete borrar;
			eliminadas++;
		} else {
			anterior = actual;
			actual = actual->siguiente;
		}
	}
	
	// 2) Luego se inserta arriba tantas filas vacias como se eliminaron
	int i = 0;
	while (i < eliminadas) {
		insertarFilaVaciaAlInicio();
		i++;
	}
	return eliminadas;
}

void Tablero::vaciar() {
	NodoFila* actual = primero;
	while (actual != nullptr) {
		int i = 0;
		while (i < COLUMNAS) {
			actual->celdas[i] = 0;
			i++;
		}
		actual = actual->siguiente;
	}
}


void Tablero::guardarEn(int destino[FILAS][COLUMNAS]) const {// lo que hace es copiar el tablero en el historial
	NodoFila* actual = primero;
	int fila = 0;
	while (actual != nullptr) {
		int i = 0;
		while (i < COLUMNAS) {
			destino[fila][i] = actual->celdas[i];
			i++;
		}
		actual = actual->siguiente;
		fila++;
	}
}

void Tablero::cargarDesde(const int origen[FILAS][COLUMNAS]) {// aquí copia desde el historial al tablero
	NodoFila* actual = primero;
	int fila = 0;
	while (actual != nullptr) {
		int i = 0;
		while (i < COLUMNAS) {
			actual->celdas[i] = origen[fila][i];
			i++;
		}
		actual = actual->siguiente;
		fila++;
	}
}


void Tablero::eliminarFila(int fila) {
	if (fila < 0 || fila >= FILAS) {
		return;
	}
	NodoFila* anterior = nullptr;
	NodoFila* actual = primero;
	int i = 0;
	while (i < fila) {
		anterior = actual;
		actual = actual->siguiente;
		i++;
	}
	if (anterior == nullptr) {
		primero = actual->siguiente;
	} else {
		anterior->siguiente = actual->siguiente;
	}
	delete actual;
	insertarFilaVaciaAlInicio();
}
