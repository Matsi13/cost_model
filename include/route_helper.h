#ifndef ROUTE_HELPER_H
#define ROUTE_HELPER_H
#endif

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

int core_dist(CoreLoc core1, CoreLoc core2);