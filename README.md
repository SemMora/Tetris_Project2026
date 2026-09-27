# Proyecto de Tetris 2026

## Librería gráfica
Se usó **raylib 4.0** para el apartado gráfico de el juego

## Cómo compilar y ejecutar (ZinjaI)
1. Tener raylib instalado en `C:\raylib` (el instalador trae su propio compilador en `C:\raylib\w64devkit`).
2. Abrir `Tetris.zpr` con ZinjaI.
3. En las opciones de compilación del proyecto:
   - Toolchain: uno que apunte a `C:\raylib\w64devkit\bin` (en mi caso se llama `raylib-mingw32`).
   - Bibliotecas a enlazar: `raylib opengl32 gdi32 winmm`
   - Estándar: C++14 o el 17
4. Compilar y ejecutar con el botón de ejecutar de ZinjaI.

El juego crea `puntajes.txt` (archivo para los 10 mejores puntajes) y `tiempos.txt` (el cual es una tabla de tiempos) en la carpeta donde se guarde el proyecto

## Controles
| Tecla | Acción |
|---|---|
| Flecha izquierda / derecha | Mover la pieza |
| Flecha arriba | Rotar |
| Flecha abajo | Bajar más rápido |
| C | Hold (una vez por pieza) |
| Z / X | Deshacer / rehacer |
| P | Pausa |
| ENTER | Jugar |
| T | Ver el top 10 (ya dentro de la pantalla de tops: presionar 1 = Bubble Sort y el 2 = Merge sort) |
| M | Medir los tiempos de los ordenamientos |
| R | Ver la repetición al terminar la partida (Espacio, flechas o botones con el mouse) |
| ESC | Volver al menú / salir|

## Estructuras de datos
| Mecánica | Estructura | Archivos |
|---|---|---|
| Piezas siguientes | Cola normal con nodos, se llena por bolsas de 7 piezas mezcladas al azar | ColaPiezas, Bolsa |
| Pieza en espera (hold) | Pila igual con nodos de capacidad 1 | PilaHold |
| Deshacer / rehacer y repetición | Lista doblemente enlazada, cada nodo guarda el estado entero de la partida por cada movimiento| Historial |
| Eventos programados | Utiliza una Cola que siempre va ordenada por el momento de disparo del evento | ColaEventos |
| Tablero | Lista enlazada simple de 20 filas, cada fila con 10 celdas representadas con un vector[10] | Tablero |
| Top 10 | Lista enlazada simple ordenada con Bubble Sort O(n²) o Merge sort O(n log n)  a elección del usuario| Puntajes, Ordenamiento |

Otros archivos: `Pieza` (las 7 piezas con sus 4 rotaciones), `Juego` (reglas del juego), `Dibujo` y `Pantallas` (todo lo de raylib), `Medicion` (comparación de tiempos), `Constantes` y `main`.

## Eventos programados
- **Velocidad:** a los 30 segundos y luego cada 30 segundos la pieza cae más rápido.
- **Puntos dobles:** a los 45 segundos los puntos valen el doble durante 10 segundos, y se repite 45 segundos después (el inicio y el fin son dos eventos).
- **Bomba:** a los 20 segundos y luego cada 40 segundos la siguiente pieza se vuelve de un color blanco y al caer borra la fila sobre la que cayó.

## Puntaje
limpiar o borrar la siguiente cantidad de lineas da: 1 línea = 100 pts, 2 líneas = 300 pts, 3 líneas = 500 pts, 4 líneas = 800 pts. Durante los puntos dobles se multiplica por 2.
La bomba cuenta como 1 línea.