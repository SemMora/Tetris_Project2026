#include "Ordenamiento.h"

void BubbleSort(NodoPuntaje* cabeza) {
	
	if (cabeza == nullptr) {
		return;
	}
	NodoPuntaje* limite = nullptr;
	bool huboCambio = true;
	
	while (huboCambio) {
		huboCambio = false;
		NodoPuntaje* actual = cabeza;
		while (actual->siguiente != limite) {
			NodoPuntaje* vecino = actual->siguiente;
			if (actual->puntaje < vecino->puntaje) { // no elimino nodos solo intercambio sus datos
				int tempPuntaje = actual->puntaje;
				actual->puntaje = vecino->puntaje;
				vecino->puntaje = tempPuntaje;
				
				std::string tempNombre = actual->nombre;
				actual->nombre = vecino->nombre;
				vecino->nombre = tempNombre;
				
				huboCambio = true;
			}
			actual = actual->siguiente;
		}
		limite = actual; // en cada ciclo el más pequeño queda al final 
	}
}

static NodoPuntaje* merge(NodoPuntaje* izquierda, NodoPuntaje* derecha) {
	NodoPuntaje inicio;
	inicio.siguiente = nullptr;
	NodoPuntaje* ultimo = &inicio;
	
	while (izquierda != nullptr && derecha != nullptr) { 
		if (izquierda->puntaje >= derecha->puntaje) {
			ultimo->siguiente = izquierda;
			izquierda = izquierda->siguiente;
		} else {
			ultimo->siguiente = derecha;
			derecha = derecha->siguiente;
		}
		ultimo = ultimo->siguiente;
	}
	if (izquierda != nullptr) { 
		ultimo->siguiente = izquierda;
	} else {
		ultimo->siguiente = derecha;
	}
	return inicio.siguiente;
}

NodoPuntaje* mergeSort(NodoPuntaje* cabeza) {
	
	if (cabeza == nullptr || cabeza->siguiente == nullptr) {
		return cabeza;
	}
	
	int longitud = 0;
	NodoPuntaje* actual = cabeza;
	while (actual != nullptr) {
		longitud++;
		actual = actual->siguiente;
	}
	
    NodoPuntaje* mitad = cabeza;
	int i = 1;
	while (i < longitud / 2) { 
		mitad = mitad->siguiente;
		i++;
	}
	NodoPuntaje* segundaMitad = mitad->siguiente; 
	mitad->siguiente = nullptr; 
	
	NodoPuntaje* izquierda = mergeSort(cabeza);
	NodoPuntaje* derecha = mergeSort(segundaMitad);
	return merge(izquierda, derecha);
}
