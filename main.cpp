#include "raylib.h"

int main(void)
{
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "Ejemplo Basico Raylib 4.0.0");dioo
	
	SetTargetFPS(60);
	
	while (!WindowShouldClose()) 
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		DrawText("Hola, esto es raylib 4.0.0!", 190, 200, 20, LIGHTGRAY);
		DrawCircle(screenWidth / 2, screenHeight / 2 - 40, 50, MAROON);
		
		EndDrawing();
	}
	
	CloseWindow();
	
	return 0;
}
