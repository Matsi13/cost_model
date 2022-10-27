#include "chipBasic.h"

ChipBasic::ChipBasic(int locrow, int loccol, float bwnoc, int netlocrow, int netloccol, float bwnet, bool isdramup, bool isdramdown, bool isdramleft, bool isdramright, float sizedram, float bwdram, int numportdram, int numcorerow, int numcorecol, float coreflops, float sizesram){
    loc_row = locrow;
    loc_col = loccol;
    BW_NoC = bwnoc;
    net_loc_row = netlocrow;
    net_loc_col = netloccol;
    BW_net = bwnet;
    is_DRAM_up = isdramup;
    is_DRAM_down = isdramdown;
    is_DRAM_left = isdramleft;
    is_DRAM_right = isdramright;
    size_DRAM = sizedram;
    BW_DRAM = bwdram;
    num_port_DRAM = numportdram;
    num_core_row = numcorerow;
    num_core_col = numcorecol;
    core_FLOPS = coreflops;
    size_SRAM = sizesram;
}

ChipBasic::ChipBasic(){
    loc_row = 0;
    loc_col = 0;
    BW_NoC = 192e9;
    net_loc_row = 0;
    net_loc_col = 0;
    BW_net = 400e9;
    is_DRAM_up = true;
    is_DRAM_down = true;
    is_DRAM_left = false;
    is_DRAM_right = false;
    size_DRAM = 6e9;
    BW_DRAM = 12e9;
    num_port_DRAM = 8;
    num_core_row = 10;
    num_core_col = 8;
    core_FLOPS = 1.375e12;
    size_SRAM = 1e6;
}