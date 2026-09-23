#ifndef JUEGO_H
#define JUEGO_H

#include "Tablero.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "Pieza.h"

class Juego {// Esta clase es la principal , une todas las clases relacionadas con el juego para construirlo pero el dibujarlo lo hace otra clase
private:
	Tablero tablero;
	ColaPiezas siguientes; // esta es la cola de piezas proximas a ser jugadas
	PilaHold hold;
	Pieza actual;   // esta pieza representa la que actualmente va cayendo en el juego
	
	int puntaje;
	int lineas;
	int nivel;
	bool holdUsado;        // para validar que el hold se use una sola vez por pieza
	bool terminado;
	
	float intervaloCaida;  // segundos entre cada bajada automatica
	float tiempoCaida;     // tiempo que paso desde la ultima bajada
	
	bool cabe(const Pieza& p) const;   //si la pieza cabe osea no choca con algo devuelve true
	void sacarSiguientePieza();        // extrae la siguiente pieza que esté en la ColaPiezas
	void fijarPieza();                 
	void terminarColocacion();         // limpia lineas de ser necesario y saca la siguiente pieza
	void sumarPuntos(int lineasLimpias);
	
public:
	Juego();
	void nuevaPartida();
	void actualizar(float dt);   // esta función se utiliza en cada frame para realizar la caida automatica de las piezas
	
	void moverIzquierda();
	void moverDerecha();
	void rotar();
	void bajar();                // baja la pieza una fila , y fija la pieza si ya no puede bajar más
	void usarHold();
	
	// Funciones que sirven para consultar a la hora de tener que dibujar cosas como el tablero piezas y demás cosas que irán en pantalla
	const Tablero& getTablero() const { return tablero; }
	const Pieza& getPieza() const { return actual; }
	int getSiguiente(int pos) const { return siguientes.verPosicion(pos); }
	int getHold() const { return hold.verTope(); }
	int getPuntaje() const { return puntaje; }
	int getLineas() const { return lineas; }
	int getNivel() const { return nivel; }
	bool estaTerminado() const { return terminado; }
};

#endif
