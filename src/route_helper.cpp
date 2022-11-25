#include "route_helper.h"
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

bool is_same_point(CoreLoc core1, CoreLoc core2){
    if (core1.chip_id == core2.chip_id && core1.row_id == core2.row_id && core1.col_id == core2.col_id) return true;
    else return false;
}

int core_dist(CoreLoc core1, CoreLoc core2){
    // currently, we only support distance within a chip
    if(core1.chip_id != core2.chip_id) return -1; 
    else return abs(core1.row_id-core2.row_id) + abs(core1.col_id-core2.col_id);
}

bool find_connect_points(CoreRange corerange1, CoreRange corerange2, vector<CoreLoc> &connect_points){
    // cores on different chips don't have direct connection
    if (corerange1.coreloc.chip_id != corerange2.coreloc.chip_id) return false;

    int upbound1 = corerange1.coreloc.row_id;
    int upbound2 = corerange2.coreloc.row_id;
    int downbound1 = upbound1 + corerange1.rows - 1;
    int downbound2 = upbound2 + corerange2.rows - 1;
    int leftbound1 = corerange1.coreloc.col_id;
    int leftbound2 = corerange2.coreloc.col_id;
    int rightbound1 = leftbound1 + corerange1.cols - 1;
    int rightbound2 = leftbound2 + corerange2.cols - 1;
    int chip_id = corerange1.coreloc.chip_id;
    
    if (upbound1 - 1 == downbound2){
        if (leftbound1 > rightbound2 || leftbound2 > rightbound1) return false;
        int left_loc = max(leftbound1, leftbound2);
        int right_loc = min(rightbound1, rightbound2);

        CoreLoc coreloc1 = {chip_id, upbound1, left_loc};
        CoreLoc coreloc2 = {chip_id, downbound2, left_loc};
        connect_points.push_back(coreloc1);
        connect_points.push_back(coreloc2);


        if (left_loc == right_loc ) return true; // only connect via one Noc

        CoreLoc coreloc3 = {chip_id, upbound1, right_loc};
        CoreLoc coreloc4 = {chip_id, downbound2, right_loc};
        connect_points.push_back(coreloc3);
        connect_points.push_back(coreloc4);


        return true;
    }
    if (upbound2 - 1 == downbound1){
        if (leftbound2 > rightbound1 || leftbound1 > rightbound2) return false;
        int left_loc = max(leftbound1, leftbound2);
        int right_loc = min(rightbound1, rightbound2);

        CoreLoc coreloc1 = {chip_id, downbound1, left_loc};
        CoreLoc coreloc2 = {chip_id, upbound2, left_loc};
        connect_points.push_back(coreloc1);
        connect_points.push_back(coreloc2);

        // cout << connect_points[0].chip_id <<" " << connect_points[0].row_id <<" " << connect_points[0].col_id <<" " << endl;
        // cout << connect_points[1].chip_id <<" " << connect_points[1].row_id <<" " << connect_points[1].col_id <<" " << endl;

        if (left_loc == right_loc ) return true; // only connect via one Noc

        CoreLoc coreloc3 = {chip_id, downbound1, right_loc};
        CoreLoc coreloc4 = {chip_id, upbound2, right_loc};
        connect_points.push_back(coreloc3);
        connect_points.push_back(coreloc4);

        return true;
    }
    if (leftbound1 - 1 == rightbound2){
        if (downbound1 < upbound2 || downbound2 < upbound1) return false;
        int down_loc = min(downbound1, downbound2);
        int up_loc = max(upbound1, upbound2);

        CoreLoc coreloc1 = {chip_id, up_loc, leftbound1};
        CoreLoc coreloc2 = {chip_id, up_loc, rightbound2};
        connect_points.push_back(coreloc1);
        connect_points.push_back(coreloc2);

        if (up_loc == down_loc ) return true; // only connect via one Noc

        CoreLoc coreloc3 = {chip_id, down_loc, leftbound1};
        CoreLoc coreloc4 = {chip_id, down_loc, rightbound2};
        connect_points.push_back(coreloc3);
        connect_points.push_back(coreloc4);

        return true;
    }
    if (leftbound2 - 1 == rightbound1){
        if (downbound1 < upbound2 || downbound2 < upbound1) return false;
        int down_loc = min(downbound1, downbound2);
        int up_loc = max(upbound1, upbound2);

        CoreLoc coreloc1 = {chip_id, up_loc, rightbound1};
        CoreLoc coreloc2 = {chip_id, up_loc, leftbound2};
        connect_points.push_back(coreloc1);
        connect_points.push_back(coreloc2);

        if (up_loc == down_loc ) return true; // only connect via one Noc

        CoreLoc coreloc3 = {chip_id, down_loc, rightbound1};
        CoreLoc coreloc4 = {chip_id, down_loc, leftbound2};
        connect_points.push_back(coreloc3);
        connect_points.push_back(coreloc4);

        return true;
    }
    // if none of the above conditions holds, there's no connection between 1 and 2
    return false;
}

