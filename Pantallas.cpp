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
	textoCentrado("T: mejores puntajes", 380, 25, WHITE);
	textoCentrado("M: medir tiempos de ordenamiento", 420, 25, WHITE);
	textoCentrado("ESC: salir", 460, 25, WHITE);
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
	textoCentrado("T: mejores puntajes", 420, 25, WHITE);
	textoCentrado("ESC: volver al menu", 460, 25, WHITE);
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

void dibujarPantallaPeticionNombre(const char* nombre, int puntaje) {
	oscurecerFondo();
	textoCentrado("ENTRASTE AL TOP 10!", 190, 40, GOLD);
	textoCentrado(TextFormat("Puntaje: %d", puntaje), 250, 30, YELLOW);
	textoCentrado("Escribe tu nombre (letras y numeros):", 320, 20, LIGHTGRAY);
	DrawRectangleLines(ANCHO_VENTANA / 2 - 150, 355, 300, 45, WHITE); // el textfield donde se va escribiendo el nombre
	textoCentrado(nombre, 365, 28, WHITE);
	textoCentrado("ENTER: guardar", 430, 22, WHITE);
}

void dibujarPantallaTopPuntajes(const ListaPuntajes& lista, int algoritmo, double microsegundos) {
	textoCentrado("MEJORES PUNTAJES", 50, 40, GOLD);
	
	int centro = ANCHO_VENTANA / 2;
	const NodoPuntaje* actual = lista.getcabeza();
	if (actual == nullptr) {
		textoCentrado("Todavia no hay puntajes guardados", 250, 20, LIGHTGRAY);
	}
	int posicion = 1;
	int y = 120;
	while (actual != nullptr) {// mientras haya nodos en la lista voy a ir dibujando cada uno de ellos , un nodo lo puedo representar como un jugador con su puntaje
		DrawText(TextFormat("%2d.", posicion), centro - 220, y, 25, LIGHTGRAY);
		DrawText(actual->nombre.c_str(), centro - 170, y, 25, WHITE);
		DrawText(TextFormat("%d", actual->puntaje), centro + 110, y, 25, YELLOW);
		actual = actual->siguiente;
		posicion++;
		y += 35;
	}
	
	textoCentrado(TextFormat("Ordenado con: %s", nombreAlgoritmo(algoritmo)), 500, 20, SKYBLUE);
	textoCentrado(TextFormat("Tardo %.2f microsegundos", microsegundos), 530, 20, SKYBLUE);
	textoCentrado("1: Burbuja     2: Merge sort", 580, 20, WHITE);
	textoCentrado("ESC: volver al menu", 615, 20, WHITE);
}

void dibujarPantallaMedicion(const ResultadoMedicion& resultado) {// aqui creo la pantalla en donde muestro los tiempos de bubble sort vs  merge sort y guardo en un txt para el informe
	textoCentrado("BURBUJA vs MERGE SORT", 60, 35, GOLD);
	textoCentrado(TextFormat("Microsegundos , promedio de %d corridas", REPETICIONES), 110, 18, LIGHTGRAY);
	
	int centro = ANCHO_VENTANA / 2;
	DrawText("n", centro - 230, 170, 22, WHITE);
	DrawText("Burbuja", centro - 90, 170, 22, WHITE);
	DrawText("Merge sort", centro + 100, 170, 22, WHITE);
	int i = 0;
	while (i < 4) { 
		int y = 215 + i * 45;
		DrawText(TextFormat("%d", resultado.tamanios[i]), centro - 230, y, 22, YELLOW);
		DrawText(TextFormat("%.2f", resultado.bubble[i]), centro - 90, y, 22, WHITE);
		DrawText(TextFormat("%.2f", resultado.merge[i]), centro + 100, y, 22, WHITE);
		i++;
	}
	
	textoCentrado("La tabla tambien se guardo en tiempos.txt", 430, 20, SKYBLUE);
	textoCentrado("ESC: volver al menu", 480, 20, WHITE);
}
