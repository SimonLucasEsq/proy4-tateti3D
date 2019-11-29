#include "Cube.h"
#include <iostream>
using namespace std;


Cube::Cube() {
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				structure[x][y][z] = 0;
			}
		}
	}
	marked = 0;
	winner = 0;
}

bool checkIfCenter(int x, int y, int z) {
	return x == 1 && y == 1 && z == 1;
}

bool twoPlanesTest(int a, int b) {
	return checkIfCenter || (a == 0 && b == 0) || (a == 3 && b == 3);
}

bool threePlanesTest(int x, int y, int z) {
	return checkIfCenter ||
		(x == 0 || x == 2) &&
		(y == 0 || y == 2) &&
		(z == 0 || z == 2);
}

// TODO: Ejecutar en otro hilo la comprobación
void Cube::checkWinner(int x, int y, int z) {
	int player = structure[x][y][z];
	// controlo si no gana paralelo al eje x o y o z
	unsigned count = 0;
	for (int i = 0; i < 3; i++) {
		if (structure[i][y][z] != player) {
			break;
		}
		count++;
	}
	if (count == 3) {
		winner = player;
		return;
	}
	count = 0;
	for (int i = 0; i < 3; i++) {
		if (structure[x][i][z] != player) {
			break;
		}
		count++;
	}
	if (count == 3) {
		winner = player;
		return;
	}
	count = 0;
	for (int i = 0; i < 3; i++) {
		if (structure[x][y][i] != player) {
			break;
		}
		count++;
	}
	if (count == 3) {
		winner = player;
		return;
	}

	//controlo si no gana en 2 planos
	if (twoPlanesTest(x, y)) {
		cout << "entro";
		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[i][i][z] != player){
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}
		cout << " y x " << count << "\n";

		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[2-i][i][z] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}
		cout << " y x " << count << "\n";
	}


	if (twoPlanesTest(y, z)) {
		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[x][i][i] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}

		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[x][2-i][i] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}
		cout << " y z " << count << "\n";
	}


	if (twoPlanesTest(x, z)) {
		cout << "entro";
		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[i][y][i] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}

		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[2-i][y][i] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}
		cout << " x z " << count << "\n";
	}

	// comprobaciones para en 3 planos
	if (threePlanesTest(x, y, z)) {
		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[i][i][i] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}


		count = 0;
		for (int i = 2; i < 0; i++) {
			if (structure[2-i][i][i] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}


		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[2-i][i][2-i] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}


		count = 0;
		for (int i = 0; i < 3; i++) {
			if (structure[i][i][2-i] != player) {
				break;
			}
			count++;
		}
		if (count == 3) {
			winner = player;
			return;
		}
	}

	// TODO: Terminar
}

int Cube::playerAt(int x, int y, int z) {
	return structure[x][y][z];
}

bool Cube::mark(int player, int x, int y, int z) {
	marked++;
	structure[x][y][z] = player;
	checkWinner(x, y, z);
	return true;
}

bool Cube::isEmpty(int x, int y, int z) {
	return structure[x][y][z] == 0;
}

int Cube::getWinner() {
	if (winner != 0) {
		return winner;
	}
	if (marked < 27)
	{
		// if there's still no winner and
		// there are moves left
		return -1;
	} 
	// if there's no winner and is out of moves
	return 0;
}

int Cube::nextPlayer() {
	return  1 + (marked % 2);
}

int Cube::plays() {
	return marked;
}