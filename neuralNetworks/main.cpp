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
/*    arma::vec A;
    A.set_size(3);
    A.fill(5);
    A.print();
    A += A + 1;
    A.print();
 */
    
    srand(time(NULL));
    Data mainData("Skin_NonSkin.txt");
    split(5,95*8,0,mainData);
    Data train("train.txt"), test("test.txt"), crossVal("crossVal.txt");  //creates data objects for training, testing and cross val, split will create needed files.
    normStats * normChange = normalize(train);
    alter(test,crossVal,normChange);
    Data normTrain("NormTrain.txt"), normTest("NormTest.txt"), normCrossVal("normCrossVal.txt");
    
    std::cout<<train.getQuestions()[0].size()-1 <<std::endl;
    
    Neural_Network_t network(normTrain.getQuestions()[0].size()-1,1,1,.01);
    network.showNetworkInfo();
    network.buildNetwork(train,10000,1,5,0);
    network.showNetworkInfo();
    std::vector<std::vector<double> > inputs = normTest.getQuestions();
    std::vector<std::vector<double> > answers = normTest.getAnswers();
    std::vector<double> tempInput;
    std::vector<double> tempOutput;
    for(int i = 0; i < 10; i++){
        tempInput = inputs[i];
        tempInput.erase(tempInput.begin());
        tempOutput = network.sendData(tempInput);
        std::cout<< "For inputs (" << tempInput[0] << "," << tempInput[1] << "), the estimated value is " << tempOutput[0] << " and the real value is " << answers[i][0] << ".\n";
    }
    for(int i = 0; i < 0; i++){
        tempInput = inputs[i];
        tempInput.erase(tempInput.begin());
        tempOutput = network.sendData(tempInput);
    std::cout << tempInput[0] << "," <<  tempInput[1] << "," << tempOutput[0] << "," << answers[i][0] << "\n";
    }
    
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
