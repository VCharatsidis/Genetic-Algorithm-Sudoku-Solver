#include "Board.cpp"
#include <iostream>
#include <string>

using std::cout;
using std::endl;

void print(Board b)
{
	for (int i = 0; i < b.size; i++)
	{
		for (int j = 0; j < b.size; j++)
		{
			int val = b.boxes[i][j]->value;

			if (val == 0)
			{
				std::cout << " _ ";
			}
			else
			{
				if (b.boxes[i][j]->fixed) 
				{
					std::cout << " " + std::to_string(val) + "'";
				}
				else 
				{
					std::cout << " " + std::to_string(val) + " ";
				}
				
			}
		}

		std::cout << "" << std::endl;
	}
}

int main()
{
	Board b = Board();
	print(b);

	for (int i = 0; i < b.size; i++)
	{
		for (int j = 0; j < b.fixed_values[i].size(); j++)
		{
			cout << std::to_string(b.fixed_values[i][j])+" ";

		}
		cout << "" << endl;
	}

	for (int i = 0; i < b.size; i++)
	{
		for (int j = 0; j < b.available_values[i].size(); j++)
		{
			cout << std::to_string(b.available_values[i][j]);
		}
		cout << "" << endl;
	}

	b.generate_board();
	print(b);
	cout << "" << endl;

	b.reset_board();
	print(b);
	cout << "" << endl;

	b.generate_board();
	print(b);
	cout << "" << endl;

	return 0;
}