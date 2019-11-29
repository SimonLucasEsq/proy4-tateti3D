#include "Cube.h"


Cube::Cube() {
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				structure[x][y][z] = 0;
			}
		}
	}
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

	// TODO: Terminar
}

int Cube::playerAt(int x, int y, int z) {
	return structure[x][y][z];
}

bool Cube::mark(int player, int x, int y, int z) {
	if (!isEmpty(x, y, z))
	{
		return false;
	}
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