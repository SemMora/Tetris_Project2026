#include "Dibujo.h"

// Constantes para el dibujo
const int TAM_MINI = 20;// esto es el tamaño que tendrán las piezas que se dibujan en el hold y en la bolsa de siguientes piezas
const int PANEL_DERECHO = MARGEN_X + COLUMNAS * TAM_CELDA + 20; // posición del panel derecho  el cual es donde se dibujan el hold , las siguientes piezas y los datos del juego
const Color COLOR_VACIO = { 35, 35, 35, 255 }; // esto es un gris para las celdas vacias del tablero

Color colorPieza(int tipo) {// Para cada tipo de pieza se le pone un color 
	switch (tipo) {
	case 0: return SKYBLUE;  
	case 1: return YELLOW;   
	case 2: return PURPLE;   
	case 3: return GREEN;    
	case 4: return RED;      
	case 5: return BLUE;     
	case 6: return ORANGE;   
	}
	return GRAY;
}

Color colorCelda(int valor) {
	if (valor == 0) {
		return COLOR_VACIO;
	}
	return colorPieza(valor - 1);
}

void dibujarCelda(int fila, int columna, Color color, int desplazamientoY) {
	int x = MARGEN_X + columna * TAM_CELDA;//ejemplo: si la columna es 0 entonces x = 200 + 0 * 30 = 200
	int y = MARGEN_Y + fila * TAM_CELDA + desplazamientoY;// ejemplo: si la fila es 0 entonces y = 40 + 0 * 30 = 40 , y el desplazamiento es lo que baja de más la pieza en la animacion
	DrawRectangle(x + 1, y + 1, TAM_CELDA - 2, TAM_CELDA - 2, color);// puse +1 para dejar un borde de un pixel y -2 para que no se desborde la celda
}

void dibujarTablero(const Tablero& tablero, bool resaltarCompletas) {
	int i = 0;
	while (i < FILAS) {
		bool blanca = resaltarCompletas && tablero.filaCompleta(i); //aquí valido si la fila está completa y si hay que resaltarla para el parpadeo
		int j = 0;
		while (j < COLUMNAS) {
			if (blanca) {
				dibujarCelda(i, j, WHITE, 0);
			} else {
				dibujarCelda(i, j, colorCelda(tablero.obtenerCelda(i, j)), 0);
			}
			j++;
		}
		i++;
	}
	DrawRectangleLines(MARGEN_X - 1, MARGEN_Y - 1, COLUMNAS * TAM_CELDA + 2, FILAS * TAM_CELDA + 2, GRAY);// esto dibuja un borde alrededor del tablero
}

void dibujarPieza(const Pieza& pieza, int desplazamientoY) { // dibuja una pieza en el tablero
	if (pieza.tipo < 0) {
		return;
	}
	int i = 0;
	while (i < 4) {// cada pieza tiene 4 bloques
		int fila, columna;
		posicionBloque(pieza, i, fila, columna);
		if (pieza.bomba) { //como la bomba es especial la pinto de blanco
			dibujarCelda(fila, columna, WHITE, desplazamientoY);
		} else {
			dibujarCelda(fila, columna, colorPieza(pieza.tipo), desplazamientoY);
		}
		i++; 
	}
}

void dibujarMiniPieza(int tipo, int x, int y) {// dibuja una mini pieza para el hold y las siguientes piezas de la bolsa 
	if (tipo < 0) {
		return;
	}
	Pieza pieza = crearPieza(tipo);
	pieza.fila = 0;
	pieza.columna = 0;
	int i = 0;
	while (i < 4) {
		int fila, columna;
		posicionBloque(pieza, i, fila, columna);
		DrawRectangle(x + columna * TAM_MINI + 1, y + fila * TAM_MINI + 1, TAM_MINI - 2, TAM_MINI - 2, colorPieza(tipo));
		i++;
	}
}

void dibujarHold(int tipo) {
	DrawText("HOLD", 20, 50, 20, WHITE);
	DrawRectangleLines(20, 75, 160, 90, GRAY);
	dibujarMiniPieza(tipo, 60, 95);
}

void dibujarSiguientes(int tipo1, int tipo2, int tipo3) {
	DrawText("SIGUIENTES", PANEL_DERECHO, 50, 20, WHITE);
	DrawRectangleLines(PANEL_DERECHO, 75, 160, 270, GRAY);
	dibujarMiniPieza(tipo1, PANEL_DERECHO + 40, 95);
	dibujarMiniPieza(tipo2, PANEL_DERECHO + 40, 185);
	dibujarMiniPieza(tipo3, PANEL_DERECHO + 40, 275);
}

