#include "Population.cpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>
#include <random>
#include <queue>
#include "Comparator.cpp"
#include "Mutator.cpp"
#include "Fitness_counter.cpp"
#include "CrossOver.cpp"
#include "PatriarchMatingStrategy.cpp"

using std::vector;

class GeneticSolver
{
public:
	const int sudoku_size = 9;

	double mutation_rate = 0.3;
	int total_breeders = 100;
	int population_size = 500;

	Population pop = new Population(true);
	Population next_gen = new Population(true);
	Mutator* mutator = new Mutator(pop, next_gen, mutation_rate);
	CrossOver* crossOver = new CrossOver(pop, next_gen);
	Patriarch* breeder = new Patriarch(crossOver, total_breeders, pop.pop_size);
	FitnessCounter fitness_counter;

	bool solved = false;
	int generations = 0;
	int best_fitness = -100;
	
	void solve() 
	{
		while (!solved) 
		{
			make_new_population();

			mutator->make_mutations();

			for (int i = 0; i < population_size; i++)
			{
				store_individual(i, i, pop, next_gen);
			}	

			if (generations % 200 == 0) 
			{
				print_info();
			}

			generations++;
		}

		print_board(next_gen, 0);
	}

	void make_new_population()
	{
		std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator> fitnesses;

		calculate_fitnesses(fitnesses);

		//we keep the first 25 fittest from the last generation

		vector<int> fittest_individuals_indexes;

		store_fittest_individuals(fitnesses, fittest_individuals_indexes);

		breeder->breed(fittest_individuals_indexes);

	}

	void calculate_fitnesses(std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator>& fitnesses)
	{
		for (int i = 0; i < population_size; i++)
		{
			Fitness_index_pair* fi = new Fitness_index_pair();
			//fi->fitness = count_fitness(i, pop);

			int fitness = fitness_counter.count_fitness(i, pop);

			fi->fitness = fitness;

			if (fitness == 0)
			{
				std::cout << "solution is individual " + std::to_string(i) << std::endl;
				solved = true;
			}

			if (fitness > best_fitness)
			{
				best_fitness = fitness;
			}

			fi->index = i;

			fitnesses.push(fi);
		}
	}

	void store_fittest_individuals(std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator>& fitnesses, 
		vector<int>& fittest_individuals_indexes)
	{
		int count = 0;

		while (!fitnesses.empty())
		{
			Fitness_index_pair* fip = fitnesses.top();

			int individual_index = fitnesses.top()->index;

			fittest_individuals_indexes.push_back(individual_index);

			store_individual(count, individual_index, next_gen, pop);
			
			count++;

			fitnesses.pop();
			delete fip;
		}
	}

	void store_individual(int individual, int index, Population a, Population b)
	{
		for (int i = 0; i < sudoku_size; i++)
		{
			for (int j = 0; j < sudoku_size; j++)
			{
				a.population[individual][i][j]->value = get_value(index, b, i, j);
			}
		}
	}

	int get_value(int individual, Population& pop, int row, int column)
	{
		return pop.population[individual][row][column]->value;
	}

	void print_info()
	{
		print_board(pop, 0);

		std::cout << "fitness " + std::to_string(fitness_counter.count_fitness(0, pop)) << std::endl;
		std::cout << "best_fitness " + std::to_string(best_fitness) << std::endl;
		std::cout << "generation " + std::to_string(generations) << std::endl;

		double total_fitness_sum = 0;

		for (int i = 0; i < population_size; i++)
		{
			int fitness = fitness_counter.count_fitness(i, next_gen);
			std::cout << " ind " + std::to_string(i) + " : " + std::to_string(fitness);

			total_fitness_sum += fitness;
		}

		std::cout << " " << std::endl;
		std::cout << "avg_fitness " + std::to_string((total_fitness_sum / (double)population_size)) << std::endl;
		std::cout << "" << std::endl;
	}

	void print_board(Population b, int individual)
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