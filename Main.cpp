//Alejandro Vico Urbano y Alonso Vives Merino, el grupo 4
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>                  
using namespace std;
const int MinJugadores = 2, MaxJugadores = 4;
const int NumFichas = 28;
typedef struct {
	short int izquierda, derecha;
} tFicha;

typedef tFicha tFichaArray[NumFichas];

typedef struct {
	tFichaArray fichas;
	int contador;
} tListaFichas;

typedef tListaFichas tJugadores[MaxJugadores];
typedef int tPuntosJugadores[MaxJugadores];

typedef struct {
	tListaFichas pozo;
	string tablero;
	tJugadores jugadores;
	tPuntosJugadores puntos;
	int numJugadores;
} tJuego;

int mostrarMenu();
void mostrarTablero(const tJuego& juego);
short int aleat(int num);
string fichaToStr(tFicha ficha);
bool puedePonerIzq(string tablero, tFicha ficha);
bool puedePonerDer(string tablero, tFicha ficha);
string ponerFichaIzq(string tablero, tFicha ficha);
string ponerFichaDer(string tablero, tFicha ficha);
void generarPozo(tListaFichas& pozo);
void desordenarPozo(tListaFichas& pozo);
void robarFicha(tListaFichas& pozo, tFicha& ficha);
void eliminarFicha(tListaFichas& lista, int fichaNum);
short int sumarPuntos(const tListaFichas& jugador);
bool puedeColocarFicha(const tListaFichas& lista, string tablero);
void annadirFicha(tListaFichas& lista, tFicha ficha);
void mostrarConjunto(const tListaFichas& conjunto);
void iniciar(tJuego& juego, int& jugador);
int quienEmpieza(tJuego& juego, int& indice);
bool tieneFicha(tListaFichas fichas, tFicha ficha, int& indice);
bool estrategia1(tJuego& juego, int jugador);
bool estrategia2(tJuego& juego, int jugador);
bool sinSalida(const tJuego& juego);
void escribirListaFichas(ofstream& salida, const tListaFichas& listaFichas);
void escribirJuego(const tJuego& juego, int jugador);
bool leerJuego(tJuego& juego, int& jugador);
void leerListaFichas(ifstream& entrada, tListaFichas& listaFichas);
bool puedeColocarIzq(const tJuego& juego, int& num);
bool puedeColocarDer(const tJuego& juego, int& num);

