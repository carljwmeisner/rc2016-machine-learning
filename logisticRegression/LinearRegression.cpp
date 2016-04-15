//
//  LinearRegression.cpp
//  
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
    cout << endl << sizeOfQuestion << endl;
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
    sum = 1/(1+exp(-sum));
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
    double temp;
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
        real = 1/(1+exp(-real));
        if(ans[j][0] == 1){
            temp = -log10(real);
            error += -log10(real);  //calc error, based off expected value
        }
        else{
            temp = -log10(1-real);
            error += -log10(1-real);
        }
        if(showPrints)
        {
            cout << ans[j][0] << " - " << real << " - " << temp << " - ";
            if(real >= .5)
            {
                cout <<  1 << endl;
            }
            else{
                cout << 0 << endl;
            }
        }
    }
    error = error/(ans.size());  //find final error
    return error;
}
// returns vector of from TP FP TN FN
int * LinearRegression::getAccInfo()
{
    int * results = new int[4];
    results[0] = 0;
    results[1] = 0;
    results[2] = 0;
    results[3] = 0;
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
        }
        real = 1/(1+exp(-real));
        if(real >= .5){
            if(ans[j][0] == 1){
                results[0]++;
            }
            else{
                results[1]++;
            }
        }
        else{
            if(ans[j][0] == 0){
                results[2]++;
            }
            else{
                results[3]++;
            }
        }
    }
    cout << "True Positive: " << results[0] << endl;
    cout << "False Positive: " << results[1] << endl;
    cout << "True Negative: " << results[2] << endl;
    cout << "False Negative: " << results[3] << endl;
    cout << ans.size() << endl;
    return results;
}


string LinearRegression::returnStringHypothesis()
{
    string nothing = "nothing";
    cout << "You returned the hypothesis in string form!\n";
    return nothing;
}