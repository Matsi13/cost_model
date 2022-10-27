#include <iostream>
#include "layer.h"
#include <list>
#include "route_helper.h"
#include "neuralNetwork.h"
using namespace std;

int main(){
    list<int> input_layer_id0 = {};
    list<int> output_layer_id0 = {1};
    CoreLoc coreloc1 = {0,0,0};
    CoreRange corerange1 = {coreloc1, 4, 8};
   
    PhysicalLayer layer0(0, true, false, 1024, 1024, "ReLU", input_layer_id0, output_layer_id0, corerange1);

    list<int> input_layer_id1 = {0};
    list<int> output_layer_id1 = {};
    CoreLoc coreloc2 = {0,4,0};
    CoreRange corerange2 = {coreloc2, 4, 8};
    
    PhysicalLayer layer1(1,true, false, 1024, 1024, "ReLU", input_layer_id1, output_layer_id1,corerange2);
    
    NeuralNetwork dnn;
    dnn.insert_layer(layer0);
    dnn.insert_layer(layer1);
    
    return 0;
}
