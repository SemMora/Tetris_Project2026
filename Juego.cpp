#include "Juego.h"
#include "Bolsa.h"
 

Juego::Juego() {
	nuevaPartida();
}

void Juego::nuevaPartida() { // practicamente limpia todo para una nueva partida
	tablero.vaciar();
	siguientes.vaciar();
	hold.vaciar();
	
	puntaje = 0;
	lineas = 0;
	nivel = 1;
	holdUsado = false;
	terminado = false;
	intervaloCaida = 0.8f; // tiempo de intervalo en segundos entre cada caída de la pieza
	tiempoCaida = 0;
	
	agregarBolsa(siguientes);
	sacarSiguientePieza();
}

bool Juego::cabe(const Pieza& pieza) const {
	for (int i = 0; i < 4; i++) {
		int fila, columna;
		posicionBloque(pieza, i, fila, columna);
		if (!tablero.estaLibre(fila, columna)) {
			return false;
		}
	}
	return true;
}

void Juego::sacarSiguientePieza() {
	actual = crearPieza(siguientes.desencolar());
	
	if (siguientes.tamanio() < 3) {
		agregarBolsa(siguientes);
	}
	
	if (!cabe(actual)) { // Si la pieza no cabe dentro del tablero se termina el juego
		terminado = true;
	}
}

void Juego::actualizar(float deltaTime) {
	if (terminado) {
		return;
	}
	tiempoCaida += deltaTime;   
	if (tiempoCaida >= intervaloCaida) { // en resumen si ya pasó el tiempo necesario para el intervalo entonces se baja la pieza una fila más abajo
		bajar();
	}
}

void Juego::moverIzquierda() {
	if (terminado) {
		return;
	}
	Pieza prueba = actual;
	prueba.columna--;
	if (cabe(prueba)) {
		actual = prueba;
	}
}

void Juego::moverDerecha() {
	if (terminado) {
		return;
	}
	Pieza prueba = actual;
	prueba.columna++;
	if (cabe(prueba)) {
		actual = prueba;
	}
}

void Juego::rotar() {
	if (terminado) {
		return;
	}
	Pieza prueba = actual;
	prueba.rotacion = (prueba.rotacion + 1) % 4;
	
	if (cabe(prueba)) { // al rotarla si no cabe simplemente no la rota
		actual = prueba;
	}
}

void Juego::bajar() {
	if (terminado) {
		return;
	}
	tiempoCaida = 0;
	Pieza prueba = actual;
	prueba.fila++;
	if (cabe(prueba)) {
		actual = prueba;
	} else {                 // mi logica es , si la prueba puede bajar y seguir cabiendo entonces la original baja y si no es por que ya tocó el fondo y debe fijarse la pieza
		fijarPieza(); 
	}
}

void Juego::usarHold() {
	if (terminado || holdUsado) {
		return;
	}
	int tipoActual = actual.tipo;
	
	if (hold.estaVacia()) { // si el hold no tiene piezas , se guarda la actual y se saca otra pieza de la cola
		hold.apilar(tipoActual);
		sacarSiguientePieza();  
	} else {
		int guardada = hold.desapilar(); // si ya había una pieza se guarda la pieza actual y se manda la que estaba en la pila
		hold.apilar(tipoActual);
		actual = crearPieza(guardada);
		if (!cabe(actual)) { // y si de casualidad esa pieza no cabe en el tablero entonces se termina el juego
			terminado = true;
		}
	}
	holdUsado = true;
	tiempoCaida = 0; // se reinicia el intervalo de caída cuando se utiliza el hold
}

void Juego::fijarPieza() {
	for (int i = 0; i < 4; i++) {
		int fila, columna;
		posicionBloque(actual, i, fila, columna); // busco la posicion del bloque
		tablero.ponerCelda(fila, columna, actual.tipo + 1); // y pongo celdas en ese mismo lugar
	}
	terminarColocacion();
}

void Juego::terminarColocacion() {
	int limpias = tablero.limpiarLineas(); // se limpian lineas si hay alguna completa 
	if (limpias > 0) {
		sumarPuntos(limpias);
	}
	holdUsado = false;   // la siguiente pieza reinicia de nuevo el uso del hold
	sacarSiguientePieza();
}

void Juego::sumarPuntos(int lineasLimpias) {
	int puntos = 0;
	if (lineasLimpias == 1) {
		puntos = 100;
	} else if (lineasLimpias == 2) {
		puntos = 300;
	} else if (lineasLimpias == 3) {
		puntos = 500;
	} else if (lineasLimpias >= 4) {
		puntos = 800;
	}
	puntaje += puntos;
	lineas += lineasLimpias;
}
