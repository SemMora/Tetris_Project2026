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

void dibujarPantallaFin(int puntaje) {
	oscurecerFondo();
	textoCentrado("FIN DEL JUEGO", 200, 50, RED);
	textoCentrado(TextFormat("Puntaje: %d", puntaje), 270, 30, YELLOW);
	textoCentrado("R: ver repeticion de la partida", 340, 25, WHITE);
	textoCentrado("ENTER: jugar otra vez", 380, 25, WHITE);
	textoCentrado("ESC: volver al menu", 420, 25, WHITE);
}

void dibujarBoton(Rectangle btn, const char* texto) {
	bool mouseEncima = CheckCollisionPointRec(GetMousePosition(), btn); 
	if (mouseEncima) {
		DrawRectangleRec(btn, DARKGRAY); // practicamente como un hover de css
	} else {
		DrawRectangleRec(btn, Color{ 45, 45, 45, 255 });
	}
	DrawRectangleLinesEx(btn, 2, LIGHTGRAY);// borde del boton
	int ancho = MeasureText(texto, 18);
	DrawText(texto, btn.x + btn.width / 2 - ancho / 2, btn.y + btn.height / 2 - 9, 18, WHITE);
}

bool botonPresionado(Rectangle btn) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), btn); // practicamente el onMouseClicked de java normal
}

void dibujarControlesRepeticion(int paso, int total, const char* movimiento, bool reproduciendo) {
	textoCentrado(TextFormat("REPETICION  -  paso %d de %d  (%s)", paso, total, movimiento), 12, 20, GOLD);
	
	dibujarBoton(BtnRetroceder, "<< Atras");
	if (reproduciendo) {  // aqui es donde cambio el texto del boton de reproducir a pausar  y viceversa solo sí se está reproduciendo
		dibujarBoton(BtnReproducir, "Pausar");
	} else {
		dibujarBoton(BtnReproducir, "Reproducir");
	}
	dibujarBoton(BtnAvanzar, "Adelante >>");
	
	DrawText("ESPACIO: reproducir", 20, 560, 16, LIGHTGRAY);
	DrawText("IZQ: paso atras", 20, 580, 16, LIGHTGRAY);
	DrawText("DER: paso adelante", 20, 600, 16, LIGHTGRAY);
	DrawText("ESC: salir", 20, 620, 16, LIGHTGRAY);
}