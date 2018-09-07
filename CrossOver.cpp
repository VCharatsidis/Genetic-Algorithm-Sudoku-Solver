
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
		
		int combination_matrix[6][3] = { { parent_a, parent_a, parent_b }, { parent_a, parent_b, parent_b }, { parent_a, parent_b, parent_a },
									{ parent_b, parent_a, parent_a }, { parent_b, parent_a, parent_b }, { parent_b, parent_b, parent_a } };

		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> combinations(0, 6 - 1);

		int comb = combinations(eng);
		copy_board(child, combination_matrix[comb][0], combination_matrix[comb][1], combination_matrix[comb][2]);

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

	void copy_board(int child, int parent_top, int parent_mid, int parent_bot)
	{
		for (int row = 0; row < 3; row++)
		{
			copy_row(row, child, parent_top);
		}

		for (int row = 3; row < 6; row++)
		{
			copy_row(row, child, parent_mid);
		}

		for (int row = 6; row < sudoku_size; row++)
		{
			copy_row(row, child, parent_bot);
		}
	}

	void copy_row(int row, int child, int parent)
	{
		for (int j = 0; j < sudoku_size; j++)
		{
			next_gen.population[child][row][j].value = get_value(parent, pop, row, j);
		}
	}

	int get_value(int individual, Population& pop, int row, int column)
	{
		return pop.population[individual][row][column].value;
	}
};
#endif