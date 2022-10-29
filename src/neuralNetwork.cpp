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
     int max_dist = 0;
    
    for (auto input_id = inputs_id.begin(); input_id != inputs_id.end(); input_id ++){
        PhysicalLayer input_layer = layers[*input_id];
        CoreLoc input_core_loc = input_layer.cores.coreloc;
        // TODO : communicate accross chips
        if (input_core_loc.chip_id != rcv_coreloc.chip_id) { return -1;}
       
        vector<CoreLoc> connect_points;
        bool is_connected = find_connect_points(input_layer.cores, rcv_layer.cores, connect_points);
         // TODO : what if these layers are on the same chip, but have no direct connection? 
         if (!is_connected) return -1;
        // how the vector is split
        bool rows_split1 = !input_layer.is_trans;
        bool rows_split2 = rcv_layer.is_trans;
        vector<CoreLoc> input_layer_vertice, rcv_layer_vertice;
        find_vertice(input_layer.cores, rcv_layer.cores, rows_split1, rows_split2, input_layer_vertice, rcv_layer_vertice);

        // CoreLoc input_start_outer_pedal, input_end_outer_pedal, input_start_inner_pedal, input_end_inner_pedal, rcv_start_outer_pedal, rcv_end_outer_pedal, rcv_start_inner_pedal, rcv_end_inner_pedal;
        vector<CoreLoc> input_pedals;
        vector<CoreLoc> rcv_pedals;

        // int input_start_outer_dist, input_end_outer_dist,  input_start_inner_dist,input_end_inner_dist, rcv_start_outer_dist, rcv_end_outer_dist, rcv_start_inner_dist, rcv_end_inner_dist;
        vector<int> input_dists;
        vector<int> rcv_dists;

        // 3. calculate the 2/4 longest distances from input to connection points
        
        for (int i = 0; i < input_layer_vertice.size() - 1; i+=2){
            CoreLoc pedal1, pedal2;
            int dist1, dist2;
            dist1 = core_line_dist(input_layer_vertice[i], connect_points[0], connect_points[2], pedal1);
            dist2 = core_line_dist(input_layer_vertice[i + 1], connect_points[0], connect_points[2], pedal2);
            if (dist1 > dist2){
                input_pedals.push_back(pedal2);
                input_dists.push_back(dist2);
            } else{
                input_pedals.push_back(pedal1);
                input_dists.push_back(dist1);
            }
        }
        
        // 4. calculate the 2/4 longest distances from conncetion points to receiver 

        for (int i = 0; i < rcv_layer_vertice.size() - 1; i += 2){
            CoreLoc pedal1, pedal2;
            int dist1, dist2;
            dist1 = core_line_dist(rcv_layer_vertice[i], connect_points[1], connect_points[3], pedal1);
            dist2 = core_line_dist(rcv_layer_vertice[i + 1], connect_points[1], connect_points[3], pedal2);
            if (dist1 > dist2){
                rcv_pedals.push_back(pedal2);
                rcv_dists.push_back(dist2);
            } else{
                rcv_pedals.push_back(pedal1);
                rcv_dists.push_back(dist1);
            }
        }

        // 5. calculate distances between pairs -- It seems that we don't need to mark which pair generate the longest distance. We only need to get the longest distance
       
        for (int i = 0; i < 2; i++){
            int input_rcv_dist = input_dists[i] + core_dist(input_pedals[i], rcv_pedals[i]) + rcv_dists[i];
            int inner_dist;
            if (input_dists.size() > 2) {
                inner_dist = input_dists[i + 2] + core_dist(input_pedals[i + 2], rcv_pedals[i]) + rcv_dists[i];
                input_rcv_dist = max(inner_dist, input_rcv_dist);
            } else if(rcv_dists.size() > 2){
                inner_dist = input_dists[i] + core_dist(input_pedals[i], rcv_pedals[i + 2]) + rcv_dists[i + 2];
                input_rcv_dist = max(inner_dist, input_rcv_dist);
            }
            if (input_rcv_dist > max_dist) max_dist = input_rcv_dist;
        }

        
    }
        

    return float(max_dist)/device.BW_NoC;

    }




   