int main() {
	int num, numFicha, numero, jugador;
	short int izquierda, derecha;
	bool ganador = false, puede = false;
	tFicha ficha;
	tJuego partida;
	///PREPARACION DE LA PARTIDA
	do {
		cout << "     DESEA  RETOMAR  LA  PARTIDA      " << endl;
		cout << "     1. SI                 2. NO      " << endl;
		cin >> num;
	} while (num < 1 || num > 2);

	if (num == 2 || !leerJuego(partida, jugador)) {
		do {
			cout << "Seleccione el numero de jugadores (entre dos y cuatro): ";
			cin >> partida.numJugadores;
		} while (partida.numJugadores < 2 || partida.numJugadores > 4);

		for (int i = 0; i < partida.numJugadores; i++) {
			partida.jugadores[i].contador = 0;
			partida.puntos[i] = 0;
		}
		partida.pozo.contador = NumFichas;
		srand(time(NULL));
		iniciar(partida, jugador);
	}
	//COMIENZO DE LA PARTIDA
	do {
		while (num != 0 && !sinSalida(partida) && !ganador) {

			mostrarTablero(partida);

			if (jugador == 0) {

				if (partida.pozo.contador == 0 && !puedeColocarFicha(partida.jugadores[0], partida.tablero)) {
					num = 3;
				}
				else {
					num = mostrarMenu();
				}

				///////////////////////////////////////////////    			
				switch (num) {
				case 0: break;
				case 1:
					if (puedeColocarIzq(partida, num)) {
						do {
							cout << "Seleccione la ficha que desea poner:  ";
							cin >> numFicha;
						} while (numFicha - 1 < 0 || numFicha - 1 > partida.jugadores[jugador].contador - 1 || !puedePonerIzq(partida.tablero, partida.jugadores[jugador].fichas[numFicha - 1]));
						numFicha = numFicha - 1;

						partida.tablero = ponerFichaIzq(partida.tablero, partida.jugadores[jugador].fichas[numFicha]);
						eliminarFicha(partida.jugadores[jugador], numFicha);
						jugador = jugador + 1;
					}
					break;

				case 2:
					if (puedeColocarDer(partida, num)) {
						do {
							cout << "Seleccione la ficha que desea poner:  ";
							cin >> numFicha;
						} while (numFicha - 1 < 0 || numFicha - 1 > partida.jugadores[jugador].contador - 1 || !puedePonerDer(partida.tablero, partida.jugadores[jugador].fichas[numFicha - 1]));
						numFicha = numFicha - 1;

						partida.tablero = ponerFichaDer(partida.tablero, partida.jugadores[jugador].fichas[numFicha]);
						eliminarFicha(partida.jugadores[jugador], numFicha);
						jugador = jugador + 1;
					}
					break;

				case 3:
					if (partida.pozo.contador == 0 && !puedeColocarFicha(partida.jugadores[0], partida.tablero)) {
						cout << "No puedes realizar ningun movimiento, pasas turno." << endl;
						jugador = jugador + 1;
					}
					else {
						if (partida.pozo.contador != 0 && !puedeColocarFicha(partida.jugadores[0], partida.tablero)) {
							robarFicha(partida.pozo, ficha);
							annadirFicha(partida.jugadores[jugador], ficha);
						}
						else {
							cout << "Mira bien, puedes poner alguna de tus fichas." << endl;
						}
					}
					break;

				case 4:
					escribirJuego(partida, jugador);
				}
				//////////////////////////////////////////////
				if (partida.jugadores[0].contador == 0)
					ganador = true;
			}
			else {     ///////////LAS MAQUINAS

				if (jugador == 1) {
					if (estrategia2(partida, jugador)) {
						if (partida.jugadores[jugador].contador == 0)
							ganador = true;
						else {
							if (jugador == partida.numJugadores - 1)
								jugador = 0;
							else
								jugador = jugador + 1;
						}
					}
					else {
						if (partida.pozo.contador != 0) {
							robarFicha(partida.pozo, ficha);
							annadirFicha(partida.jugadores[jugador], ficha);
						}
						else {
							if (jugador == partida.numJugadores - 1)
								jugador = 0;
							else
								jugador = jugador + 1;
						}
					}

				}
				else {
					if (estrategia1(partida, jugador)) {
						if (partida.jugadores[jugador].contador == 0)
							ganador = true;
						else {
							if (jugador == partida.numJugadores - 1)
								jugador = 0;
							else
								jugador = jugador + 1;
						}
					}
					else {
						if (partida.pozo.contador != 0) {
							robarFicha(partida.pozo, ficha);
							annadirFicha(partida.jugadores[jugador], ficha);
						}
						else {
							if (jugador == partida.numJugadores - 1)
								jugador = 0;
							else
								jugador = jugador + 1;
						}
					}
				}
			}/**/////////////////////////////////////
		}

		if (num != 0) {
			if (sinSalida(partida)) {
				cout << endl << "BLOQUEO, NO HAY GANADOR" << endl;
				for (int i = 0; i < partida.numJugadores; i++) {
					cout << "El jugador  " << i << " tiene " << sumarPuntos(partida.jugadores[i]) << " puntos." << endl;
					partida.puntos[i] = partida.puntos[i] + sumarPuntos(partida.jugadores[i]);
				}
			}
			if (partida.jugadores[jugador].contador == 0) {
				mostrarTablero(partida);
				cout << endl << "ESTA RONDA LA HA GANADO EL JUGADOR " << jugador << endl << endl;     // hacerlo mas bonito
				for (int i = 0; i < partida.numJugadores; i++) {
					cout << "El jugador  " << i << " tiene " << sumarPuntos(partida.jugadores[i]) << " puntos." << endl;
					partida.puntos[i] = partida.puntos[i] + sumarPuntos(partida.jugadores[i]);
				}
			}
			do {
				cout << endl << "DESEA JUGAR OTRA RONDA? " << endl;
				cout << "   1. SI        2. NO   " << endl;
				cin >> num;
			} while (num < 1 || num > 2);

			if (num == 1) {
				for (int i = 0; i < partida.numJugadores; i++) {
					partida.jugadores[i].contador = 0;
				}
				partida.pozo.contador = NumFichas;
				srand(time(NULL));
				iniciar(partida, jugador);
				ganador = false;
			}
		}

	} while (num == 1);


	for (int i = 0; i < partida.numJugadores; i++) {
		partida.puntos[i] = partida.puntos[i] + sumarPuntos(partida.jugadores[i]);
		cout << "El jugador  " << i << " tiene " << partida.puntos[i] << " puntos." << endl;
	}
	jugador = 0;
	for (int i = 1; i < partida.numJugadores; i++) {
		if (partida.puntos[i] < partida.puntos[jugador])
			jugador = i;
	}

	cout << endl << "HA GANADO EL JUGADOR " << jugador << endl;


	return 0;
}


