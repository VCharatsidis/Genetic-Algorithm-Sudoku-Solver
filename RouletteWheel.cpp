#include "MateStrategy.cpp"
#include <cmath> 

class RouletteWheel
{
public:
	CrossOver* crossOver;
	int population_size;
	double total_fitness;
	vector<int> fitnesses;

	RouletteWheel(CrossOver* co, int pop_size, double total_fitness, vector<int> fitnesses)
		:  population_size(pop_size), crossOver(co), total_fitness(total_fitness), fitnesses(fitnesses) {};

	void breed(vector<int> fittest_individuals_indexes)
	{
		int total_children = 0;
		//int max_children_per_breeder = (population_size / total_breeders);

		while (total_children < population_size)
		{
			int parent_a = get_individual();
			int parent_b = get_individual();

			crossOver->cross_over(fittest_individuals_indexes[parent_a], fittest_individuals_indexes[parent_b], total_children);
			total_children++;
		}
	}

	int get_individual()
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<double> unif(0, 1);
		double value = std::abs(unif(eng) * (total_fitness));

		for (int i = 0; i < population_size; i++)
		{
			int fitness = std::abs(fitnesses[i]-2*fitnesses[499]);
			value -= fitness;

			if (value < 0)
			{
				return i;
			}
		}

		return population_size - 1;
	}
};