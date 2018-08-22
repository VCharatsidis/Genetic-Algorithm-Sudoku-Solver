# SudokuSolver-Genetic-Algorithm

A genetic algorithm sudoku solver in c++. 
Chromosome: Board full of numbers with the rows having the numbers correctly from 1-9.
            So rows are like sub-chromosomes.
            
Fitness: 162 is the best fitness, a solution. 
        For every missing value (from 1 to 9) in a column or a container box the fitness of the board is reduced by 1.
        Rows cannot have errors since they have numbers from 1-9.
        
CrossOver: The parents are choosen with roullette wheel.
           Three consecutive rows are used as sub chromosome so that the container boxes are preserved.
           There are 6 different combinations equally probable. An example is that rows from 1 to 3 and from 7 to 9 are copied in the     child  from parent A and rows from 4 to 6 are copied from parent B to the child.
        
Mutation: The mutation is done by swaping 2 elements in one of the rows. Of course i do not swap fixed elements. A good mutation rate is 15%

The easy sudoku is solved pretty fast.
The hard sudoku is not solved.
So far the best result in the hard sudoku is 2 errors. So 2 missing values in all columns and container boxes.

           
