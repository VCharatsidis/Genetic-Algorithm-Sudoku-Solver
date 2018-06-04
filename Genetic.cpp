#include <vector>
using std::vector;

class Genetic {

	int population_number;
	int number_of_shufles = 200;
	int sudoku_size = 9;
	vector<vector<vector<int>>> population;
	vector<vector<int>> to_solve;

	void create_population() {
		for (int individual = 0; individual < population_number; individual++) {

		}
	}

	void create_individual() {
		vector<vector<int>> board;
		for (int row = 0; row < sudoku_size; row++) {
			for (int column = 0; column < sudoku_size; column++) {
				if (to_solve[row][column] == 0) {
					board[row][column] = column + 1;
				}
			}
			for (int shufle = 0; shufle < number_of_shufles; shufle++) {
				swap(board[row]);
			}
		}
	}

	void swap(vector<int> row) {
		int a = rand() % 25 + 1;
		int b = rand() % 25 + 1;

		int temp = row[a];
		row[a] = b;
		row[b] = temp;
	}
};