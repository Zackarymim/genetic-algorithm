#include "pkg.h"


int main(){

    /*PARAMETRE DE L'ALGORITHME*/

    /*BEGIN*/
    vector<double> params{ 1, 1, 1, 1};

    int nbr_individus = 10;

    double mutation = 0.2;

    int Generations = 5;

    /*END*/

    vector < vector<double> > population = create_population(nbr_individus,3),children;

    

    calculate_fitness(population,params);

    calculate_probs(population);

    print(population);

    population = select_mating_pool(population,nbr_individus/2,true);

    print(population);

    children = crossover(population,nbr_individus/2);

    calculate_fitness(children,params);

    calculate_probs(children);

    print(children);

    Random_Reset_Mutation(children,mutation);

    calculate_fitness(children,params);

    calculate_probs(children);

    print(children);

    population.insert(population.end(),children.begin(),children.end());

    calculate_fitness(population,params);

    calculate_probs(population);

    print(population);



    

    return 0;
}