#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

const int FILAS = 7;
const int COLUMNAS = 7;
const char RATON = 'R';
const char GATO = 'G';
const char SALIDA = 'S';
const char VACIO = '.';
const int PROFUNDIDAD_MAXIMA = 3;

vector<vector<char>> tablero(FILAS, vector<char>(COLUMNAS, VACIO));

void inicializarTablero() {
    srand(time(0));
    int filaRaton, columnaRaton, filaGato, columnaGato;
    tablero[0][0] = SALIDA;

    // Generar una posición aleatoria para el ratón que no esté adyacente a la salida
    do {
        filaRaton = rand() % FILAS;
        columnaRaton = rand() % COLUMNAS;
    } while ((filaRaton == 0 && columnaRaton == 1) || (filaRaton == 1 && columnaRaton == 0) || tablero[filaRaton][columnaRaton] == SALIDA);

    do {
        filaGato = rand() % FILAS;
        columnaGato = rand() % COLUMNAS;
    } while ((abs(filaRaton - filaGato) < 3 && abs(columnaRaton - columnaGato) < 3) || tablero[filaGato][columnaGato] == SALIDA);

    tablero[filaRaton][columnaRaton] = RATON;
    tablero[filaGato][columnaGato] = GATO;
}

void imprimirTablero(vector<vector<char>>& tablero) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero[i][j] == RATON) {
                cout << "R ";
            } else if (tablero[i][j] == GATO) {
                cout << "G ";
            } else if (tablero[i][j] == SALIDA) {
                cout << "S ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
}


bool ratonAlcanzaSalida(vector<vector<char>> &tablero) {
    int filaRaton, columnaRaton;
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero[i][j] == RATON) {
                filaRaton = i;
                columnaRaton = j;
                break;
            }
        }
    }
    return (filaRaton == 0 && columnaRaton == 1) || (filaRaton == 1 && columnaRaton == 0);
}

bool gatoAtrapaRaton(vector<vector<char>> &tablero)
{
    int filaRaton, columnaRaton;
    int filaGato, columnaGato;
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (tablero[i][j] == RATON)
            {
                filaRaton = i;
                columnaRaton = j;
            }
            else if (tablero[i][j] == GATO)
            {
                filaGato = i;
                columnaGato = j;
            }
        }
    }

    // Verificar si el gato está adyacente al ratón (en direcciones horizontales o verticales)
    if ((filaRaton == filaGato && abs(columnaRaton - columnaGato) == 1) || (abs(filaRaton - filaGato) == 1 && columnaRaton == columnaGato))
    {
        return true; // El gato está adyacente al ratón
    }
    return false; // El gato no está adyacente al ratón en direcciones horizontales o verticales
}


bool esPosicionValida(vector<vector<char>> &tablero, int fila, int columna) {
    int filaGato, columnaGato;
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero[i][j] == GATO) {
                filaGato = i;
                columnaGato = j;
            }
        }
    }
    return (fila == filaGato && (columna == columnaGato + 1 || columna == columnaGato - 1)) || ((fila == filaGato + 1 || fila == filaGato - 1) && columna == columnaGato);
}

int evaluarGato(vector<vector<char>>& tablero) {
    int filaGato, columnaGato;
    int filaRaton, columnaRaton;
    
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero[i][j] == GATO) {
                filaGato = i;
                columnaGato = j;
            } else if (tablero[i][j] == RATON) {
                filaRaton = i;
                columnaRaton = j;
            }
        }
    }
    
    int distancia = abs(filaGato - filaRaton) + abs(columnaGato - columnaRaton);
    
    // Devolver el inverso de la distancia como puntuación, o sea que, cuanto más cercano esté el gato al ratón, mayor será la puntuación
    return -distancia;
}

