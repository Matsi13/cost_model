#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include "layer.h"
using namespace std;
class NeuralNetwork{
public :
    NeuralNetwork(){};
    ~NeuralNetwork(){};
    vector<PhysicalLayer> layers;
    ChipBasic device;

    int insert_layer(PhysicalLayer newlayer);


    float forward_input_time(int i); // layer i's time of getting forward input
    float forward_compute_time();
    float forward_reduce_time();
    float forward_non_linear_func_time();
    float forward_input2DRAM_time();
    float forward_output2DRAM_time();
    float backward_input_time();           // get the gradient
    float backward_original_output_time(); // get the unactivated output
    float backward_original_input_time();
    float backward_non_linear_func_time();
    float backward_weight_update_time();
    float backward_compute_time();
    float backward_reduce_time();
    float backward_weight_sync_time();
};


#endif