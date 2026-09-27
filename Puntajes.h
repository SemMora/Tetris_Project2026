#ifndef PUNTAJES_H
#define PUNTAJES_H

#include <string>
          
struct NodoPuntaje {// Lista simple
	std::string nombre;
	int puntaje;
	NodoPuntaje* siguiente;
};

class ListaPuntajes {
private:
	NodoPuntaje* cabeza;
	int cantidad;
	
public:
	ListaPuntajes();
	~ListaPuntajes();
	
	void agregar(const std::string& nombre, int puntaje); 
	void ordenar(int algoritmo);        
	void recortar(int maximo);        // elimina los nodos que estén se pasen de 10 que es el máximo de puntajes que se guardan
	bool entraAlTop(int puntaje) const; 
	bool cargar(const char* archivo);
	bool guardar(const char* archivo) const;
	void vaciar();
	int tamanio() const;
	const NodoPuntaje* getcabeza() const { return cabeza; }
	
	// igual que en las otras estructuras , metodos para no copiar la lista y poder liberarla de forma eficiente
	ListaPuntajes(const ListaPuntajes&) = delete;
	ListaPuntajes& operator=(const ListaPuntajes&) = delete;
};

const char* nombreAlgoritmo(int algoritmo);// devuelve el nombre del algoritmo para mostrarlo en pantalla

#endif
