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
    CoreLoc coreloc2 = {0,8,10};
    CoreRange corerange2 = {coreloc2, 2, 2};
    find_subconnect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 2;
    point0_right = is_same_point(connect_points[0], {0,6,7});
    point1_right = is_same_point(connect_points[1], {0,8,10});

    if (!size_right || !point0_right || !point1_right ){
        cout << "fail in case 1" << endl;
        return -1;
    }

    // case 2
    connect_points.clear();
    coreloc1 = {0,3,4};
    corerange1 = {coreloc1, 4, 4}; 
    coreloc2 = {0,8,6};
    corerange2 = {coreloc2, 2, 4};
    find_subconnect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 4;
    point0_right = is_same_point(connect_points[0], {0,6,6});
    point1_right = is_same_point(connect_points[1], {0,8,6});
    point2_right = is_same_point(connect_points[2], {0,6,7});
    point3_right = is_same_point(connect_points[3], {0,8,7});

    if (!size_right || !point0_right || !point1_right || !point2_right || !point3_right ){
        cout << "fail in case 2" << endl;
        return -1;
    }

    // case 3
    connect_points.clear();
    coreloc1 = {0,3,4};
    corerange1 = {coreloc1, 4, 4}; 
    coreloc2 = {0,8,0};
    corerange2 = {coreloc2, 2, 3};
    find_subconnect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 2;
    point0_right = is_same_point(connect_points[0], {0,6,4});
    point1_right = is_same_point(connect_points[1], {0,8,2});
   

    if (!size_right || !point0_right || !point1_right  ){
        cout << "fail in case 3" << endl;
        return -1;
    }

    // case 4
    connect_points.clear();
    coreloc1 = {0,3,4};
    corerange1 = {coreloc1, 4, 4}; 
    coreloc2 = {0,4,9};
    corerange2 = {coreloc2, 2, 2};
    find_subconnect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 4;
    point0_right = is_same_point(connect_points[0], {0,4,7});
    point1_right = is_same_point(connect_points[1], {0,4,9});
    point2_right = is_same_point(connect_points[2], {0,5,7});
    point3_right = is_same_point(connect_points[3], {0,5,9});

    if (!size_right || !point0_right || !point1_right || !point2_right || !point3_right ){
        cout << "fail in case 4" << endl;
        return -1;
    }

    // case 5
    connect_points.clear();
    coreloc1 = {0,3,4};
    corerange1 = {coreloc1, 4, 4}; 
    coreloc2 = {0,0,1};
    corerange2 = {coreloc2, 2, 9};
    find_subconnect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 4;
    point0_right = is_same_point(connect_points[0], {0,3,4});
    point1_right = is_same_point(connect_points[1], {0,1,4});
    point2_right = is_same_point(connect_points[2], {0,3,7});
    point3_right = is_same_point(connect_points[3], {0,1,7});

    if (!size_right || !point0_right || !point1_right || !point2_right || !point3_right ){
        cout << "fail in case 5" << endl;
        return -1;
    }

    // case 6
    connect_points.clear();
    coreloc1 = {0,3,4};
    corerange1 = {coreloc1, 4, 4}; 
    coreloc2 = {0,0,2};
    corerange2 = {coreloc2, 2, 3};
    find_subconnect_points(corerange1, corerange2,connect_points);

    size_right = connect_points.size() == 2;
    point0_right = is_same_point(connect_points[0], {0,3,4});
    point1_right = is_same_point(connect_points[1], {0,1,4});
   

    if (!size_right || !point0_right || !point1_right  ){
        cout << "fail in case 6" << endl;
        return -1;
    }

    return 0;
}