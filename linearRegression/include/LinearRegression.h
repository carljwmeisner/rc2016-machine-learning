//
//  LinearRegression.hpp
//  
//
//  Created by Gableman, Michael on 2/2/16.
//
//

#ifndef LinearRegression_hpp
#define LinearRegression_hpp

#include <stdio.h>
#include <vector>
#include <cstring>
#include "data.h"  //change this to the correct data file once done


class LinearRegression
{
private:
    bool expansion;
    Data train, test, crossVal;
    std::vector<double> hypothosis;
    double testSingleHypothesis(int, std::vector<double>, bool);  //helper function for createHypothesis
    //consider adding bool to mark if hypothosis is created
    // Expansion exTrain, exCrossVal;  //will be uncommented when expansion class is added
public:
    LinearRegression(Data trainingData, Data testingData, Data crossValidationData); //sets up data that class will use
    void createExpansion(int power = 1); //does nothing...for now
    std::vector<double> createHypothesis(double growthFactor = .001, int steps = 2, bool showPrints = false); //creates hypothesis
    std::vector<double> returnVectorHypothesis();
    std::string returnStringHypothesis();  //not working
    double checkAcc(bool showPrints = false);  // prints error based on TESTING data
};

#endif /* LinearRegression_hpp */
