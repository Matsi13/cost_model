#include <iostream>

#include <vector>
#include "route_helper.h"

using namespace std;

int main(){
    
    vector<CoreLoc> connect_points;
    bool size_right, point0_right, point1_right, point2_right, point3_right; 

    // case 1
    CoreLoc coreloc1 = {0,3,4};
    CoreRange corerange1 = {coreloc1, 4, 4}; 
    CoreLoc coreloc2 = {0,7,2};
    CoreRange corerange2 = {coreloc2, 3, 3};
    find_connect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 4;
    point0_right = is_same_point(connect_points[0], {0,6,4});
    point1_right = is_same_point(connect_points[1], {0,7,4});

    if (!size_right || !point0_right || !point1_right ){
        cout << "fail in case 1" << endl;
        return -1;
    }

    // case 2
    connect_points.clear();
    coreloc1 = {0,3,4};
    corerange1 = {coreloc1, 4, 4}; 
    coreloc2 = {0,7,5};
    corerange2 = {coreloc2, 3, 3};
    find_connect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 4;
    point0_right = is_same_point(connect_points[0], {0,6,5});
    point1_right = is_same_point(connect_points[1], {0,7,5});
    point2_right = is_same_point(connect_points[2], {0,6,7});
    point3_right = is_same_point(connect_points[3], {0,7,7});

    if (!size_right || !point0_right || !point1_right || !point2_right || !point3_right ){
        cout << "fail in case 2" << endl;
        return -1;
    }

    // case 3
    connect_points.clear();
    coreloc1 = {0,3,4};
    corerange1 = {coreloc1, 4, 4}; 
    coreloc2 = {0,5,8};
    corerange2 = {coreloc2, 5, 1};
    find_connect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 4;
    point0_right = is_same_point(connect_points[0], {0,5,7});
    point1_right = is_same_point(connect_points[1], {0,5,8});
    point2_right = is_same_point(connect_points[2], {0,6,7});
    point3_right = is_same_point(connect_points[3], {0,6,8});

    if (!size_right || !point0_right || !point1_right || !point2_right || !point3_right ){
        cout << "fail in case 3" << endl;
        return -1;
    }

    // case 4
    connect_points.clear();
    coreloc1 = {0,3,4};
    corerange1 = {coreloc1, 4, 4}; 
    coreloc2 = {0,5,8};
    corerange2 = {coreloc2, 1, 3};
    find_connect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 4;
    point0_right = is_same_point(connect_points[0], {0,5,7});
    point1_right = is_same_point(connect_points[1], {0,5,8});

    if (!size_right || !point0_right || !point1_right ){
        cout << "fail in case 4" << endl;
        cout << connect_points[0].chip_id <<" " << connect_points[0].row_id <<" " << connect_points[0].col_id <<" " << endl;
        cout << connect_points[1].chip_id <<" " << connect_points[1].row_id <<" " << connect_points[1].col_id <<" " << endl;
        return -1;
    }
    
    return 0;
}