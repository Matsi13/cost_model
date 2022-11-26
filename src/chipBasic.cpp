#include "chipBasic.h"
#include <vector>
#include <string>
using namespace std;

ChipBasic::ChipBasic(int chipid, int locrow, int loccol, CoreLoc netportloc, float bwnet, vector<CoreLoc> DRAMlocs, vector<float> DRAMsizes, float BWDRAM, int numcorerow, int numcorecol,float bwnoc, float coreflopsfp8, float coreflopsfp16, float sizesram){
    chip_id = chipid;
    loc_row = locrow;
    loc_col = loccol;

    net_port_loc = netportloc;
    BW_net = bwnet;

    for (int i = 0; i < DRAMlocs.size(); i++){
        DRAM_locs.push_back(DRAMlocs[i]);
    }
    for (int i = 0; i < DRAMsizes.size(); i++){
        DRAM_sizes.push_back(DRAMsizes[i]);
    }
    BW_DRAM = BWDRAM;
    
    num_core_row = numcorerow;
    num_core_col = numcorecol;
    BW_NoC = bwnoc;
    core_FLOPS_fp8 = coreflopsfp8;
    core_FLOPS_fp16 = coreflopsfp16;
    size_SRAM = sizesram;
}

ChipBasic::ChipBasic(){
    chip_id = 0;
    loc_row = 0;
    loc_col = 0;

    net_port_loc = {0, 0, 0};
    BW_net = 400e9;

    DRAM_locs.push_back({0,0,0});
    DRAM_locs.push_back({0,0,11});
    DRAM_locs.push_back({0,9,0});
    DRAM_locs.push_back({0,9,11});
    
    DRAM_sizes.push_back(6e9);
    DRAM_sizes.push_back(6e9);

    BW_DRAM = 1e9;

    num_core_row = 10;
    num_core_col = 12;
    BW_NoC = 192e9;
    core_FLOPS_fp8 = 110e12/120;
    core_FLOPS_fp16 = 430e12/120;
    size_SRAM = 1e6;
}

float ChipBasic::non_linear_func_time(string func_name){
    if (func_name == "ReLU") return 1;
    if (func_name == "sigmoid") return 3;
    if (func_name == "tanh") return 5;
    else return 2;
}

float ChipBasic::non_linear_func_back_time(string func_name){
    if (func_name == "ReLU") return 1;
    if (func_name == "sigmoid") return 4;
    if (func_name == "tanh") return 5;
    else return 2;
}