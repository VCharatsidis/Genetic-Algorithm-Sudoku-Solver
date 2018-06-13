#include "Population.cpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>
#include <random>
#include <queue>
#include "Comparator.cpp"

class CrossOver
{
public:

	Population& pop;
	Population& next_gen;
	int sudoku_size;

	CrossOver(Population& pop, Population& next_gen) : pop(pop), next_gen(next_gen)
	{
		sudoku_size = pop.size;
	}

	void cross_over(int parent_a, int parent_b, int child)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<double> unif(0, 1);

		double prob = unif(eng);

		bool top_6_rows_parent_a = false;

		if (prob > 0.4)
		{
			top_6_rows_parent_a = true;
		}

		for (int row = 0; row < sudoku_size; row++)
		{
			if (top_6_rows_parent_a)
			{
				dispatch(row, child, parent_a, parent_b);
			}
			else
			{
				dispatch(row, child, parent_b, parent_a);
			}
		}
	}

	void dispatch(int row, int child, int parent_a, int parent_b)
	{
		if (row <= 5)
		{
			copy_row(row, child, parent_a);
		}
		else
		{
			copy_row(row, child, parent_b);
		}
	}

	void copy_row(int row, int child, int parent)
	{
		for (int j = 0; j < sudoku_size; j++)
		{
			next_gen.population[child][row][j]->value = get_value(parent, pop, row, j);
		}
	}

	int get_value(int individual, Population& pop, int row, int column)
	{
		return pop.population[individual][row][column]->value;
	}
};