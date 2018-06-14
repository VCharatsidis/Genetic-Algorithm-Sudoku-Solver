
#include "MateStrategy.cpp"

using std::vector;

//we mate the first x breeders fittest with the first (population_size / x) fittest individuals each.

class Patriarch : public MateStrategy
{
public:

	CrossOver* crossOver;
	int population_size;
	int total_breeders;

	Patriarch(CrossOver* co, int total_breeders, int pop_size) 
		: total_breeders(total_breeders), population_size(pop_size), crossOver(co) {};

	void breed(vector<int> fittest_individuals_indexes)
	{
		int total_children = 0;
		int max_children_per_breeder = (population_size / total_breeders);

		for (int parent_a = 0; parent_a < total_breeders; parent_a++)
		{
			int start = parent_a + 1;
			int stop = start + max_children_per_breeder;

			for (int parent_b = start; parent_b < stop; parent_b++)
			{
				crossOver->cross_over(fittest_individuals_indexes[parent_a], fittest_individuals_indexes[parent_b], total_children);
				total_children++;
			}
		}
	}
};