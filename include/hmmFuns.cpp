//
//  hmmFuns.cpp
//  xRue
//
//  Created by Adam Willats on 7/19/17.
//  Copyright © 2017 Adam Willats. All rights reserved.
//

#include "hmmFuns.hpp" //not needed here?
//#include <iostream>
//#include <vector>
using namespace std;

vector<int> genHMM(vector<double> frs, vector<double> trs, int numSteps){
    vector<int> states(numSteps);
    vector<int> spikes(numSteps);
    
    states[0] = 0;
    
    //intialize random number generator

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> unif(0, 1);



    for (int i=0; i<numSteps; i++){
        
        //get emiss
        //get transit
        
        //spit spikes
//	double r1 = rand()/double(RAND_MAX);

        spikes[i] = (unif(gen) < frs[states[i]]) ? 1 : 0;


        //update future state from current transition probability
	//NB: this clause is not necessary outside rtxi...
	if (i<(numSteps-1))
	{
        	states[i+1] = (unif(gen) < trs[states[i]]) ? 1-states[i] : states[i];
	}
	else
	{
		spikes[0]=i;
	}
    }

    
    return spikes;
}
