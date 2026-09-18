#include "raylib.h"
#include "Constantes.h"
#include "Pieza.h"
#include "ColaPiezas.h"
#include "Bolsa.h"
#include <cstdlib>
#include <ctime>

int main() {
	srand((unsigned)time(nullptr));
	
	InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "Tetris - EIF207");
	SetTargetFPS(60);
	
	ColaPiezas cola;
	agregarBolsa(cola);
	
	while (!WindowShouldClose()) {
		// Prueba temporal: ENTER saca una pieza de la cola
		if (IsKeyPressed(KEY_ENTER)) {
			cola.desencolar();
			if (cola.tamanio() < 3) {
				agregarBolsa(cola);
			}
		}
		
		BeginDrawing();
		ClearBackground(BLACK);
		
		DrawText("TETRIS", 20, 20, 30, WHITE);
		DrawRectangleLines(MARGEN_X, MARGEN_Y, COLUMNAS * TAM_CELDA, FILAS * TAM_CELDA, GRAY);
		
		// Prueba temporal: mostrar todo lo que hay en la cola
		for (int i = 0; i < cola.tamanio(); i++) {
			char texto[2] = { letraPieza(cola.verPosicion(i)), '\0' };
			DrawText(texto, MARGEN_X + i * 25, 660, 20, YELLOW);
		}
		
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}
