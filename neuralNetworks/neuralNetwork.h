
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
    
    Neural_Network_Layer_t(int nodes_ahead, int nodes_in_layer, double range = 15);
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
    double accuracy;
    std::vector<Neural_Network_Layer_t * > layers;
    bool forwardPropigation(std::vector<double> inputs);
    bool backPropigation(std::vector< std::vector<double> > inputs, std::vector< std::vector<double> > answers, double stepSize);
    bool getDeltas(std::vector<double> answers, double stepSize);
    double getOneError( std::vector< std::vector<double> > inputs, std::vector< std::vector<double> > answers);
    void buildNetworkPart(Data training, int number_runs, double stepSize);
    void restartNetwork(double range);
    arma::vec finalValues;
    
public:
    Neural_Network_t(int inputs, int layers, int outputs, double range = 1);
    std::vector<double> sendData(std::vector<double> inputs);
    
    void showNetworkInfo();
    void buildNetwork(Data training, int number_runs, int stepSize = 1 ,int number_restarts = 100, double range = 15);
    double getError( std::vector< std::vector<double> > inputs, std::vector< std::vector<double> > answers);
    void setWeights(std::vector<arma::mat> weights);
};

#endif /* Expansion_hpp */
