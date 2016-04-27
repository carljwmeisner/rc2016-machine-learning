#include "neuralNetwork.h"
#include "data.h"
#include <iostream>
#include <vector>
#include <cmath>

Neural_Network_t::Neural_Network_t(int inputs, int layers, int outputs,double range){
    number_of_inputs = inputs;
    number_of_layers = layers;
    number_of_outputs = outputs;
    //this->layers.push_back(new Neural_Network_Layer_t(inputs,inputs));
    //create hidden layers
    this->layers.push_back(new Neural_Network_Layer_t(1,inputs+1,range));
    for(int i = 0; i < number_of_layers; i++){
        this->layers.push_back(new Neural_Network_Layer_t(inputs,inputs,range));
        //this->layers[i]->weights(0,0) = i;
    }
    this->layers.push_back(new Neural_Network_Layer_t(inputs,outputs,range)); //creates one last layer - output layer
    this->layers[0]->weights.zeros();
    
}

void Neural_Network_t::showNetworkInfo(){
    std::cout << "Number of inputs: " << number_of_inputs << std::endl;
    std::cout << "Number of hidden layers: " << number_of_layers << std::endl;
    for(int i = 0; i < number_of_layers+2; i++){
        std::cout << "Layer " << i+1 << " ";
        layers[i]->print();
    }
}


bool Neural_Network_t::forwardPropigation(std::vector<double> inputs){
    std::vector<double > tempOut;
    inputs.insert(inputs.begin(),1);
    layers[0]->values = inputs;
    for(int i = 1; i < layers.size(); i++){
        layers[i]->values = (layers[i]->weights.t() * layers[i-1]->values);
        tempOut = arma::conv_to<std::vector<double> >::from(layers[i]->values);
        //if(i != layers.size()-1){
            tempOut.insert(tempOut.begin(),1);
        //}
        layers[i]->values = tempOut;
        layers[i]->evalf();
    }
    tempOut = arma::conv_to<std::vector<double> >::from(layers[layers.size()-1]->values);
    tempOut.erase(tempOut.begin());
    layers[layers.size()-1]->values = tempOut;
    finalValues = layers[layers.size()-1]->values;
    //std::cout << "forward prop works.\n";
    return true;
}

bool Neural_Network_t::backPropigation(std::vector< std::vector<double> > inputs, std::vector< std::vector<double> > answers,double stepSize){
    //set deltas to 0
    for(int i=0; i<layers.size(); i++){
        layers[i]->deltas.zeros();
    }
    for(int i = 0; i < inputs.size(); i++){
        forwardPropigation(inputs[i]);
        getDeltas(answers[i], stepSize);
    }
    //std::cout << "Back prop works.\n";
    return true;
}

//steps 3 and 4 in our back propagation flow chart
bool Neural_Network_t::getDeltas(std::vector<double>answers, double stepSize){
    arma::mat A;
    arma::vec temp2;
    std::vector<double> temp_vec;
    //in final output layer, calculate singleDeltas
    arma::vec temp(answers);
    layers[layers.size()-1] ->singleDelta = (finalValues - temp[0]) % (layers[layers.size()-1] ->values % (1-layers[layers.size()-1] ->values));
    //std::cout << temp[0] << std::endl;
    //compute singleDeltas for layers L-1 until L^1 (not input layer)
    for(int i=number_of_layers; i>0; i--){
       /* std::cout << "LAYER: " << i+1 << std::endl;
        layers[i+1] -> weights.print();
        layers[i+1] -> singleDelta.print();*/
        temp2 = (layers[i+1] -> weights * layers[i+1] -> singleDelta) % (layers[i] ->values % (1-layers[i] ->values));
        temp_vec = arma::conv_to<std::vector<double> >::from(temp2);
        temp_vec.erase(temp_vec.begin());
        temp2 = temp_vec;
        layers[i]->singleDelta = temp2;
    }
    //std::cout <<"DONE WITH PART I!\n";
    //temp.print();
   // layers[layers.size()-1]->deltas += temp * layers[layers.size()-1]->singleDelta.t();
    for(int i = layers.size()-1; i > 0; i--){
        /*std::cout << "LAYER: " << i << std::endl;
        layers[i] -> values.print();
        std::cout << std::endl;
        layers[i] -> singleDelta.print();*/
        /*temp_vec = arma::conv_to<std::vector<double> >::from(layers[i]->values);
        temp_vec.erase(temp_vec.begin());
        temp2 = temp_vec;*/
        layers[i]->deltas += (layers[i]->singleDelta*layers[i-1]->values.t()).t();
    }
    //std::cout << "DONE\n";
}



