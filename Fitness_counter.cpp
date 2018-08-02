#include "Population.cpp"
#include <set>

class FitnessCounter
{
public:

	int sudoku_size;

	int count_fitness(int individual, Population& pop)
	{
		sudoku_size = pop.sudoku_size;
		int errors = 0;
		int fitness = 162;

		errors += count_column_errors(individual, pop);
		errors += count_container_errors(individual, pop);

		fitness = fitness - errors;

		if (errors == 0)
		{
			std::cout << "solution is individual " + std::to_string(individual) << std::endl;
		}

		return fitness;
	}

	int count_column_errors(int individual, Population& pop)
	{
		int column_errors = 0;
		
		for (int column = 0; column < sudoku_size; column++)
		{
			bool unique_values[] = { true, true, true, true, true, true, true, true, true };

			for (int row = 0; row < sudoku_size; row++)
			{
				int value_box = get_value(individual, pop, row, column) - 1;

				if (unique_values[value_box])
				{
					unique_values[value_box] = false;
				}
				else
				{
					column_errors++;
				}
				
			}

		}
		
		return column_errors;
	}

	int count_container_errors(int individual, Population& pop)
	{
		int container_size = sqrt(sudoku_size);
		int total_container_errors = 0;
		
		for (int row = 0; row < sudoku_size; row += container_size)
		{
			for (int column = 0; column < sudoku_size; column += container_size)
			{
				total_container_errors += count_container_value_reapearences(individual, pop, row, column);
			}
		}

		return total_container_errors;
	}

	int count_container_value_reapearences(int individual, Population& pop, int container_starting_row, int container_starting_column)
	{
		int container_size = sqrt(sudoku_size);
		
		int until_row = container_starting_row + container_size;
		int until_column = container_starting_column + container_size;
		bool unique_values[] = { true, true, true, true, true, true, true, true, true };

		int container_errors = 0;

		for (int i = container_starting_row; i < until_row; i++)
		{
			for (int j = container_starting_column; j < until_column; j++)
			{
				int value_box = get_value(individual, pop, i, j) - 1;

				if (unique_values[value_box])
				{
					unique_values[value_box] = false;
				}
				else
				{
					container_errors++;
				}
			}
		}

		return container_errors;
	}

	int get_value(int individual, Population& pop, int row, int column)
	{
		return pop.population[individual][row][column]->value;
	}

	void print_board(Population& b, int individual)
	{
		std::cout << "Individual " + std::to_string(individual) << std::endl;
		for (int i = 0; i < b.sudoku_size; i++)
		{
			for (int j = 0; j < b.sudoku_size; j++)
			{
				int val = b.population[individual][i][j]->value;

				if (val == 0)
				{
					std::cout << " _ ";
				}
				else
				{
					if (b.population[individual][i][j]->fixed)
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
};