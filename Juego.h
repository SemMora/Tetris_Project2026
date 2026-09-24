#ifndef JUEGO_H
#define JUEGO_H

#include "Tablero.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "Pieza.h"
#include "Historial.h"

class Juego {// Esta clase es la principal , une todas las clases relacionadas con el juego para construirlo pero el dibujarlo lo hace otra clase
private:
	Tablero tablero;
	ColaPiezas siguientes;
	PilaHold hold;
	Historial historial; 
	Pieza actual;  
	
	int puntaje;
	int lineas;
	int nivel;
	bool holdUsado;        
	bool terminado;
	bool navegando;        // se pone en true cuando se deshace o rehace , y mientras esté así la pieza no cae
	
	float intervaloCaida;  // segundos entre cada bajada automatica
	float tiempoCaida;     // acumulado de tiempo desde la ultima bajada automatica
	
	bool cabe(const Pieza& p) const;   
	void sacarSiguientePieza();      
	void fijarPieza();                 
	void terminarColocacion();         // limpia lineas de ser necesario y saca la siguiente pieza
	void sumarPuntos(int lineasLimpias);
	void registrar(int movimiento);    
	void restaurar(const Estado& e); 
	
public:
	Juego();
	void nuevaPartida();
	void actualizar(float dt);   // esta función se utiliza en cada frame para realizar la caida automatica de las piezas
	
	void moverIzquierda();
	void moverDerecha();
	void rotar();
	void bajar();                // baja la pieza una fila , y fija la pieza si ya no puede bajar más
	void usarHold();
	void deshacer();
	void rehacer();
	
	// Funciones que sirven para consultar a la hora de tener que dibujar cosas como el tablero piezas y demás cosas que irán en pantalla
	const Tablero& getTablero() const { return tablero; }
	const Pieza& getPieza() const { return actual; }
	int getSiguiente(int pos) const { return siguientes.verPosicion(pos); }
	int getHold() const { return hold.verTope(); }
	int getPuntaje() const { return puntaje; }
	int getLineas() const { return lineas; }
	int getNivel() const { return nivel; }
	bool estaTerminado() const { return terminado; }
	bool estaNavegando() const { return navegando; }
	int getPaso() const { return historial.getPosicion(); }
	int getTotalPasos() const { return historial.tamanio(); }
	Historial& getHistorial() { return historial; }
};

#endif
