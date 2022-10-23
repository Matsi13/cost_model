#include "layer.h"

#include <string>
using namespace std;

PhysicalLayer::PhysicalLayer(){
    weight_size_row = 1024;
    weight_size_col = 1024;
    layer_id = 1;
    non_linear_func = "ReLU";
    has_weight = true;
    // input_onchip = true;
    // output_onchip = false;
    // chip_id = 0;
    // core_left = 0;
    // core_up = 0;
    // core_right = 8;
    // core_down = 4;
    // input_layer_id = 0;
    // output_layer_id = 2;
}

