#ifndef ROUTE_HELPER_H
#define ROUTE_HELPER_H

#include <vector>
using namespace std;
struct CoreLoc{
    int chip_id;
    int row_id;
    int col_id;
};

struct CoreRange{
    CoreLoc coreloc;
    int rows;
    int cols;
};

bool is_same_point(CoreLoc core1, CoreLoc core2);

// calculate distance between two cores on the same chip
int core_dist(CoreLoc core1, CoreLoc core2);

// calculate distance between a core and a line. Return the distance and the pedal 
int core_line_dist(CoreLoc core1, CoreLoc line_start, CoreLoc line_end, CoreLoc & pedal);

/* 
Check if two group of cores have direct Noc connection. If true, these layers can communicate with their own Noc. Otherwise, they have to use other layer's Noc. 
Connect points are stored in order of (connection1, corerange1), (connection1, corerange2), (connection2, corerange1), (connection2, corerange2)
*/
bool find_connect_points(CoreRange corerange1, CoreRange corerange2, vector<CoreLoc> &connect_points);

/* 
if two groups of cores have no direct connection, they have to communicate via other cores. This function find the nearest points between two groups. 
Subconnect points are stored in order of (subconnect1, corerange1), (subconnect1, corerange2), (subconnect2, corerange1), (subconncet2, corerange2)
*/
bool find_subconnect_points(CoreRange corerange1, CoreRange corerange2, vector<CoreLoc> &subconnect_points);

/*
Find the vertice of each layer. They are stored in the following order:
(1st element), (1st element), (last element), (last element), 
(1st element, inner), (1st element, inner), (last element, inner), (last element, inner)
*/
void find_vertice(CoreRange corerange1, CoreRange corerange2, bool rows_split1, bool rows_split2, vector<CoreLoc> &layer1_vertice, vector<CoreLoc> &layer2_vertice);


#endif