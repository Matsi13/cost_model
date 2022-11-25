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

// float NeuralNetwork::forward_input_time(int i){
//     PhysicalLayer rcv_layer = layers[i];
//     CoreLoc rcv_core_loc = rcv_layer.cores.coreloc;
//     list<int> inputs_id = layers[i].input_layers_id;
//     // If the layer has no input, it's one of the first layers. Assume the input is ready.
//     if (inputs_id.empty()) return 0; 

//     int max_dist_input_rcv = 0, max_dist_input = 0, max_dist_rcv = 0;
//     float max_transfer_time = 0;
    
//     for (auto input_id = inputs_id.begin(); input_id != inputs_id.end(); input_id ++){
//         PhysicalLayer input_layer = layers[*input_id];
//         CoreLoc input_core_loc = input_layer.cores.coreloc;

//         // size of input data and rcv data
//         int input_total_size = input_layer.weight_size_row;
//         float input_shard_size = input_layer.is_trans ? ceil(input_layer.weight_size_row/input_layer.cores.cols) : ceil(input_layer.weight_size_row/input_layer.cores.rows); 
//         float rcv_shard_size = rcv_layer.is_trans ? ceil(rcv_layer.weight_size_col/rcv_layer.cores.rows) : ceil(rcv_layer.weight_size_col/rcv_layer.cores.cols);
//         // find vertice of each layer
//         bool rows_split1 = !input_layer.is_trans;
//         bool rows_split2 = rcv_layer.is_trans;
//         vector<CoreLoc> input_layer_vertice, rcv_layer_vertice;
//         find_vertice(input_layer.cores, rcv_layer.cores, rows_split1, rows_split2, input_layer_vertice, rcv_layer_vertice);
//         // communicate accross chips
//         if (input_core_loc.chip_id != rcv_core_loc.chip_id) { 
//             vector<int> input_dists, rcv_dists;
//             // find the shortest distance from input to network port 
//             for (int i = 0; i < input_layer_vertice.size() - 1; i += 2){
//                 int dist1, dist2;
//                 dist1 = core_dist(input_layer_vertice[i], device.net_port_loc);
//                 dist2 = core_dist(input_layer_vertice[i + 1], device.net_port_loc);
//                 if (dist1 > dist2){ 
//                     input_dists.push_back(dist2);
//                 } else{
//                     input_dists.push_back(dist1);
//                 }
//             }  
//             // find the longest distance from rcv to network port
//             for (int i = 0; i < rcv_layer_vertice.size() - 1; i += 2){
//                 CoreLoc pedal1, pedal2;
//                 int dist1, dist2;
//                 dist1 = core_dist(rcv_layer_vertice[i], device.net_port_loc);
//                 dist2 = core_dist(rcv_layer_vertice[i + 1], device.net_port_loc);
//                 if (dist1 > dist2){            
//                     rcv_dists.push_back(dist1);
//                 } else{                 
//                     rcv_dists.push_back(dist2);
//                 }
//             }     
//             // find the longest distance
//             for (int i = 0; i < 2; i++){
//                 int temp_dist_input_rcv, temp_dist_input, temp_dist_rcv;

//                 int dist_input_rcv_outer = input_dists[i] * input_shard_size + rcv_dists[i] * min(input_shard_size, rcv_shard_size);
//                 temp_dist_input_rcv = dist_input_rcv_outer;
//                 temp_dist_input = input_dists[i];
//                 temp_dist_rcv = rcv_dists[i];

