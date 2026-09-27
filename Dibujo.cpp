#include "Dibujo.h"

// Constantes para el dibujo
const int TAM_MINI = 20;// esto es el tamaño que tendrán las piezas que se dibujan en el hold y en la bolsa de siguientes piezas
const int COLUMNA_IZQ = 10;
const int ANCHO_IZQ = 178;
const int PANEL_DERECHO = MARGEN_X + COLUMNAS * TAM_CELDA + 20; // posición del panel derecho  el cual es donde se dibujan el hold , las siguientes piezas y los datos del juego
const int ANCHO_DERECHO = 300;
const Color COLOR_VACIO = { 18, 18, 30, 255 }; // esto es un azul casi negro para las celdas vacias del tablero
const Color COLOR_BORDE = { 80, 80, 130, 255 }; // el color de los bordes de los paneles

Color colorPieza(int tipo) {// Para cada tipo de pieza se le pone un color
	switch (tipo) {
	case 0: return Color{ 0, 210, 255, 255 };   // celeste
	case 1: return Color{ 255, 210, 0, 255 };   // amarillo
	case 2: return Color{ 180, 80, 255, 255 };  // morado
	case 3: return Color{ 0, 220, 110, 255 };   // verde
	case 4: return Color{ 255, 60, 80, 255 };   // rojo
	case 5: return Color{ 40, 120, 255, 255 };  // azul
	case 6: return Color{ 255, 140, 0, 255 };   // naranja
	}
	return GRAY;
}

Color colorCelda(int valor) {
	if (valor == 0) {
		return COLOR_VACIO;
	}
	return colorPieza(valor - 1);
}

void dibujarBloque(int x, int y, int tam, Color color) {
	DrawRectangle(x + 1, y + 1, tam - 2, tam - 2, color);
	DrawRectangle(x + 1, y + 1, tam - 2, tam / 8, Fade(WHITE, 0.4f)); 
	DrawRectangle(x + 1, y + tam - 1 - tam / 8, tam - 2, tam / 8, Fade(BLACK, 0.3f)); // el fade es para lucir ese relieve
}

void dibujarCaja(int x, int y, int ancho, int alto, Color borde) {
	Rectangle caja = { (float)x, (float)y, (float)ancho, (float)alto };
	DrawRectangleRounded(caja, 0.12f, 6, Color{ 16, 16, 32, 240 }); // el 0.12 es para saber que tan redondeadas quedan las esquinas
	DrawRectangleRoundedLines(caja, 0.12f, 6, 2, borde);
}

int dibujarTecla(const char* texto, int x, int y) {
	int ancho = MeasureText(texto, 10) + 14;
	DrawRectangleRounded(Rectangle{ (float)x, (float)y, (float)ancho, 20 }, 0.3f, 4, Color{ 60, 60, 95, 255 });
	DrawText(texto, x + 7, y + 5, 10, WHITE);
	return ancho;
}

void dibujarCelda(int fila, int columna, Color color, int desplazamientoY) {
	int x = MARGEN_X + columna * TAM_CELDA;//ejemplo: si la columna es 0 entonces x = 200 + 0 * 30 = 200
	int y = MARGEN_Y + fila * TAM_CELDA + desplazamientoY;// ejemplo: si la fila es 0 entonces y = 40 + 0 * 30 = 40 , y el desplazamiento es lo que baja de más la pieza en la animacion
	dibujarBloque(x, y, TAM_CELDA, color);
}

static void dibujarFondoTablero() {// esto crea todo el marco del tablero y las celdas vacias , ademas de los espacios entre celdas quedan como una cuadricula
	DrawRectangle(MARGEN_X - 4, MARGEN_Y - 4, COLUMNAS * TAM_CELDA + 8, FILAS * TAM_CELDA + 8, COLOR_BORDE);
	DrawRectangle(MARGEN_X, MARGEN_Y, COLUMNAS * TAM_CELDA, FILAS * TAM_CELDA, Color{ 32, 32, 50, 255 });
	int i = 0;
	while (i < FILAS) {
		int j = 0;
		while (j < COLUMNAS) {
			DrawRectangle(MARGEN_X + j * TAM_CELDA + 1, MARGEN_Y + i * TAM_CELDA + 1, TAM_CELDA - 2, TAM_CELDA - 2, COLOR_VACIO);
			j++;
		}
		i++;
	}
}

