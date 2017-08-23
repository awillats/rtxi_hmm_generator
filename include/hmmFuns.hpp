//
//  hmmFuns.hpp
//  xRue
//
//  Created by Adam Willats on 7/19/17.
//  Copyright © 2017 Adam Willats. All rights reserved.
//

//this is the header guard which stops from being multi-included!
#ifndef hmmFuns_hpp
#define hmmFuns_hpp

#include <stdio.h>
#include <vector>
//#include <stdlib.h>
#include <random>

std::vector<int> genHMM(std::vector<double> frs, std::vector<double> trs, int numSteps);


#endif /* hmmFuns_hpp */