int mostrarMenu() {
	string seguridad;
	int num;

	do {
		do {
			cout << "  -------------  " << endl;
			cout << " |    MENU     | " << endl;
			cout << "  -------------  " << endl;

			cout << "1. Poner ficha por la izquierda" << endl;
			cout << "2. Poner ficha por la derecha" << endl;
			cout << "3. Robar ficha nueva" << endl;
			cout << "4. Guardar partida" << endl;
			cout << "0. Salir" << endl << endl;
			cout << "Elija opcion: ";
			cin >> seguridad;
		} while (seguridad.length() != 1);
		num = int(seguridad[0]) - int('0');
	} while (num < 0 || num > 4);

	return num;
}

void mostrarTablero(const tJuego& juego) {

	cout << " -------------- " << endl;
	cout << " |  TABLERO   | " << endl;
	cout << " -------------- " << endl;



	cout << juego.tablero << endl << endl;
	for (int i = 1; i < juego.numJugadores; i++) {
		cout << "Maquina #" << i << "  ";
		mostrarConjunto(juego.jugadores[i]);
		cout << endl;
	}
	cout << "Jugador     ";
	mostrarConjunto(juego.jugadores[0]);
	cout << endl;
	cout << "POZO:       ";
	mostrarConjunto(juego.pozo);

}

void iniciar(tJuego& juego, int& jugador) {
	tFicha pieza;
	int indice;

	do {
		generarPozo(juego.pozo);
		cout << endl << "-------------------------------- POZO ORDENADO -------------------------------------" << endl;
		mostrarConjunto(juego.pozo);
		desordenarPozo(juego.pozo);
		cout << endl << "-------------------------------- POZO DESORDENADO -------------------------------------" << endl;
		mostrarConjunto(juego.pozo);

		jugador = -1;
		do {
			jugador = jugador + 1;
			for (int i = 0; i < 7; i++) {
				robarFicha(juego.pozo, pieza);
				annadirFicha(juego.jugadores[jugador], pieza);
			}
		} while (jugador < juego.numJugadores - 1);

		cout << endl << "----------------------------- POZO DESPUES DE INICIAR -------------------------------------" << endl;
		mostrarConjunto(juego.pozo);

		jugador = quienEmpieza(juego, indice);
	} while (juego.jugadores[jugador].fichas[indice].izquierda != juego.jugadores[jugador].fichas[indice].derecha);

	juego.tablero = fichaToStr(juego.jugadores[jugador].fichas[indice]);
	eliminarFicha(juego.jugadores[jugador], indice);
	
}

int quienEmpieza(tJuego& juego, int& indice) {
	tFicha pieza;
	int jugador;
	bool tiene = false;

	pieza.izquierda = 7;
	pieza.derecha = 7;
	do {
		jugador = -1;
		pieza.izquierda = pieza.izquierda - 1;
		pieza.derecha = pieza.derecha - 1;
		do {
			jugador = jugador + 1;
			tiene = tieneFicha(juego.jugadores[jugador], pieza, indice);
		} while (tiene == false && jugador < juego.numJugadores - 1);
	} while (pieza.izquierda > 0 && tiene == false);

	return jugador;
}
bool tieneFicha(tListaFichas fichas, tFicha ficha, int& indice) {
	bool tiene = false;
	indice = -1;
	do {
		indice = indice + 1;
		if (fichas.fichas[indice].izquierda == ficha.izquierda && fichas.fichas[indice].derecha == ficha.derecha)
			tiene = true;
	} while (indice < fichas.contador - 1 && tiene == false);

	return tiene;
}

short int aleat(int num) {
	return rand() % 7;
}

string fichaToStr(tFicha ficha) {
	return '|' + to_string(ficha.izquierda) + '-' + to_string(ficha.derecha) + '|';
}

bool puedePonerIzq(string tablero, tFicha ficha) {
	bool puede = false;
	int numero = int(tablero[1]) - int('0');

	if (numero == ficha.izquierda || numero == ficha.derecha) {
		puede = true;
	}
	return puede;
}
bool puedePonerDer(string tablero, tFicha ficha) {
	bool puede = false;
	int numero = int(tablero[tablero.length() - 2]) - int('0');
	if (numero == ficha.izquierda || numero == ficha.derecha) {
		puede = true;
	}
	return puede;
}
bool puedeColocarFicha(const tListaFichas& lista, string tablero) {
	bool puede = false;
	int contador = lista.contador;
	do {
		if (puedePonerIzq(tablero, lista.fichas[contador - 1]) || puedePonerDer(tablero, lista.fichas[contador - 1])) {
			puede = true;
		}
		contador = contador - 1;
	} while (!puede && contador > 0);
	return puede;
}

