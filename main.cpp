#include "raylib.h"
#include "Constantes.h"
#include "Juego.h"
#include "Dibujo.h"
#include "Pantallas.h"
#include "Puntajes.h"
#include "Medicion.h"
#include <cstdlib>
#include <ctime>
#include <string>

enum Pantalla { PANTALLA_INICIO, PANTALLA_JUEGO, PANTALLA_PAUSA, PANTALLA_NOMBRE, PANTALLA_FIN, PANTALLA_REPETICION, PANTALLA_TOPPUNTAJES, PANTALLA_MEDICION }; // estas son todas las pantallas por las que puede pasar el programa

const float PASO_REPETICION = 0.08f; // cada cuantos segundos avanza un paso la repeticion cuando se reproduce solo
const char* ARCHIVO_PUNTAJES = "puntajes.txt"; 
const char* ARCHIVO_TIEMPOS = "tiempos.txt"; // aquí se guarda la tabla de tiempos de los ordenamientos para el informe


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
	static float tiempoZ = 0;
	static float tiempoX = 0;
	static float tiempoAbajo = 0; // Son Static para que sin importar los frames o en donde se llamen siempre conserven su valor y así tener un tiempo acumulado por tecla
	
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

void controlarRepeticion(Historial& historial, bool& reproduciendo, float& tiempoReplay, float deltaTime) {
	static float tiempoIzq = 0;
	static float tiempoDer = 0;
	
	if (IsKeyPressed(KEY_SPACE) || botonPresionado(BtnReproducir)) {
		if (!reproduciendo && historial.getPosicion() == historial.tamanio()) { // si ya llegó al final y se le da reproducir entonces empieza otra vez desde el inicio
			historial.irAlPrimero();
		}
		reproduciendo = !reproduciendo;
	}
	if (teclaConRepeticion(KEY_LEFT, tiempoIzq, deltaTime, 0.05f) || botonPresionado(BtnRetroceder)) {
		historial.retroceder();
		reproduciendo = false; //si el jugador retrocede de forma manual entonces se detiene la reproducción automática
	}
	if (teclaConRepeticion(KEY_RIGHT, tiempoDer, deltaTime, 0.05f) || botonPresionado(BtnAvanzar)) {
		historial.avanzar();
		reproduciendo = false;
	}
	
	if (reproduciendo) {
		tiempoReplay += deltaTime;
		if (tiempoReplay >= PASO_REPETICION) {// solo va a reproducirse si ya pasó el tiempo necesario para avanzar un paso
			tiempoReplay = 0;
			if (!historial.avanzar()) {
				reproduciendo = false;
			}
		}
	}
}

void leerNombre(std::string& nombre) {//aqui se lee el nombre del jugador para guardarlo en la tabla de mejores puntajes
	int tecla = GetCharPressed(); // GetCharPressed() devuelve numeros y si es 0 significa que se terminó de escribir
	while (tecla > 0) {
		bool letra = (tecla >= 'a' && tecla <= 'z') || (tecla >= 'A' && tecla <= 'Z');
		bool numero = tecla >= '0' && tecla <= '9';
		if ((letra || numero) && nombre.size() < 12) { 
			nombre += (char)tecla;
		}
		tecla = GetCharPressed();
	}
	if (IsKeyPressed(KEY_BACKSPACE) && !nombre.empty()) {
		nombre.erase(nombre.size() - 1);
	}
}