int gatoMinimax(vector<vector<char>>& tablero, int profundidad, bool esMaximizador) {
    // Verificar si se alcanzó la profundidad máxima o si el juego ha terminado
    if (profundidad == 0 || gatoAtrapaRaton(tablero) || ratonAlcanzaSalida(tablero)) {
        return evaluarGato(tablero); // Usar la función de evaluación para terminar la recursión
    }

    if (esMaximizador) {
        int mejorPuntuacion = INT_MIN;
        for (int fila = 0; fila < FILAS; fila++) {
            for (int columna = 0; columna < COLUMNAS; columna++) {
                if (tablero[fila][columna] == VACIO && esPosicionValida(tablero, fila, columna)) {
                    tablero[fila][columna] = GATO;
                    int puntuacion = gatoMinimax(tablero, profundidad - 1, false);
                    tablero[fila][columna] = VACIO;
                    mejorPuntuacion = max(mejorPuntuacion, puntuacion);
                }
            }
        }
        return mejorPuntuacion;
    } else {
        int mejorPuntuacion = INT_MAX;
        for (int fila = 0; fila < FILAS; fila++) {
            for (int columna = 0; columna < COLUMNAS; columna++) {
                if (tablero[fila][columna] == VACIO && esPosicionValida(tablero, fila, columna)) {
                    tablero[fila][columna] = RATON;
                    int puntuacion = gatoMinimax(tablero, profundidad - 1, true);
                    tablero[fila][columna] = VACIO;
                    mejorPuntuacion = min(mejorPuntuacion, puntuacion);
                }
            }
        }
        return mejorPuntuacion;
    }
}

bool movimientoValido(int fila, int columna, int filaGato, int columnaGato) {
    return fila >= 0 && fila < FILAS && columna >= 0 && columna < COLUMNAS && !((fila == filaGato) && (columna == columnaGato));
}

int evaluarRaton(vector<vector<char>>& tablero) {
    int filaRaton, columnaRaton;
    int filaSalida, columnaSalida;
    
    // Encontrar las posiciones del ratón y la salida
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero[i][j] == RATON) {
                filaRaton = i;
                columnaRaton = j;
            } else if (tablero[i][j] == SALIDA) {
                filaSalida = i;
                columnaSalida = j;
            }
        }
    }
    
    int distancia = abs(filaRaton - filaSalida) + abs(columnaRaton - columnaSalida);
    
    // Devolver la distancia como puntuación, cuanto más cercano esté el ratón a la salida, menor será la puntuación
    return distancia;
}

int gatoMinimaxRaton(vector<vector<char>>& tablero, int profundidad, bool esMaximizador, int filaRaton, int columnaRaton, int filaGato, int columnaGato) {
    // Verificar si se alcanzó la profundidad máxima o si el juego ha terminado
    if (profundidad == 0 || gatoAtrapaRaton(tablero) || ratonAlcanzaSalida(tablero)) {
        return evaluarRaton(tablero); 
    }

    if (esMaximizador) {
        int mejorPuntuacion = INT_MIN;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if ((i == 0 || j == 0) && movimientoValido(filaRaton + i, columnaRaton + j, filaGato, columnaGato)) {
                    tablero[filaRaton][columnaRaton] = VACIO; // Hacer el movimiento
                    tablero[filaRaton + i][columnaRaton + j] = RATON;
                    int puntuacion = gatoMinimaxRaton(tablero, profundidad - 1, false, filaRaton + i, columnaRaton + j, filaGato, columnaGato);
                    tablero[filaRaton][columnaRaton] = RATON; // Deshacer el movimiento
                    tablero[filaRaton + i][columnaRaton + j] = VACIO;
                    mejorPuntuacion = max(mejorPuntuacion, puntuacion);
                }
            }
        }
        return mejorPuntuacion;
    } else {
        int mejorPuntuacion = INT_MAX;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if ((i == 0 || j == 0) && movimientoValido(filaRaton + i, columnaRaton + j, filaGato, columnaGato)) {
                    tablero[filaRaton][columnaRaton] = VACIO; // Hacer el movimiento
                    tablero[filaRaton + i][columnaRaton + j] = RATON;
                    int puntuacion = gatoMinimaxRaton(tablero, profundidad - 1, true, filaRaton + i, columnaRaton + j, filaGato, columnaGato);
                    tablero[filaRaton][columnaRaton] = RATON; // Deshacer el movimiento
                    tablero[filaRaton + i][columnaRaton + j] = VACIO;
                    mejorPuntuacion = min(mejorPuntuacion, puntuacion);
                }
            }
        }
        return mejorPuntuacion;
    }
}

