
//
//  neuralNetwork.h
//  
//
//  Created by Gableman, Michael on 2/2/16.
//  Added helper functions for back propagation Carl M. 4/19/16
//

#ifndef Neural_Network_t_hpp
#define Neural_Network_t_hpp

#include <vector>
#include <armadillo>
#include "data.h"

struct Neural_Network_Layer_t{
    int number_of_nodes;
    int nodes_in_sending_layer;
    arma::mat weights;    //matrix of weights
    arma::mat deltas;     //matrix of deltas
    arma::vec values;     //vector of inputs
    arma::vec singleDelta;//vector of deltas
    
    Neural_Network_Layer_t(int nodes_ahead, int nodes_in_layer);
    void print();
    
public:
    void evalf();
};

class Neural_Network_t
{
private:
    int number_of_inputs;
    int number_of_layers;
    int number_of_outputs;
    std::vector<Neural_Network_Layer_t * > layers;
    bool forwardPropigation(std::vector<double> inputs);
    bool backPropigation(std::vector< std::vector<double> > inputs, std::vector< std::vector<double> > answers);
    bool getDeltas(std::vector<double> answers);
    arma::vec finalValues;
    
public:
    Neural_Network_t(int inputs, int layers, int outputs);
    std::vector<double> sendData(std::vector<double> inputs);
    void buildNetwork(Data training, int number_runs);
    void showNetworkInfo();
};

#endif /* Expansion_hpp */