int main() {
	srand((unsigned)time(nullptr)); // se toman numeros aleatorios siempre para que al iniciar el juego nunca empiece de la misma forma
	
	InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "Tetris");
	SetExitKey(KEY_NULL); // así solo cuando yo quiera cerrar la ventana se cierra y no cuando se presiona ESC
	SetTargetFPS(60);
	
	Juego juego;
	Pantalla pantalla = PANTALLA_INICIO;
	bool salir = false;
	bool reproduciendo = false; // esto es para saber si la repetición del historial se está reproduciendo sola o no
	float tiempoReplay = 0;
	
	ListaPuntajes puntajes; // aquí cargo el txt de top 10 mejores puntajes
	int algoritmo = 2;
	puntajes.cargar(ARCHIVO_PUNTAJES);
	puntajes.ordenar(algoritmo);
	std::string nombre;
	double tiempoOrden = 0; // lo que tardó el ultimo ordenamiento de la tabla , se muestra en la pantalla del top
	ResultadoMedicion resultado; 
	
	while (!WindowShouldClose() && !salir) {
		float deltaTime = GetFrameTime(); // Por aquello el tiempo delta es el tiempo que ocurre entre cada frame 
		
		// primero reviso las teclas y la logica según la pantalla en la que esté
		if (pantalla == PANTALLA_INICIO) {
			if (IsKeyPressed(KEY_ENTER)) {
				juego.nuevaPartida();
				pantalla = PANTALLA_JUEGO;
			}
			if (IsKeyPressed(KEY_T)) {
				tiempoOrden = ordenarMidiendo(puntajes, algoritmo);
				pantalla = PANTALLA_TOPPUNTAJES;
			}
			if (IsKeyPressed(KEY_M)) {
				BeginDrawing(); 
				ClearBackground(BLACK);
				textoCentrado("Midiendo tiempos, espera un momento...", 330, 25, WHITE);
				EndDrawing();
				resultado = medirTiempos(ARCHIVO_TIEMPOS);
				pantalla = PANTALLA_MEDICION;
			}
			if (IsKeyPressed(KEY_ESCAPE)) {
				salir = true;
			}
		} else if (pantalla == PANTALLA_JUEGO) {
			if (!juego.estaAnimando()) { // mientras hago la animación de parpadeo no dejaré que se puedan leer los controles
				leerControles(juego, deltaTime);
			}
			juego.actualizar(deltaTime);
			if (IsKeyPressed(KEY_P)) {
				pantalla = PANTALLA_PAUSA;
			}
			if (juego.estaTerminado()) {
				if (puntajes.entraAlTop(juego.getPuntaje())) { // si el puntaje entra al top primero se pide el nombre
					nombre = "";
					pantalla = PANTALLA_NOMBRE;
				} else {
					pantalla = PANTALLA_FIN;
				}
			}
		} else if (pantalla == PANTALLA_PAUSA) { // durante la pausa no se llama a actualizar , por eso todo se queda quieto
			if (IsKeyPressed(KEY_P)) {
				pantalla = PANTALLA_JUEGO;
			}
			if (IsKeyPressed(KEY_ESCAPE)) {
				pantalla = PANTALLA_INICIO;
			}
		} else if (pantalla == PANTALLA_NOMBRE) {
			leerNombre(nombre);
			if (IsKeyPressed(KEY_ENTER)) {
				if (nombre.empty()) {
					nombre = "Jugador";
				}
				puntajes.agregar(nombre, juego.getPuntaje());
				puntajes.ordenar(algoritmo);
				puntajes.recortar(10);
				puntajes.guardar(ARCHIVO_PUNTAJES);
				pantalla = PANTALLA_FIN;
			}
		} else if (pantalla == PANTALLA_FIN) {
			if (IsKeyPressed(KEY_R)) {
				juego.getHistorial().irAlPrimero(); 
				reproduciendo = true;
				tiempoReplay = 0;
				pantalla = PANTALLA_REPETICION;
			}
			if (IsKeyPressed(KEY_ENTER)) {
				juego.nuevaPartida();
				pantalla = PANTALLA_JUEGO;
			}
			if (IsKeyPressed(KEY_T)) {
				tiempoOrden = ordenarMidiendo(puntajes, algoritmo);
				pantalla = PANTALLA_TOPPUNTAJES;
			}
			if (IsKeyPressed(KEY_ESCAPE)) {
				pantalla = PANTALLA_INICIO;
			}
		} else if (pantalla == PANTALLA_REPETICION) {
			controlarRepeticion(juego.getHistorial(), reproduciendo, tiempoReplay, deltaTime);
			if (IsKeyPressed(KEY_ESCAPE)) {
				pantalla = PANTALLA_FIN;
			}
		} else if (pantalla == PANTALLA_TOPPUNTAJES) {
			if (IsKeyPressed(KEY_ONE)) { 
				algoritmo = 1;
				tiempoOrden = ordenarMidiendo(puntajes, algoritmo);
			}
			if (IsKeyPressed(KEY_TWO)) {
				algoritmo = 2;
				tiempoOrden = ordenarMidiendo(puntajes, algoritmo);
			}
			if (IsKeyPressed(KEY_ESCAPE)) {
				pantalla = PANTALLA_INICIO;
			}
		} else if (pantalla == PANTALLA_MEDICION) {
			if (IsKeyPressed(KEY_ESCAPE)) {
				pantalla = PANTALLA_INICIO;
			}
		}
		
		BeginDrawing(); // función para preparar a raylib a dibujar
		ClearBackground(BLACK);
		
		//aquí se dibuja todo según la pantalla en la que esté el juego
		if (pantalla == PANTALLA_INICIO) {
			dibujarPantallaInicio();
		} else if (pantalla == PANTALLA_JUEGO) {
			dibujarJuego(juego);
		} else if (pantalla == PANTALLA_PAUSA) {
			dibujarJuego(juego);
			dibujarPantallaPausa();
		} else if (pantalla == PANTALLA_NOMBRE) {
			dibujarJuego(juego);
			dibujarPantallaPeticionNombre(nombre.c_str(), juego.getPuntaje());
		} else if (pantalla == PANTALLA_FIN) {
			dibujarJuego(juego);
			dibujarPantallaFin(juego.getPuntaje());
		} else if (pantalla == PANTALLA_REPETICION) { // aqui uso otros metodos separados para dibujar el historial y los controles de la repetición
			Historial& historial = juego.getHistorial();
			const Estado& estado = historial.estadoActual();
			dibujarEstado(estado);
			dibujarControlesRepeticion(historial.getPosicion(), historial.tamanio(), nombreMovimiento(estado.movimiento), reproduciendo);
		} else if (pantalla == PANTALLA_TOPPUNTAJES) {
			dibujarPantallaTopPuntajes(puntajes, algoritmo, tiempoOrden);
		} else if (pantalla == PANTALLA_MEDICION) {
			dibujarPantallaMedicion(resultado);
		}
		
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}
