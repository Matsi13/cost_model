#include "layer.h"

#include <string>
using namespace std;

PhysicalLayer::PhysicalLayer(){
    weight_size_row = 1024;
    weight_size_col = 1024;
    layer_id = 1;
    non_linear_func = "ReLU";
    has_weight = true;
}

PhysicalLayer::PhysicalLayer(int layerid, bool hasweight, int weightsizerow, int weightsizecol, string nonlinearfunc, list<int> inputlayerid, list<int> outputlayerid, list<CoreRange> thecores){
    layer_id = layerid;
    has_weight = hasweight;
    if (has_weight){
        weight_size_row = weightsizerow;
        weight_size_col = weightsizecol;
    } else{
        weight_size_row = 0;
        weight_size_col = 0;
    }
    non_linear_func = nonlinearfunc;
    input_layer_id = inputlayerid;
    output_layer_id = outputlayerid;
    cores = thecores;

}