#include "Population.cpp"

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

		for (int column = 0; column < sudoku_size; column++)
		{
			for (int box = 0; box < sudoku_size - 1; box++)
			{
				int value_box = get_value(individual, pop, box, column);

				for (int other_box = box + 1; other_box < sudoku_size; other_box++)
				{
					int value_other = get_value(individual, pop, other_box, column);

					if (value_box == value_other)
					{
						column_errors--;
					}
				}
			}
		}

		return column_errors;
	}

	int count_container_errors(int individual, Population& pop)
	{
		int container_errors = 0;

		for (int row = 0; row < sudoku_size; row++)
		{
			for (int column = 0; column < sudoku_size; column++)
			{
				container_errors += count_container_value_reapearences(individual, pop, row, column);
			}
		}

		return container_errors;
	}

	int count_container_value_reapearences(int individual, Population& pop, int row, int column)
	{
		int value_reapearences = 0;
		int container_size = sqrt(sudoku_size);
		int value_box = get_value(individual, pop, row, column);

		int container_starting_row = find_container_starting_box(row, column)[0];
		int container_starting_column = find_container_starting_box(row, column)[1];

		int end_row = container_size + container_starting_row;
		int end_col = container_size + container_starting_column;

		for (int other_row = row; other_row < end_row; other_row++)
		{
			for (int other_column = container_starting_column; other_column < end_col; other_column++)
			{
				if ((row == other_row) && (column == other_column)) {
					continue;
				}
				else
				{
					int value_other = get_value(individual, pop, other_row, other_column);

					bool reapearance = (value_box == value_other);

					if (reapearance)
					{
						value_reapearences--;
					}
				}
			}
		}

		return value_reapearences;
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