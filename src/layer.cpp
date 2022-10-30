#include "layer.h"

#include <string>
using namespace std;

PhysicalLayer::PhysicalLayer(){
    weight_size_row = 1024;
    weight_size_col = 1024;
    layer_id = 1;
    non_linear_func = "ReLU";
    has_weight = true;
    is_trans = false;
    mem_byte_wid = 4;
    cal_byte_wid = 2;
}

PhysicalLayer::PhysicalLayer(int layerid, bool hasweight, bool istrans, int weightsizerow, int weightsizecol, int membytewid, int calbytewid, string nonlinearfunc, list<int> inputlayerid, list<int> outputlayerid, CoreRange thecores){
    layer_id = layerid;
    has_weight = hasweight;
    if (has_weight){
        weight_size_row = weightsizerow;
        weight_size_col = weightsizecol;
        is_trans = istrans;
        
    } else{
        weight_size_row = 0;
        weight_size_col = 0;
        is_trans = false;
    }
    mem_byte_wid = membytewid;
    cal_byte_wid = calbytewid;
    non_linear_func = nonlinearfunc;
    input_layers_id = inputlayerid;
    output_layers_id = outputlayerid;
    cores = thecores;

}