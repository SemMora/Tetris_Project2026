#include "raylib.h"
#include "Constantes.h"

int main() {
	InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "Tetris");
	SetTargetFPS(60);
	
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		
		DrawText("TETRIS", 20, 20, 30, WHITE);
		DrawRectangleLines(MARGEN_X, MARGEN_Y, COLUMNAS * TAM_CELDA, FILAS * TAM_CELDA, GRAY);
		
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}
