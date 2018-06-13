
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Box.cpp"
#include <string>
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>

using std::vector;

class Board {
public:
	
	const int size = 9;
	Box* boxes[9][9];
	vector<int> fixed_values[9];
	vector<int> available_values[9];
	vector<int> available_boxes[9];

	Board() 
	{
		for (int i = 0; i < size; i++) 
		{
			for (int j = 0; j < size; j++) 
			{
				Box* box = new Box();
				boxes[i][j] = box;
			}
		}
		
		make_hardest_sudoku();

		for (int i = 0; i < size; i++) 
		{
			for (int j = 0; j < size; j++) 
			{
				if (boxes[i][j]->value != 0) 
				{
					boxes[i][j]->fixed = true;
				}
				else {
					available_boxes[i].push_back(j);
				}
			}
		}

		store_fixed_values();
		store_available_values();
	}

	void store_fixed_values()
	{
		for (int i = 0; i < size; i++)
		{
			vector<int> row_fixed_values;
			for (int j = 0; j < size; j++)
			{
				if (boxes[i][j]->fixed)
				{
					row_fixed_values.push_back(boxes[i][j]->value);
				}
			}

			fixed_values[i] = row_fixed_values;
		}
	}

	void store_available_values()
	{
		for (int i = 0; i < size; i++)
		{
			vector<int> row_available_values;

			for (int value = 1; value < size+1; value++)
			{
				int n_fixed_values = fixed_values[i].size();
				bool fixed = false;

				for (int f_value = 0; f_value < n_fixed_values; f_value++)
				{
					if (value == fixed_values[i][f_value])
					{
						fixed = true;
						break;
					}
				}

				if (!fixed)
				{
					row_available_values.push_back(value);
				}
			}

			available_values[i] = row_available_values;
		}
	}

	void reset_board()
	{
		for (int i = 0; i < size; i++)
		{
			int k = 0;
			for (int j = 0; j < size; j++)
			{
				if (boxes[i][j]->fixed)
				{
					boxes[i][j]->value = fixed_values[i][k];
					k++;
				}else
				{
					boxes[i][j]->value = 0;
				}
			}
		}
	}

	void generate_board() 
	{
		std::random_device randomdevice;
		std::mt19937 generator(randomdevice());

		for (int i = 0; i < size; i++)
		{
			std::shuffle(begin(available_values[i]), end(available_values[i]), generator);
			int k = 0;

			for (int j = 0; j < size; j++)
			{
				bool empty_box = !(boxes[i][j]->fixed);

				if (empty_box) 
				{
					boxes[i][j]->value = available_values[i][k];
					k++;
				}
			}
		}
	}

	void make_hardest_sudoku()
	{
		//HARDEST SUDOKU
		boxes[0][0]->value = 8;

		boxes[1][2]->value = 3;
		boxes[1][3]->value = 6;

		boxes[2][1]->value = 7;
		boxes[2][4]->value = 9;
		boxes[2][6]->value = 2;

		boxes[3][1]->value = 5;
		boxes[3][5]->value = 7;

		boxes[4][4]->value = 4;
		boxes[4][5]->value = 5;
		boxes[4][6]->value = 7;

		boxes[5][3]->value = 1;
		boxes[5][7]->value = 3;

		boxes[6][2]->value = 1;
		boxes[6][7]->value = 6;
		boxes[6][8]->value = 8;

		boxes[7][2]->value = 8;
		boxes[7][3]->value = 5;
		boxes[7][7]->value = 1;

		boxes[8][1]->value = 9;
		boxes[8][6]->value = 4;
	}

	void make_easy_sudoku()
	{
		//EASY SUDOKU
		boxes[0][0]->value = 4;
		boxes[0][2]->value = 1;
		boxes[0][3]->value = 2;
		boxes[0][4]->value = 9;
		boxes[0][7]->value = 7;
		boxes[0][8]->value = 5;

		boxes[1][0]->value = 2;
		boxes[1][3]->value = 3;
		boxes[1][6]->value = 8;

		boxes[2][1]->value = 7;
		boxes[2][4]->value = 8;
		boxes[2][8]->value = 6;

		boxes[3][3]->value = 1;
		boxes[3][5]->value = 3;
		boxes[3][7]->value = 6;
		boxes[3][8]->value = 2;

		boxes[4][0]->value = 1;
		boxes[4][2]->value = 5;
		boxes[4][6]->value = 4;
		boxes[4][8]->value = 3;

		boxes[5][0]->value = 7;
		boxes[5][1]->value = 3;
		boxes[5][3]->value = 6;
		boxes[5][5]->value = 8;

		boxes[6][0]->value = 6;
		boxes[6][4]->value = 2;
		boxes[6][7]->value = 3;

		boxes[7][2]->value = 7;
		boxes[7][5]->value = 1;
		boxes[7][8]->value = 4;

		boxes[8][0]->value = 8;
		boxes[8][1]->value = 9;
		boxes[8][4]->value = 6;
		boxes[8][5]->value = 5;
		boxes[8][6]->value = 1;
		boxes[8][8]->value = 7;
	}

};

#endif