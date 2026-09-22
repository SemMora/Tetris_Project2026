#ifndef PILAHOLD_H
#define PILAHOLD_H

// Nodo de la pila: guarda el tipo de la pieza en espera (0 al 6)
struct NodoPila {
	int tipo;
	NodoPila* siguiente;  
};

// Pila de capacidad 1 para la pieza en espera (hold).
class PilaHold {
private:
	static const int CAPACIDAD = 1;
	NodoPila* tope;
	int cantidad;
	
public:
	PilaHold();
	~PilaHold();
	
	bool apilar(int tipo);    // devuelve false si ya esta llena la pila
	int desapilar();          //  devuelve un -1 como codigo de error si esta vaciala pila
	int verTope() const;      
	bool estaVacia() const;
	bool estaLlena() const;
	void vaciar();
	
	//metodos para no copiar la pila y poder liberarla de forma eficiente
	PilaHold(const PilaHold&) = delete;
	PilaHold& operator=(const PilaHold&) = delete;
};

#endif
