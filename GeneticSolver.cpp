#include "Population.cpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>
#include <random>
#include <queue>
#include "Comparator.cpp"

using std::vector;

class GeneticSolver
{
public:
	const int sudoku_size = 9;
	double mutation_rate = 0.5;
	const bool elitism = true;
	Population pop = new Population(true);
	Population next_gen = new Population(true);
	Board board = Board();
	bool solved = false;
	int population_size = 100;
	int total_fitness_sum;
	int count_breeds = 0;
	int best_fitness = -100;
	

	bool threshold(int threshold_fitness)
	{
		bool other_strategy;
		other_strategy = (best_fitness == threshold_fitness);

		return other_strategy;
	}

	void solve() 
	{
		while (!solved) 
		{
			if (threshold(0))
			{
				mutation_rate = 1.0;
			}
			make_new_population();
			make_mutations();

			for (int i = 0; i < population_size; i++)
			{
				store_individual(i, i, pop, next_gen);
			}	

			if (count_breeds % 10000 == 0) 
			{
				print(pop, 0);

				std::cout << "fitness " + std::to_string(count_fitness(0, pop)) << std::endl;
				std::cout << "best_fitness " + std::to_string(best_fitness) << std::endl;
				std::cout << "generation " + std::to_string(count_breeds) << std::endl;
				total_fitness_sum = 0;
				for (int i = 0; i < population_size; i++)
				{
					std::cout << " ind "+std::to_string(i)+" : " + std::to_string(count_fitness(i, next_gen));
				}
				
				std::cout << " "  << std::endl;
				std::cout << "avg_fitness " + std::to_string((total_fitness_sum/ population_size)) << std::endl;
				std::cout << "" << std::endl;
			}

			count_breeds++;
		}

		print(next_gen, 0);
	}

	int count_fitness(int individual, Population pop)
	{
		int fitness = 0;
		for (int column = 0; column < sudoku_size; column++)
		{
			for (int box = 0; box < sudoku_size-1; box++)
			{
				int value_box = pop.population[individual][box][column]->value;

				for (int other_box = box + 1; other_box < sudoku_size; other_box++)
				{
					int value_other = pop.population[individual][other_box][column]->value;

					if (value_box == value_other)
					{
						fitness--;
					}
				}
			}
		}
		int container_size = sqrt(sudoku_size);

		for (int row = 0; row < container_size; row++)
		{
			for (int column = 0; column < container_size; column++)
			{
				int value_box = pop.population[individual][row][column]->value;

				int container_x = find_container_starting_box(row, column)[0];
				int container_y = find_container_starting_box(row, column)[1];

				int start_row = container_x + row;
				int end_row = container_size + container_x;

				for (int other_row = start_row; other_row < end_row; other_row++)
				{
					int start_col = container_y + column;
					int end_col = container_size + container_y;

					for (int other_column = start_col; other_column < end_col; other_column++)
					{
						int value_other = pop.population[individual][other_row][other_column]->value;

						if (value_box == value_other)
						{
							fitness--;
						}
					}
				}
			}
		}
		if (fitness == 0) {
			std::cout << "solution is individual "+std::to_string(individual) << std::endl;
			solved = true;
		}
		if (fitness > best_fitness)
		{
			best_fitness = fitness;
		}
		total_fitness_sum += fitness;
		return fitness;
	}

	void cross_over(int parent_a, int parent_b, int child)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<double> unif(0, 1);

