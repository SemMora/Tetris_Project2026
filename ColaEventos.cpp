#include "ColaEventos.h"

ColaEventos::ColaEventos() {
	frente = nullptr;
	cantidad = 0;
}

ColaEventos::~ColaEventos() {
	vaciar();
}

void ColaEventos::insertar(int tipo, float momento) {
	NodoEvento* nuevo = new NodoEvento {tipo, momento, nullptr};	
	
	if (frente == nullptr || momento < frente->evento.momento) { 
		nuevo->siguiente = frente;
		frente = nuevo;
	} else {
		NodoEvento* actual = frente;
		while (actual->siguiente != nullptr && actual->siguiente->evento.momento <= momento) { 
			actual = actual->siguiente;
		}
		nuevo->siguiente = actual->siguiente;
		actual->siguiente = nuevo;
	}
	cantidad++;
}

bool ColaEventos::hayEventoListo(float tiempoActual) const {
	if (frente == nullptr) {
		return false;
	}
	if (frente->evento.momento <= tiempoActual) {
		return true;
	}
	return false;
}

Evento ColaEventos::sacar() {
	Evento evento;
	evento = verFrente();
	if (frente != nullptr) {
		NodoEvento* viejo = frente;
		frente = frente->siguiente;
		delete viejo;
		cantidad--;
	}
	return evento;
}

Evento ColaEventos::verFrente() const {
	Evento evento;
	evento.tipo = -1;
	evento.momento = 0;
	if (frente != nullptr) {
		evento = frente->evento;
	}
	return evento;
}

bool ColaEventos::estaVacia() const {
	return frente == nullptr;
}

int ColaEventos::tamanio() const {
	return cantidad;
}

void ColaEventos::vaciar() {
	while (!estaVacia()) {
		sacar();
	}
}

const char* nombreEvento(int tipo) {
	switch (tipo) {
	case EVENTO_VELOCIDAD: return "Mas velocidad";
	case EVENTO_DOBLE_INICIO: return "Puntos dobles";
	case EVENTO_DOBLE_FIN: return "Fin puntos dobles";
	case EVENTO_BOMBA: return "Pieza bomba";
	}
	return "?";
}
