#ifndef CHIPBASIC_H
#define CHIPBASIC_H
#endif

class ChipBasic{
public :
    ChipBasic(int locrow, int loccol, float bwnoc, int netlocrow, int netloccol, float bwnet, bool isdramup, bool isdramdown, bool isdramleft, bool isdramright, float sizedram, float bwdram, int numportdram, int numcorerow, int numcorecol, float coreflops, float sizesram);
    ChipBasic();
    ~ChipBasic(){};
    int loc_row;           // location of the Chip
    int loc_col;
    float BW_NoC;          // bandwidth of onchip network, Byte/s
    int net_loc_row;       // location of the Ethernet port
    int net_loc_col;
    float BW_net;          // bandwidth of Ethernet
    bool is_DRAM_up;     
    bool is_DRAM_down;
    bool is_DRAM_left;
    bool is_DRAM_right;
    float size_DRAM;        // size of each DRAM, measured in Byte
    float BW_DRAM;          // total bandwidth of each DRAM. That's to say, sum up the ports, but leave each DRAM alone
    int num_port_DRAM;      // number of DRAM ports
    int num_core_row;       // number of core rows
    int num_core_col;       // number of core cols
    float core_FLOPS;
    float size_SRAM;        // size of SRAM per core, measured in bytes



};