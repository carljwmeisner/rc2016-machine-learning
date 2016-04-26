//
//  main.cpp
//  
//
//  Created by Gableman, Michael on 2/10/16.
//
//

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

#include "data.h"
#include "LinearRegression.h"
#include "Expansion.h"
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

int main(int argc, char **argv)
{
  string targetDataFile = "demo.txt";
  int stepSize = 1;
  int numberOfSteps = 500;
  bool verbose = false;
  int option_char;

  
  while((option_char = getopt(argc, argv, "d:s:n:v")) != -1)
    {
      switch(option_char)
	{
	case 'd': targetDataFile.assign(optarg); cout << optarg << " " << "\n"; break;
	case 's': stepSize = atoi(optarg); break;
	case 'n': numberOfSteps = atoi(optarg); break;
	case 'v': verbose = true; break;
	}
    }
  cout << targetDataFile << " " << stepSize << " " << numberOfSteps << " " << verbose << "\n";
  	
  Data mainData(targetDataFile);  //set this to file containing data
  split(7,2,1,mainData);          //select ratio of split between training:testing:crossValidation
  Data train("train.txt"), test("test.txt"), crossVal("crossVal.txt");  //creates data objects for training, testing and cross val, split will create needed files.
  
  
  normStats * normChange = normalize(train);
  alter(test,crossVal,normChange);
  Data normTrain("NormTrain.txt"), normTest("NormTest.txt"), normCrossVal("normCrossVal.txt");
  
  
  LinearRegression predictor(normTrain, normTest, normCrossVal);
  predictor.createHypothesis(stepSize,numberOfSteps,verbose);  // step size, number steps, include status print statments (output will get long if marked true)
  cout << "Error from hypothesis after run: " << predictor.checkAcc(verbose) << endl;  //include status print statements (output will get long if marked true)
  vector<double> guess = predictor.returnVectorHypothesis();
  cout << "Coeffents on vector are : ";
  for(int i = 0; i < guess.size(); i++)
    {
      cout << guess[i] << " : ";
    }
  cout << endl;
  vector<double> newGuess = addAlter(normChange,guess);
  cout << "Altered guess: ";
  for(int i = 0; i < newGuess.size(); i++)
    {
      cout << newGuess[i] << " : ";
    }
  cout << endl;
  
  
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

