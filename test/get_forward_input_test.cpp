#include <iostream>
#include <cmath>
#include "layer.h"
#include "neuralNetwork.h"
#include "route_helper.h"

using namespace std;

int main(){

    float input_time;

    list<int> input_layer_id0 = {};
    list<int> output_layer_id0 = {1};
    CoreLoc coreloc1 = {0,0,0};
    CoreRange corerange1 = {coreloc1, 4, 8};
   
     PhysicalLayer layer0(0, true, false, 1024, 1024, 4, 2, "ReLU", input_layer_id0, output_layer_id0, corerange1);

    list<int> input_layer_id1 = {0};
    list<int> output_layer_id1 = {};
    CoreLoc coreloc2 = {0,4,0};
    CoreRange corerange2 = {coreloc2, 4, 8};
    
    PhysicalLayer layer1(1,true, false, 1024, 1024, 4, 2, "ReLU", input_layer_id1, output_layer_id1,corerange2);
    
    NeuralNetwork dnn;
    dnn.insert_layer(layer0);
    dnn.insert_layer(layer1);
    
    input_time = dnn.forward_input_time(1);
    if (abs(input_time - 2.933e-8) > 1e-10) {
        cout << "fail in case 1 " << 2.933e-8 << " " << input_time << endl;
        return -1; 
    }
    return 0;
}