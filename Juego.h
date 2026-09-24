#ifndef JUEGO_H
#define JUEGO_H

#include "Tablero.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "Pieza.h"
#include "Historial.h"
#include "ColaEventos.h"

class Juego {// Esta clase es la principal , une todas las clases relacionadas con el juego para construirlo pero el dibujarlo lo hace otra clase
private:
	Tablero tablero;
	ColaPiezas siguientes; 
	PilaHold hold;
	Historial historial;    
	ColaEventos eventos;   
	Pieza actual;  
	
	int puntaje;
	int lineas;
	int nivel;
	int multiplicador;     // x1 o x2 dependiendo de si hay un evento
	bool holdUsado;       
	bool proximaEsBomba;   // si es true entonces la proxima pieza será una bomba
	bool terminado;
	bool navegando;       
	
	float intervaloCaida;  // segundos entre cada bajada automatica
	float tiempoCaida;     // tiempo que paso desde la ultima bajada
	float tiempoJuego;     // el reloj de la partida , con este se sabe cuando se dispara cada evento
	
	const char* mensaje;   // el aviso del ultimo evento que se usó
	float tiempoMensaje;   
	
	bool cabe(const Pieza& p) const;   
	void sacarSiguientePieza();        
	void fijarPieza();                 
	void explotarBomba();              
	void terminarColocacion();         
	void sumarPuntos(int lineasLimpias);
	void aplicarEvento(const Evento& e); 
	void registrar(int movimiento);    
	void restaurar(const Estado& e);   
	
public:
	Juego();
	void nuevaPartida();
	void actualizar(float dt);   
	
	void moverIzquierda();
	void moverDerecha();
	void rotar();
	void bajar();       // baja la pieza una fila , y fija la pieza si ya no puede bajar más
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
	int getMultiplicador() const { return multiplicador; }
	bool estaTerminado() const { return terminado; }
	bool estaNavegando() const { return navegando; }
	int getPaso() const { return historial.getPosicion(); }
	int getTotalPasos() const { return historial.tamanio(); }
	Historial& getHistorial() { return historial; }
	Evento getProximoEvento() const { return eventos.verFrente(); }
	float getTiempoJuego() const { return tiempoJuego; }
	const char* getMensaje() const;  
};

#endif
