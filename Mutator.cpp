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
	int mutated_boards = 0;
	std::random_device rd;

	Mutator(Population& population, Population& next_gen, double mt) : pop(population), mutation_rate(mt), next_gen(next_gen)
	{
		population_size = pop.pop_size;
	}

	void make_mutations()
	{
		mutated_boards = 0;
		std::uniform_real_distribution<double> unif(0, 1);

		for (int individual = 0; individual < population_size; individual++)
		{
			std::mt19937 eng(rd());
			double prob = unif(eng);
			
			if (prob < mutation_rate)
			{
				mutated_boards++;
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
		int* boxes = choose_boxes_to_swap(row, individual);
	
		swap(boxes[0], boxes[1], row, individual);
	}

	int* choose_boxes_to_swap(int row, int individual)
	{
		int number_av_values = board.available_boxes[row].size();

		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, number_av_values - 1);
		std::uniform_real_distribution<double> unif(0, 1);

		int a = distr(eng);
		int b = distr(eng);

		while (a == b)
		{
			b = distr(eng);
		}

		int column_a = board.available_boxes[row][a];
		int column_b = board.available_boxes[row][b];

		/*double prob = unif(eng);

		if (prob > 0.9)
		{
		double next = unif(eng);

		column_b = mutate_neighbours(row, column_a, column_b, next);
		}*/

		int boxes[] = { column_a, column_b };

		return boxes;
	}

	//if possible we mutate neighbours so as to not ruin the balance of container boxes.
	int mutate_neighbours(int row, int column_a, int  column_b, double next)
	{
		if (column_a == 1 || column_a == 4 || column_a == 7)
		{
			column_b = dispatch(row, column_a, column_b, 1, -1, next);
		}
		else if (column_a == 2 || column_a == 5 || column_a == 8)
		{
			column_b = dispatch(row, column_a, column_b, -1, -2, next);
		}
		else if (column_a == 0 || column_a == 3 || column_a == 6)
		{
			column_b = dispatch(row, column_a, column_b, 1, 2, next);
		}

		return column_b;
	}

	int dispatch(int row, int column_a, int column_b, int neighbour_1, int neighbour_2, double next)
	{
		if (next > 0.5)
		{
			column_b = check_fixed_and_assign(row, column_a, column_b, neighbour_1, neighbour_2);
		}
		else
		{
			column_b = check_fixed_and_assign(row, column_a, column_b, neighbour_2, neighbour_1);
		}

		return column_b;
	}

	int check_fixed_and_assign(int row, int column_a, int column_b, int neighbour_1, int neighbour_2)
	{
		if (!board.boxes[row][column_a + neighbour_1]->fixed)
		{
			column_b = column_a + neighbour_1;
		}
		else if (!board.boxes[row][column_a + neighbour_2]->fixed)
		{
			column_b = column_a + neighbour_2;
		}
		//std::cout << "b = " + std::to_string(b) + " neigh_1 = " +std::to_string(neighbour_1)+" neigh_2 = "+std::to_string(neighbour_2) +" a = "+std::to_string(a) +" row = "+std::to_string(row)+" column_a = "+std::to_string(column_a) << std::endl;
		return column_b;
	}

	void swap(int box_a, int box_b, int row, int individual)
	{
		int temp = get_value(individual, next_gen, row, box_a);
		next_gen.population[individual][row][box_a]->value = get_value(individual, next_gen, row, box_b);
		next_gen.population[individual][row][box_b]->value = temp;
	}

	int get_value(int individual, Population& pop, int row, int column)
	{
		return pop.population[individual][row][column]->value;
	}
};