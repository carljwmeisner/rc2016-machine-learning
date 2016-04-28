
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "data.h"

typedef std::vector <double> record_t;
typedef std::vector <record_t> data_t;

std::istream& operator >> ( std::istream& ins, record_t& record)
{
  record.clear();
  std::string line;
  getline(ins, line );
  std::stringstream ss( line );
  std::string field;
  while(getline( ss, field,'\t'))
  //  getline(ss,field,',');
    {
      std::stringstream fs( field );
      double f = 0.0;
      fs >> f;

      record.push_back(f);
    }
  return ins;
}

std::istream& operator >> (std::istream& ins, data_t& data)
{
  data.clear();
  record_t record;
  while(ins >> record)
    {
      data.push_back( record );
    }
  return ins;
}

std::vector< std::vector<double> > Data::getQuestions() //this will return a vector containing all of the questions
{
  std::vector< std::vector<double> > returnThis;
  int i = 0;
  for(std::vector< std::vector<double> >::iterator it = dataSet.begin(); it != dataSet.end(); ++it){
    returnThis.push_back(getQuestion(i));
    ++i;
  }
  return returnThis;
}
std::vector<double> Data::getQuestion(int ARG) //this will return a vector, but only containing one entry of questions
{
  std::vector<double> returnThis;
  returnThis = dataSet[ARG];
  returnThis.pop_back(); //we don't want the answer included
  return returnThis;
}
std::vector< std::vector<double> > Data::getAnswers() //similar to question, but with answers
{
  std::vector< std::vector<double> > returnThis;
    int i=0;
    for(std::vector< std::vector<double> >::iterator it = dataSet.begin(); it !=dataSet.end(); ++it){
      returnThis.push_back(getAnswer(i));
      ++i;
    }
    return returnThis;
}
std::vector<double> Data::getAnswer(int ARG) //see above
{
  std::vector<double> returnThis;
  returnThis.push_back(dataSet[ARG].back());
  return returnThis;
  }

std::vector< std::vector<double> > Data::getBoth() //gets everything
{
  return dataSet;
}

bool Data::readInFile(std::string TARGET)//read in a file from location given by TARGET returning true indicates everything worked
{
  
  std::fstream fs;
  fs.open(TARGET.c_str());
  if(!fs.is_open()){
    //if we can't open the file, we're S.O.L., and we're going to print an error and crash
    std::cerr << "File open fail\n";
    exit(1);
  }

  fs >> dataSet;
  //std::cout << dataSet.size() << std::endl;
  /*std::string tmpLine, tmpString;
    std::vector<double> tmpVector;
    while(std::getline(fs, tmpLine))
    {
    std::cout<<tmpLine <<std::endl;
    //each line will be a new vector added to dataSet.
    int pos;
    //while(tmpLine.find(",") != -1){
    pos = tmpLine.find(",");
    tmpString = tmpLine.substr(0,pos);
    std::cout<<tmpString<<std::endl;//this tests to see what we toss in
    tmpVector.push_back(atof(tmpString.c_str()));
    dataSet.push_back(tmpVector);
    tmpVector.clear();
    tmpString.erase(0,pos+1);
    //	}
    }*/
}

bool Data::acceptEntry(std::vector<double> insert) //sticks another entry into this data set
{ 
  dataSet.push_back(insert);
  return true;
}

Data::Data()
{
  int nothing;
}
//simply creates the vector o' vectors
Data::Data(std::string TARGET) //fills the dataSet with info from the file at TARGET - TARGET needs to be a .csv file right meow
{
  readInFile(TARGET);
}
  

  
