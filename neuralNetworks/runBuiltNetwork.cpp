//
//  main.cpp
//  
//
//  Created by Gableman, Michael on 4/13/16.
//
//

#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include "neuralNetwork.h"
#include "data.h"
using namespace std;


struct normStats{
    double avgValues;
    double maxSize;
    
    normStats(double avg = 0, double max = 1)
    {
        avgValues = avg;
        maxSize = max;
    }
};

void split(int,int,int,Data);  //takes ratio to split data between train, test, cross Val, and takes Data to be split.  Will create files containing broken up data called test.txt, train.txt, and crossVal.txt.  Also, will add ones in front of data (needed for LinearRegression class)

normStats* normalize(Data);

vector<double> addAlter(normStats*, vector<double>);

void alter(Data,Data,normStats*);

int main(){
    srand(time(NULL));
    Data mainData("Skin_NonSkin.txt");
    //split(5,95*8,0,mainData);
    Data train("train.txt"), test("test.txt"), crossVal("crossVal.txt");  //creates data objects for training, testing and cross val, split will create needed files.
    //normStats * normChange = normalize(train);
    //alter(test,crossVal,normChange);
    Data normTrain("NormTrain.txt"), normTest("NormTest.txt"), normCrossVal("normCrossVal.txt");
    
    
    
    Neural_Network_t network(3,1,1,.01);
    std::vector<arma::mat> sendMe;
    arma::mat weightsOne;
    arma::mat weightsTwo;
    weightsOne.set_size(4,3);
    
    weightsOne << 0.5673 <<   0.7447 << -4.3408 << arma::endr
    << -2.3698  << 0.8888 << -3.6324 << arma::endr
    << -1.1970  << 0.7179 << -1.0472 << arma::endr
    <<1.2679  << 1.1514 <<  3.8023 << arma::endr;
    weightsOne.print();
    
    weightsTwo.set_size(4,1);
    weightsTwo <<  4.7375 << arma::endr
    << 4.7359 << arma::endr
    << 1.7817 << arma::endr
    << -8.6897 << arma::endr;
    weightsTwo.print();
    
    /*
    weightsOne << 0.8326 << -1.1673  << 0.0933 << arma::endr
    << 2.8074 << -1.2232 << -1.2314 << arma::endr
    << 0.8194 << -0.8608 << -0.4006 << arma::endr
    << -3.0406 << -0.9780 <<  0.4412 << arma::endr;
    weightsOne.print();
    
    weightsTwo.set_size(4,1);
    weightsTwo <<  -1.2931 << arma::endr
    << 5.7940 << arma::endr
    << -0.6751 << arma::endr
    << 1.8217 << arma::endr;
    weightsTwo.print();
*/
    
 /*   weightsOne << -0.0153 << -0.2908 <<  0.2523 << arma::endr
    << -0.1914 << -1.3957 <<  0.7390 << arma::endr
    << -0.1803 << -0.1742  << 0.5404 << arma::endr
    << -0.0314  << 1.3114  << 0.3609 << arma::endr;
    weightsOne.print();
    
    weightsTwo.set_size(4,1);
    weightsTwo <<  1.7554 << arma::endr
    << 0.2563 << arma::endr
    << -2.2630 << arma::endr
    << 0.4760 << arma::endr;
    weightsTwo.print();
*/
    
    sendMe.push_back(weightsOne);
    sendMe.push_back(weightsTwo);
    network.setWeights(sendMe);
    network.showNetworkInfo();
    
    std::vector<std::vector<double> > inputs = normTrain.getQuestions();
    std::vector<std::vector<double> > answers = normTrain.getAnswers();
    for(int i = 0; i < inputs.size(); i++)
        inputs[i].erase(inputs[i].begin());
    std::vector<double> tempInput;
    std::vector<double> tempOutput;
    
    network.getError(inputs,answers);
    network.showNetworkInfo();
    return 0;
}

void split(int p_train,int p_test,int p_crossVal,Data original)
{
    int total_stride = p_train + p_test + p_crossVal;
    
    vector<vector<double> > information = original.getBoth();
    ofstream ftrain, ftest, fcrossVal;
    ftrain.open("train.txt");
    ftest.open("test.txt");
    fcrossVal.open("crossVal.txt");
    if(!ftrain.is_open() || !ftest.is_open() || !fcrossVal.is_open())
    {
        cout << "Error in opening file in split function.\n";
        exit(1);
    }
    for(int i = 0; i < information.size(); i++)
    {
        if(i%total_stride < p_train)
        {
            ftrain << 1 << "\t";
            for(int j = 0; j < information[i].size(); j++)
            {
                ftrain << information[i][j] << "\t";
            }
            ftrain << endl;
        }
        else if(i%total_stride < p_test + p_train)
        {
            ftest << 1 << "\t";
            for(int j = 0; j < information[i].size(); j++)
            {
                ftest << information[i][j] << "\t";
            }
            ftest << endl;
        }
        else
        {
            fcrossVal << 1 << "\t";
            for(int j = 0; j < information[i].size(); j++)
            {
                fcrossVal << information[i][j] << "\t";
            }
            fcrossVal << endl;
        }
    }
    ftrain.close();
    ftest.close();
    fcrossVal.close();
}

