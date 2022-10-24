#include <iostream>
#include "layer.h"
#include <list>
#include "route_helper.h"
using namespace std;

int main(){
    list<int> input_layer_id = {0};
    list<int> output_layer_id = {2,3};
    CoreLoc coreloc1 = {0,0,0};
    CoreRange corerange1 = {coreloc1, 8, 8};
    CoreLoc coreloc2 = {1,8,8};
    CoreRange corerange2 = {coreloc2, 4, 4};
    list<CoreRange> cores = {corerange1, corerange2};
    PhysicalLayer layer1(1, true, 1024, 256, "ReLU", input_layer_id, output_layer_id, cores);
    
    return 0;
}
