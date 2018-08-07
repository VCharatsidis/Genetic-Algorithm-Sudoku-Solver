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
#include "AlmostRandom.cpp"
#include "RouletteWheel.cpp";

using std::vector;

class GeneticSolver
{
public:
	const int sudoku_size = 9;

	// hyperparameters
	double mutation_rate = 0.15;
	int total_breeders = 100;
	int population_size = 1000;
	int mutated_boards = 0;
	int elites = (200);
	double best_avg_fitness = 0;

	Population pop = new Population(true);
	Population next_gen = new Population(true);
	Mutator* mutator = new Mutator(pop, next_gen, mutation_rate);
	CrossOver* crossOver = new CrossOver(pop, next_gen);

	//Patriarch* breeder = new Patriarch(crossOver, total_breeders, pop.pop_size);
	//AlmostRandom* random_breeder = new AlmostRandom(crossOver, 200, pop.pop_size, elitism);

	FitnessCounter fitness_counter;

	bool solved = false;
	int generations = 0;
	int best_fitness = -100;
	double total_fitness;
	
	void solve() 
	{
		while (!solved && generations < 3501) 
		{
			make_new_population();

			mutator->make_mutations();

			for (int i = 0; i < population_size; i++)
			{
				store_individual(i, i, pop, next_gen);
			}	

			generations++;
		}

		print_board(next_gen, 0);
	}

	void make_new_population()
	{
		std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator> fitnesses;

		calculate_fitnesses(fitnesses);

		vector<int> fittest_individuals_indexes;
		vector<int> fittest_individuals_ftinesses;

		store_fittest_individuals(fitnesses, fittest_individuals_indexes, fittest_individuals_ftinesses);

		if (elites > 0)
		{
			int elites_stored = 0;
			int next_indiv = 0;

			while(elites_stored < elites)
			{
				bool elite_exists = false;

				if (elites_stored > 0)
				{
					for (int stored = 0; stored < elites_stored; stored++)
					{
						if (same_boards(stored, fittest_individuals_indexes[next_indiv], next_gen, pop))
						{
							elite_exists = true;
							break;
						}
					}
				}
				
				if (!elite_exists)
				{
					for (int row = 0; row < sudoku_size; row++)
					{
						crossOver->copy_row(row, elites_stored, fittest_individuals_indexes[next_indiv]);
					}

					elites_stored++;
				}	

				next_indiv++;
			}
		}

		if (generations % 500 == 0)
		{
			print_info();
		}

		RouletteWheel roulette = RouletteWheel(crossOver, population_size, total_fitness, fittest_individuals_ftinesses, elites);

		roulette.breed(fittest_individuals_indexes);
		//breeder->breed(fittest_individuals_indexes);
		//random_breeder->breed(fittest_individuals_indexes);
	}

	void calculate_fitnesses(std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator>& fitnesses)
	{
		// We start from the end because when we compare a -2 fitness with -2 the first will stay,
		// so if we use elitism and chose to not mutate the top x individuals
		// they will never change.
		total_fitness = 0;

		for (int i = (population_size-1); i >= 0; i--)
		{
			Fitness_index_pair* fi = new Fitness_index_pair();

			int fitness = fitness_counter.count_fitness(i, pop);
			total_fitness += fitness;

			fi->fitness = fitness;
			fi->index = i;
			
			if (fitness == 162)
			{
				std::cout << "solution is individual " + std::to_string(i) << std::endl;
				std::cout << "generation " + std::to_string(generations) << std::endl;
				solved = true;
			}

			if (fitness > best_fitness)
			{
				best_fitness = fitness;
			}
			
			fitnesses.push(fi);
		}
		
	}

	void store_fittest_individuals(std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator>& fitnesses, 
		vector<int>& fittest_individuals_indexes, vector<int>& fittest_individuals_ftinesses)
	{
		int count = 0;

		while (!fitnesses.empty())
		{
			Fitness_index_pair* fip = fitnesses.top();

			int individual_index = fitnesses.top()->index;
			fittest_individuals_indexes.push_back(individual_index);

			int individual_fitness = fitnesses.top()->fitness;
			fittest_individuals_ftinesses.push_back(individual_fitness);

			store_individual(count, individual_index, next_gen, pop);
			
			count++;

			fitnesses.pop();
			delete fip;
		}
	}

