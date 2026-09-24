#include "Juego.h"
#include "Bolsa.h"

const float INTERVALO_MINIMO = 0.1f;  // la caída nunca va a ser más rápida que esto
const float DURACION_MENSAJE = 2.5f;

Juego::Juego() {
	nuevaPartida();
}

void Juego::nuevaPartida() { // practicamente limpia todo para una nueva partida
	tablero.vaciar();
	siguientes.vaciar();
	hold.vaciar();
	historial.vaciar();
	eventos.vaciar();
	
	puntaje = 0;
	lineas = 0;
	nivel = 1;
	multiplicador = 1;
	holdUsado = false;
	proximaEsBomba = false;
	terminado = false;
	navegando = false;
	intervaloCaida = 0.8f; // tiempo de intervalo en segundos entre cada caída de la pieza
	tiempoCaida = 0;
	tiempoJuego = 0;
	mensaje = nullptr;
	tiempoMensaje = 0;
	
	eventos.insertar(EVENTO_VELOCIDAD, 30);// los eventos van en desorden para que la cola efectivamente los ordene
	eventos.insertar(EVENTO_DOBLE_INICIO, 45);
	eventos.insertar(EVENTO_BOMBA, 20);
	
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
	
	if (proximaEsBomba) { 
		actual.bomba = true;
		proximaEsBomba = false;
	}
	
	if (!cabe(actual)) { // Si la pieza no cabe dentro del tablero se termina el juego
		terminado = true;
	}
}

void Juego::actualizar(float deltaTime) {
	if (tiempoMensaje > 0) {
		tiempoMensaje -= deltaTime;
	}
	
	if (terminado || navegando) {
		return;
	}
	
	tiempoJuego += deltaTime;
	while (eventos.hayEventoListo(tiempoJuego)) { 
		aplicarEvento(eventos.sacar());
	}
	
	tiempoCaida += deltaTime;   
	if (tiempoCaida >= intervaloCaida) { // en resumen si ya pasó el tiempo necesario para el intervalo entonces se baja la pieza una fila más abajo
		bajar();
	}
}

void Juego::aplicarEvento(const Evento& estado) {
	if (estado.tipo == EVENTO_VELOCIDAD) { 
		nivel++;
		intervaloCaida = intervaloCaida * 0.8f;
		if (intervaloCaida < INTERVALO_MINIMO) {
			intervaloCaida = INTERVALO_MINIMO;
		}
		eventos.insertar(EVENTO_VELOCIDAD, estado.momento + 30);
		mensaje = "VELOCIDAD AUMENTADA";
	} else if (estado.tipo == EVENTO_DOBLE_INICIO) {
		multiplicador = 2;
		eventos.insertar(EVENTO_DOBLE_FIN, estado.momento + 10);
		mensaje = "PUNTOS DOBLES POR 10 SEGUNDOS";
	} else if (estado.tipo == EVENTO_DOBLE_FIN) { 
		multiplicador = 1;
		eventos.insertar(EVENTO_DOBLE_INICIO, estado.momento + 45);
		mensaje = "SE ACABARON LOS PUNTOS DOBLES";
	} else if (estado.tipo == EVENTO_BOMBA) {
		proximaEsBomba = true;
		eventos.insertar(EVENTO_BOMBA, estado.momento + 40);
		mensaje = "LA SIGUIENTE PIEZA ES UNA BOMBA";
	}
	tiempoMensaje = DURACION_MENSAJE;
}

void Juego::moverIzquierda() {
	if (terminado) {
		return;
	}
	navegando = false; // si el jugador mueve la pieza es porque quiere seguir jugando desde aquí
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
	} else {                 // mi logica es , si la prueba puede bajar y seguir cabiendo entonces la original baja y si no, es por que ya tocó el fondo y debe fijarse la pieza
		fijarPieza(); 
	}
}

void Juego::usarHold() {
	if (terminado || holdUsado || actual.bomba) { // creo que la bomba sería mejor no guardarla en el hold
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
	if (actual.bomba) { // la bomba explota no tiene que fijarse en el tablero
		explotarBomba();
	} else {
		int i = 0;
		while (i < 4) {
			int fila, columna;
			posicionBloque(actual, i, fila, columna); // busco la posicion del bloque 
			tablero.ponerCelda(fila, columna, actual.tipo + 1);// y pongo las celdas en ese mismo lugar
			i++;
		}
	}
	terminarColocacion();
}

void Juego::explotarBomba() {
	int filaMasBaja = 0;
	int i = 0;
	while (i < 4) { // busco cual es la fila más baja que ocupa la bomba
		int fila, columna;
		posicionBloque(actual, i, fila, columna);
		if (fila > filaMasBaja) {
			filaMasBaja = fila;
		}
		i++;
	}
	int filaBorrar = filaMasBaja + 1;// la bomba borra la fila de abajo de donde cayó
	if (filaBorrar >= FILAS) {
		filaBorrar = FILAS - 1; 
	}
	tablero.eliminarFila(filaBorrar);
	sumarPuntos(1);
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
	puntaje += puntos * multiplicador; 
	lineas += lineasLimpias;
}

void Juego::registrar(int movimiento) {
	Estado estado;
	tablero.guardarEn(estado.celdas);
	estado.pieza = actual;
	if (terminado) {
		estado.pieza.tipo = -1;   // -1 por que no hay pieza si el juego terminó
	}
	estado.hold = hold.verTope();
	estado.holdUsado = holdUsado;
	
	estado.cantSiguientes = 0;
	while (estado.cantSiguientes < siguientes.tamanio() && estado.cantSiguientes < 14) { // copio las piezas de la cola en el mismo orden
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
	
	siguientes.vaciar(); // vacío la cola y la vuelvo a llenar con las piezas que tenía en ese momento
	int i = 0;
	while (i < estado.cantSiguientes) {
		siguientes.encolar(estado.siguientes[i]);
		i++;
	}
	
	puntaje = estado.puntaje;
	lineas = estado.lineas;
	nivel = estado.nivel;
	tiempoCaida = 0;
}

const char* Juego::getMensaje() const {
	if (tiempoMensaje > 0) { // el aviso solo se devuelve mientras todavía le quede tiempo
		return mensaje;
	}
	return nullptr;
}
