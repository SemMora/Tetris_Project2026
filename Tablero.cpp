#include "Tablero.h"

Tablero::Tablero() {
	primero = nullptr;
	for (int i = 0; i < FILAS; i++) {
		insertarFilaVaciaAlInicio();
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
	NodoFila* nuevo = new NodoFila;
	int i = 0;
	while (i < COLUMNAS) {
		nuevo->celdas[i] = 0;
		i++;
	}
	nuevo->siguiente = primero;
	primero = nuevo;
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
	
	// 1) Recorrer la lista y solo limpia si tienen una fila completa
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
	
	// 2) luego se insertan la misma cantidad de filas vacias nuevas , que de las que se eliminaron
	int i = 0;
	while (i < eliminadas) {
		insertarFilaVaciaAlInicio();
		i++;
	}
	return eliminadas; // y devuelvo el total de filas que se eliminaron
}

void Tablero::vaciar() {
	NodoFila* actual = primero;
	while (actual != nullptr) {
		int i = 0;
		while (i < COLUMNAS) {
			actual->celdas[i] = 0; // limpio todo el tablero recorriendo cada nodo y limpiando su vector que tienen las celdas osea columnas
			i++;
		}
		actual = actual->siguiente;
	}
}
