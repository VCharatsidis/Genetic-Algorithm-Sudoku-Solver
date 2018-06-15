# include "MateStrategy.cpp"

class AlmostRandom : public MateStrategy
{
public:
	CrossOver* crossOver;
	int population_size;
	int total_breeders;
	bool elitism;

	AlmostRandom(CrossOver* co, int total_breeders, int pop_size, bool elitism)
		: total_breeders(total_breeders), population_size(pop_size), crossOver(co), elitism(elitism) {};

	void breed(vector<int> fittest_individuals_indexes) 
	{
		int total_children = 0;
		//int max_children_per_breeder = (population_size / total_breeders);

		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> population(0, population_size - 1);
		std::uniform_int_distribution<> breeders(0, total_breeders - 1);

		if (elitism)
		{
			for (int i = 0; i < 20; i++)
			{
				crossOver->copy_board(i, fittest_individuals_indexes[i], fittest_individuals_indexes[i]);
				total_children++;
			}
		}

		while (total_children < population_size)
		{
			int parent_a = breeders(eng);
			int parent_b = population(eng);

			crossOver->cross_over(fittest_individuals_indexes[parent_a], fittest_individuals_indexes[parent_b], total_children);
			total_children++;
		}
		
	}
};