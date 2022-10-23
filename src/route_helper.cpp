#include "route_helper.h"
#include <cmath>
int core_dist(CoreLoc core1, CoreLoc core2){
    // currently, we only support distance within a chip
    if(core1.chip_id != core2.chip_id) return -1; 
    else return abs(core1.row_id-core2.row_id) + abs(core1.col_id-core2.col_id);
}