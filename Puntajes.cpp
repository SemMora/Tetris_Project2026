#include "Puntajes.h"
#include "Ordenamiento.h"
#include <fstream>

ListaPuntajes::ListaPuntajes() {
	cabeza = nullptr;
	cantidad = 0;
}

ListaPuntajes::~ListaPuntajes() {
	vaciar();
}

void ListaPuntajes::agregar(const std::string& nombre, int puntaje) {
	NodoPuntaje* nuevo = new NodoPuntaje{nombre, puntaje, nullptr};
	nuevo->siguiente = cabeza; 
	cabeza = nuevo;
	cantidad++;
}

void ListaPuntajes::ordenar(int algoritmo) {
	if (algoritmo == 1) {
		BubbleSort(cabeza);
	} else {
		cabeza = mergeSort(cabeza);
	}
}

void ListaPuntajes::recortar(int maximo) {// recorta solo si hay más nodos que el máximo permitido (que son 10)
	if (cabeza == nullptr || maximo <= 0) {
		return;
	}
	NodoPuntaje* actual = cabeza;
	int posicion = 1;
	while (actual != nullptr && posicion < maximo) {
		actual = actual->siguiente;
		posicion++;
	}
	if (actual == nullptr) { 
		return;
	}
	NodoPuntaje* borrar = actual->siguiente;
	actual->siguiente = nullptr; 
	while (borrar != nullptr) {
		NodoPuntaje* siguiente = borrar->siguiente;
		delete borrar;
		cantidad--;
		borrar = siguiente;
	}
}

bool ListaPuntajes::entraAlTop(int puntaje) const { 
	if (cantidad < 10) { 
		return true;
	}
	NodoPuntaje* actual = cabeza;
	while (actual->siguiente != nullptr) { 
		actual = actual->siguiente;
	}
	
	if (puntaje > actual->puntaje) {
		return true;
	}
	return false;
}

bool ListaPuntajes::cargar(const char* archivo) {
	std::ifstream entrada(archivo);
	if (!entrada.is_open()) { 
		return false;
	}
	vaciar();
	int puntaje;
	std::string nombre;
	while (entrada >> puntaje >> nombre) { 
		agregar(nombre, puntaje);
	}
	return true;
}

bool ListaPuntajes::guardar(const char* archivo) const {
	std::ofstream salida(archivo);
	if (!salida.is_open()) {
		return false;
	}
	NodoPuntaje* actual = cabeza;
	while (actual != nullptr) {
		salida << actual->puntaje << " " << actual->nombre << "\n";
		actual = actual->siguiente;
	}
	return true;
}

void ListaPuntajes::vaciar() {
	while (cabeza != nullptr) {
		NodoPuntaje* borrar = cabeza;
		cabeza = cabeza->siguiente;
		delete borrar;
	}
	cantidad = 0;
}

int ListaPuntajes::tamanio() const {
	return cantidad;
}

const char* nombreAlgoritmo(int algoritmo) {
	if (algoritmo == 1) {
		return "Bubble Sort O(n^2)";
	}
	return "Merge sort O(n log n)";
}
