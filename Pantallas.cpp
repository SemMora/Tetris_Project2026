#include "Pantallas.h"
#include "Constantes.h"
#include "Dibujo.h"

const int CENTRO = ANCHO_VENTANA / 2;

// Constantes para la decoracion del menu de inicio
static const char* const PILA_IZQUIERDA[5] = { "1.....", "1...6.", "1.3366", "133346", "224446" };
static const char* const PILA_DERECHA[5] = { ".....5", ".2...5", "22.755", "247773", "444333" };

void textoCentrado(const char* texto, int y, int tamanio, Color color) {
	int ancho = MeasureText(texto, tamanio); // mido cuanto ocupa el texto para restarle la mitad al centro de la ventana
	DrawText(texto, ANCHO_VENTANA / 2 - ancho / 2, y, tamanio, color);
}

void oscurecerFondo() {
	DrawRectangle(0, 0, ANCHO_VENTANA, ALTO_VENTANA, Fade(BLACK, 0.75f)); // el 0.75 es para que el fondo esté medio transparente
}

void dibujarFondo() {// usa toda la pantalla 
	DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 26, 22, 52, 255 }, Color{ 8, 8, 18, 255 }); // pinta un degradado de color azul oscuro
}

static int margenLateral() {// con esto los bloques de decoración quedan fijos a los lados de la ventana sin que importe el F11
	float escala = (float)GetScreenHeight() / ALTO_VENTANA; 
	float escalaAncho = (float)GetScreenWidth() / ANCHO_VENTANA;
	if (escalaAncho < escala) {
		escala = escalaAncho;
	}
	return (int)((GetScreenWidth() / escala - ANCHO_VENTANA) / 2);
}

static void opcion(const char* tecla, const char* texto, int x, int y) {// una tecla dibujada y lo que hace a la par , la uso en todos los menus
	dibujarTecla(tecla, x, y);
	DrawText(texto, x + 90, y, 20, WHITE);
}

static int opcionEnLinea(const char* tecla, const char* texto, int x, int y) {// la tecla con su texto pegado , devuelve donde puede empezar la siguiente para ponerlas en filita
	x += dibujarTecla(tecla, x, y) + 8;
	DrawText(texto, x, y, 20, WHITE);
	return x + MeasureText(texto, 20) + 30;
}

static void opcionPequenia(const char* tecla, const char* texto, int y) {// igual pero con letra pequeña , para el cuadrito de ayuda de la repeticion
	dibujarTecla(tecla, 22, y);
	DrawText(texto, 96, y + 5, 10, LIGHTGRAY);
}

static void dibujarPila(const char* const filas[], int x, int yBase) {// dibuja los bloques de la decoracion de la pantalla principal
	int tam = 26;
	int i = 0;
	while (i < 5) {
		int j = 0;
		while (filas[i][j] != '\0') {
			if (filas[i][j] != '.') {
				dibujarBloque(x + j * tam, yBase - (5 - i) * tam, tam, colorPieza(filas[i][j] - '1')); // el caracter '1' menos '1' da 0 , que es el primer color
			}
			j++;
		}
		i++;
	}
}

static void dibujarTitulo(int y) { // titulo del menu principal  dice TETRIS en colores
	const char* titulo = "TETRIS";
	int tamanio = 90;
	int x = CENTRO - MeasureText(titulo, tamanio) / 2;
	int i = 0;
	while (titulo[i] != '\0') { 
		char letra[2] = { titulo[i], '\0' };
		DrawText(letra, x + 6, y + 6, tamanio, Fade(BLACK, 0.5f)); // sombreado para reslatar más el titulo
		DrawText(letra, x, y, tamanio, colorPieza(i));
		x += MeasureText(letra, tamanio) + tamanio / 10; // raylib deja un espacio entre letras del tamaño entre 10
		i++;
	}
}

void dibujarPantallaInicio() {// este es el menu principal antes de jugar
	int margen = margenLateral(); // así los bloques quedan pegados a las esquinas del monitor aunque esté en pantalla completa
	dibujarPila(PILA_IZQUIERDA, -margen, ALTO_VENTANA);
	dibujarPila(PILA_DERECHA, ANCHO_VENTANA + margen - 6 * 26, ALTO_VENTANA);
	dibujarTitulo(110);
	textoCentrado("Developed By Sem", 220, 20, LIGHTGRAY);

	dibujarCaja(CENTRO - 230, 280, 460, 196, SKYBLUE);
	int x = CENTRO - 200;
	opcion("ENTER", "Jugar", x, 304);
	opcion("T", "Mejores puntajes", x, 346);
	opcion("M", "Medir tiempos de ordenamiento", x, 388);
	opcion("ESC", "Salir", x, 430);

	textoCentrado("Presiona ENTER para jugar", 520, 20, GOLD);
	textoCentrado("F11: pantalla completa", 672, 10, GRAY);
}

