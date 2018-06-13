# SudokuSolver-Genetic-Algorithm

A genetic algorithm sudoku solver in c++.

Chromosome: Board full of numbers with the rows having the numbers correctly from 1-9.
            So rows are like sub-chromosomes.
            
Population: 100 full boards
            I keep the 25 fittest individuals,
            also i make 75 offsprings by mating the first 3 fittest individuals with the first 25 individuals.

Fitness: 0 is the best fitness so a solution. 
        For every cooccurancy of a value in a column or a container box the fitness of the board is reduced by 1.
        Rows cannot have errors since they have numbers from 1-9.
        
CrossOver: I take either the first 6 rows of parent a and the last 3 rows of parent b OR
           the last 3 rows of parent a and the first 6 rows of parent b, though there is a higher chanse for the first possibility.
        
Mutation: I mutate 40 of my 100 individuals by swaping 2 elements in one of their rows. Of course i do not swap fixed elements.
           
