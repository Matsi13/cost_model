#include <iostream>
#include "route_helper.h"
using namespace std;

int main(){
    CoreLoc core, line_start, line_end, pedal;
    int dist;
    bool pedal_right, dist_right;
    bool success;

    // case 1
    core = {0, 3, 1};
    line_start = {0, 5, 3};
    line_end = {0, 5, 7};
    dist = core_line_dist(core, line_start, line_end, pedal);
    pedal_right = is_same_point(pedal, {0, 5, 3});
    dist_right = dist == 4;
    success = pedal_right && dist_right;
    if (!success) {
        cout << "fail in case 1" << endl;
        return -1;
    }

    // case 2
    core = {0, 8, 5};
    line_start = {0, 5, 3};
    line_end = {0, 5, 7};
    dist = core_line_dist(core, line_start, line_end, pedal);
    pedal_right = is_same_point(pedal, {0, 5, 5});
    dist_right = dist == 3;
    success = pedal_right && dist_right;
    if (!success) {
        cout << "fail in case 2" << endl;
        return -1;
    }

    // case 3
    core = {0, 3, 10};
    line_start = {0, 5, 3};
    line_end = {0, 5, 7};
    dist = core_line_dist(core, line_start, line_end, pedal);
    pedal_right = is_same_point(pedal, {0, 5, 7});
    dist_right = dist == 5;
    success = pedal_right && dist_right;
    if (!success) {
        cout << "fail in case 3" << endl;
        return -1;
    }

    // case 4
    core = {0, 3, 4};
    line_start = {0, 3, 5};
    line_end = {0, 7, 5};
    dist = core_line_dist(core, line_start, line_end, pedal);
    pedal_right = is_same_point(pedal, {0, 3, 5});
    dist_right = dist == 1;
    success = pedal_right && dist_right;
    if (!success) {
        cout << "fail in case 4" << endl;
        return -1;
    }

    // case 5
    core = {0, 6, 8};
    line_start = {0, 3, 5};
    line_end = {0, 7, 5};
    dist = core_line_dist(core, line_start, line_end, pedal);
    pedal_right = is_same_point(pedal, {0, 6, 5});
    dist_right = dist == 3;
    success = pedal_right && dist_right;
    if (!success) {
        cout << "fail in case 5" << endl;
        return -1;
    }

    // case 6
    core = {0, 8, 5};
    line_start = {0, 3, 5};
    line_end = {0, 7, 5};
    dist = core_line_dist(core, line_start, line_end, pedal);
    pedal_right = is_same_point(pedal, {0, 7, 5});
    dist_right = dist == 1;
    success = pedal_right && dist_right;
    if (!success) {
        cout << "fail in case 6" << endl;
        return -1;
    }


    return 0;
}