#include "Pantallas.h"
#include "Constantes.h"

void textoCentrado(const char* texto, int y, int tamanio, Color color) {
	int ancho = MeasureText(texto, tamanio); // mido cuanto ocupa el texto para restarle la mitad al centro de la ventana
	DrawText(texto, ANCHO_VENTANA / 2 - ancho / 2, y, tamanio, color);
}

void oscurecerFondo() {
	DrawRectangle(0, 0, ANCHO_VENTANA, ALTO_VENTANA, Fade(BLACK, 0.75f)); // el 0.75 es para que el fondo esté medio transparente
}

void dibujarPantallaInicio() {// este es el menu principal antes de jugar
	textoCentrado("TETRIS", 140, 80, SKYBLUE);
	textoCentrado("Developed By Sem", 240, 20, LIGHTGRAY);
	textoCentrado("ENTER: jugar", 340, 25, WHITE);
	textoCentrado("ESC: salir", 380, 25, WHITE);
}

void dibujarPantallaPausa() {// se dibuja encima del juego , por eso primero oscurezco el fondo
	oscurecerFondo();
	textoCentrado("PAUSA", 260, 60, YELLOW);
	textoCentrado("P: continuar", 350, 25, WHITE);
	textoCentrado("ESC: volver al menu", 390, 25, WHITE);
}

void dibujarPantallaFin(int puntaje) {// aqui tambien primero oscurezco el fondo y luego escribo el puntaje final
	oscurecerFondo();
	textoCentrado("FIN DEL JUEGO", 200, 50, RED);
	textoCentrado(TextFormat("Puntaje: %d", puntaje), 270, 30, YELLOW);
	textoCentrado("ENTER: jugar otra vez", 350, 25, WHITE);
	textoCentrado("ESC: volver al menu", 390, 25, WHITE);
}
