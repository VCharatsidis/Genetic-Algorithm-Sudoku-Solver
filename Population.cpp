#include "Board.cpp"

class Population 
{
	Board* population[] ;

public:
	Population(int pop_size, bool initialize)
	{
		Board* population = new Board[pop_size];

		if (initialize)
		{
			for (int i = 0; i < pop_size; i++)
			{
				Board* board = new Board();
				//board.
			}
		}
	}
};