	void store_individual(int individual, int index, Population& a, Population& b)
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
		print_board(next_gen, 0);
		std::cout << "fitness 0 : " + std::to_string(fitness_counter.count_fitness(0, next_gen)) << std::endl;
		print_board(next_gen, 1);
		std::cout << "fitness 1 : " + std::to_string(fitness_counter.count_fitness(1, next_gen)) << std::endl;
		std::cout << "mutated_boards " + std::to_string(mutator->mutated_boards) << std::endl;
		std::cout << "best_fitness " + std::to_string(best_fitness) << std::endl;
		
		if (elites == 0)
		{
			std::cout << "distinct boards in the first 20 : " + std::to_string(compare_boards(20, next_gen, next_gen)) << std::endl;
			std::cout << "distinct boards in the first 100 : " + std::to_string(compare_boards(100, next_gen, next_gen)) << std::endl;
		}
		else
		{
			std::cout << "fitness of the last elite " + std::to_string(elites-1) + " : " + std::to_string(fitness_counter.count_fitness(elites-1, next_gen)) << std::endl;
			std::cout << "distinct boards in the elites " + std::to_string(elites) + " : " + std::to_string(compare_boards(elites, next_gen, next_gen)) << std::endl;
		}
		
		std::cout << "unique boards in the population "+ std::to_string(population_size)+" : " + std::to_string(compare_boards(population_size, next_gen, next_gen)) << std::endl;
		std::cout << "generation " + std::to_string(generations) << std::endl;

		double total_fitness = 0;
		int fitness160 = 0;
		int fitness159 = 0;
		int fitness158 = 0;
		int individuals_to_print = 20;

		for (int i = 0; i < population_size; i++)
		{
			int fitness = fitness_counter.count_fitness(i, next_gen);

			if (fitness == 160)
			{
				fitness160++;
			}

			if (fitness == 159)
			{
				fitness159++;
			}

			if (fitness == 158)
			{
				fitness158++;
			}

			total_fitness += fitness;

			if (i < individuals_to_print)
			{
				std::cout << " ind " + std::to_string(i) + " : " + std::to_string(fitness);
			}
			
		}
		std::cout << " " << std::endl;
		std::cout << "fitness 160 indivs : " + std::to_string(fitness160) << std::endl;
		std::cout << "fitness 159 indivs : " + std::to_string(fitness159) << std::endl;
		std::cout << "fitness 158 indivs : " + std::to_string(fitness158) << std::endl;
	
		double avg_fitness = total_fitness / (double)population_size;

		if (avg_fitness > best_avg_fitness)
		{
			best_avg_fitness = avg_fitness;
		}

		std::cout << "avg_fitness " + std::to_string(avg_fitness) << std::endl;
		std::cout << "best_avg_fitness " + std::to_string(best_avg_fitness) << std::endl;
		std::cout << "" << std::endl;
	}

	int compare_boards(int comparisons, Population& pop_a, Population& pop_b)
	{
		int distinct_boards = comparisons;
		vector<int> skip_boards;

		for (int i = 0; i < comparisons-1; i++)
		{
			if (std::find(skip_boards.begin(), skip_boards.end(), i) != skip_boards.end()) {
				continue;
			}

			for (int j = i+1; j < comparisons; j++)
			{
				if (same_boards(i, j, pop_a, pop_b))
				{
					distinct_boards--;
					skip_boards.push_back(j);
				}
			}
		}

		return distinct_boards;
	}

	bool same_boards(int a, int b, Population& pop_a, Population& pop_b)
	{
		for (int row = 0; row < sudoku_size; row++)
		{
			for (int col = 0; col < sudoku_size; col++)
			{
				int value_a = pop_a.population[a][row][col]->value;
				int value_b = pop_b.population[b][row][col]->value;

				if (value_a != value_b)
				{
					return false;
				}
			}
		}

		return true;
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