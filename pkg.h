#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "TextTable.h"
#include <bits/stdc++.h>

using namespace std;

/* PARAMETRES DE PROBLEME */
/*BEGIN*/

int Genes = 3;

int MIN = 0, MAX=15;

/*END*/

int calculate_obj(vector<double> v){
	int i;
	int res = 1;
	for(i=0;i<v.size()-3;i++){
		res *= v[i];
	}
	return res;
}

void best_fitness(vector< vector<double> > arr){
	vector<double> sol = arr[0];

	TextTable t( '-', '|', '+' );

	t.add( "x" );t.add( "y" );t.add( "z" );t.add( "x*y*z" );t.add( "x+y+z=36" );t.add( "x<=y" );t.add( "z<10" );
	t.endOfRow();

	double prod = arr[0][0] * arr[0][1] * arr[0][2];

	t.add( to_string(arr[0][0]) );t.add( to_string(arr[0][1]) );t.add( to_string(arr[0][2]) );t.add( to_string(prod) );

	if(arr[0][0] + arr[0][1] + arr[0][2] == 36)	
		t.add( "Respected!" );
	else
		t.add( "Not Respected!" );
	
	if(arr[0][0] <= arr[0][1])	
		t.add( "Respected!" );
	else
		t.add( "Not Respected!" );
	
	if(arr[0][2] < 10)	
		t.add( "Respected!" );
	else
		t.add( "Not Respected!" );

	t.endOfRow();

	std::cout << t <<endl;



}

bool diff( const vector<double>& v1, const vector<double>& v2 ) { 
    return v1[3] > v2[3]; 
} 

void print(vector< vector<double> > arr){

	TextTable t( '-', '|', '+' );

	t.add( "x" );t.add( "y" );t.add( "z" );t.add( "fitness" );t.add( "proba" );t.add( "proba cumule" );t.endOfRow();

    int i,j;
    for(i=0 ; i<arr.size() ; i++){
			for(j=0 ; j<arr[i].size() ; j++){
				t.add( to_string( arr[i][j]) );
			}
			t.endOfRow();
			cout<<endl;
		}

	t.setAlignment( 6, TextTable::Alignment::RIGHT );
    std::cout << t;

    cout<<endl;
}

void printd(vector<double> arr){
    cout<<endl;

	TextTable t( '-', '|', '+' );

    int i;
    for(i=0 ; i<arr.size() ; i++){	
		t.add( to_string( arr[i]) );
	}

	
    std::cout << t;

    cout<<endl;
}



vector< vector<double> > create_population(int indv,int genes){
	int i,j,k;
	

	std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(MIN,MAX);
	
	vector< vector<double> > arr(indv,vector<double>(genes+3));
	
	for(j=0 ; j<indv ; j++){
		for(k=0 ; k<genes ; k++){
			arr[j][k] = distribution(generator);
		}

        arr[j][genes] = 0;
		arr[j][genes+1] = 0;
		arr[j][genes+2] = 0;
	}
	
	
	
	return arr;
}

vector<double> calculate_fitness(vector< vector<double> >& population, vector<double> params, int Generation){

    if (population[0].size() != 6){
        cout<<"Le changement de nombre des genes entraine la redefinition de la fonction de fitness";
        exit(0);
    }
	int i,j,k;
    double constraint1,constraint3,constraint4,constraint2,f;

	vector<double> fitness(population.size());

	for(i=0 ; i<population.size() ; i++){

		constraint1 = population[i][0] * population[i][1] * population[i][2];
        constraint2 = -fabs((population[i][0] + population[i][1] + population[i][2]) - 36);
        constraint3 = population[i][1] >= population[i][0]? 1:0;
        constraint4 = 10 - population[i][2] > 0? 1:0;


		f = params[0] * constraint1 + params[1] * Generation * constraint2 + params[2] * Generation * constraint3 + params[3] * Generation * constraint4;
		



		if (f >= 0)
        	fitness[i] = f;
		else{
			fitness[i] = 0;
			cout<<"zero in"<<(i+1)<<endl;
		}
        population[i][population[0].size()-3] = fitness[i];
	}

	
	return fitness;
}

