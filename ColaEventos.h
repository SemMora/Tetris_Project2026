#ifndef COLAEVENTOS_H
#define COLAEVENTOS_H


enum TipoEvento { EVENTO_VELOCIDAD, EVENTO_DOBLE_INICIO, EVENTO_DOBLE_FIN, EVENTO_BOMBA };

struct Evento {
	int tipo;       
	float momento;  
};

struct NodoEvento {
	Evento evento;
	NodoEvento* siguiente;
};

class ColaEventos {// la cola está ordenada por el momento en que ocurren los eventos
private:
	NodoEvento* frente;
	int cantidad;
	
public:
	ColaEventos();
	~ColaEventos();
	
	void insertar(int tipo, float momento);       // inserta de forma ordenada
	bool hayEventoListo(float tiempoActual) const; 
	Evento sacar();                                
	Evento verFrente() const;                    
	bool estaVacia() const;
	int tamanio() const;
	void vaciar(); 
	
	// igual que en las otras clases , metodos para no copiar la cola y poder liberarla de forma eficiente
	ColaEventos(const ColaEventos&) = delete;
	ColaEventos& operator=(const ColaEventos&) = delete;
};

const char* nombreEvento(int tipo);// depende de el numero del evento da el nombre

#endif
