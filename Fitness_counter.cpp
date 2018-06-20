#include "Population.cpp"
#include <set>

class FitnessCounter
{
public:

	int sudoku_size;
	//double total_fitness;

	int count_fitness(int individual, Population& pop)
	{
		sudoku_size = pop.sudoku_size;
		int fitness = 0;

		fitness += count_column_errors(individual, pop);
		fitness += count_container_errors(individual, pop);

		if (fitness == 0)
		{
			std::cout << "solution is individual " + std::to_string(individual) << std::endl;
		}

		return fitness;
	}

	int count_column_errors(int individual, Population& pop)
	{
		int column_errors = 0;
		//std::set<int> unique_values;
		
		for (int column = 0; column < sudoku_size; column++)
		{
			bool unique_values[] = { true, true, true, true, true, true, true, true, true };

			for (int row = 0; row < sudoku_size; row++)
			{
				int value_box = get_value(individual, pop, row, column);
				unique_values[value_box-1] = false;
			}

			for (int i = 0; i < sudoku_size; i++)
			{
				if (unique_values[i])
				{
					column_errors -= 1;
				}
			}

		}
		
		return column_errors;
	}

	int count_container_errors(int individual, Population& pop)
	{
		int container_size = sqrt(sudoku_size);
		int container_errors = 0;
		
		for (int row = 0; row < sudoku_size; row += container_size)
		{
			for (int column = 0; column < sudoku_size; column += container_size)
			{
				container_errors += count_container_value_reapearences(individual, pop, row, column);
			}
		}

		return container_errors;
	}

	int count_container_value_reapearences(int individual, Population& pop, int container_starting_row, int container_starting_column)
	{
		int container_size = sqrt(sudoku_size);
		//std::set<int> unique_values;
		
		int until_row = container_starting_row + container_size;
		int until_column = container_starting_column + container_size;
		bool unique_values[] = { true, true, true, true, true, true, true, true, true };

		int errors = 0;

		for (int i = container_starting_row; i < until_row; i++)
		{
			for (int j = container_starting_column; j < until_column; j++)
			{
				int value_box = get_value(individual, pop, i, j);
				unique_values[value_box-1] = false;
				//unique_values.insert(value_box);
			}
		}

		for (int i = 0; i < sudoku_size; i++)
		{
			if (unique_values[i])
			{
				errors -= 1;
			}
		}

		//int errors = sudoku_size - unique_values.size();

		return errors;
	}

	int* find_container_starting_box(int row, int column)
	{
		int container_size = sqrt(sudoku_size);

		int container_x = row / container_size;
		int container_y = column / container_size;

		int starting_box_x = container_x * container_size;
		int starting_box_y = container_y * container_size;

		int coords[] = { starting_box_x, starting_box_y };
		return coords;
	}

	int get_value(int individual, Population& pop, int row, int column)
	{
		return pop.population[individual][row][column]->value;
	}
};