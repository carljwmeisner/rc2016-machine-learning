//
//  Expansion.cpp
//  
//
//  Created by Gableman, Michael on 2/2/16.
//
//


#include<iostream>
#include<vector>

#include "Expansion.h"
#include "data.h"

Expansion::Expansion(Data input)
{
    std::vector<std::vector<double> > temp = input.getBoth();
    std::vector<double> singleEntry;
    singleEntry.clear();
    for(int i = 0; i < temp.size(); i++)
    {
        singleEntry.clear();
        for(int j = 0; j < temp[i].size()-1; j++)
        {
            singleEntry.push_back(temp[i][j]);
        }
        question.push_back(singleEntry);
        answer.push_back(temp[i].back());
    }
}

//Data * Expansion::expandData(int power)
//{
//    if(alreadyExpanded == false)
//    {
//        cout << "You expanded the data to a power of "
//         << power << "!\n";
//        alreadyExpanded = true;
//        vector<vector<double> > expandedData;
//        vector<double> singleEntry;
//        for(int i = 0; i < expandMe.size(); i++)
//        {
//            singleEntry.clear();
//            singleEntry.push_back(expandMe->getQuestion(i));
//            for(int j = 1; j <= power; j++)
//            {
//                singleEntry.push_back(singleEntry[0]*singleEntry[j-1])
//            }
//            expandedData.push_back(singleEntry);
//        }
//    }
//    else
//    {
//        cout << "Data already has been expanded.\n";
//    }
//    return expandMe;
//}

void Expansion::showData()
{
    for(int i = 0; i < question.size(); i++)
    {
        for(int j = 0; j < question[i].size(); j++)
        {
            std::cout << question[i][j] << " ";
        }
        std::cout << " : " << answer[i];
        std::cout << std::endl;
    }
}