#ifndef LAYER_H
#define LAYER_H
#endif
#include <string>
#include <list>
#include "chipBasic.h"
#include "route_helper.h"
using namespace std;
class PhysicalLayer{
public :
    PhysicalLayer();
    ~PhysicalLayer(){};
    int weight_size_row;      // rows of weight     
    int weight_size_col;      // columns of weight
    int layer_id;             // each layer has a unique id
    string non_linear_func;   // currently, support ReLU and tanh
    list<int> input_layer_id;
    list<int> output_layer_id;
    bool has_weight;          // if false, this layer is just a non-linear function
    list<CoreRange> cores;      // allocated cores

    // the varaibles below will be calculated by class DNN
    float fw_input_time;
    float fw_compute_time;
    float fw_reduce_time;
    float fw_non_linear_func_time;
    float fw_input2DRAM_time;
    float fw_output2DRAM_time;
    float bw_input_time;           // get the gradient
    float bw_original_output_time; // get the unactivated output
    float bw_original_input_time;
    float bw_non_linear_func_time;
    float bw_weight_update_time;
    float bw_compute_time;
    float bw_reduce_time;
    float bw_weight_sync_time;


    
};