void dibujarTablero(const Tablero& tablero, bool resaltarCompletas) {
	dibujarFondoTablero();
	int i = 0;
	while (i < FILAS) {
		bool blanca = resaltarCompletas && tablero.filaCompleta(i); //aquí valido si la fila está completa y si hay que resaltarla para el parpadeo
		int j = 0;
		while (j < COLUMNAS) {
			if (blanca) {
				dibujarCelda(i, j, WHITE, 0); 
			} else if (tablero.obtenerCelda(i, j) != 0) { // las celdas vacias se hicieron en dibujarFondoTablero así que solo dibujo las que están ocupadas
				dibujarCelda(i, j, colorCelda(tablero.obtenerCelda(i, j)), 0);
			}
			j++;
		}
		i++;
	}
}

void dibujarPieza(const Pieza& pieza, int desplazamientoY) { // dibuja una pieza en el tablero
	if (pieza.tipo < 0) {
		return;
	}
	int i = 0;
	while (i < 4) {// cada pieza tiene 4 bloques
		int fila, columna;
		posicionBloque(pieza, i, fila, columna);
		if (pieza.bomba) { //como la bomba es especial la pinto de blanco con un punto rojo en el centro para que se diferencie más
			dibujarCelda(fila, columna, WHITE, desplazamientoY);
			DrawRectangle(MARGEN_X + columna * TAM_CELDA + 11, MARGEN_Y + fila * TAM_CELDA + desplazamientoY + 11, 8, 8, RED);
		} else {
			dibujarCelda(fila, columna, colorPieza(pieza.tipo), desplazamientoY);
		}
		i++;
	}
}