void moverGatogatoMinimax(vector<vector<char>>& tablero) {
    int mejorPuntuacion = INT_MIN;
    int mejorFila, mejorColumna;
    int filaGato, columnaGato;
    
    // Encontrar la posición actual del gato
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero[i][j] == GATO) {
                filaGato = i;
                columnaGato = j;
            }
        }
    }

    // Calcular la mejor posición para el gato usando gatoMinimax
    for (int fila = 0; fila < FILAS; fila++) {
        for (int columna = 0; columna < COLUMNAS; columna++) {
            if (tablero[fila][columna] == VACIO && esPosicionValida(tablero, fila, columna)) {
                tablero[filaGato][columnaGato] = VACIO; // Actualizar posición actual del gato
                tablero[fila][columna] = GATO; // Mover temporalmente el gato a una nueva posición
                int puntuacion = gatoMinimax(tablero, PROFUNDIDAD_MAXIMA - 1, false); // Reducir la profundidad
                tablero[fila][columna] = VACIO; // Deshacer el movimiento temporal del gato
                tablero[filaGato][columnaGato] = GATO; // Restaurar la posición original del gato
                if (puntuacion > mejorPuntuacion) {
                    mejorPuntuacion = puntuacion;
                    mejorFila = fila;
                    mejorColumna = columna;
                }
            }
        }
    }

    // Actualizar la posición del gato en el tablero
    tablero[filaGato][columnaGato] = VACIO;
    tablero[mejorFila][mejorColumna] = GATO;
}


void moverRatongatoMinimax(vector<vector<char>>& tablero) {
    int mejorPuntuacion = INT_MAX;
    int mejorFila, mejorColumna;
    int filaRaton, columnaRaton;
    int filaGato, columnaGato;

    // Encontrar la posición actual del ratón y el gato
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (tablero[i][j] == RATON) {
                filaRaton = i;
                columnaRaton = j;
            } else if (tablero[i][j] == GATO) {
                filaGato = i;
                columnaGato = j;
            }
        }
    }

    // Calcular la mejor posición para el ratón usando gatoMinimax
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if ((i == 0 || j == 0) && movimientoValido(filaRaton + i, columnaRaton + j, filaGato, columnaGato)) {
                tablero[filaRaton][columnaRaton] = VACIO; // Hacer el movimiento
                tablero[filaRaton + i][columnaRaton + j] = RATON;
                int puntuacion = gatoMinimaxRaton(tablero, PROFUNDIDAD_MAXIMA - 1, false, filaRaton + i, columnaRaton + j, filaGato, columnaGato);
                tablero[filaRaton][columnaRaton] = RATON; // Deshacer el movimiento
                tablero[filaRaton + i][columnaRaton + j] = VACIO;
                if (puntuacion < mejorPuntuacion) {
                    mejorPuntuacion = puntuacion;
                    mejorFila = filaRaton + i;
                    mejorColumna = columnaRaton + j;
                }
            }
        }
    }

    // Actualizar la posición del ratón en el tablero
    tablero[filaRaton][columnaRaton] = VACIO;
    tablero[mejorFila][mejorColumna] = RATON;
}

void jugarJuego() {
    inicializarTablero();
    cout << endl << "!JUEGO DEL GATO Y EL RATON!" << endl;
    imprimirTablero(tablero);
    cout << endl;

    while (true) {
        moverGatogatoMinimax(tablero);
        cout << endl << "MOVIMIENTO DEL GATO" << endl;
        imprimirTablero(tablero);
        
        if (gatoAtrapaRaton(tablero)) {
            cout << "¡Gana el gato!" << endl;
            break;
        }
        
        moverRatongatoMinimax(tablero);
        cout << endl << "Movimiento del raton" << endl;
        imprimirTablero(tablero);
        
        // Verificar si el ratón alcanza la salida
        if (ratonAlcanzaSalida(tablero)) {
            // Si el gato está en la posición (1,1), el gato gana
            int filaGato, columnaGato;
            for (int i = 0; i < FILAS; i++) {
                for (int j = 0; j < COLUMNAS; j++) {
                    if (tablero[i][j] == GATO) {
                        filaGato = i;
                        columnaGato = j;
                        break;
                    }
                }
            }
            
            if (filaGato == 1 && columnaGato == 1) {
                cout << "¡Gana el gato!" << endl;
            } else {
                cout << "¡Gana el ratón!" << endl;
            }
            break;
        }
    }
}

int main() {
    jugarJuego();
    return 0;
}