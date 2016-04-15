#ifndef data_hpp
#define data_hpp


#include <fstream>
#include <vector>
#include <string>
#include <iostream>

typedef std::vector <double> record_t;
typedef std::vector <record_t> data_t;

class Data
{
public:
  std::vector<std::vector<double> > dataSet;

  std::vector< std::vector<double> > getQuestions(); //this will return a vector containing all of the questions
  std::vector<double> getQuestion(int ARG); //this will return a vector, but only containing one entry of questions
  std::vector< std::vector<double> > getAnswers(); //similar to question, but with answers
  std::vector<double> getAnswer(int ARG); //see above
  std::vector< std::vector<double> > getBoth(); //gets everything
  bool readInFile(std::string TARGET); //read in a file from location given by TARGET returning true indicates everything worked
  bool acceptEntry(std::vector<double>); //sticks another entry into this data set
  Data(); //simply creates the vector o' vectors
  Data(std::string TARGET); //fills the dataSet with info from the file at TARGET - TARGET needs to be a .csv file right meow

};
  
#endif /* data_hpp */