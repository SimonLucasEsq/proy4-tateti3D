#include "Cube.h"


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
		count = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				if (structure[i][j][z] != player){
					break;
				}
				count++;
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}

		count = 0;
		for (int i = 2; i >= 0; i--) {
			for (int j = 0; j < 2; j++) {
				if (structure[i][j][z] != player) {
					break;
				}
				count++;
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}
	}


	if (twoPlanesTest(y, z)) {
		count = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				if (structure[x][i][j] != player) {
					break;
				}
				count++;
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}

		count = 0;
		for (int i = 2; i >= 0; i--) {
			for (int j = 0; j < 2; j++) {
				if (structure[x][i][j] != player) {
					break;
				}
				count++;
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}
	}


	if (twoPlanesTest(x, z)) {
		count = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				if (structure[i][y][j] != player) {
					break;
				}
				count++;
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}

		count = 0;
		for (int i = 2; i >= 0; i--) {
			for (int j = 0; j < 2; j++) {
				if (structure[i][y][j] != player) {
					break;
				}
				count++;
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}
	}

	// comprobaciones para en 3 planos
	if (threePlanesTest(x, y, z)) {
		count = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					if (structure[i][j][k] != player) {
						break;
					}
					count++;
				}
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}


		count = 0;
		for (int i = 2; i >= 0; i--) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					if (structure[i][j][k] != player) {
						break;
					}
					count++;
				}
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}


		count = 0;
		for (int i = 2; i >= 0; i--) {
			for (int j = 0; j < 2; j++) {
				for (int k = 2; k >= 0; k--) {
					if (structure[i][j][k] != player) {
						break;
					}
					count++;
				}
			}
		}
		if (count == 3) {
			winner = player;
			return;
		}


		count = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 2; k >= 0; k--) {
					if (structure[i][j][k] != player) {
						break;
					}
					count++;
				}
			}
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