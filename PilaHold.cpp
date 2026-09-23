#include "PilaHold.h"

PilaHold::PilaHold() {
	tope = nullptr;
	cantidad = 0;
}

PilaHold::~PilaHold() {
	vaciar();
}

bool PilaHold::apilar(int tipo) {
	if (estaLlena()) {
		return false;
	}
	NodoPila* nuevo = new NodoPila{ tipo, tope };
	tope = nuevo;
	cantidad++;
	return true;
}

int PilaHold::desapilar() {
	if (estaVacia()) {
		return -1;
	}
	NodoPila* viejo = tope;
	int tipo = viejo->tipo;
	tope = tope->siguiente;
	delete viejo;
	cantidad--;
	return tipo;
}

int PilaHold::verTope() const {
	if (estaVacia()) {
		return -1;
	}
	return tope->tipo;
}

bool PilaHold::estaVacia() const {
	return tope == nullptr;
}

bool PilaHold::estaLlena() const {
	return cantidad == CAPACIDAD;
}

void PilaHold::vaciar() {
	while (!estaVacia()) {
		desapilar();
	}
}
