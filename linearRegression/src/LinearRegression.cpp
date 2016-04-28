//
//  LinearRegression.cpp
//  used for linear regression?
//
//  Created by Gableman, Michael on 2/2/16.
//
//

#include "LinearRegression.h"
#include "data.h"  //change to correct data file when done

#include <stdio.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <cmath>

using namespace std;

LinearRegression::LinearRegression(Data trainingData, Data testingData, Data crossValidationData)
{
    train = trainingData;
    test = testingData;
    crossVal = crossValidationData;
    expansion = false;
}

void LinearRegression::createExpansion(int power)
{
    if(expansion == false)
    {
        cout << "You created an expansion of order " << power << "!\n";
        expansion = true;
    }
    else
    {
        cout << "Warning: expansion already exists.\n";
    }
}

vector<double> LinearRegression::createHypothesis(double growthFactor, int steps, bool showPrints)
{
    vector<double> coeffients, coeffientsTemp, singleQuestion, theta;  //theta stores the sum of the error
    double singleAnswer, sum;                                          //coeffientsTemp stores values until coeffients can be overwritten
    int sizeOfData = train.getQuestions().size();
    int sizeOfQuestion = train.getQuestion(0).size();
    //    cout << endl << sizeOfQuestion << endl;
    for(int i = 0; i < sizeOfQuestion; i++)  //loop to initilize theta, and coeffients
    {
        theta.push_back(0.0);
        coeffients.push_back(0.0);
        coeffientsTemp.push_back(0.0);
    }
    for(int numberGradSteps = 0; numberGradSteps < steps; numberGradSteps++)  //Loop used to determine number of steps for grad. descent
    {
        for(int i = 0; i < sizeOfQuestion; i++)  //loop through each var. in data entry
        {
            theta[i] = 0;
            for(int j = 0; j < sizeOfData; j++)  //Loop through each data entry
            {
                theta[i] += testSingleHypothesis(j,coeffients,false)*train.getQuestion(j)[i];  //finds hypothesis value - real value, then multiplies it by data var. in data entry value
                if(showPrints)
                    cout << testSingleHypothesis(j,coeffients,showPrints) << " * " << train.getQuestion(j)[i] << " := " << theta[i] << endl;
            }
            if(showPrints)
	      cout << endl;
            theta[i] = theta[i]*growthFactor/sizeOfData;
            coeffientsTemp[i] = coeffients[i] - theta[i];  // finds new coeffient
        }
        for(int i = 0; i < coeffients.size(); i++)  //updates coeffients
        {
            coeffients[i] = coeffientsTemp[i];
        }
    }
    hypothosis = coeffients;
    return coeffients;
}

double LinearRegression::testSingleHypothesis(int number, vector<double> coe, bool showPrints)  //finds hypothesis - real value for a single data entry, number is case number of data being checked
{
    double sum = 0;
    vector<double> ques = train.getQuestion(number);
    for(int i = 0; i < ques.size(); i++)  //finds sum of each data entry times its corresponding coeffient
    {
        sum += coe[i]*ques[i];
        if(showPrints)
            cout << ques[i] << " ( " << coe[i] << " ) " << " : ";
    }
    if(showPrints)
        cout << train.getAnswer(number)[0];
    sum = sum - train.getAnswer(number)[0];  //subtracts expacted value
    if(showPrints)
        cout << "= " << sum <<" -----";
    return sum;
}

vector<double> LinearRegression::returnVectorHypothesis()
{
    vector<double> nothing;
    return hypothosis;
}

double LinearRegression::checkAcc(bool showPrints)
{
    vector<vector<double> > ans = test.getAnswers();
    vector<double> ques;
    double real  = 0, error = 0;
    for(int j = 0; j < ans.size(); j++)  // for each data entry
    {
        real = 0;
        ques = test.getQuestion(j);
        for(int i = 0; i < ques.size(); i++)  //find value according to hypothosis
        {
            real += hypothosis[i]*ques[i];
            if(showPrints)
                cout << ques[i] << " ( " << hypothosis[i] << " ) " << " : ";
        }
        error += (real - ans[j][0])*(real - ans[j][0]);  //calc error, based off expected value
        if(showPrints)
            cout << ans[j][0] << " - " << real-ans[j][0] << " - " << endl;
    }
    error = error/(2*ans.size());  //find final error
    return error;
}


string LinearRegression::returnStringHypothesis()
{
    string nothing = "nothing";
    cout << "You returned the hypothesis in string form!\n";
    return nothing;
}
