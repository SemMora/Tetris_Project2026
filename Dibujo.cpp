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

void dibujarCelda(int fila, int columna, Color color) {
	int x = MARGEN_X + columna * TAM_CELDA;//ejemplo: si la columna es 0 entonces x = 200 + 0 * 30 = 200
	int y = MARGEN_Y + fila * TAM_CELDA;// ejemplo: si la fila es 0 entonces y = 40 + 0 * 30 = 40
	DrawRectangle(x + 1, y + 1, TAM_CELDA - 2, TAM_CELDA - 2, color);// puse +1 para dejar un borde de un pixel y -2 para que no se desborde la celda
}

void dibujarTablero(const Tablero& tablero) {
	int i = 0;
	while (i < FILAS) {
		int j = 0;
		while (j < COLUMNAS) {
			dibujarCelda(i, j, colorCelda(tablero.obtenerCelda(i, j)));
			j++;
		}
		i++;
	}
	DrawRectangleLines(MARGEN_X - 1, MARGEN_Y - 1, COLUMNAS * TAM_CELDA + 2, FILAS * TAM_CELDA + 2, GRAY);// esto dibuja un borde alrededor del tablero
}

void dibujarPieza(const Pieza& pieza) { // dibuja una pieza en el tablero
	if (pieza.tipo < 0) {
		return;
	}
	int i = 0;
	while (i < 4) {// cada pieza tiene 4 bloques
		int fila, columna;
		posicionBloque(pieza, i, fila, columna);
		dibujarCelda(fila, columna, colorPieza(pieza.tipo));
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

void dibujarControles() {
	int y = 380;
	DrawText("CONTROLES DEL JUEGO", PANEL_DERECHO, y, 20, WHITE);
	DrawText("Flecha Izquierda / Derecha: para mover", PANEL_DERECHO, y + 30, 16, LIGHTGRAY);
	DrawText("Flecha Arriba: para rotar", PANEL_DERECHO, y + 50, 16, LIGHTGRAY);
	DrawText("Flecha Abajo:para bajar", PANEL_DERECHO, y + 70, 16, LIGHTGRAY);
	DrawText("Tecla C: para hold", PANEL_DERECHO, y + 90, 16, LIGHTGRAY);
	DrawText("Tecla Z: para deshacer", PANEL_DERECHO, y + 110, 16, LIGHTGRAY);
	DrawText("Tecla X: para rehacer", PANEL_DERECHO, y + 130, 16, LIGHTGRAY);
}

void dibujarJuego(const Juego& juego) {
	dibujarTablero(juego.getTablero());
	if (!juego.estaTerminado()) {//
		dibujarPieza(juego.getPieza());
	}// si el juego ya está terminado entonces no hay que dibujar la pieza que está bajando
	dibujarHold(juego.getHold());
	dibujarSiguientes(juego.getSiguiente(0), juego.getSiguiente(1), juego.getSiguiente(2));
	dibujarDatos(juego.getPuntaje(), juego.getLineas(), juego.getNivel());
	dibujarControles();
	if (juego.estaNavegando()) { //esto dibuja mensaje de historial cuando se está deshaciendo o rehaciendo movimientos
		DrawText(TextFormat("HISTORIAL: paso %d de %d", juego.getPaso(), juego.getTotalPasos()), MARGEN_X, 648, 20, SKYBLUE);
		DrawText("Mueve la pieza para seguir jugando", MARGEN_X, 672, 16, LIGHTGRAY);
	}
}
