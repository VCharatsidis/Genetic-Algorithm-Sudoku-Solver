#ifndef POPULATION_H
#define POPULATION_H

#include "Board.cpp"

class Population 
{
	
public:
	
	int sudoku_size = 9;
	int pop_size =  1000;
	Box* population [1000][9][9];

	Population(bool initialize) 
	{
		if (initialize)
		{
			for (int i = 0; i < pop_size; i++)
			{
				Board board = Board();
				board.generate_board();

				for (int z = 0; z < sudoku_size; z++)
				{
					for (int j = 0; j < sudoku_size; j++)
					{
						population[i][z][j] = board.boxes[z][j];
					}
				}
			}
		}
	}

};

#endif