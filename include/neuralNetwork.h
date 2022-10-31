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
    float forward_compute_time(int i); // layer i's independent calculation time
    float forward_reduce_time(int i);
    float forward_non_linear_func_time(int i);
    float forward_input2DRAM_time(int i);// suppose each core use the nearest DRAM. They may store same data in different DRAMs
    float forward_output2DRAM_time(int i);
    float backward_input_time(int i);           // get the gradient
    float backward_original_output_time(int i); // get the unactivated output
    float backward_original_input_time(int i);
    float backward_non_linear_func_time(int i);
    float backward_weight_update_time(int i);
    float backward_compute_time(int i);
    float backward_reduce_time(int i);
    float backward_weight_sync_time(int i);
};


#endif