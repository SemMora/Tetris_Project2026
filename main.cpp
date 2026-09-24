#include "raylib.h"
#include "Constantes.h"
#include "Juego.h"
#include "Dibujo.h"
#include <cstdlib>
#include <ctime>


bool teclaConRepeticion(int tecla, float& acumulado, float deltaTime, float intervalo) { // funcion para poder dejar estripada la tecla y que siga acumulando y haciendo sus eventos
	if (IsKeyPressed(tecla)) {
		acumulado = 0; // si se presionó la tecla , reinicio el tiempo entre teclas 
		return true;
	}
	if (IsKeyDown(tecla)) { // si esa tecla sigue presionada entonces cada vez que el tiempo acumulado supere el intervalo ->
		acumulado += deltaTime;//  -> entre cada tecla pues se genera su accion y reinicio el tiempo acumulado ->
		if (acumulado >= intervalo) {// -> ese ciclo lo repito hasta que se suelte la tecla
			acumulado = 0; 
			return true;
		}
	}
	return false;
}


void leerControles(Juego& juego, float deltaTime) {
	static float tiempoIzq = 0;
	static float tiempoDer = 0;
	static float tiempoAbajo = 0; // Son Static para que sin importar los frames o en donde se llamen siempre conserven su valor y así tener un tiempo acumulado por tecla
	static float tiempoZ = 0;
	static float tiempoX = 0;
	
	if (teclaConRepeticion(KEY_LEFT, tiempoIzq, deltaTime, 0.1f)) {
		juego.moverIzquierda();
	}
	if (teclaConRepeticion(KEY_RIGHT, tiempoDer, deltaTime, 0.1f)) {
		juego.moverDerecha();
	}
	if (teclaConRepeticion(KEY_DOWN, tiempoAbajo, deltaTime, 0.05f)) {
		juego.bajar();
	}
	if (IsKeyPressed(KEY_UP)) { // La tecla de rotacion no necesita que se pueda mantener precionado , es sobretodo por presicion al jugar
		juego.rotar();
	}
	if (IsKeyPressed(KEY_C)) { // y esta como solo se puede usar 1 vez por pieza no necesita repetirse varias veces con intervalos
		juego.usarHold();
	}
	
	if (teclaConRepeticion(KEY_Z, tiempoZ, deltaTime, 0.1f)) { 
		juego.deshacer();
	}
	if (teclaConRepeticion(KEY_X, tiempoX, deltaTime, 0.1f)) { 
		juego.rehacer();
	}
}

int main() {
	srand((unsigned)time(nullptr)); // se toman numeros aleatorios siempre para que al iniciar el juego nunca empiece de la misma forma
	
	InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "Tetris");
	SetTargetFPS(60);
	
	Juego juego;
	
	while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime(); // Por aquello el tiempo delta es el tiempo que ocurre entre cada frame 
		
		if (!juego.estaTerminado()) {
			leerControles(juego, deltaTime);
			juego.actualizar(deltaTime);
		} else if (IsKeyPressed(KEY_ENTER)) {
			juego.nuevaPartida();
		}
		
		BeginDrawing(); // función para preparar a raylib a dibujar
		ClearBackground(BLACK);
		
		dibujarJuego(juego);
		if (juego.estaTerminado()) {
			DrawText("Fin Del Juego Presiona ENTER Para Volver a Jugar", MARGEN_X - 20, 660, 20, RED);
		}
		
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}
