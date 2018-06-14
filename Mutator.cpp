#include "Population.cpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>
#include <random>
#include <queue>
#include "Comparator.cpp"

class Mutator
{
public:

	Board board = Board();
	Population& pop;
	Population& next_gen;
	int population_size;
	double mutation_rate;
	std::random_device rd;

	Mutator(Population& population, Population& next_gen, double mt) : pop(population), mutation_rate(mt), next_gen(next_gen)
	{
		population_size = pop.pop_size;
	}

	void make_mutations()
	{
		std::mt19937 eng(rd());
		std::uniform_real_distribution<double> unif(0, 1);

		//we mutate all except the best individual

		for (int individual = 0; individual < population_size; individual++)
		{
			double prob = unif(eng);
			
			if (prob < mutation_rate)
			{
				mutate(individual);
			}
		}
	}

	void mutate(int individual)
	{
		int sudoku_size = board.size;

		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, sudoku_size - 1);

		int row = distr(eng);
		swap(row, individual);
	}

	void swap(int row, int individual)
	{
		int number_av_values = board.available_boxes[row].size();

		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, number_av_values - 1);
		std::uniform_real_distribution<double> unif(0, 1);

		int a = distr(eng);
		int column_a = board.available_boxes[row][a];
		
		int b = distr(eng);
		double prob = unif(eng);

		if (prob > 0.7)
		{
			double next = unif(eng);
			//if possible we mutate neighbours so as to not ruin the balance of container boxes.
			b = mutate_neighbours(a, row, b, next);
		}
		else
		{
			while (a == b)
			{
				b = distr(eng);
			}
		}

		int column_b = board.available_boxes[row][b];
		
		int temp = get_value(individual, next_gen, row, column_a);
		next_gen.population[individual][row][column_a]->value = get_value(individual, next_gen, row, column_b);
		next_gen.population[individual][row][column_b]->value = temp;
	}

	int mutate_neighbours(int a, int row, int b, double next)
	{
		int column_a = board.available_boxes[row][a];

		if (column_a == 1 || column_a == 4 || column_a == 7)
		{
			if (next > 0.5)
			{
				if (!board.boxes[row][column_a + 1]->fixed)
				{
					b = a + 1;
				}
				else if (!board.boxes[row][column_a - 1]->fixed)
				{
					b = a - 1;
				}
			}
			else
			{
				if (!board.boxes[row][column_a - 1]->fixed)
				{
					b = a - 1;
				}
				else if (!board.boxes[row][column_a + 1]->fixed)
				{
					b = a + 1;
				}
			}
		}
		else if (column_a == 2 || column_a == 5 || column_a == 8)
		{
			if (!board.boxes[row][column_a - 1]->fixed)
			{
				b = a - 1;
			}
		}
		else if (column_a == 0 || column_a == 3 || column_a == 6)
		{
			if (!board.boxes[row][column_a + 1]->fixed)
			{
				b = a + 1;
			}
		}

		return b;
	}

	int get_value(int individual, Population& pop, int row, int column)
	{
		return pop.population[individual][row][column]->value;
	}
};