#include <cmath>
#include <vector>

#include "neuralNetwork.h"
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
    CoreLoc rcv_core_loc = rcv_layer.cores.coreloc;
    list<int> inputs_id = layers[i].input_layers_id;
    // If the layer has no input, it's one of the first layers. Assume the input is ready.
    if (inputs_id.empty()) return 0; 

    int max_dist_input_rcv = 0, max_dist_input = 0, max_dist_rcv = 0;
    float max_transfer_time = 0;
    
    for (auto input_id = inputs_id.begin(); input_id != inputs_id.end(); input_id ++){
        PhysicalLayer input_layer = layers[*input_id];
        CoreLoc input_core_loc = input_layer.cores.coreloc;

        // size of input data and rcv data
        int input_total_size = input_layer.is_trans ? input_layer.weight_size_col : input_layer.weight_size_row;
        float input_shard_size = input_layer.is_trans ? ceil(input_layer.weight_size_col/input_layer.cores.cols) : ceil(input_layer.weight_size_row/input_layer.cores.rows); 
        float rcv_shard_size = rcv_layer.is_trans ? ceil(rcv_layer.weight_size_row/rcv_layer.cores.rows) : ceil(rcv_layer.weight_size_col/rcv_layer.cores.cols);
        // find vertice of each layer
        bool rows_split1 = !input_layer.is_trans;
        bool rows_split2 = rcv_layer.is_trans;
        vector<CoreLoc> input_layer_vertice, rcv_layer_vertice;
        find_vertice(input_layer.cores, rcv_layer.cores, rows_split1, rows_split2, input_layer_vertice, rcv_layer_vertice);
        // communicate accross chips
        if (input_core_loc.chip_id != rcv_core_loc.chip_id) { 
            vector<int> input_dists, rcv_dists;
            // find the shortest distance from input to network port 
            for (int i = 0; i < input_layer_vertice.size() - 1; i += 2){
                int dist1, dist2;
                dist1 = core_dist(input_layer_vertice[i], {input_core_loc.chip_id, device.net_loc_row, device.net_loc_col});
                dist2 = core_dist(input_layer_vertice[i + 1], {input_core_loc.chip_id, device.net_loc_row, device.net_loc_col});
                if (dist1 > dist2){ 
                    input_dists.push_back(dist2);
                } else{
                    input_dists.push_back(dist1);
                }
            }  
            // find the longest distance from rcv to network port
            for (int i = 0; i < rcv_layer_vertice.size() - 1; i += 2){
                CoreLoc pedal1, pedal2;
                int dist1, dist2;
                dist1 = core_dist(rcv_layer_vertice[i], {rcv_core_loc.chip_id, device.net_loc_row, device.net_loc_col});
                dist2 = core_dist(rcv_layer_vertice[i + 1], {rcv_core_loc.chip_id, device.net_loc_row, device.net_loc_col});
                if (dist1 > dist2){            
                    rcv_dists.push_back(dist1);
                } else{                 
                    rcv_dists.push_back(dist2);
                }
            }     
            // find the longest distance
            for (int i = 0; i < 2; i++){
                int temp_dist_input_rcv, temp_dist_input, temp_dist_rcv;

                int dist_input_rcv_outer = input_dists[i] * input_shard_size + rcv_dists[i] * min(input_shard_size, rcv_shard_size);
                temp_dist_input_rcv = dist_input_rcv_outer;
                temp_dist_input = input_dists[i];
                temp_dist_rcv = rcv_dists[i];

                int dist_input_rcv_inner;
                if (input_dists.size() > 2) {
                    dist_input_rcv_inner = input_dists[i + 2] * input_shard_size + rcv_dists[i] * min(input_shard_size, rcv_shard_size);
                    if (dist_input_rcv_inner > dist_input_rcv_outer){
                        temp_dist_input_rcv = dist_input_rcv_inner;
                        temp_dist_input = input_dists[i + 2];
                        temp_dist_rcv = rcv_dists[i];
                    }               
                } else if(rcv_dists.size() > 2){
                    dist_input_rcv_inner = input_dists[i] * input_shard_size + rcv_dists[i + 2] * min(input_shard_size, rcv_shard_size);
                    if (dist_input_rcv_inner > dist_input_rcv_outer){
                        temp_dist_input_rcv = dist_input_rcv_inner;
                        temp_dist_input = input_dists[i];
                        temp_dist_rcv =  rcv_dists[i + 2];
                    }       
                }
                if (temp_dist_input_rcv > max_dist_input_rcv) {
                    max_dist_input_rcv = temp_dist_input_rcv;
                    max_dist_input = temp_dist_input;
                    max_dist_rcv = temp_dist_rcv;   
                }
            } 
            float temp_transfer_time = ( max_dist_input * input_shard_size + max_dist_rcv * min(input_shard_size, rcv_shard_size)) * rcv_layer.mem_byte_wid / device.BW_NoC + input_total_size * rcv_layer.mem_byte_wid / device.BW_net;
            if (temp_transfer_time > max_transfer_time)  max_transfer_time = temp_transfer_time;
        } else {
            // find connect points
            vector<CoreLoc> connect_points;
            bool is_connected = find_connect_points(input_layer.cores, rcv_layer.cores, connect_points);
            // TODO : what if these layers are on the same chip, but have no direct connection? 
            if (!is_connected) return -1;    
            // CoreLoc input_start_outer_pedal, input_end_outer_pedal, input_start_inner_pedal, input_end_inner_pedal, rcv_start_outer_pedal, rcv_end_outer_pedal, rcv_start_inner_pedal, rcv_end_inner_pedal;
            vector<CoreLoc> input_pedals;
            vector<CoreLoc> rcv_pedals;
            // int input_start_outer_dist, input_end_outer_dist,  input_start_inner_dist,input_end_inner_dist, rcv_start_outer_dist, rcv_end_outer_dist, rcv_start_inner_dist, rcv_end_inner_dist;
            vector<int> input_dists;
            vector<int> rcv_dists;
            // 3. calculate the 2/4 shortest distances from input to connection points       
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
                    rcv_pedals.push_back(pedal1);
                    rcv_dists.push_back(dist1);
                } else{
                    rcv_pedals.push_back(pedal2);
                    rcv_dists.push_back(dist2);
                }
            }
            // 5. calculate distances between pairs. The longest path = min(dist(input, connect)) + max(dist(connect, rcv)). It seems that we don't need to mark which pair generate the longest distance. We only need to get the longest distance       
            for (int i = 0; i < 2; i++){
                int temp_dist_input_rcv, temp_dist_input, temp_dist_rcv;

                int dist_input_rcv_outer = input_dists[i] + core_dist(input_pedals[i], rcv_pedals[i]) + rcv_dists[i];
                temp_dist_input_rcv = dist_input_rcv_outer;
                temp_dist_input = input_dists[i];
                temp_dist_rcv = core_dist(input_pedals[i], rcv_pedals[i]) + rcv_dists[i];

                int dist_input_rcv_inner;
                if (input_dists.size() > 2) {
                    dist_input_rcv_inner = input_dists[i + 2] + core_dist(input_pedals[i + 2], rcv_pedals[i]) + rcv_dists[i];
                    if (dist_input_rcv_inner > dist_input_rcv_outer){
                        temp_dist_input_rcv = dist_input_rcv_inner;
                        temp_dist_input = input_dists[i + 2];
                        temp_dist_rcv = core_dist(input_pedals[i + 2], rcv_pedals[i]) + rcv_dists[i];
                    }               
                } else if(rcv_dists.size() > 2){
                    dist_input_rcv_inner = input_dists[i] + core_dist(input_pedals[i], rcv_pedals[i + 2]) + rcv_dists[i + 2];
                    if (dist_input_rcv_inner > dist_input_rcv_outer){
                        temp_dist_input_rcv = dist_input_rcv_inner;
                        temp_dist_input = input_dists[i];
                        temp_dist_rcv = core_dist(input_pedals[i], rcv_pedals[i + 2]) + rcv_dists[i + 2];
                    }       
                }
                if (temp_dist_input_rcv > max_dist_input_rcv) {
                    max_dist_input_rcv = temp_dist_input_rcv;
                    max_dist_input = temp_dist_input;
                    max_dist_rcv = temp_dist_rcv;   
                }
            } 
            float temp_transfer_time = ( max_dist_input * input_shard_size + max_dist_rcv * min(input_shard_size, rcv_shard_size)) * rcv_layer.mem_byte_wid / device.BW_NoC;
            if (temp_transfer_time > max_transfer_time)  max_transfer_time = temp_transfer_time;
        }    
    }
        
    
    return max_transfer_time;

    }




   