std::vector<double> Neural_Network_t::sendData(std::vector<double> inputs){
    //std::cout << "Calculating values...\n";
    forwardPropigation(inputs);
    std::vector<double> results;
    results = arma::conv_to<std::vector<double> >::from(finalValues);
    return results;
}

void Neural_Network_t::buildNetwork(Data training, int number_runs, int stepSize ,int number_restarts, double range){
    std::vector<arma::mat > bestWeights;
    buildNetworkPart(training, number_runs, stepSize);
    double bestAccuracy = accuracy;
    for(int i = 0; i < layers.size(); i++){
        bestWeights.push_back(layers[i]->weights);
    }
    for(int i = 0; i < number_restarts; i++){
        std::cout << "Percent Complete: " << i*100/number_restarts << std::endl << std::endl;
        restartNetwork(range);
        buildNetworkPart(training, number_runs, stepSize);
        if(accuracy > bestAccuracy){
            for(int j = 0; j < layers.size(); j++){
                bestWeights[j] = layers[j]->weights;
                bestAccuracy = accuracy;
            }
        }
    }
    for(int i = 0; i < layers.size(); i++){
        layers[i]->weights = bestWeights[i];
    }
    std::vector< std::vector<double> > inputs = training.getQuestions();
    std::vector< std::vector<double> > answers = training.getAnswers();
    for(int i = 0; i < inputs.size(); i++)
    inputs[i].erase(inputs[i].begin());
    std::cout << "The Best network had an error of " << getOneError(inputs,answers) << ".\n";
    getError(inputs,answers);
}

void Neural_Network_t::buildNetworkPart(Data training, int number_runs, double stepSize){
    arma::mat partial;
    std::vector< std::vector<double> > inputs = training.getQuestions();
    std::vector< std::vector<double> > answers = training.getAnswers();
    for(int i = 0; i < inputs.size(); i++)
        inputs[i].erase(inputs[i].begin());
    int m = inputs.size();
    double factor = 0.0;
    double  errorAtLastStep;
    double error;
    bool happens = false;
    std::cout << "Building network...\n";
    errorAtLastStep = getOneError(inputs,answers);
    std::cout << "Error at step 0 " << errorAtLastStep << std::endl << std::endl;
    for(int j = 0; j < number_runs; j++){
        backPropigation(inputs, answers, stepSize);
        for(int LAYERS = 0; LAYERS < layers.size(); LAYERS++){
            partial.zeros(arma::size(layers[LAYERS]->deltas));
            for(int i_axis = 0; i_axis < layers[LAYERS]->deltas.n_rows; i_axis++){
                for(int j_axis = 0; j_axis < layers[LAYERS]->deltas.n_cols; j_axis++){
                    if(j_axis != 0)
                    {
                        partial(i_axis,j_axis) = layers[LAYERS]->deltas(i_axis,j_axis)/m + factor*layers[LAYERS]->weights(i_axis,j_axis);
                    }
                    else{
                        partial(i_axis,j_axis) = layers[LAYERS]->deltas(i_axis,j_axis)/m;
                    }
                    //partial.print();
                    //std::cout << std::endl;
                }
            }
            /*std::cout << "Old layer weights for layer " << LAYERS << std::endl;
            layers[LAYERS]->weights.print();
            std::cout << "Change: \n";
            (stepSize*partial).print();*/
            //partial.print();
            layers[LAYERS]->weights = layers[LAYERS]->weights - stepSize*partial;
        }
        error = getOneError(inputs,answers);
        
        if(fabs(errorAtLastStep) - fabs(error) < 0 ){
            // stepSize = stepSize*.99;
            std::cout << "Error at step " << j+1 << ": " << error << std::endl;
            break;
            happens = true;
        }
        /*if(happens == false && fabs(errorAtLastStep) < fabs(error)){
            stepSize*=1.1;
        }*/
        
        errorAtLastStep = error;
        if((j+1)%100 == 0){
            std::cout << "Error at step " << j+1 << ": " << error << std::endl;
            /*if(errorAtLastStep < error){
                break;
            }*/
            //getError(inputs,answers);
        }
    }
    accuracy = getError(inputs,answers);
}

