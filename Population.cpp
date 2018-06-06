#include "Board.cpp"

class Population 
{
	
public:
	
	int size = 9;
	const int pop_size = 100;
	Box* population [100][9][9];

	Population(bool initialize)
	{
		if (initialize)
		{
			for (int i = 0; i < pop_size; i++)
			{
				Board board = Board();
				board.generate_board();

				for (int z = 0; z < size; z++)
				{
					for (int j = 0; j < size; j++)
					{
						population[i][z][j] = board.boxes[z][j];
					}
				}
			}
		}
	}

};