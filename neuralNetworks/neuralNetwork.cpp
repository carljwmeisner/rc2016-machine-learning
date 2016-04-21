//
//  neuralNetwork.cpp
//  
//
//  Created by Gableman, Michael on 4/13/16.
//
//

#include "neuralNetwork.h"
#include <iostream>
#include <vector>
#include <cmath>


Neural_Network_t::Neural_Network_t(int inputs, int layers, int outputs){
    number_of_inputs = inputs;
    number_of_layers = layers;
    number_of_outputs = outputs;
    //this->layers.push_back(new Neural_Network_Layer_t(inputs,inputs));
    //create hidden layers
    for(int i = 0; i < number_of_layers; i++){
        this->layers.push_back(new Neural_Network_Layer_t(inputs,inputs));
        //this->layers[i]->weights(0,0) = i;
    }
    this->layers.push_back(new Neural_Network_Layer_t(inputs,outputs)); //creates one last layer - output layer
    
}

void Neural_Network_t::showNetworkInfo(){
    std::cout << "Number of inputs: " << number_of_inputs << std::endl;
    std::cout << "Number of hidden layers: " << number_of_layers << std::endl;
    for(int i = 0; i < number_of_layers; i++){
        std::cout << "Layer " << i << " ";
        layers[i]->print();
    }
}

bool Neural_Network_t::forwardPropigation(std::vector<double> inputs){
    std::cout << "Calling forward propigation.\n";
    std::vector<double > tempOut;
    inputs.insert(inputs.begin(),1);
    layers[0]->values = inputs;
    std::cout << "Values of layer " << 0 << " : ";
    layers[0]->values.print();
    for(int i = 1; i < layers.size(); i++){
        //layers[i-1]->weights.print();
        //layers[i-1]->values.print();
        layers[i]->values = (layers[i-1]->weights * layers[i-1]->values);
        tempOut = arma::conv_to<std::vector<double> >::from(layers[i]->values);
        tempOut.insert(tempOut.begin(),1);
        layers[i]->values = tempOut;
        std::cout << "Values of layer " << i << " : ";
        //layers[i]->values.print();
        layers[i]->evalf();
        //std::cout << "Altered : ";
        layers[i]->values.print();
    }
    finalValues = (layers[layers.size()-1]->weights * layers[layers.size()-1]->values);
    tempOut = arma::conv_to<std::vector<double> >::from(finalValues);
    tempOut.insert(tempOut.begin(),1);
    finalValues = tempOut;

    std::cout << "Final Values : ";
    finalValues.print();
    return true;
}



bool Neural_Network_t::backPropigation(std::vector<double> answers){
    std::cout << "Calling backward propigation.\n";
    //set deltas to 0
    for(int i=0; i<number_of_layers+1; i++){
      layers[i]->deltas.zeros();
    }
    //for all training data
    //assume data is in train
    {
      
    }
    
    return true;
}

//steps 3 and 4 in our back propagation flow chart
bool Neural_Network_t::getDeltas(std::vector<double>answers){
  //in final output layer, calculate singleDeltas
  arma::vec temp(answers);
  layers[number_of_layers+1] ->singleDelta = layers[number_of_layers+1]-> values - temp;
  //compute singleDeltas for layers L-1 until L^1 (not input layer)
  for(int i=number_of_layers; i>=1; i--){
    layers[i] -> singleDelta = (layers[i] -> weights.t() * layers[i+1] -> singleDelta) % (layers[i] ->values % (1-layers[i] ->values));
  }
  //adjust weights for each layer
  for(int i=number_of_layers; i>=1; i--){
    layers[i] -> deltas += layers[i+1] -> singleDelta * layers[i] -> value.t();
  }
}


std::vector<double> Neural_Network_t::sendData(std::vector<double> inputs){
    std::cout << "Calculating values...\n";
    forwardPropigation(inputs);
    return inputs;
}

void Neural_Network_t::buildNetwork(std::vector<std::vector<double> > training, int number_runs){
    std::cout << "Building network...\n";
    backPropigation(training[0]);
}

/////////////////////////////////////////////

Neural_Network_Layer_t::Neural_Network_Layer_t(int nodes_ahead, int nodes_in_layer){
    number_of_nodes = nodes_in_layer;
    nodes_in_sending_layer = nodes_ahead+1;
    arma::mat temp1(nodes_in_layer,nodes_in_sending_layer);
    arma::mat temp2(nodes_in_layer,nodes_in_sending_layer);
    weights = temp1;
    deltas = temp2;
    weights.fill(1);
    deltas.fill(0);
    for(int i = 0; i < nodes_in_layer; i++){
        values << 0;
        singleDelta << 0;
    }
}

void Neural_Network_Layer_t::print(){
    weights.print("Weights: ");
}

void Neural_Network_Layer_t::evalf(){
    std::vector<double> tempOut;
    tempOut = arma::conv_to<std::vector<double> >::from(values);
//    std::cout << values.size() << "\n";
    for(int i = 1; i < tempOut.size(); i++)
    {
        tempOut[i] = (1/(1+std::exp(-tempOut[i])));
    }
    values = tempOut;
}

////////////////////////////////////
