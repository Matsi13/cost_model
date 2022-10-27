#include "neuralNetwork.h"
#include <vector>
#include "route_helper.h"
using namespace std;

int NeuralNetwork::insert_layer(PhysicalLayer newlayer){
    layers.push_back(newlayer);
    if (layers.size() - 1 != newlayer.layer_id) {
        // wrong layer id, cannot insert
        layers.pop_back();
        return -1; 
    } else return newlayer.layer_id;

}

float NeuralNetwork::forward_input_time(int i){
    PhysicalLayer rcv_layer = layers[i];
    CoreLoc rcv_coreloc = rcv_layer.cores.coreloc;
    list<int> inputs_id = layers[i].input_layers_id;
    // If the layer has no input, it's one of the first layers. Assume the input is ready.
    if (inputs_id.empty()) return 0; 
    
    for (auto input_id = inputs_id.begin(); input_id != inputs_id.end(); input_id ++){
        PhysicalLayer input_layer = layers[*input_id];
        CoreLoc input_core_loc = input_layer.cores.coreloc;
        // TODO : communicate accross chips
        if (input_core_loc.chip_id != rcv_coreloc.chip_id) { return -1;}
       
        vector<CoreLoc> connect_points(4);
        bool is_connected = find_connect_points(input_layer.cores, rcv_layer.cores, connect_points);
         // TODO : what if these layers are on the same chip, but have no direct connection?
         if (!is_connected) return -1;
        // how the vector is split
        bool rows_split1 = !input_layer.is_trans;
        bool rows_split2 = rcv_layer.is_trans;
        vector<CoreLoc> input_layer_vertice(8), rcv_layer_vertice(8);
        find_vertice(input_layer.cores, rcv_layer.cores, rows_split1, rows_split2, input_layer_vertice, rcv_layer_vertice);
        // 3. calculate the 2/4 longest distances from input to connection points
        // 4. calculate the 2/4 longest distances from conncetion points to receiver 
    }

    }




   