double Neural_Network_t::getOneError(std::vector<std::vector<double> > inputs, std::vector< std::vector<double> > answers){
    std::vector<double> tempInput;
    std::vector<double> tempOutput;
    double total = 0;
    int m = inputs.size();
    for(int i = 0; i < inputs.size(); i++){
        tempInput = inputs[i];
        tempOutput = sendData(tempInput);
        total += (answers[i][0]*log(tempOutput[0])+(1-answers[i][0]*log(1-tempOutput[0])));
    }
    return total/m;
}

double Neural_Network_t::getError(std::vector<std::vector<double> > inputs, std::vector< std::vector<double> > answers){
    std::vector<double> tempInput;
    std::vector<double> tempOutput;
    std::vector<double> results;
    int total;
    for(int i = 0; i < 4; i++){
        results.push_back(0);
    }
    for(int i = 0; i < inputs.size(); i++){
        tempInput = inputs[i];
        tempOutput = sendData(tempInput);
        tempOutput[0] = round(tempOutput[0]);
        if(tempOutput[0] == answers[i][0] && answers[i][0] == 1){
            results[0]++;
        }
        else if (tempOutput[0] == answers[i][0]){
            results[1]++;
        }
        else if (answers[i][0] == 1){
            results[2]++;
        }
        else{
            results[3]++;
        }
    }
    total = results[0]+results[1]+results[2]+results[3];
    std::cout << "True Positives: " << results[0] << "(" << results[0]/total*100 << ")" << std::endl;
    std::cout << "True Negaitve: " << results[1] << "(" << results[1]/total*100 << ")" << std::endl;
    std::cout << "False Positives: " << results[3] << "(" << results[3]/total*100 << ")" << std::endl;
    std::cout << "False Negative: " << results[2] << "(" << results[2]/total*100 << ")" << std::endl;
    std::cout << "Overall: " << (results[0]+results[1])/total*100 << std::endl;
    
    return (results[0]+results[1])/total*100;
}

void Neural_Network_t::restartNetwork(double range){
    for(int i = 0; i < layers.size(); i++){
        layers[i]->weights.randu();
        layers[i]->weights = layers[i]->weights*(range-range/2);
        //this->layers[i]->weights(0,0) = i;
    }
    layers[0]->weights.zeros();
}

/////////////////////////////////////////////

Neural_Network_Layer_t::Neural_Network_Layer_t(int nodes_ahead, int nodes_in_layer,double range){
    number_of_nodes = nodes_in_layer;
    nodes_in_sending_layer = nodes_ahead+1;
    arma::mat temp1(nodes_in_sending_layer,nodes_in_layer);
    arma::mat temp2(nodes_in_sending_layer,nodes_in_layer);
    weights = temp1;
    deltas = temp2;
    weights.randu();
    weights = weights * range;
    deltas.fill(0);
    values.zeros(nodes_in_layer);
    singleDelta.zeros(nodes_in_layer);
}

void Neural_Network_Layer_t::print(){
    weights.print("Weights: ");
  //  std::cout << std::endl;
  //  values.print();
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
