#ifndef COLAPIEZAS_H
#define COLAPIEZAS_H

struct NodoCola {
	int tipo; // tipo de una pieza del 0 al 6 
	NodoCola* siguiente;
};


class ColaPiezas {
private:
	NodoCola* frente;
	NodoCola* ultimo;
	int cantidad;
	
public:
	ColaPiezas();
	~ColaPiezas();
	
	void encolar(int tipo);          // O(1)
	int desencolar();                // O(1), devuelve -1 si esta vacia
	int verPosicion(int pos) const;  // O(pos), solo mira, no saca nada
	bool estaVacia() const;
	int tamanio() const;
	void vaciar();                   // O(n)
	
	
	// estas funciones son para evitar copiar 2 veces la cola 
	ColaPiezas(const ColaPiezas&) = delete;
	ColaPiezas& operator=(const ColaPiezas&) = delete;
};

#endif
