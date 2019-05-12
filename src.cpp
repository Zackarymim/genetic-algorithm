#include "pkg.h"


int main(){

    /*PARAMETRE DE L'ALGORITHME*/

    /*BEGIN*/
    vector<double> params{ 10, 50, 10, 50};

    int nbr_individus = 20;

    double mutation = 0.4;

    int Generations = 10;

    /*END*/

    int i;

    vector < vector<double> > population = create_population(nbr_individus,3),children;

    for(i=0;i<Generations;i++){

        calculate_fitness(population,params);

        calculate_probs(population);

        cout<<"Generation: "<<(i+1)<<endl;

        print(population);




        population = select_mating_pool(population,nbr_individus/2,true);

        calculate_fitness(population,params);

        calculate_probs(population);

        /*cout<<"Selected Parents:"<<endl;

        print(population);*/




        children = weighted_avg_crossover(population,nbr_individus/2);

        calculate_fitness(children,params);

        calculate_probs(children);

        /*cout<<"Produced Children:"<<endl;

        print(children);*/





        Random_Reset_Mutation(children,mutation);

        calculate_fitness(children,params);

        calculate_probs(children);

        /*cout<<"Mutation Applied:"<<endl;

        print(children);*/




        population.insert(population.end(),children.begin(),children.end());

    }

    return 0;
}