static bool cabeEnTablero(const Tablero& tablero, const Pieza& pieza) {// es lo mismo que cabe() de Juego pero para usarlo al dibujar el fantasma de la pieza
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

void dibujarFantasma(const Tablero& tablero, const Pieza& pieza) {
	if (pieza.tipo < 0) {
		return;
	}
	Pieza sombra = pieza;
	sombra.fila++;
	while (cabeEnTablero(tablero, sombra)) { // bajo una copia de la pieza hasta que ya no quepa al fondo de el tablero
		sombra.fila++;
	}
	sombra.fila--; // retrocedo una fila para que quede en la posición correcta
	Color color = colorPieza(pieza.tipo);
	if (pieza.bomba) {
		color = WHITE;
	}
	int i = 0;
	while (i < 4) {
		int fila, columna;
		posicionBloque(sombra, i, fila, columna);
		DrawRectangleLines(MARGEN_X + columna * TAM_CELDA + 2, MARGEN_Y + fila * TAM_CELDA + 2, TAM_CELDA - 4, TAM_CELDA - 4, Fade(color, 0.5f));// esto ya es el fantasma en sí de la pieza
		i++;
	}
}

void dibujarMiniPieza(int tipo, int centroX, int centroY) {// dibuja una mini pieza para el hold y las siguientes piezas de la bolsa
	if (tipo < 0) {
		return;
	}
	Pieza pieza = crearPieza(tipo);
	pieza.fila = 0;
	pieza.columna = 0;
	int ancho = 3; // casi todas las piezas miden 3 bloques de ancho y 2 de alto , la I y la O son las diferentes
	int alto = 2;
	int filaInicio = 0;
	int columnaInicio = 0;
	if (tipo == 0) { // la I es de 4 de ancho y en su caja empieza en la fila 1
		ancho = 4;
		alto = 1;
		filaInicio = 1;
	} else if (tipo == 1) { // la O es de 2 y empieza en la columna 1
		ancho = 2;
		columnaInicio = 1;
	}
	int x = centroX - ancho * TAM_MINI / 2; // con esto la pieza queda centrada en su cajita
	int y = centroY - alto * TAM_MINI / 2;
	int i = 0;
	while (i < 4) {
		int fila, columna;
		posicionBloque(pieza, i, fila, columna);
		dibujarBloque(x + (columna - columnaInicio) * TAM_MINI, y + (fila - filaInicio) * TAM_MINI, TAM_MINI, colorPieza(tipo));
		i++;
	}
}

void dibujarHold(int tipo) {
	DrawText("HOLD", COLUMNA_IZQ + 4, 44, 20, SKYBLUE);
	dibujarCaja(COLUMNA_IZQ, 70, ANCHO_IZQ, 96, SKYBLUE);
	dibujarMiniPieza(tipo, COLUMNA_IZQ + ANCHO_IZQ / 2, 118);
}

void dibujarSiguientes(int tipo1, int tipo2, int tipo3) { // hasta 3 piezas siguientes termina dibujando
	int centroX = PANEL_DERECHO + ANCHO_DERECHO / 2;
	DrawText("SIGUIENTES", PANEL_DERECHO + 4, 44, 20, GOLD);
	dibujarCaja(PANEL_DERECHO, 70, ANCHO_DERECHO, 270, GOLD);
	DrawRectangleRounded(Rectangle{ (float)(PANEL_DERECHO + 30), 82, (float)(ANCHO_DERECHO - 60), 72 }, 0.2f, 6, Fade(GOLD, 0.12f)); // la que sigue va resaltada
	dibujarMiniPieza(tipo1, centroX, 118);
	dibujarMiniPieza(tipo2, centroX, 208);
	dibujarMiniPieza(tipo3, centroX, 288);
}

void dibujarDatos(int puntaje, int lineas, int nivel) {
	dibujarCaja(COLUMNA_IZQ, 186, ANCHO_IZQ, 124, COLOR_BORDE);
	DrawText("PUNTAJE", COLUMNA_IZQ + 14, 198, 10, LIGHTGRAY);
	DrawText(TextFormat("%d", puntaje), COLUMNA_IZQ + 14, 212, 30, GOLD);
	DrawText("LINEAS", COLUMNA_IZQ + 14, 256, 10, LIGHTGRAY);
	DrawText(TextFormat("%d", lineas), COLUMNA_IZQ + 14, 270, 20, WHITE);
	DrawText("NIVEL", COLUMNA_IZQ + 96, 256, 10, LIGHTGRAY);
	DrawText(TextFormat("%d", nivel), COLUMNA_IZQ + 96, 270, 20, SKYBLUE);
}

void dibujarEventos(int multiplicador, Evento proximo, float tiempoJuego, const char* mensaje) { // aquí dibujo todos los eventos que se estén dando en el momento
	if (multiplicador > 1) {
		DrawRectangleRounded(Rectangle{ (float)COLUMNA_IZQ, 322, (float)ANCHO_IZQ, 30 }, 0.4f, 6, GOLD);
		int ancho = MeasureText("PUNTOS x2", 20);
		DrawText("PUNTOS x2", COLUMNA_IZQ + ANCHO_IZQ / 2 - ancho / 2, 327, 20, BLACK);
	}
	if (proximo.tipo != -1) { // aqui dibujo el proximo evento que se usará y cuanto tiempo falta para que se use
		int segundos = (int)(proximo.momento - tiempoJuego) + 1;
		dibujarCaja(COLUMNA_IZQ, 366, ANCHO_IZQ, 84, COLOR_BORDE);
		DrawText("PROXIMO EVENTO", COLUMNA_IZQ + 14, 378, 10, LIGHTGRAY);
		DrawText(nombreEvento(proximo.tipo), COLUMNA_IZQ + 14, 396, 16, SKYBLUE);
		DrawText(TextFormat("en %d s", segundos), COLUMNA_IZQ + 14, 420, 20, WHITE);
	}
	if (mensaje != nullptr) { // el aviso del evento sale centrado arriba del tablero en una cajita
		int centroTablero = MARGEN_X + COLUMNAS * TAM_CELDA / 2;
		int ancho = MeasureText(mensaje, 20);
		Rectangle aviso = { (float)(centroTablero - ancho / 2 - 12), 6, (float)(ancho + 24), 28 };
		DrawRectangleRounded(aviso, 0.5f, 6, Fade(BLACK, 0.7f));
		DrawRectangleRoundedLines(aviso, 0.5f, 6, 2, GOLD);// el borde de la cajita en dorado
		DrawText(mensaje, centroTablero - ancho / 2, 10, 20, GOLD);
	}
}

static void filaDeControl(const char* tecla, const char* accion, int y) {// dibuja un mini boton con la tecla y su texto
	dibujarTecla(tecla, PANEL_DERECHO + 16, y); 
	DrawText(accion, PANEL_DERECHO + 110, y, 20, LIGHTGRAY);
}

void dibujarControles() { // esto dibuja las instrucciones de los controles del juego 
	DrawText("CONTROLES", PANEL_DERECHO + 4, 360, 20, SKYBLUE);
	dibujarCaja(PANEL_DERECHO, 386, ANCHO_DERECHO, 230, SKYBLUE);
	filaDeControl("IZQ / DER", "Mover", 400);
	filaDeControl("ARRIBA", "Rotar", 426);
	filaDeControl("ABAJO", "Bajar", 452);
	filaDeControl("C", "Hold", 478);
	filaDeControl("Z", "Deshacer", 504);
	filaDeControl("X", "Rehacer", 530);
	filaDeControl("P", "Pausa", 556);
	filaDeControl("F11", "Pantalla completa", 582);
}

void dibujarJuego(const Juego& juego) {
	dibujarTablero(juego.getTablero(), juego.parpadeoEncendido());
	if (!juego.estaTerminado() && !juego.estaAnimando()) {//
		dibujarFantasma(juego.getTablero(), juego.getPieza());
		dibujarPieza(juego.getPieza(), juego.getDesplazamientoCaida());
	}// si el juego ya está terminado o se están limpiando lineas entonces no hay que dibujar la pieza que está bajando
	dibujarHold(juego.getHold());
	dibujarSiguientes(juego.getSiguiente(0), juego.getSiguiente(1), juego.getSiguiente(2));
	dibujarDatos(juego.getPuntaje(), juego.getLineas(), juego.getNivel());
	dibujarEventos(juego.getMultiplicador(), juego.getProximoEvento(), juego.getTiempoJuego(), juego.getMensaje());
	dibujarControles();
	if (juego.estaNavegando()) { // mientras se deshace o rehace se muestra en que paso del historial vamos
		DrawText(TextFormat("HISTORIAL: paso %d de %d", juego.getPaso(), juego.getTotalPasos()), MARGEN_X, 650, 20, SKYBLUE);
		DrawText("Mueve la pieza para seguir jugando", MARGEN_X, 674, 10, LIGHTGRAY);
	}
}


void dibujarCeldasGuardadas(const int celdas[FILAS][COLUMNAS]) {// dibuja el tablero tal como estaba en un nodo del historial que se pasa por parametro
	dibujarFondoTablero();
	int i = 0;
	while (i < FILAS) {
		int j = 0;
		while (j < COLUMNAS) {
			if (celdas[i][j] != 0) {
				dibujarCelda(i, j, colorCelda(celdas[i][j]), 0);
			}
			j++;
		}
		i++;
	}
}// solo dibuja el tablero

void dibujarEstado(const Estado& estado) {// dibuja todo el estado de un nodo del historial , osea el tablero , la pieza , el hold y las siguientes piezas
	dibujarCeldasGuardadas(estado.celdas);
	dibujarPieza(estado.pieza, 0); // en el replay la pieza no lleva animacion de caida
	dibujarHold(estado.hold);
	dibujarSiguientes(estado.siguientes[0], estado.siguientes[1], estado.siguientes[2]);
	dibujarDatos(estado.puntaje, estado.lineas, estado.nivel);
}