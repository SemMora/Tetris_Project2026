#include "ColaPiezas.h"

ColaPiezas::ColaPiezas() {
	frente = nullptr;
	ultimo = nullptr;
	cantidad = 0;
}

ColaPiezas::~ColaPiezas() {
	vaciar();
}

void ColaPiezas::encolar(int tipo) {
	NodoCola* nuevo = new NodoCola{ tipo, nullptr };
	if (estaVacia()) {
		frente = nuevo;
	} else {
		ultimo->siguiente = nuevo;
	}
	ultimo = nuevo;
	cantidad++;
}

int ColaPiezas::desencolar() {
	if (estaVacia()) {
		return -1;
	}
	NodoCola* viejo = frente;
	int tipo = viejo->tipo;
	frente = frente->siguiente;
	if (frente == nullptr) {
		ultimo = nullptr;
	}
	delete viejo;
	cantidad--;
	return tipo;
}

int ColaPiezas::verPosicion(int pos) const {
	NodoCola* actual = frente;
	int i = 0;
	while (actual != nullptr && i < pos) {
		actual = actual->siguiente;
		i++;
	}
	if (actual == nullptr) {
		return -1;
	}
	return actual->tipo;
}

bool ColaPiezas::estaVacia() const {
	return frente == nullptr;
}

int ColaPiezas::tamanio() const {
	return cantidad;
}

void ColaPiezas::vaciar() {
	while (!estaVacia()) {
		desencolar();
	}
}
