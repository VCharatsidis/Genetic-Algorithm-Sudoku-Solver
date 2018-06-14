
#ifndef CROSSOVER_H
#define CROSSOVER_H

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
	std::uniform_real_distribution<double> unif;

	CrossOver(Population& pop, Population& next_gen) : pop(pop), next_gen(next_gen)
	{
		sudoku_size = pop.sudoku_size;
		std::uniform_real_distribution<double> unif(0, 1);
	}

	void cross_over(int parent_a, int parent_b, int child)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
	
		double prob = unif(eng);

		bool top_6_rows_parent_a = false;

		if (prob > 0.5)
		{
			top_6_rows_parent_a = true;
		}

		if (top_6_rows_parent_a)
		{
			copy_board(child, parent_a, parent_b);
		}
		else
		{
			copy_board(child, parent_b, parent_a);
		}

	}

	void copy_board(int child, int parent_a, int parent_b)
	{
		for (int row = 0; row < 6; row++)
		{
			copy_row(row, child, parent_a);
		}

		for (int row = 6; row < sudoku_size; row++)
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
#endif