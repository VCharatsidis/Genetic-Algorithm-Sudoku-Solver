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
	bool solved = false;
	Board board = Board();
	int population_size = 100;
	double total_fitness_sum;
	int generations = 0;
	int best_fitness = -100;
	
	void solve() 
	{
		while (!solved) 
		{
			make_new_population();

			make_mutations();

			for (int i = 0; i < population_size; i++)
			{
				store_individual(i, i, pop, next_gen);
			}	

			if (generations % 10000 == 0) 
			{
				print(pop, 0);

				std::cout << "fitness " + std::to_string(count_fitness(0, pop)) << std::endl;
				std::cout << "best_fitness " + std::to_string(best_fitness) << std::endl;
				std::cout << "generation " + std::to_string(generations) << std::endl;
				total_fitness_sum = 0;

				for (int i = 0; i < population_size; i++)
				{
					std::cout << " ind "+std::to_string(i)+" : " + std::to_string(count_fitness(i, next_gen));
				}
				
				std::cout << " "  << std::endl;
				std::cout << "avg_fitness " + std::to_string((total_fitness_sum/(double)population_size)) << std::endl;
				std::cout << "" << std::endl;
			}

			generations++;
		}

		print(next_gen, 0);
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

	int get_value(int individual, Population& pop, int row, int column)
	{
		return pop.population[individual][row][column]->value;
	}

	int count_fitness(int individual, Population& pop)
	{
		int fitness = 0;
		
		fitness += count_column_errors(individual, pop);
		fitness += count_container_errors(individual, pop);
		
		if (fitness == 0)
		{
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
		//double percentage_of_first_parent = 0.5;

		bool top_6_rows_parent_a = false;

		if (prob > 0.3)
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

	void make_mutations()
	{
		std::random_device rd;
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

	void calculate_fitnesses(std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator>& fitnesses)
	{
		total_fitness_sum = 0;

		for (int i = 0; i < population_size; i++)
		{
			Fitness_index_pair* fi = new Fitness_index_pair();
			fi->fitness = count_fitness(i, pop);
			fi->index = i;

			fitnesses.push(fi);
		}
	}

	void calculate_fittest_individuals(std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator>& fitnesses,
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

	void make_new_population()
	{
		std::priority_queue<Fitness_index_pair*, vector<Fitness_index_pair*>, Comparator> fitnesses;

		calculate_fitnesses(fitnesses);
		
		//we keep the first 25 fittest from the last generation

		vector<int> fittest_individuals_indexes;

		/*int count = 0;

		while (!fitnesses.empty())
		{
			Fitness_index_pair* fip = fitnesses.top();

			int individual_index = fitnesses.top()->index;

			fittest_individuals_indexes.push_back(individual_index);

			store_individual(count, individual_index, next_gen, pop);
			count++;

			fitnesses.pop();
			delete fip;
		}*/
		
		calculate_fittest_individuals(fitnesses, fittest_individuals_indexes);

		//we mate the first 3 fittest with the first 25 fittest each.
		//tottaly the new population has again 100 individuals, 25 old + 75 children

		int total_children = population_size / 4; 
		int breeders = 0;
		int max_children_per_breeder = population_size / 4;
		
		for (auto const& parent_a : fittest_individuals_indexes)
		{
			int breeder_children = 0;

			for (auto const& parent_b : fittest_individuals_indexes)
			{
				if (parent_a != parent_b)
				{
					cross_over(parent_a, parent_b, total_children);
					total_children++;
					breeder_children++;

					if (breeder_children == max_children_per_breeder)
					{
						break;
					}
				}	
			}

			breeders++;

			if (total_children == population_size || breeders == 3)
			{
				break;
			}
		}
	}

	void mutate(int individual) 
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, sudoku_size - 1);

		int row = distr(eng);
		swap(row, individual);

	}

	void swap(int row, int individual)
	{
		int number_av_values = board.available_boxes[row].size();
	
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, number_av_values-1);

		int a = distr(eng);
		int b = distr(eng);

		while (a == b)
		{
			b = distr(eng);
		}

		int column_a = board.available_boxes[row][a];
		int column_b = board.available_boxes[row][b];

		int temp = get_value(individual, next_gen, row, column_a);
		next_gen.population[individual][row][column_a]->value = get_value(individual, next_gen, row, column_b);
		next_gen.population[individual][row][column_b]->value = temp;
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