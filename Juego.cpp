#include "Juego.h"
#include "Bolsa.h"


Juego::Juego() {
	nuevaPartida();
}

void Juego::nuevaPartida() { // practicamente limpia todo para una nueva partida
	tablero.vaciar();
	siguientes.vaciar();
	hold.vaciar();
	historial.vaciar();
	
	puntaje = 0;
	lineas = 0;
	nivel = 1;
	holdUsado = false;
	terminado = false;
	navegando = false;
	intervaloCaida = 0.8f; // tiempo de intervalo en segundos entre cada caída de la pieza
	tiempoCaida = 0;
	
	agregarBolsa(siguientes);
	sacarSiguientePieza();
	registrar(MOV_INICIO); 
}

bool Juego::cabe(const Pieza& pieza) const {
	int i = 0;
	while (i < 4) {
		int fila, columna;
		posicionBloque(pieza, i, fila, columna);
		if (!tablero.estaLibre(fila, columna)) {
			return false;
		}
		i++;
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
	if (terminado || navegando) { 
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
	navegando = false; 
	Pieza prueba = actual;
	prueba.columna--;
	if (cabe(prueba)) {
		actual = prueba;
		registrar(MOV_MOVER);
	}
}

void Juego::moverDerecha() {
	if (terminado) {
		return;
	}
	navegando = false;
	Pieza prueba = actual;
	prueba.columna++;
	if (cabe(prueba)) {
		actual = prueba;
		registrar(MOV_MOVER);
	}
}

void Juego::rotar() {
	if (terminado) {
		return;
	}
	navegando = false;
	Pieza prueba = actual;
	prueba.rotacion = (prueba.rotacion + 1) % 4;
	
	if (cabe(prueba)) { // al rotarla si no cabe simplemente no la rota
		actual = prueba;
		registrar(MOV_ROTAR);
	}
}

void Juego::bajar() {
	if (terminado) {
		return;
	}
	navegando = false;
	tiempoCaida = 0;
	Pieza prueba = actual;
	prueba.fila++;
	if (cabe(prueba)) {
		actual = prueba;
		registrar(MOV_BAJAR);
	} else {                 // mi logica es , si la prueba puede bajar y seguir cabiendo entonces la original baja y si no es por que ya tocó el fondo y debe fijarse la pieza
		fijarPieza(); 
	}
}

void Juego::usarHold() {
	if (terminado || holdUsado) {
		return;
	}
	navegando = false;
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
	registrar(MOV_HOLD);
}

void Juego::deshacer() {
	if (terminado) {
		return;
	}
	if (historial.retroceder()) { 
		restaurar(historial.estadoActual());
		navegando = true;
	}
}

void Juego::rehacer() {
	if (terminado) {
		return;
	}
	if (historial.avanzar()) {
		restaurar(historial.estadoActual());
		navegando = true;
	}
}

void Juego::fijarPieza() {
	int i = 0;
	while (i < 4) {
		int fila, columna;
		posicionBloque(actual, i, fila, columna); // busco la posicion del bloque 
		tablero.ponerCelda(fila, columna, actual.tipo + 1);// y pongo las celdas en ese mismo lugar
		i++;
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
	registrar(MOV_COLOCAR);
}

void Juego::sumarPuntos(int lineasLimpias) {// a mayor cantidad de lineas mayor serán los puntos excepto que si son más de 4 lineas ahí lo dejé fijo en 800
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

void Juego::registrar(int movimiento) {// guarda todo lo que este sucediendo en el juego y añade un nodo al historial
	Estado estado;
	tablero.guardarEn(estado.celdas);
	estado.pieza = actual;
	if (terminado) {
		estado.pieza.tipo = -1;  
	}
	estado.hold = hold.verTope();
	estado.holdUsado = holdUsado;
	
	estado.cantSiguientes = 0;
	while (estado.cantSiguientes < siguientes.tamanio() && estado.cantSiguientes < 14) { 
		estado.siguientes[estado.cantSiguientes] = siguientes.verPosicion(estado.cantSiguientes);
		estado.cantSiguientes++;
	}
	
	estado.puntaje = puntaje;
	estado.lineas = lineas;
	estado.nivel = nivel;
	estado.movimiento = movimiento;
	historial.agregar(estado);
}

void Juego::restaurar(const Estado& estado) {
	tablero.cargarDesde(estado.celdas);
	actual = estado.pieza;
	
	hold.vaciar();
	if (estado.hold != -1) {
		hold.apilar(estado.hold);
	}
	holdUsado = estado.holdUsado;
	
	siguientes.vaciar(); 
	int i = 0;
	while (i < estado.cantSiguientes) {
		siguientes.encolar(estado.siguientes[i]);
		i++;
	}
	
	puntaje = estado.puntaje;
	lineas = estado.lineas;
	tiempoCaida = 0;
}