//                 int dist_input_rcv_inner;
//                 if (input_dists.size() > 2) {
//                     dist_input_rcv_inner = input_dists[i + 2] * input_shard_size + rcv_dists[i] * min(input_shard_size, rcv_shard_size);
//                     if (dist_input_rcv_inner > dist_input_rcv_outer){
//                         temp_dist_input_rcv = dist_input_rcv_inner;
//                         temp_dist_input = input_dists[i + 2];
//                         temp_dist_rcv = rcv_dists[i];
//                     }               
//                 } else if(rcv_dists.size() > 2){
//                     dist_input_rcv_inner = input_dists[i] * input_shard_size + rcv_dists[i + 2] * min(input_shard_size, rcv_shard_size);
//                     if (dist_input_rcv_inner > dist_input_rcv_outer){
//                         temp_dist_input_rcv = dist_input_rcv_inner;
//                         temp_dist_input = input_dists[i];
//                         temp_dist_rcv =  rcv_dists[i + 2];
//                     }       
//                 }
//                 if (temp_dist_input_rcv > max_dist_input_rcv) {
//                     max_dist_input_rcv = temp_dist_input_rcv;
//                     max_dist_input = temp_dist_input;
//                     max_dist_rcv = temp_dist_rcv;   
//                 }
//             } 
//             float temp_transfer_time = ( max_dist_input * input_shard_size + max_dist_rcv * min(input_shard_size, rcv_shard_size)) * rcv_layer.mem_byte_wid / min(device.BW_NoC, device.BW_net) ;
//             if (temp_transfer_time > max_transfer_time)  max_transfer_time = temp_transfer_time;
//         } else {
//             // find connect points
//             vector<CoreLoc> connect_points;
//             bool is_connected = find_connect_points(input_layer.cores, rcv_layer.cores, connect_points);
//             if (!is_connected) find_subconnect_points(input_layer.cores, rcv_layer.cores, connect_points);    
//             // CoreLoc input_start_outer_pedal, input_end_outer_pedal, input_start_inner_pedal, input_end_inner_pedal, rcv_start_outer_pedal, rcv_end_outer_pedal, rcv_start_inner_pedal, rcv_end_inner_pedal;
//             vector<CoreLoc> input_pedals;
//             vector<CoreLoc> rcv_pedals;
//             // int input_start_outer_dist, input_end_outer_dist,  input_start_inner_dist,input_end_inner_dist, rcv_start_outer_dist, rcv_end_outer_dist, rcv_start_inner_dist, rcv_end_inner_dist;
//             vector<int> input_dists;
//             vector<int> rcv_dists;
//             // 3. calculate the 2/4 shortest distances from input to connection points       
//             for (int i = 0; i < input_layer_vertice.size() - 1; i += 2){
//                 CoreLoc pedal1, pedal2;
//                 int dist1, dist2;
//                 dist1 = core_line_dist(input_layer_vertice[i], connect_points[0], connect_points[2], pedal1);
//                 dist2 = core_line_dist(input_layer_vertice[i + 1], connect_points[0], connect_points[2], pedal2);
//                 if (dist1 > dist2){
//                     input_pedals.push_back(pedal2);
//                     input_dists.push_back(dist2);
//                 } else{
//                     input_pedals.push_back(pedal1);
//                     input_dists.push_back(dist1);
//                 }
//             }       
//             // 4. calculate the 2/4 longest distances from conncetion points to receiver 
//             for (int i = 0; i < rcv_layer_vertice.size() - 1; i += 2){
//                 CoreLoc pedal1, pedal2;
//                 int dist1, dist2;
//                 dist1 = core_line_dist(rcv_layer_vertice[i], connect_points[1], connect_points[3], pedal1);
//                 dist2 = core_line_dist(rcv_layer_vertice[i + 1], connect_points[1], connect_points[3], pedal2);
//                 if (dist1 > dist2){
//                     rcv_pedals.push_back(pedal1);
//                     rcv_dists.push_back(dist1);
//                 } else{
//                     rcv_pedals.push_back(pedal2);
//                     rcv_dists.push_back(dist2);
//                 }
//             }
//             // 5. calculate distances between pairs. The longest path = min(dist(input, connect)) + max(dist(connect, rcv)). It seems that we don't need to mark which pair generate the longest distance. We only need to get the longest distance       
//             for (int i = 0; i < 2; i++){
//                 int temp_dist_input_rcv, temp_dist_input, temp_dist_rcv;

//                 int dist_input_rcv_outer = input_dists[i] + core_dist(input_pedals[i], rcv_pedals[i]) + rcv_dists[i];
//                 temp_dist_input_rcv = dist_input_rcv_outer;
//                 temp_dist_input = input_dists[i];
//                 temp_dist_rcv = core_dist(input_pedals[i], rcv_pedals[i]) + rcv_dists[i];