string ponerFichaIzq(string tablero, tFicha ficha) {
	int numero = int(tablero[1]) - int('0');
	if (ficha.derecha == numero) {
		return "|" + to_string(ficha.izquierda) + "-" + to_string(ficha.derecha) + "|" + tablero;
	}
	else {
		return "|" + to_string(ficha.derecha) + "-" + to_string(ficha.izquierda) + "|" + tablero;
	}

}
string ponerFichaDer(string tablero, tFicha ficha) {
	int numero = int(tablero[tablero.length() - 2]) - int('0');
	if (ficha.izquierda == numero) {
		return tablero + "|" + to_string(ficha.izquierda) + "-" + to_string(ficha.derecha) + "|";
	}
	else {
		return tablero + "|" + to_string(ficha.derecha) + "-" + to_string(ficha.izquierda) + "|";
	}

}

void generarPozo(tListaFichas& pozo) {
	pozo.contador = 0;
	for (int i = 0; i < 7; i++) {
		for (int j = i; j < 7; j++) {
			pozo.fichas[pozo.contador].izquierda = i;
			pozo.fichas[pozo.contador].derecha = j;
			pozo.contador = pozo.contador + 1;
		}
	}
}
void desordenarPozo(tListaFichas& pozo) {
	int idx, i;
	tFicha tmp;
	for (int i = pozo.contador - 1; i >= 0; i--) {
		idx = rand() % (i + 1);
		if (i != idx) {
			tmp = pozo.fichas[i];
			pozo.fichas[i] = pozo.fichas[idx];
			pozo.fichas[idx] = tmp;
		}
	}
}


void robarFicha(tListaFichas& pozo, tFicha& ficha) {
	ficha.izquierda = pozo.fichas[pozo.contador - 1].izquierda;
	ficha.derecha = pozo.fichas[pozo.contador - 1].derecha;
	pozo.contador = pozo.contador - 1;
}

void eliminarFicha(tListaFichas& lista, int fichaNum) {
	lista.fichas[fichaNum] = lista.fichas[lista.contador - 1];
	lista.contador = lista.contador - 1;
}

short int sumarPuntos(const tListaFichas& jugador) {
	int puntos = 0;
	for (int i = 0; i < jugador.contador; i++) {
		puntos = puntos + jugador.fichas[i].izquierda + jugador.fichas[i].derecha;
	}
	return puntos;
}
void annadirFicha(tListaFichas& lista, tFicha ficha) {
	lista.fichas[lista.contador].izquierda = ficha.izquierda;
	lista.fichas[lista.contador].derecha = ficha.derecha;
	lista.contador = lista.contador + 1;
}
void mostrarConjunto(const tListaFichas& conjunto) {
	for (int i = 0; i < conjunto.contador; i++)
		cout << fichaToStr(conjunto.fichas[i]);
	cout << endl;
}
bool estrategia1(tJuego& juego, int jugador) {
	bool puede = false;
	int numFicha = -1;
	puede = puedeColocarFicha(juego.jugadores[jugador], juego.tablero);

	if (puede) {
		do {
			numFicha = numFicha + 1;
		} while (!puedePonerIzq(juego.tablero, juego.jugadores[jugador].fichas[numFicha]) && !puedePonerDer(juego.tablero, juego.jugadores[jugador].fichas[numFicha]));

		if (puedePonerIzq(juego.tablero, juego.jugadores[jugador].fichas[numFicha])) {
			juego.tablero = ponerFichaIzq(juego.tablero, juego.jugadores[jugador].fichas[numFicha]);
			eliminarFicha(juego.jugadores[jugador], numFicha);
		}
		else {
			juego.tablero = ponerFichaDer(juego.tablero, juego.jugadores[jugador].fichas[numFicha]);
			eliminarFicha(juego.jugadores[jugador], numFicha);
		}
	}

	return puede;
}
bool estrategia2(tJuego& juego, int jugador) {
	bool puede = false;
	int valor, laFicha, puntos = 0;
	puede = puedeColocarFicha(juego.jugadores[jugador], juego.tablero);

	if (puede) {
		for (int i = 0; i < juego.jugadores[jugador].contador - 1; i++) {
			if (puedePonerIzq(juego.tablero, juego.jugadores[jugador].fichas[i]) || puedePonerDer(juego.tablero, juego.jugadores[jugador].fichas[i])) {
				valor = juego.jugadores[jugador].fichas[i].izquierda + juego.jugadores[jugador].fichas[i].derecha;
				if (valor > puntos) {
					puntos = valor;
					laFicha = i;
				}
			}
		}

		if (puedePonerIzq(juego.tablero, juego.jugadores[jugador].fichas[laFicha])) {
			juego.tablero = ponerFichaIzq(juego.tablero, juego.jugadores[jugador].fichas[laFicha]);
			eliminarFicha(juego.jugadores[jugador], laFicha);
		}
		else {
			juego.tablero = ponerFichaDer(juego.tablero, juego.jugadores[jugador].fichas[laFicha]);
			eliminarFicha(juego.jugadores[jugador], laFicha);
		}
	}

	return puede;
}

