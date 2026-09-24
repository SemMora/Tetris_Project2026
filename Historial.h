#ifndef HISTORIAL_H
#define HISTORIAL_H

#include "Constantes.h"
#include "Pieza.h"



enum Movimiento { MOV_INICIO, MOV_MOVER, MOV_ROTAR, MOV_BAJAR, MOV_COLOCAR, MOV_HOLD }; // enum de movimientos para poder registrar en el historial que fue lo que pasó en cada paso

struct Estado {// El Estado es para guardar un momento de la partida ,y así poder rehacer y deshacer movimientos
	int celdas[FILAS][COLUMNAS];     
	Pieza pieza;                     
	int hold;                       
	bool holdUsado;
	int siguientes[14];  
	int cantSiguientes;
	int puntaje;
	int lineas;
	int nivel;
	int movimiento;    
};

struct NodoHistorial {//Decidí usar una lista doble para poder ir hacia adelante y hacia atrás en el historial de movimientos
	Estado estado;
	NodoHistorial* anterior;
	NodoHistorial* siguiente;
};

class Historial {
private:
	NodoHistorial* primero;
	NodoHistorial* ultimo;
	NodoHistorial* actual;
	int cantidad;
	int posicion;  
	
	void borrarDespuesDeActual();  
	
public:
	Historial();
	~Historial();
	
	void agregar(const Estado& e);  
	bool retroceder();              
	bool avanzar();                 
	void irAlPrimero();            
	const Estado& estadoActual() const;
	
	bool estaVacio() const;
	int tamanio() const;
	int getPosicion() const;
	void vaciar();                  
	
	// igual que lo que hice en PilaHold , metodos para no copiar el historial y poder liberarlo de forma eficiente
	Historial(const Historial&) = delete;
	Historial& operator=(const Historial&) = delete;
};

const char* nombreMovimiento(int movimiento);

#endif
