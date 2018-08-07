#include "MateStrategy.cpp"
#include <cmath> 

class RouletteWheel
{
public:
	CrossOver* crossOver;
	int population_size;
	double total_fitness;
	vector<int> fitnesses;
	int elites;

	RouletteWheel(CrossOver* co, int pop_size, double total_fitness, vector<int> fitnesses, int elites)
		:  population_size(pop_size), crossOver(co), total_fitness(total_fitness), fitnesses(fitnesses), elites(elites) {};

	void breed(vector<int> fittest_individuals_indexes)
	{
		int total_children = elites;

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

		int selection_pressure = 110;
		double value = unif(eng) * (total_fitness-(population_size * selection_pressure));

		while (value >= 0)
		{
			for (int i = 0; i < population_size; i++)
			{
				value -= (fitnesses[i] - selection_pressure);

				if (value < 0)
				{
					return i;
				}
			}
		}
		
		return 0;
	}
};