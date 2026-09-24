#include "Historial.h"

Historial::Historial() {
	primero = nullptr;
	ultimo = nullptr;
	actual = nullptr;
	cantidad = 0;
	posicion = 0;
}

Historial::~Historial() {
	vaciar();
}

void Historial::borrarDespuesDeActual() {
	if (actual == nullptr) {
		return;
	}
	NodoHistorial* borrar = actual->siguiente;
	while (borrar != nullptr) {
		NodoHistorial* siguiente = borrar->siguiente;
		delete borrar;
		cantidad--;
		borrar = siguiente;
	}
	actual->siguiente = nullptr;
	ultimo = actual; 
}

void Historial::agregar(const Estado& e) {
	borrarDespuesDeActual();
	
	NodoHistorial* nuevo = new NodoHistorial;
	nuevo->estado = e;
	nuevo->siguiente = nullptr;
	nuevo->anterior = ultimo; 
	
	if (ultimo == nullptr) { 
		primero = nuevo;
	} else {
		ultimo->siguiente = nuevo;
	}
	ultimo = nuevo;
	actual = nuevo;
	cantidad++;
	posicion = cantidad;
}

bool Historial::retroceder() {
	if (actual == nullptr || actual->anterior == nullptr) {
		return false;
	}
	actual = actual->anterior; 
	posicion--;
	return true;
}

bool Historial::avanzar() {
	if (actual == nullptr || actual->siguiente == nullptr) {
		return false;
	}
	actual = actual->siguiente;
	posicion++;
	return true;
}

void Historial::irAlPrimero() {
	actual = primero;
	if (primero != nullptr) {
		posicion = 1;
	}
}

const Estado& Historial::estadoActual() const {
	return actual->estado;
}

bool Historial::estaVacio() const {
	return primero == nullptr;
}

int Historial::tamanio() const {
	return cantidad;
}

int Historial::getPosicion() const {
	return posicion;
}

void Historial::vaciar() {
	while (primero != nullptr) {
		NodoHistorial* borrar = primero;
		primero = primero->siguiente;
		delete borrar;
	}
	ultimo = nullptr;
	actual = nullptr;
	cantidad = 0;
	posicion = 0;
}

const char* nombreMovimiento(int movimiento) {
	switch (movimiento) {
	case MOV_INICIO: return "Inicio";
	case MOV_MOVER: return "Mover";
	case MOV_ROTAR: return "Rotar";
	case MOV_BAJAR: return "Bajar";
	case MOV_COLOCAR: return "Colocar";
	case MOV_HOLD: return "Hold";
	}
	return "?";
}