bool find_subconnect_points(CoreRange corerange1, CoreRange corerange2, vector<CoreLoc> &subconnect_points){
    // cores on different chips don't have direct connection
    if (corerange1.coreloc.chip_id != corerange2.coreloc.chip_id) return false;

    int upbound1 = corerange1.coreloc.row_id;
    int upbound2 = corerange2.coreloc.row_id;
    int downbound1 = upbound1 + corerange1.rows - 1;
    int downbound2 = upbound2 + corerange2.rows - 1;
    int leftbound1 = corerange1.coreloc.col_id;
    int leftbound2 = corerange2.coreloc.col_id;
    int rightbound1 = leftbound1 + corerange1.cols - 1;
    int rightbound2 = leftbound2 + corerange2.cols - 1;
    int chip_id = corerange1.coreloc.chip_id; 

    if (upbound2 - 1 > downbound1){
        if (leftbound2 >= rightbound1) {
            CoreLoc coreloc1 = {chip_id, downbound1, rightbound1};
            CoreLoc coreloc2 = {chip_id, upbound2, leftbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
        } else if (rightbound2 <= leftbound1){
            CoreLoc coreloc1 = {chip_id, downbound1, leftbound1};
            CoreLoc coreloc2 = {chip_id, upbound2, rightbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
        } else {
            int leftloc = max(leftbound1, leftbound2);
            int rightloc = min(rightbound1, rightbound2);
            CoreLoc coreloc1 = {chip_id, downbound1, leftloc};
            CoreLoc coreloc2 = {chip_id, upbound2, leftloc};
            CoreLoc coreloc3 = {chip_id, downbound1, rightloc};
            CoreLoc coreloc4 = {chip_id, upbound2, rightloc};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
            subconnect_points.push_back(coreloc3);
            subconnect_points.push_back(coreloc4);
        } 
        return true;
    } 
    if (downbound2 + 1 < upbound1){
        if (leftbound2 >= rightbound1) {
            CoreLoc coreloc1 = {chip_id, upbound1, rightbound1};
            CoreLoc coreloc2 = {chip_id, downbound2, leftbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);  
        } else if (rightbound2 <= leftbound1){
            CoreLoc coreloc1 = {chip_id, upbound1, leftbound1};
            CoreLoc coreloc2 = {chip_id, downbound2, rightbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
        } else {
            int leftloc = max(leftbound1, leftbound2);
            int rightloc = min(rightbound1, rightbound2);
            CoreLoc coreloc1 = {chip_id, upbound1, leftloc};
            CoreLoc coreloc2 = {chip_id, downbound2, leftloc};
            CoreLoc coreloc3 = {chip_id, upbound1, rightloc};
            CoreLoc coreloc4 = {chip_id, downbound2, rightloc};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
            subconnect_points.push_back(coreloc3);
            subconnect_points.push_back(coreloc4);
        } 
        return true;
    } 
    if (leftbound2 - 1 > rightbound1){
        if (upbound2 >= downbound1) {
            CoreLoc coreloc1 = {chip_id, downbound1, rightbound1};
            CoreLoc coreloc2 = {chip_id, upbound2, leftbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
        } else if (downbound2 <= upbound1){
            CoreLoc coreloc1 = {chip_id, upbound1, rightbound1};
            CoreLoc coreloc2 = {chip_id, downbound2, leftbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
        } else {
            int uploc = max(upbound1, upbound2);
            int downloc = min(downbound1, downbound2);
            CoreLoc coreloc1 = {chip_id, uploc, rightbound1};
            CoreLoc coreloc2 = {chip_id, uploc, leftbound2};
            CoreLoc coreloc3 = {chip_id, downloc, rightbound1};
            CoreLoc coreloc4 = {chip_id, downloc, leftbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
            subconnect_points.push_back(coreloc3);
            subconnect_points.push_back(coreloc4);
        } 
        return true;       
    }
    if (rightbound2 + 1 < leftbound1){
        if (upbound2 >= downbound1) {
            CoreLoc coreloc1 = {chip_id, downbound1, leftbound1};
            CoreLoc coreloc2 = {chip_id, upbound2, rightbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
        } else if (downbound2 <= upbound1){
            CoreLoc coreloc1 = {chip_id, upbound1, leftbound1};
            CoreLoc coreloc2 = {chip_id, downbound2, rightbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
        } else {
            int uploc = max(upbound1, upbound2);
            int downloc = min(downbound1, downbound2);
            CoreLoc coreloc1 = {chip_id, uploc, leftbound1};
            CoreLoc coreloc2 = {chip_id, uploc, rightbound2};
            CoreLoc coreloc3 = {chip_id, downloc, leftbound1};
            CoreLoc coreloc4 = {chip_id, downloc, rightbound2};
            subconnect_points.push_back(coreloc1);
            subconnect_points.push_back(coreloc2);
            subconnect_points.push_back(coreloc3);
            subconnect_points.push_back(coreloc4);
        } 
        return true;       
    }
    return false;
}

void find_vertice(CoreRange corerange1, CoreRange corerange2, bool rows_split1, bool rows_split2, vector<CoreLoc> &layer1_vertice, vector<CoreLoc> &layer2_vertice){
    int rows1 = corerange1.rows;
    int rows2 = corerange2.rows;
    int cols1 = corerange1.cols;
    int cols2 = corerange2.cols;
    
    int chip_id1 = corerange1.coreloc.chip_id;
    int row_id1 = corerange1.coreloc.row_id;
    int col_id1 = corerange1.coreloc.col_id;
    if (rows_split1) {
        layer1_vertice.push_back({chip_id1, row_id1, col_id1});
        layer1_vertice.push_back({chip_id1, row_id1, col_id1 + cols1 - 1});
        layer1_vertice.push_back({chip_id1, row_id1 + rows1 - 1, col_id1});
        layer1_vertice.push_back({chip_id1, row_id1 + rows1 - 1, col_id1 + cols1 - 1});
    } else {
        layer1_vertice.push_back({chip_id1, row_id1, col_id1});
        layer1_vertice.push_back({chip_id1, row_id1 + rows1 - 1, col_id1});
        layer1_vertice.push_back({chip_id1, row_id1, col_id1 + cols1 - 1});
        layer1_vertice.push_back({chip_id1, row_id1 + rows1 - 1, col_id1 + cols1 - 1});
    }

    int chip_id2 = corerange2.coreloc.chip_id;
    int row_id2 = corerange2.coreloc.row_id;
    int col_id2 = corerange2.coreloc.col_id;
    if (rows_split2) {
        layer2_vertice.push_back({chip_id2, row_id2, col_id2});
        layer2_vertice.push_back({chip_id2, row_id2, col_id2 + cols2 - 1});
        layer2_vertice.push_back({chip_id2, row_id2 + rows2 - 1, col_id2});
        layer2_vertice.push_back({chip_id2, row_id2 + rows2 - 1, col_id2 + cols2 - 1});
    } else {
        layer2_vertice.push_back({chip_id2, row_id2, col_id2});
        layer2_vertice.push_back({chip_id2, row_id2 + rows2 - 1, col_id2});
        layer2_vertice.push_back({chip_id2, row_id2, col_id2 + cols2 - 1});
        layer2_vertice.push_back({chip_id2, row_id2 + rows2 - 1, col_id2 + cols2 - 1});
    }

    float shards1 = rows_split1 ? rows1 : cols1;
    float shards2 = rows_split2 ? rows2 : cols2;

    if (shards1 > shards2){
        int step = ceil(shards1/shards2) - 1;
        if (rows_split1){
        // some points may coincide. As far as I'm concerned, it doesn't matter.
        layer1_vertice.push_back({chip_id1, row_id1 + step, col_id1});
        layer1_vertice.push_back({chip_id1, row_id1 + step, col_id1 + cols1 - 1});
        layer1_vertice.push_back({chip_id1, row_id1 + rows1 - 1 - step, col_id1});
        layer1_vertice.push_back({chip_id1, row_id1 + rows1 - 1 - step, col_id1 + cols1 - 1});
      
        } else {
        layer1_vertice.push_back({chip_id1, row_id1, col_id1 + step});
        layer1_vertice.push_back({chip_id1, row_id1 + rows1 - 1, col_id1 + step});
        layer1_vertice.push_back({chip_id1, row_id1, col_id1 + cols1 - 1 - step});
        layer1_vertice.push_back({chip_id1, row_id1 + rows1 - 1, col_id1 + cols1 - 1 - step});
        
        }
    } else if (shards1 < shards2){
        int step = ceil(shards2/shards1) - 1;
        if (rows_split2){
        // some points may coincide. As far as I'm concerned, it doesn't matter.
        layer2_vertice.push_back({chip_id2, row_id2 + step, col_id2});
        layer2_vertice.push_back({chip_id2, row_id2 + step, col_id2 + cols2 - 1});
        layer2_vertice.push_back({chip_id2, row_id2 + rows2 - 1 - step, col_id2});
        layer2_vertice.push_back({chip_id2, row_id2 + rows2 - 1 - step, col_id2 + cols2 - 1});
        } else {
        layer2_vertice.push_back({chip_id2, row_id2, col_id2 + step});
        layer2_vertice.push_back({chip_id2, row_id2 + rows2 - 1, col_id2 + step});
        layer2_vertice.push_back({chip_id2, row_id2, col_id2 + cols2 - 1 - step});
        layer2_vertice.push_back({chip_id2, row_id2 + rows2 - 1, col_id2 + cols2 - 1 - step});
        
        }
    }
    return;
}

int core_line_dist(CoreLoc core, CoreLoc line_start, CoreLoc line_end, CoreLoc & pedal){
    bool same_chip = (core.chip_id == line_start.chip_id) && (line_start.chip_id == line_end.chip_id);
    int dist;
    if (!same_chip ) return -1;
    pedal.chip_id = core.chip_id;
    if (is_same_point(line_start, line_end)) {
        pedal = line_start;
        dist = core_dist(core, line_start);
    } else if (line_start.row_id == line_end.row_id) {
        // horizontal line
        int leftbound = min(line_start.col_id, line_end.col_id);
        int rightbound = max(line_start.col_id, line_end.col_id);
        if (core.col_id < leftbound){
            pedal.col_id = leftbound;
            pedal.row_id = line_start.row_id;
            dist = core_dist(pedal, core);
        } else if (core.col_id > rightbound){
            pedal.col_id = rightbound;
            pedal.row_id = line_start.row_id;
            dist = core_dist(pedal, core);
        } else {
            dist = abs(core.row_id - line_start.row_id);
            pedal.row_id = line_start.row_id;
            pedal.col_id = core.col_id;
            
        }
        
    } else if (line_start.col_id == line_end.col_id){
        // vertical line
        int upbound = min(line_start.row_id, line_end.row_id);
        int downbound = max(line_start.row_id, line_end.row_id);
        if (core.row_id < upbound){
            pedal.row_id = upbound;
            pedal.col_id = line_start.col_id;
            dist = core_dist(pedal, core);
        } else if (core.row_id > downbound){
            pedal.row_id = downbound;
            pedal.col_id = line_start.col_id;
            dist = core_dist(pedal, core);
        } else {
            dist = abs(core.col_id - line_start.col_id);
            pedal.row_id = core.row_id;
            pedal.col_id = line_start.col_id;
        }

        
    } else return -1;
    return dist;
}

