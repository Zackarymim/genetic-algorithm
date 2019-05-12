#include "pkg.h"


int main(){

    /*PARAMETRE DE L'ALGORITHME*/

    /*BEGIN*/
    vector<double> params{ 1, 1, 1, 50};

    int nbr_individus = 10;

    double mutation = 0.4;

    int Generations = 10;

    /*END*/

    int i;

    vector < vector<double> > population = create_population(nbr_individus,3),children;

    for(i=0;i<Generations;i++){

        calculate_fitness(population,params,(i+1));

        calculate_probs(population);

        cout<<"Generation: "<<(i+1)<<endl;

        print(population);

        best_fitness(population);


        population = select_mating_pool(population,nbr_individus/2,false);

        calculate_fitness(population,params,(i+1));

        calculate_probs(population);

        /*cout<<"Selected Parents:"<<endl;

        print(population);*/




        children = one_point_crossover(population,nbr_individus/2);

        calculate_fitness(children,params,(i+1));

        calculate_probs(children);

        /*cout<<"Produced Children:"<<endl;

        print(children);*/





        Random_Reset_Mutation(children,mutation);

        calculate_fitness(children,params,(i+1));

        calculate_probs(children);

        /*cout<<"Mutation Applied:"<<endl;

        print(children);*/




        population.insert(population.end(),children.begin(),children.end());

    }

    return 0;
}