normStats* normalize(Data original)
{
    
    vector<vector<double> > information = original.getQuestions();
    vector<vector<double> > answers = original.getAnswers();
    vector<double> avgValues;
    vector<double> maxSize;
    ofstream fNorm, fStats;
    fNorm.open("NormTrain.txt");
    fStats.open("NormStats.txt");
    if(!fNorm.is_open() || !fStats.is_open())
    {
        cout << "Error in opening file in normalize function.\n";
        exit(1);
    }
    for(int i = 0; i < information[0].size(); i++)
    {
        avgValues.push_back(0.0);
        maxSize.push_back(information[0][i]);
    }
    for(int i = 0; i < information.size(); i++)
    {
        for(int j = 1; j < information[i].size(); j++)
        {
            avgValues[j] += information[i][j];
            if(information[i][j] > maxSize[j])
            {
                maxSize[j] = information[i][j];
            }
        }
    }
    for(int i = 1; i < avgValues.size(); i++)
    {
        avgValues[i] = avgValues[i]/information.size();
    }
    for(int i = 0; i < information.size(); i++)
    {
        for(int j = 1; j < information[i].size(); j++)
        {
            information[i][j] = information[i][j] - avgValues[j];
            information[i][j] = information[i][j]/maxSize[j];
        }
    }
    for(int i = 0; i < information.size(); i++)
    {
        for(int j = 0; j < information[i].size(); j++)
        {
            fNorm << information[i][j] << "\t";
        }
        fNorm << answers[i][0];
        fNorm << endl;
    }
    for(int i = 0; i < avgValues.size(); i++)
    {
        fStats << avgValues[i] << "\t";
    }
    fStats << endl;
    for(int i = 0; i < maxSize.size(); i++)
    {
        fStats << maxSize[i] << "\t";
    }
    fStats << endl;
    fStats.close();
    fNorm.close();
    normStats* results = new normStats[avgValues.size()];
    for(int i = 0; i < avgValues.size(); i++)
    {
        results[i].maxSize = maxSize[i];
        results[i].avgValues = avgValues[i];
    }
    
    return results;
}

void alter(Data questions, Data crossVal,normStats* alterations)
{
    vector<vector<double> > questionInfo = questions.getBoth();
    vector<vector<double> > crossValInfo = crossVal.getBoth();
    double singleStat;
    ofstream ftest, fcrossVal;
    ftest.open("NormTest.txt");
    fcrossVal.open("NormCrossVal.txt");
    if(!ftest.is_open() || !fcrossVal.is_open())
    {
        cout << "Error in opening file in alter function.\n";
        exit(1);
    }
    for(int i = 0; i < questionInfo.size(); i++)
    {
        for(int j = 0; j < questionInfo[i].size()-1; j++)
        {
            singleStat = questionInfo[i][j];
            singleStat = (singleStat-alterations[j].avgValues)/alterations[j].maxSize;
            ftest << singleStat << "\t";
        }
        ftest << questionInfo[i][questionInfo[0].size()-1] << endl;
    }
    for(int i = 0; i < crossValInfo.size(); i++)
    {
        for(int j = 0; j < crossValInfo[i].size()-1; j++)
        {
            singleStat = crossValInfo[i][j];
            singleStat = (singleStat-alterations[j].avgValues)/alterations[j].maxSize;
            ftest << singleStat << "\t";
        }
        ftest << crossValInfo[i][crossValInfo[0].size()-1] << endl;
    }
    
    
    ftest.close();
    fcrossVal.close();
}

vector<double> addAlter(normStats* changes, vector<double> answers){
    double constantAlters = 0.0;
    vector<double> newAnswers;
    for(int i = 0; i < answers.size(); i++)
    {
        newAnswers.push_back(answers[i]/changes[i].maxSize);
        constantAlters += changes[i].avgValues/changes[i].maxSize;
    }
    newAnswers[0] = constantAlters;
    return newAnswers;
}
