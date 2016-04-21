//
//  Expansion.hpp
//  
//
//  Created by Gableman, Michael on 2/2/16.
//
//

#ifndef Expansion_hpp
#define Expansion_hpp

#include <stdio.h>
#include <vector>
#include "data.h"

class Expansion
{
private:
    std::vector<std::vector<double> > question;
    std::vector<double> answer;
    bool alreadyExpanded;
public:
    Expansion(Data);  //reads in Data to question and answer (placing correct values in correct vectors) sets already Expanded to false.
    //Data * expandData(int power = 1);  //work in progress... does not work... yet
    void showData();  //Prints out data stored
};

#endif /* Expansion_hpp */