void dibujarPantallaPausa() {// se dibuja encima del juego , por eso primero oscurezco el fondo
	oscurecerFondo();
	dibujarCaja(CENTRO - 180, 230, 360, 210, YELLOW);
	textoCentrado("PAUSA", 256, 60, YELLOW);
	opcion("P", "Continuar", CENTRO - 130, 350);
	opcion("ESC", "Volver al menu", CENTRO - 130, 390);
}

void dibujarPantallaFin(int puntaje) {
	oscurecerFondo();
	dibujarCaja(CENTRO - 230, 170, 460, 330, RED);
	textoCentrado("FIN DEL JUEGO", 196, 50, RED);
	textoCentrado(TextFormat("Puntaje: %d", puntaje), 262, 30, GOLD);
	int x = CENTRO - 190;
	opcion("R", "Ver repeticion de la partida", x, 330);
	opcion("ENTER", "Jugar otra vez", x, 370);
	opcion("T", "Mejores puntajes", x, 410);
	opcion("ESC", "Volver al menu", x, 450);
}

void dibujarBoton(Rectangle btn, const char* texto) {
	bool mouseEncima = CheckCollisionPointRec(GetMousePosition(), btn);
	if (mouseEncima) {
		DrawRectangleRounded(btn, 0.3f, 6, Color{ 70, 70, 110, 255 }); // practicamente como un hover de css
		DrawRectangleRoundedLines(btn, 0.3f, 6, 2, SKYBLUE);
	} else {
		DrawRectangleRounded(btn, 0.3f, 6, Color{ 40, 40, 66, 255 });
		DrawRectangleRoundedLines(btn, 0.3f, 6, 2, LIGHTGRAY);// borde del boton
	}
	int ancho = MeasureText(texto, 20);
	DrawText(texto, btn.x + btn.width / 2 - ancho / 2, btn.y + btn.height / 2 - 10, 20, WHITE);
}

bool botonPresionado(Rectangle btn) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), btn); // practicamente el onMouseClicked de java normal
}

void dibujarControlesRepeticion(int paso, int total, const char* movimiento, bool reproduciendo) {
	const char* titulo = TextFormat("REPETICION  -  paso %d de %d  (%s)", paso, total, movimiento);
	int ancho = MeasureText(titulo, 20);
	Rectangle cajaTitulo = { (float)(CENTRO - ancho / 2 - 14), 6, (float)(ancho + 28), 28 };
	DrawRectangleRounded(cajaTitulo, 0.5f, 6, Fade(BLACK, 0.7f));
	DrawRectangleRoundedLines(cajaTitulo, 0.5f, 6, 2, GOLD);
	textoCentrado(titulo, 10, 20, GOLD);

	if (total > 0) { // dibujo una barrita de progreso de la repeticion
		DrawRectangle(MARGEN_X, MARGEN_Y + FILAS * TAM_CELDA + 1, COLUMNAS * TAM_CELDA * paso / total, 3, GOLD);
	}

	dibujarBoton(BtnRetroceder, "<< Atras");
	if (reproduciendo) {  // aqui es donde cambio el texto del boton de reproducir a pausar  y viceversa solo sí se está reproduciendo
		dibujarBoton(BtnReproducir, "Pausar");
	} else {
		dibujarBoton(BtnReproducir, "Reproducir");
	}
	dibujarBoton(BtnAvanzar, "Adelante >>");

	dibujarCaja(10, 530, 178, 110, Color{ 80, 80, 130, 255 });
	opcionPequenia("ESPACIO", "Reproducir", 544);
	opcionPequenia("IZQ", "Paso atras", 568);
	opcionPequenia("DER", "Paso adelante", 592);
	opcionPequenia("ESC", "Salir", 616);
}