void dibujarDatos(int puntaje, int lineas, int nivel) {
	DrawText("PUNTAJE", 20, 190, 20, WHITE);
	DrawText(TextFormat("%d", puntaje), 20, 215, 30, YELLOW);
	DrawText("LINEAS", 20, 260, 20, WHITE);
	DrawText(TextFormat("%d", lineas), 20, 285, 30, YELLOW);
	DrawText("NIVEL", 20, 330, 20, WHITE);
	DrawText(TextFormat("%d", nivel), 20, 355, 30, YELLOW);
}

void dibujarEventos(int multiplicador, Evento proximo, float tiempoJuego, const char* mensaje) { // aquí dibujo todos los eventos que se estén dando en el momento
	if (multiplicador > 1) { 
		DrawText("PUNTOS x2", 20, 400, 20, GOLD);
	}
	if (proximo.tipo != -1) { // aqui dibujo el proximo evento que se usará y cuanto tiempo falta para que se use
		int segundos = (int)(proximo.momento - tiempoJuego) + 1;
		DrawText("PROXIMO EVENTO", 20, 440, 16, LIGHTGRAY);
		DrawText(nombreEvento(proximo.tipo), 20, 460, 16, SKYBLUE);
		DrawText(TextFormat("en %d s", segundos), 20, 480, 16, LIGHTGRAY);
	}
	if (mensaje != nullptr) { // el aviso del evento sale centrado arriba del tablero
		int centroTablero = MARGEN_X + COLUMNAS * TAM_CELDA / 2;
		int ancho = MeasureText(mensaje, 20);
		DrawText(mensaje, centroTablero - ancho / 2, 12, 20, GOLD);
	}
}

void dibujarControles() {
	int y = 380;
	DrawText("CONTROLES DEL JUEGO", PANEL_DERECHO, y, 20, WHITE);
	DrawText("Flecha Izquierda / Derecha: para mover", PANEL_DERECHO, y + 30, 16, LIGHTGRAY);
	DrawText("Flecha Arriba: para rotar", PANEL_DERECHO, y + 50, 16, LIGHTGRAY);
	DrawText("Flecha Abajo:para bajar", PANEL_DERECHO, y + 70, 16, LIGHTGRAY);
	DrawText("Tecla C: para hold", PANEL_DERECHO, y + 90, 16, LIGHTGRAY);
	DrawText("Tecla Z: para deshacer", PANEL_DERECHO, y + 110, 16, LIGHTGRAY);
	DrawText("Tecla X: para rehacer", PANEL_DERECHO, y + 130, 16, LIGHTGRAY);
	DrawText("Tecla P: para pausar", PANEL_DERECHO, y + 150, 16, LIGHTGRAY);
}

void dibujarJuego(const Juego& juego) {
	dibujarTablero(juego.getTablero(), juego.parpadeoEncendido());
	if (!juego.estaTerminado() && !juego.estaAnimando()) {//
		dibujarPieza(juego.getPieza(), juego.getDesplazamientoCaida());
	}// si el juego ya está terminado o se están limpiando lineas entonces no hay que dibujar la pieza que está bajando
	dibujarHold(juego.getHold());
	dibujarSiguientes(juego.getSiguiente(0), juego.getSiguiente(1), juego.getSiguiente(2));
	dibujarDatos(juego.getPuntaje(), juego.getLineas(), juego.getNivel());
	dibujarEventos(juego.getMultiplicador(), juego.getProximoEvento(), juego.getTiempoJuego(), juego.getMensaje());
	dibujarControles();
	if (juego.estaNavegando()) { // mientras se deshace o rehace se muestra en que paso del historial vamos
		DrawText(TextFormat("HISTORIAL: paso %d de %d", juego.getPaso(), juego.getTotalPasos()), MARGEN_X, 648, 20, SKYBLUE);
		DrawText("Mueve la pieza para seguir jugando", MARGEN_X, 672, 16, LIGHTGRAY);
	}
}


void dibujarCeldasGuardadas(const int celdas[FILAS][COLUMNAS]) {// dibuja el tablero tal como estaba en un nodo del historial que se pasa por parametro
	int i = 0;
	while (i < FILAS) {
		int j = 0;
		while (j < COLUMNAS) {
			dibujarCelda(i, j, colorCelda(celdas[i][j]), 0);
			j++;
		}
		i++;
	}
	DrawRectangleLines(MARGEN_X - 1, MARGEN_Y - 1, COLUMNAS * TAM_CELDA + 2, FILAS * TAM_CELDA + 2, GRAY);
}// solo dibuja el tablero

void dibujarEstado(const Estado& estado) {// dibuja todo el estado de un nodo del historial , osea el tablero , la pieza , el hold y las siguientes piezas
	dibujarCeldasGuardadas(estado.celdas);
	dibujarPieza(estado.pieza, 0); // en el replay la pieza no lleva animacion de caida
	dibujarHold(estado.hold);
	dibujarSiguientes(estado.siguientes[0], estado.siguientes[1], estado.siguientes[2]);
	dibujarDatos(estado.puntaje, estado.lineas, estado.nivel);
}