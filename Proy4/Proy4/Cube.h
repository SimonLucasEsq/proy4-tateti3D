class Cube {
private:
	/* A structure to represent the cube where
	 * 1 is player 1, 2 is player 2 and 0 mean that is empty
	 */
	int structure[3][3][3];
	/* keep te amount of market spaces
	 */
	int marked;
	int winner;

	void checkWinner(int x, int y, int z);

public:
	/* mark a piece for a player, true if succes, false if is ocupied */
	bool mark(int player, int x, int y, int z);
	/* Check if is empty */
	bool isEmpty(int x, int y, int z);
	/* return 0 if is a tie, -1 if the game isn't  over yet, or the winner otherwise */
	int getWinner();
	/* return the number of the next player to play */
	int nextPlayer();
	/* return the number of plays so far */
	int plays();
};