//                 int dist_input_rcv_inner;
//                 if (input_dists.size() > 2) {
//                     dist_input_rcv_inner = input_dists[i + 2] + core_dist(input_pedals[i + 2], rcv_pedals[i]) + rcv_dists[i];
//                     if (dist_input_rcv_inner > dist_input_rcv_outer){
//                         temp_dist_input_rcv = dist_input_rcv_inner;
//                         temp_dist_input = input_dists[i + 2];
//                         temp_dist_rcv = core_dist(input_pedals[i + 2], rcv_pedals[i]) + rcv_dists[i];
//                     }               
//                 } else if(rcv_dists.size() > 2){
//                     dist_input_rcv_inner = input_dists[i] + core_dist(input_pedals[i], rcv_pedals[i + 2]) + rcv_dists[i + 2];
//                     if (dist_input_rcv_inner > dist_input_rcv_outer){
//                         temp_dist_input_rcv = dist_input_rcv_inner;
//                         temp_dist_input = input_dists[i];
//                         temp_dist_rcv = core_dist(input_pedals[i], rcv_pedals[i + 2]) + rcv_dists[i + 2];
//                     }       
//                 }
//                 if (temp_dist_input_rcv > max_dist_input_rcv) {
//                     max_dist_input_rcv = temp_dist_input_rcv;
//                     max_dist_input = temp_dist_input;
//                     max_dist_rcv = temp_dist_rcv;   
//                 }
//             } 
//             float temp_transfer_time = ( max_dist_input * input_shard_size + max_dist_rcv * min(input_shard_size, rcv_shard_size)) * rcv_layer.mem_byte_wid / device.BW_NoC;
//             if (temp_transfer_time > max_transfer_time)  max_transfer_time = temp_transfer_time;
//         }    
//     }
        
    
//     return max_transfer_time;