		double prob = unif(eng);
		int half = sudoku_size / 2;
		//bool parent_a_top_half = i > half;
		double percentage_of_first_parent = 0.6;
		if (best_fitness == 0)
		{
			percentage_of_first_parent = 1.0;
		}
		for (int i = 0; i < sudoku_size; i++)
		{
			if (prob > percentage_of_first_parent)
			{
				for (int j = 0; j < sudoku_size; j++)
				{
					next_gen.population[child][i][j]->value = pop.population[parent_b][i][j]->value;
				}
			}
			else
			{
				for (int j = 0; j < sudoku_size; j++)
				{
					next_gen.population[child][i][j]->value = pop.population[parent_a][i][j]->value;
				}
			}

		}
	}

	void make_mutations()
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<double> unif(0, 1);

		//we mutate all except the best individual
		
		int i = 0;
		if (threshold(0))
		{
			i = 3;
		}
		for (i; i < population_size; i++)
		{
			double prob = unif(eng);

			if (prob < mutation_rate)
			{
				mutate(i);
			}
		}
	}

	void make_new_population()
	{
		std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator> fitnesses;
		
		total_fitness_sum = 0;
		for (int i = 0; i < population_size; i++)
		{
			Fitness_index_pair* fi = new Fitness_index_pair();
			fi->fitness = count_fitness(i, pop);
			fi->index = i;

			fitnesses.push(fi);
		}

		//we arbitrarily use the first 30 individuals to breed next gen
		int count = 0;
		int individuals_to_keep = 30;
		vector<int> fittest_individuals_indexes;

		while (!fitnesses.empty())
		{
			Fitness_index_pair* fip = fitnesses.top();

			int individual_index = fitnesses.top()->index;
			int individual_fitness = fitnesses.top()->fitness;

			fittest_individuals_indexes.push_back(individual_index);

			if (count_breeds % 10000 == 0)
			{
				//std::cout << " ind " + std::to_string(individual_index) + " : " + std::to_string(individual_fitness);
			}
			
			store_individual(count, individual_index, next_gen, pop);
			count++;

			fitnesses.pop();
			delete fip;
		}

		if (count_breeds % 10000 == 0)
		{
			std::cout << "" << std::endl;
		}

		//we keep the first 25 fittest from the last generation
		//we mate the first 3 fittest with the first 25 fittest each
		//tottaly the new population has again 100 individuals, 25 old + 75 children

		int children = 25; 
		int breeders = 0;
		
		for (auto const& parent_a : fittest_individuals_indexes)
		{
			if (children == population_size || breeders == 3) {
				break;
			}

			int breeder_children = 0;

			for (auto const& parent_b : fittest_individuals_indexes)
			{
				if (parent_a != parent_b)
				{
					cross_over(parent_a, parent_b, children);
					children++;
					breeder_children++;
				}	

				if (breeder_children == 25) {
					break;
				}
			}

			breeders++;
		}
	}

	void mutate(int individual) 
	{
		if (threshold(0))
		{
			for (int i = 0; i < sudoku_size; i++)
			{
				swap(i, individual);
			}
		}
		else {
			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_int_distribution<> distr(0, sudoku_size - 1);

			int row = distr(eng);
			swap(row, individual);
		}
		
	}

	void swap(int row, int individual) {
		int number_av_values = board.available_boxes[row].size();
		//std::cout << "number_av_values " + std::to_string(number_av_values) << std::endl;
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, number_av_values-1);

		int a = distr(eng);
		int b = distr(eng);

		/*std::cout << "row " + std::to_string(row) << std::endl;
		std::cout << "int a " + std::to_string(a) << std::endl;
		std::cout << "int b " + std::to_string(b) << std::endl;*/

		int index_a = board.available_boxes[row][a];
		int index_b = board.available_boxes[row][b];

		/*std::cout << "index a " + std::to_string(index_a) << std::endl;
		std::cout << "index b " + std::to_string(index_b) << std::endl;*/

		int temp = next_gen.population[individual][row][index_a]->value;
		next_gen.population[individual][row][index_a]->value = next_gen.population[individual][row][index_b]->value;
		next_gen.population[individual][row][index_b]->value = temp;
	}

	void store_individual(int individual, int index, Population a, Population b)
	{
		for (int i = 0; i < sudoku_size; i++)
		{
			for (int j = 0; j < sudoku_size; j++)
			{
				a.population[individual][i][j]->value = b.population[index][i][j]->value;
			}
		}
	}

	int* find_container_starting_box(int row, int column) {
		int container_size = sqrt(sudoku_size);

		int container_x = row / container_size;
		int container_y = column / container_size;

		int starting_box_x = container_x * container_size;
		int starting_box_y = container_y * container_size;

		int coords[] = { starting_box_x, starting_box_y };
		return coords;
	}

	void print(Population b, int individual)
	{
		std::cout << "Individual " + std::to_string(individual) << std::endl;
		for (int i = 0; i < b.size; i++)
		{
			for (int j = 0; j < b.size; j++)
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