#ifndef CHIPBASIC_H
#define CHIPBASIC_H
#include <string>
#include <vector>
#include "route_helper.h"
using namespace std;

class ChipBasic{
public :
    ChipBasic(int chipid, int locrow, int loccol, CoreLoc netportloc, float bwnet, vector<CoreLoc> DRAMlocs, vector<float> DRAMsizes, float BWDRAM, int numcorerow, int numcorecol,float bwnoc, float coreflopsfp8, float coreflopsfp16, float sizesram);
    ChipBasic();
    ~ChipBasic(){};
    float non_linear_func_time(string func_name);
    float non_linear_func_back_time(string func_name);
    int chip_id;
    int loc_row;           // location of the Chip
    int loc_col;

    CoreLoc net_port_loc;
    float BW_net;          // bandwidth of Ethernet

    vector<CoreLoc> DRAM_locs;       // start1, end1, start2, end2, ……
    vector<float> DRAM_sizes;        // size of each DRAM, measured in Byte
    float BW_DRAM;          // bandwidth of each port

    int num_core_row;       // number of core rows
    int num_core_col;       // number of core cols
    float BW_NoC;           // bandwidth of onchip network, Byte/s
    float core_FLOPS_fp8;   // FLOPS per core
    float core_FLOPS_fp16;
    float size_SRAM;        // size of SRAM per core, measured in bytes



};


#endif