//     }
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
        int input_total_size = input_layer.weight_size_row;
        float input_shard_size = input_layer.is_trans ? ceil(input_layer.weight_size_row/input_layer.cores.cols) : ceil(input_layer.weight_size_row/input_layer.cores.rows); 
        float rcv_shard_size = rcv_layer.is_trans ? ceil(rcv_layer.weight_size_col/rcv_layer.cores.rows) : ceil(rcv_layer.weight_size_col/rcv_layer.cores.cols);
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
                dist1 = core_dist(input_layer_vertice[i], device.net_port_loc);
                dist2 = core_dist(input_layer_vertice[i + 1], device.net_port_loc);
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
                dist1 = core_dist(rcv_layer_vertice[i], device.net_port_loc);
                dist2 = core_dist(rcv_layer_vertice[i + 1], device.net_port_loc);
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
            float temp_transfer_time = ( max_dist_input * input_shard_size + max_dist_rcv * min(input_shard_size, rcv_shard_size)) * rcv_layer.mem_byte_wid / min(device.BW_NoC, device.BW_net) ;
            if (temp_transfer_time > max_transfer_time)  max_transfer_time = temp_transfer_time;
        } else {
            // find connect points
            vector<CoreLoc> connect_points;
            bool is_connected = find_connect_points(input_layer.cores, rcv_layer.cores, connect_points);
            if (!is_connected) find_subconnect_points(input_layer.cores, rcv_layer.cores, connect_points);    
            if (connect_points.size() < 4){
                connect_points.push_back(connect_points[0]);
                connect_points.push_back(connect_points[1]);
            }
            // CoreLoc input_start_outer_pedal, input_end_outer_pedal, input_start_inner_pedal, input_end_inner_pedal, rcv_start_outer_pedal, rcv_end_outer_pedal, rcv_start_inner_pedal, rcv_end_inner_pedal;
            vector<CoreLoc> input_pedals;
            vector<CoreLoc> rcv_pedals;
            // int input_start_outer_dist, input_end_outer_dist,  input_start_inner_dist,input_end_inner_dist, rcv_start_outer_dist, rcv_end_outer_dist, rcv_start_inner_dist, rcv_end_inner_dist;
            vector<int> input_dists;
            vector<int> rcv_dists;
            // 3. calculate the 2/4 shortest distances from input to connection points       
            for (int i = 0; i < input_layer_vertice.size() - 1; i += 2){
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
float NeuralNetwork::forward_compute_time(int i){
    if (!layers[i].has_weight) return 0;
    int matrix_row_split = layers[i].is_trans ? layers[i].cores.cols : layers[i].cores.rows;
    int matrix_col_split = layers[i].is_trans ? layers[i].cores.rows : layers[i].cores.cols;
    int row_size = ceil(layers[i].weight_size_row / matrix_row_split);
    int col_size = ceil(layers[i].weight_size_col / matrix_col_split);
    float compute_flops = 2 * row_size * col_size - row_size;
    float compute_power;
    if (layers[i].cal_byte_wid == 1) compute_power = device.core_FLOPS_fp8;
    else if (layers[i].cal_byte_wid == 2) compute_power = device.core_FLOPS_fp16;
    else return -1;
    return compute_flops/compute_power;

}

float NeuralNetwork::forward_reduce_time(int i){
    if (!layers[i].has_weight) return 0;

    float compute_power;
    if (layers[i].cal_byte_wid == 1) compute_power = device.core_FLOPS_fp8;
    else if (layers[i].cal_byte_wid == 2) compute_power = device.core_FLOPS_fp16;
    else return -1;

    int matrix_row_split = layers[i].is_trans ? layers[i].cores.cols : layers[i].cores.rows;
    int matrix_col_split = layers[i].is_trans ? layers[i].cores.rows : layers[i].cores.cols;
    int row_size = ceil(layers[i].weight_size_row / matrix_row_split);
    int col_size = ceil(layers[i].weight_size_col / matrix_col_split);

    float compute_flops = (matrix_col_split - 1) * row_size;
    float compute_time = compute_flops/compute_power;
    float communicate_time = (matrix_col_split - 1) * row_size / device.BW_NoC;

    return max(communicate_time, compute_time);
    

}
   
float NeuralNetwork::forward_non_linear_func_time(int i){
    float compute_power;
    if (layers[i].cal_byte_wid == 1) compute_power = device.core_FLOPS_fp8;
    else if (layers[i].cal_byte_wid == 2) compute_power = device.core_FLOPS_fp16;
    else return -1;

    int matrix_row_split = layers[i].is_trans ? layers[i].cores.cols : layers[i].cores.rows;
    int row_size = ceil(layers[i].weight_size_row / matrix_row_split);

    float per_flops = device.non_linear_func_time(layers[i].non_linear_func);
    return (float)row_size * per_flops/compute_power;
}

float NeuralNetwork::forward_input2DRAM_time(int i){
    int max_dist = 0;
    float input_data_shard;
    vector<CoreLoc> vertices; // first element, first element, last element, last elememt
    CoreLoc upleft = layers[i].cores.coreloc;
    CoreLoc upright = {upleft.chip_id, upleft.row_id, upleft.col_id + layers[i].cores.cols};
    CoreLoc downleft = {upleft.chip_id, upleft.row_id + layers[i].cores.rows, upleft.col_id};
    CoreLoc downright = {upleft.chip_id, upleft.row_id + layers[i].cores.rows, upleft.col_id + layers[i].cores.cols};
    if (layers[i].is_trans){
        vertices.push_back(upleft);
        vertices.push_back(upright);
        vertices.push_back(downleft);
        vertices.push_back(downright);
        input_data_shard = ceil(layers[i].weight_size_col/layers[i].cores.rows);
    } else {
        vertices.push_back(upleft);
        vertices.push_back(downleft);
        vertices.push_back(upright);
        vertices.push_back(downright);
        input_data_shard = ceil(layers[i].weight_size_col/layers[i].cores.cols);
    }
    vector<int> dists2DRAM;
    for(int i = 0; i < vertices.size() - 1; i ++){
        int vertice_min_dist = 0;
        for (int j = 0; j < device.DRAM_locs.size() - 1; j += 2){
            CoreLoc pedal;
            int temp_dist = core_line_dist(vertices[i], device.DRAM_locs[j], device.DRAM_locs[j + 1], pedal);
            if (j == 0) {
                vertice_min_dist = temp_dist;
            } else if(vertice_min_dist > temp_dist){
                vertice_min_dist = temp_dist;
            }
        }
        dists2DRAM.push_back(vertice_min_dist);
    }
    for (int i = 0; i < dists2DRAM.size(); i += 2){
        int temp_dist = min(dists2DRAM[i], dists2DRAM[i + 1]);
        if (temp_dist > max_dist) max_dist = temp_dist;
    }
    return input_data_shard * layers[i].mem_byte_wid *  max_dist / device.BW_NoC + input_data_shard * layers[i].mem_byte_wid / device.BW_DRAM; 
}

float NeuralNetwork::forward_output2DRAM_time(int i){
    int max_dist = 0;
    float output_data_shard;
    vector<CoreLoc> vertices; // first element, first element, last element, last elememt
    CoreLoc upleft = layers[i].cores.coreloc;
    CoreLoc upright = {upleft.chip_id, upleft.row_id, upleft.col_id + layers[i].cores.cols};
    CoreLoc downleft = {upleft.chip_id, upleft.row_id + layers[i].cores.rows, upleft.col_id};
    CoreLoc downright = {upleft.chip_id, upleft.row_id + layers[i].cores.rows, upleft.col_id + layers[i].cores.cols};
    if (layers[i].is_trans){
        vertices.push_back(upleft);
        vertices.push_back(downleft);
        vertices.push_back(upright);
        vertices.push_back(downright);
        output_data_shard = ceil(layers[i].weight_size_row/layers[i].cores.cols);
    } else {
        vertices.push_back(upleft);
        vertices.push_back(upright);
        vertices.push_back(downleft);
        vertices.push_back(downright);
        output_data_shard = ceil(layers[i].weight_size_row/layers[i].cores.rows);
    }
    vector<int> dists2DRAM;
    for(int i = 0; i < vertices.size() - 1; i ++){
        int vertice_min_dist = 0;
        for (int j = 0; j < device.DRAM_locs.size() - 1; j += 2){
            CoreLoc pedal;
            int temp_dist = core_line_dist(vertices[i], device.DRAM_locs[j], device.DRAM_locs[j + 1], pedal);
            if (j == 0) {
                vertice_min_dist = temp_dist;
            } else if(vertice_min_dist > temp_dist){
                vertice_min_dist = temp_dist;
            }
        }
        dists2DRAM.push_back(vertice_min_dist);
    }
    for (int i = 0; i < dists2DRAM.size(); i += 2){
        int temp_dist = min(dists2DRAM[i], dists2DRAM[i + 1]);
        if (temp_dist > max_dist) max_dist = temp_dist;
    }
    return output_data_shard * layers[i].mem_byte_wid *  max_dist / device.BW_NoC + output_data_shard * layers[i].mem_byte_wid / device.BW_DRAM; 
}

float NeuralNetwork::backward_input_time(int i){
    PhysicalLayer rcv_layer = layers[i];
    CoreLoc rcv_core_loc = rcv_layer.cores.coreloc;
    list<int> outputs_id = layers[i].output_layers_id;
    // If the layer has no input, it's one of the last layers. Assume the input is ready.
    if (outputs_id.empty()) return 0; 

    int max_dist_output_rcv = 0, max_dist_output = 0, max_dist_rcv = 0;
    float max_transfer_time = 0;
    
    for (auto output_id = outputs_id.begin(); output_id != outputs_id.end(); output_id ++){
        PhysicalLayer output_layer = layers[*output_id];
        CoreLoc output_core_loc = output_layer.cores.coreloc;

        // size of input data and rcv data
        int output_total_size = output_layer.weight_size_col;
        float output_shard_size = output_layer.is_trans ? ceil(output_layer.weight_size_col/output_layer.cores.rows) : ceil(output_layer.weight_size_col/output_layer.cores.cols); 
        float rcv_shard_size = rcv_layer.is_trans ? ceil(rcv_layer.weight_size_row/rcv_layer.cores.cols) : ceil(rcv_layer.weight_size_row/rcv_layer.cores.rows);
        // find vertice of each layer
        bool rows_split1 = output_layer.is_trans;
        bool rows_split2 = !rcv_layer.is_trans;
        vector<CoreLoc> output_layer_vertice, rcv_layer_vertice;
        find_vertice(output_layer.cores, rcv_layer.cores, rows_split1, rows_split2, output_layer_vertice, rcv_layer_vertice);
        // communicate accross chips
        if (output_core_loc.chip_id != rcv_core_loc.chip_id) { 
            vector<int> output_dists, rcv_dists;
            // find the shortest distance from input to network port 
            for (int i = 0; i < output_layer_vertice.size() - 1; i += 2){
                int dist1, dist2;
                dist1 = core_dist(output_layer_vertice[i], device.net_port_loc);
                dist2 = core_dist(output_layer_vertice[i + 1], device.net_port_loc);
                if (dist1 > dist2){ 
                    output_dists.push_back(dist2);
                } else{
                    output_dists.push_back(dist1);
                }
            }  
            // find the longest distance from rcv to network port
            for (int i = 0; i < rcv_layer_vertice.size() - 1; i += 2){
                CoreLoc pedal1, pedal2;
                int dist1, dist2;
                dist1 = core_dist(rcv_layer_vertice[i], device.net_port_loc);
                dist2 = core_dist(rcv_layer_vertice[i + 1], device.net_port_loc);
                if (dist1 > dist2){            
                    rcv_dists.push_back(dist1);
                } else{                 
                    rcv_dists.push_back(dist2);
                }
            }     
            // find the longest distance
            for (int i = 0; i < 2; i++){
                int temp_dist_output_rcv, temp_dist_output, temp_dist_rcv;

                int dist_output_rcv_outer = output_dists[i] * output_shard_size + rcv_dists[i] * min(output_shard_size, rcv_shard_size);
                temp_dist_output_rcv = dist_output_rcv_outer;
                temp_dist_output = output_dists[i];
                temp_dist_rcv = rcv_dists[i];

                int dist_output_rcv_inner;
                if (output_dists.size() > 2) {
                    dist_output_rcv_inner = output_dists[i + 2] * output_shard_size + rcv_dists[i] * min(output_shard_size, rcv_shard_size);
                    if (dist_output_rcv_inner > dist_output_rcv_outer){
                        temp_dist_output_rcv = dist_output_rcv_inner;
                        temp_dist_output = output_dists[i + 2];
                        temp_dist_rcv = rcv_dists[i];
                    }               
                } else if(rcv_dists.size() > 2){
                    dist_output_rcv_inner = output_dists[i] * output_shard_size + rcv_dists[i + 2] * min(output_shard_size, rcv_shard_size);
                    if (dist_output_rcv_inner > dist_output_rcv_outer){
                        temp_dist_output_rcv = dist_output_rcv_inner;
                        temp_dist_output = output_dists[i];
                        temp_dist_rcv =  rcv_dists[i + 2];
                    }       
                }
                if (temp_dist_output_rcv > max_dist_output_rcv) {
                    max_dist_output_rcv = temp_dist_output_rcv;
                    max_dist_output = temp_dist_output;
                    max_dist_rcv = temp_dist_rcv;   
                }
            } 
            float temp_transfer_time = ( max_dist_output * output_shard_size + max_dist_rcv * min(output_shard_size, rcv_shard_size)) * rcv_layer.mem_byte_wid / min(device.BW_NoC, device.BW_net) ;
            if (temp_transfer_time > max_transfer_time)  max_transfer_time = temp_transfer_time;
        } else {
            // find connect points
            vector<CoreLoc> connect_points;
            bool is_connected = find_connect_points(output_layer.cores, rcv_layer.cores, connect_points);
            // TODO : what if these layers are on the same chip, but have no direct connection? 
            if (!is_connected) return -1;    
            // CoreLoc input_start_outer_pedal, input_end_outer_pedal, input_start_inner_pedal, input_end_inner_pedal, rcv_start_outer_pedal, rcv_end_outer_pedal, rcv_start_inner_pedal, rcv_end_inner_pedal;
            vector<CoreLoc> output_pedals;
            vector<CoreLoc> rcv_pedals;
            // int input_start_outer_dist, input_end_outer_dist,  input_start_inner_dist,input_end_inner_dist, rcv_start_outer_dist, rcv_end_outer_dist, rcv_start_inner_dist, rcv_end_inner_dist;
            vector<int> output_dists;
            vector<int> rcv_dists;
            // 3. calculate the 2/4 shortest distances from input to connection points       
            for (int i = 0; i < output_layer_vertice.size() - 1; i+=2){
                CoreLoc pedal1, pedal2;
                int dist1, dist2;
                dist1 = core_line_dist(output_layer_vertice[i], connect_points[0], connect_points[2], pedal1);
                dist2 = core_line_dist(output_layer_vertice[i + 1], connect_points[0], connect_points[2], pedal2);
                if (dist1 > dist2){
                    output_pedals.push_back(pedal2);
                    output_dists.push_back(dist2);
                } else{
                    output_pedals.push_back(pedal1);
                    output_dists.push_back(dist1);
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
                int temp_dist_output_rcv, temp_dist_output, temp_dist_rcv;

                int dist_output_rcv_outer = output_dists[i] + core_dist(output_pedals[i], rcv_pedals[i]) + rcv_dists[i];
                temp_dist_output_rcv = dist_output_rcv_outer;
                temp_dist_output = output_dists[i];
                temp_dist_rcv = core_dist(output_pedals[i], rcv_pedals[i]) + rcv_dists[i];

                int dist_output_rcv_inner;
                if (output_dists.size() > 2) {
                    dist_output_rcv_inner = output_dists[i + 2] + core_dist(output_pedals[i + 2], rcv_pedals[i]) + rcv_dists[i];
                    if (dist_output_rcv_inner > dist_output_rcv_outer){
                        temp_dist_output_rcv = dist_output_rcv_inner;
                        temp_dist_output = output_dists[i + 2];
                        temp_dist_rcv = core_dist(output_pedals[i + 2], rcv_pedals[i]) + rcv_dists[i];
                    }               
                } else if(rcv_dists.size() > 2){
                    dist_output_rcv_inner = output_dists[i] + core_dist(output_pedals[i], rcv_pedals[i + 2]) + rcv_dists[i + 2];
                    if (dist_output_rcv_inner > dist_output_rcv_outer){
                        temp_dist_output_rcv = dist_output_rcv_inner;
                        temp_dist_output = output_dists[i];
                        temp_dist_rcv = core_dist(output_pedals[i], rcv_pedals[i + 2]) + rcv_dists[i + 2];
                    }       
                }
                if (temp_dist_output_rcv > max_dist_output_rcv) {
                    max_dist_output_rcv = temp_dist_output_rcv;
                    max_dist_output = temp_dist_output;
                    max_dist_rcv = temp_dist_rcv;   
                }
            } 
            float temp_transfer_time = ( max_dist_output * output_shard_size + max_dist_rcv * min(output_shard_size, rcv_shard_size)) * rcv_layer.mem_byte_wid / device.BW_NoC;
            if (temp_transfer_time > max_transfer_time)  max_transfer_time = temp_transfer_time;
        }    
    }
    return max_transfer_time;
}

float NeuralNetwork::backward_original_output_time(int i){
    int max_dist = 0;
    float output_data_shard;
    vector<CoreLoc> vertices; // first element, first element, last element, last elememt
    CoreLoc upleft = layers[i].cores.coreloc;
    CoreLoc upright = {upleft.chip_id, upleft.row_id, upleft.col_id + layers[i].cores.cols};
    CoreLoc downleft = {upleft.chip_id, upleft.row_id + layers[i].cores.rows, upleft.col_id};
    CoreLoc downright = {upleft.chip_id, upleft.row_id + layers[i].cores.rows, upleft.col_id + layers[i].cores.cols};
    if (layers[i].is_trans){
        vertices.push_back(upleft);
        vertices.push_back(downleft);
        vertices.push_back(upright);
        vertices.push_back(downright);
        output_data_shard = ceil(layers[i].weight_size_row/layers[i].cores.cols);
    } else {
        vertices.push_back(upleft);
        vertices.push_back(upright);
        vertices.push_back(downleft);
        vertices.push_back(downright);
        output_data_shard = ceil(layers[i].weight_size_row/layers[i].cores.rows);
    }
    vector<int> dists2DRAM;
    for(int i = 0; i < vertices.size() - 1; i ++){
        int vertice_min_dist = 0;
        for (int j = 0; j < device.DRAM_locs.size() - 1; j += 2){
            CoreLoc pedal;
            int temp_dist = core_line_dist(vertices[i], device.DRAM_locs[j], device.DRAM_locs[j + 1], pedal);
            if (j == 0) {
                vertice_min_dist = temp_dist;
            } else if(vertice_min_dist > temp_dist){
                vertice_min_dist = temp_dist;
            }
        }
        dists2DRAM.push_back(vertice_min_dist);
    }
    for (int i = 0; i < dists2DRAM.size(); i += 2){
        int temp_dist = max(dists2DRAM[i], dists2DRAM[i + 1]);
        if (temp_dist > max_dist) max_dist = temp_dist;
    }
    return output_data_shard * layers[i].mem_byte_wid *  max_dist / device.BW_NoC + output_data_shard * layers[i].mem_byte_wid / device.BW_DRAM; 
}

float NeuralNetwork::backward_original_input_time(int i){
    int max_dist = 0;
    float input_data_shard;
    vector<CoreLoc> vertices; // first element, first element, last element, last elememt
    CoreLoc upleft = layers[i].cores.coreloc;
    CoreLoc upright = {upleft.chip_id, upleft.row_id, upleft.col_id + layers[i].cores.cols};
    CoreLoc downleft = {upleft.chip_id, upleft.row_id + layers[i].cores.rows, upleft.col_id};
    CoreLoc downright = {upleft.chip_id, upleft.row_id + layers[i].cores.rows, upleft.col_id + layers[i].cores.cols};
    if (layers[i].is_trans){
        vertices.push_back(upleft);
        vertices.push_back(upright);
        vertices.push_back(downleft);
        vertices.push_back(downright);
        input_data_shard = ceil(layers[i].weight_size_col/layers[i].cores.rows);
    } else {
        vertices.push_back(upleft);
        vertices.push_back(downleft);
        vertices.push_back(upright);
        vertices.push_back(downright);
        input_data_shard = ceil(layers[i].weight_size_col/layers[i].cores.cols);
    }
    vector<int> dists2DRAM;
    for(int i = 0; i < vertices.size() - 1; i ++){
        int vertice_min_dist = 0;
        for (int j = 0; j < device.DRAM_locs.size() - 1; j += 2){
            CoreLoc pedal;
            int temp_dist = core_line_dist(vertices[i], device.DRAM_locs[j], device.DRAM_locs[j + 1], pedal);
            if (j == 0) {
                vertice_min_dist = temp_dist;
            } else if(vertice_min_dist > temp_dist){
                vertice_min_dist = temp_dist;
            }
        }
        dists2DRAM.push_back(vertice_min_dist);
    }
    for (int i = 0; i < dists2DRAM.size(); i += 2){
        int temp_dist = max(dists2DRAM[i], dists2DRAM[i + 1]);
        if (temp_dist > max_dist) max_dist = temp_dist;
    }
    return input_data_shard * layers[i].mem_byte_wid *  max_dist / device.BW_NoC + input_data_shard * layers[i].mem_byte_wid / device.BW_DRAM; 
}

float NeuralNetwork::backward_non_linear_func_time(int i){
    float compute_power;
    if (layers[i].cal_byte_wid == 1) compute_power = device.core_FLOPS_fp8;
    else if (layers[i].cal_byte_wid == 2) compute_power = device.core_FLOPS_fp16;
    else return -1;

    int matrix_col_split = layers[i].is_trans ? layers[i].cores.rows : layers[i].cores.cols;
    int col_size = ceil(layers[i].weight_size_col / matrix_col_split);

    float per_flops = device.non_linear_func_back_time(layers[i].non_linear_func);
    return (float)col_size * per_flops/compute_power;
}

float NeuralNetwork::backward_weight_update_time(int i){
    if (!layers[i].has_weight) return 0;

    int matrix_row_split = layers[i].is_trans ? layers[i].cores.cols : layers[i].cores.rows;
    int matrix_col_split = layers[i].is_trans ? layers[i].cores.rows : layers[i].cores.cols;
    int row_size = ceil(layers[i].weight_size_row / matrix_row_split);
    int col_size = ceil(layers[i].weight_size_col / matrix_col_split);

    float per_flops = row_size * col_size * 2;

    float compute_power;
    if (layers[i].cal_byte_wid == 1) compute_power = device.core_FLOPS_fp8;
    else if (layers[i].cal_byte_wid == 2) compute_power = device.core_FLOPS_fp16;
    else return -1;
    return per_flops / compute_power;
    
}

float NeuralNetwork::backward_compute_time(int i){
    if (!layers[i].has_weight) return 0;

    float compute_power;
    if (layers[i].cal_byte_wid == 1) compute_power = device.core_FLOPS_fp8;
    else if (layers[i].cal_byte_wid == 2) compute_power = device.core_FLOPS_fp16;
    else return -1;

    int matrix_row_split = layers[i].is_trans ? layers[i].cores.cols : layers[i].cores.rows;
    int matrix_col_split = layers[i].is_trans ? layers[i].cores.rows : layers[i].cores.cols;
    int row_size = ceil(layers[i].weight_size_row / matrix_row_split);
    int col_size = ceil(layers[i].weight_size_col / matrix_col_split);

    float per_flops = row_size * col_size * 2 - col_size;
    return per_flops / compute_power;

}

float NeuralNetwork::backward_reduce_time(int i){
    if (!layers[i].has_weight) return 0;

    float compute_power;
    if (layers[i].cal_byte_wid == 1) compute_power = device.core_FLOPS_fp8;
    else if (layers[i].cal_byte_wid == 2) compute_power = device.core_FLOPS_fp16;
    else return -1;

    int matrix_row_split = layers[i].is_trans ? layers[i].cores.cols : layers[i].cores.rows;
    int matrix_col_split = layers[i].is_trans ? layers[i].cores.rows : layers[i].cores.cols;
    int row_size = ceil(layers[i].weight_size_row / matrix_row_split);
    int col_size = ceil(layers[i].weight_size_col / matrix_col_split);

    float compute_flops = (matrix_row_split - 1) * col_size;
    float compute_time = compute_flops/compute_power;
    float communicate_time = (matrix_row_split - 1) * col_size / device.BW_NoC;

    return max(communicate_time, compute_time);
}