vector<double> calculate_probs(vector< vector<double> >& population){
	int i,j,k;

	sort(population.begin(), population.end(), diff);

	for(j=0 ; j<population.size() ; j++){

        population[j][population[0].size()-1] = 0;
		population[j][population[0].size()-2] = 0;
	}

	double total_fitness = 0;

	vector<double> probs(population.size());


	for(i=0 ; i<population.size() ; i++){
		total_fitness += population[i][population[0].size()-3];
	}
	
	for(i=0 ; i<population.size() ; i++){
		population[i][population[0].size()-2] = (double)population[i][population[0].size()-3] / (double)total_fitness;
		probs[i] = (double)population[i][population[0].size()-3] / (double)total_fitness;
	}

	population[0][population[0].size()-1] = population[0][population[0].size()-2];
	
	for(i=1 ; i<population.size() ; i++){
		for(j=i;j>=0;j--){
			population[i][population[0].size()-1] += population[j][population[0].size()-2];
		}
	}
	
	return probs;
}



vector< vector<double> > select_mating_pool(vector< vector<double> >& population, int parents, bool Elitism){
	int i;
	double prob;
	std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(0,1);
	
	vector< vector<double> > arr;

	if(Elitism){
		cout<<"Elitism:ON"<<endl;
		arr.insert(arr.end(), population[0]);
		population.erase(population.begin());
	}
	else{
		cout<<"Elitism:OFF"<<endl;
	}
	
	
	while(arr.size() < parents){
		prob = distribution(generator);
		for(i=0;i<population.size();i++){
			if(prob <= population[i][population[0].size()-1]){
				arr.insert(arr.end(),population[i]);
				population.erase(population.begin() + i);
				calculate_probs(population);
				i = population.size();
			}
		}
		
	}
	
	return arr;
}

vector< vector<double> > one_point_crossover(vector< vector<double> > population, int children){
	int i,j,crossover_point;
	int parent1,parent2;
	
	std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(0,Genes-1);
	
	vector< vector<double> > arr;
	vector<double> temp;

	i=0;
	while(arr.size() < children){
		crossover_point = distribution(generator);

		parent1 = i%(population.size());
		parent2 = (i+1)%(population.size());

		

		for(j=0;j<crossover_point;j++){
			temp.insert(temp.end(),population[parent1][j]);
		}

		for(j=crossover_point;j<Genes;j++){
			temp.insert(temp.end(),population[parent2][j]);
		}

		temp.insert(temp.end(),0);
		temp.insert(temp.end(),0);
		temp.insert(temp.end(),0);

		arr.insert(arr.end(),temp);

		temp.clear();

		/*for(j=0;j<crossover_point;j++){
			temp.insert(temp.end(),population[parent2][j]);
		}

		for(j=crossover_point;j<population[0].size();j++){
			temp.insert(temp.end(),population[parent1][j]);
		}

		temp.clear();*/


		i++;
		
		
	}
	
	return arr;
}


void Random_Reset_Mutation(vector< vector<double> >& children, double mutation){
	int i,j,pos;
	double prob;
	vector< vector<double> > arr;

	std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(0,1);

	std::random_device rd2;
    std::default_random_engine generator2(rd2());
    std::uniform_int_distribution<int> distribution2(MIN,MAX);

	std::random_device rd3;
    std::default_random_engine generator3(rd3());
    std::uniform_int_distribution<int> distribution3(0,Genes-1);

	for(i=0;i<children.size();i++){
		prob = distribution(generator);
		if(prob<mutation){
			pos = distribution3(generator3);
			children[i][pos] = distribution2(generator2);
		}

	}

}


vector< vector<double> > weighted_avg_crossover(vector< vector<double> > population, int children){
	int i,j;
	int parent1,parent2;

	double proba;
	
	std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(0,1);
	
	vector< vector<double> > arr;
	vector<double> temp;

	i=0;
	while(arr.size() < children){

		proba = distribution(generator);

		parent1 = i%(population.size());
		parent2 = (i+1)%(population.size());

		for(j=0;j<Genes;j++){
			temp.insert(temp.end(),floor((proba * population[parent1][j] + (1 - proba) * population[parent2][j])));
		}

		temp.insert(temp.end(),0);
		temp.insert(temp.end(),0);
		temp.insert(temp.end(),0);

		arr.insert(arr.end(),temp);

		temp.clear();

		i++;
		
		
	}
	
	return arr;
}

