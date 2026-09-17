#ifndef PIEZA_H
#define PIEZA_H

// Tipos de pieza: I=0, O=1, T=2, S=3, Z=4, J=5, L=6
const int CANT_TIPOS = 7;

struct Pieza {
	int tipo;  // alguno de los tipos anteriormente dichos
	int rotacion;  // la rotación va del 0 al 3 para poder tener los 4 angulos
	int fila;      
	int columna;   // fila y columna de la pieza dentro de si misma osea del bloque
};

Pieza crearPieza(int tipo);// Esta función crea un tipo de pieza especifica y la posiciona al inicio osea en la parte superior del tablero y centrada

void posicionBloque(const Pieza& p, int b, int& filaBloque, int& colBloque);//funcion para ir moviendo el bloque de posicion y de rotación

char letraPieza(int tipo);// depende del tipo devuelve su letra para poderla mostrar

#endif