bool sinSalida(const tJuego& juego) {
	bool bloqueo = true;
	int cont = -1;

	if (juego.pozo.contador > 0) {
		bloqueo = false;
	}
	else {
		do {
			cont = cont + 1;
			if (puedeColocarFicha(juego.jugadores[cont], juego.tablero)) {
				bloqueo = false;
			}
		} while (bloqueo == true && cont < juego.numJugadores - 1);
	}

	return bloqueo;
}
void escribirListaFichas(ofstream& salida, const tListaFichas& listaFichas) {
	for (int i = 0; i < listaFichas.contador; i++)
		salida << listaFichas.fichas[i].izquierda << " " << listaFichas.fichas[i].derecha << "  ";
	salida << endl;

}
void escribirJuego(const tJuego& juego, int jugador) {
	ofstream salida;
	string nombre;

	cout << "Con que nombre quieres guardarla?  " << endl;
	cin >> nombre;
	nombre = nombre + '.' + 't' + 'x' + 't';

	salida.open(nombre);

	salida << juego.numJugadores << endl;
	salida << juego.tablero << endl << endl;
	salida << juego.pozo.contador << endl;
	escribirListaFichas(salida, juego.pozo);
	salida << endl;
	for (int i = 0; i < juego.numJugadores; i++) {
		salida << juego.jugadores[i].contador << endl;
		escribirListaFichas(salida, juego.jugadores[i]);
		salida << juego.puntos[i] << endl;
	}
	salida << endl << endl << jugador << endl;
	salida.close();
}
bool leerJuego(tJuego& juego, int& jugador) {
	ifstream entrada;
	bool puede = false;
	string nombre;

	cout << "INDIQUE EL NOMBRE DE LA PARTIDA QUE DESEA RETOMAR: ";
	cin >> nombre;
	nombre = nombre + '.' + 't' + 'x' + 't';

	entrada.open(nombre);
	puede = entrada.is_open();

	if (puede) {
		entrada >> juego.numJugadores;
		entrada >> juego.tablero;
		entrada >> juego.pozo.contador;
		leerListaFichas(entrada, juego.pozo);
		for (int i = 0; i < juego.numJugadores; i++) {
			entrada >> juego.jugadores[i].contador;
			leerListaFichas(entrada, juego.jugadores[i]);
			entrada >> juego.puntos[i];
		}
		entrada >> jugador;
	}
	entrada.close();

	return puede;
}
void leerListaFichas(ifstream& entrada, tListaFichas& listaFichas) {
	for (int i = 0; i < listaFichas.contador; i++)
		entrada >> listaFichas.fichas[i].izquierda >> listaFichas.fichas[i].derecha;

}
bool puedeColocarIzq(const tJuego& juego, int& num) {
	bool puede = false;

	for (int i = 0; i < juego.jugadores[0].contador; i++) {
		if (puedePonerIzq(juego.tablero, juego.jugadores[0].fichas[i])) {
			puede = true;
		}
	}
	if (!puede) {
		do {
			mostrarTablero(juego);
			cout << "No puedes poner ficha por la izquierda" << endl;
			num = mostrarMenu();
		} while (num == 1);
	}

	return puede;
}
bool puedeColocarDer(const tJuego& juego, int& num) {
	bool puede = false;

	for (int i = 0; i < juego.jugadores[0].contador; i++) {
		if (puedePonerDer(juego.tablero, juego.jugadores[0].fichas[i])) {
			puede = true;
		}
	}
	if (!puede) {
		do {
			mostrarTablero(juego);
			cout << "No puedes poner ficha por la derecha" << endl;
			num = mostrarMenu();
		} while (num == 2);
	}

	return puede;
}