void dibujarPantallaPeticionNombre(const char* nombre, int puntaje) {
	oscurecerFondo();
	dibujarCaja(CENTRO - 230, 170, 460, 290, GOLD);
	textoCentrado("ENTRASTE AL TOP 10!", 194, 30, GOLD);
	textoCentrado(TextFormat("Puntaje: %d", puntaje), 238, 20, WHITE);
	textoCentrado("Escribe tu nombre (letras y numeros):", 286, 20, LIGHTGRAY);
	Rectangle caja = { (float)(CENTRO - 150), 316, 300, 46 }; // el textfield donde se va escribiendo el nombre
	DrawRectangleRounded(caja, 0.2f, 6, Color{ 10, 10, 20, 255 });
	DrawRectangleRoundedLines(caja, 0.2f, 6, 2, SKYBLUE);
	DrawText(nombre, CENTRO - 136, 324, 30, WHITE);
	DrawText("_", CENTRO - 136 + MeasureText(nombre, 30) + 4, 324, 30, SKYBLUE); // el cursor queda justo despues de la ultima letra
	opcionEnLinea("ENTER", "Guardar", CENTRO - 67, 400);
}

void dibujarPantallaTopPuntajes(const ListaPuntajes& lista, int algoritmo, double microsegundos) {
	textoCentrado("MEJORES PUNTAJES", 40, 40, GOLD);
	dibujarCaja(CENTRO - 260, 100, 520, 380, GOLD);

	const NodoPuntaje* actual = lista.getcabeza();
	if (actual == nullptr) {
		textoCentrado("Todavia no hay puntajes guardados", 270, 20, LIGHTGRAY);
	}
	int posicion = 1;
	int y = 120;
	while (actual != nullptr) {// mientras haya nodos en la lista voy a ir dibujando cada uno de ellos , un nodo lo puedo representar como un jugador con su puntaje
		if (posicion % 2 == 1) {
			DrawRectangle(CENTRO - 250, y - 6, 500, 34, Fade(WHITE, 0.05f));
		}
		Color color = WHITE;
		if (posicion == 1) { // colores de zoro , plata y bronce para los primeros 3
			color = GOLD;
		} else if (posicion == 2) {
			color = Color{ 200, 200, 215, 255 };
		} else if (posicion == 3) {
			color = Color{ 205, 127, 50, 255 };
		}
		DrawText(TextFormat("%2d.", posicion), CENTRO - 236, y, 20, color);
		DrawText(actual->nombre.c_str(), CENTRO - 180, y, 20, color);
		const char* texto = TextFormat("%d", actual->puntaje);
		DrawText(texto, CENTRO + 236 - MeasureText(texto, 20), y, 20, color);
		actual = actual->siguiente;
		posicion++;
		y += 36;
	}

	textoCentrado(TextFormat("Ordenado con: %s", nombreAlgoritmo(algoritmo)), 500, 20, SKYBLUE);
	textoCentrado(TextFormat("Tardo %.2f microsegundos", microsegundos), 526, 20, LIGHTGRAY);
	int x = opcionEnLinea("1", "Burbuja", CENTRO - 250, 580);
	x = opcionEnLinea("2", "Merge sort", x, 580);
	opcionEnLinea("ESC", "Volver al menu", x, 580);
}

void dibujarPantallaMedicion(const ResultadoMedicion& resultado) {// aqui creo la pantalla en donde muestro los tiempos de bubble sort vs  merge sort y guardo en un txt para el informe
	textoCentrado("BURBUJA vs MERGE SORT", 50, 40, GOLD);
	textoCentrado(TextFormat("Microsegundos , promedio de %d corridas", REPETICIONES), 100, 20, LIGHTGRAY);

	dibujarCaja(CENTRO - 260, 140, 520, 230, SKYBLUE);
	DrawText("n", CENTRO - 230, 158, 20, LIGHTGRAY);
	DrawText("Burbuja", CENTRO - 90, 158, 20, LIGHTGRAY);
	DrawText("Merge sort", CENTRO + 100, 158, 20, LIGHTGRAY);
	DrawRectangle(CENTRO - 240, 186, 480, 2, SKYBLUE); // la rayita debajo de los titulos de la tabla
	int i = 0;
	while (i < 4) {
		int y = 202 + i * 40;
		if (i % 2 == 0) {
			DrawRectangle(CENTRO - 250, y - 8, 500, 36, Fade(WHITE, 0.05f));
		}
		DrawText(TextFormat("%d", resultado.tamanios[i]), CENTRO - 230, y, 20, YELLOW);
		DrawText(TextFormat("%.2f", resultado.bubble[i]), CENTRO - 90, y, 20, WHITE);
		DrawText(TextFormat("%.2f", resultado.merge[i]), CENTRO + 100, y, 20, WHITE);
		i++;
	}

	textoCentrado("La tabla tambien se guardo en tiempos.txt", 400, 20, SKYBLUE);
	opcionEnLinea("ESC", "Volver al menu", CENTRO - 95, 450);
}
