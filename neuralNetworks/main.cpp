//
//  main.cpp
//  
//
//  Created by Gableman, Michael on 4/13/16.
//
//

#include<iostream>
#include<vector>
#include "neuralNetwork.h"
using namespace std;

int main(){
    cout << "Hello, World!\n";
    vector<double> oneInput;
    vector<vector<double> > allInput;
    Neural_Network_t test(3,1,5);
    
    oneInput.push_back(5.2);
    oneInput.push_back(10);
    oneInput.push_back(-1);
//    allInput.push_back(oneInput);
    
    test.showNetworkInfo();
    test.sendData(oneInput);
//    test.buildNetwork(allInput,5);
    
    return 0;
}
