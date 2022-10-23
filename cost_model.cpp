#include <iostream>
#include "layer.h"
using namespace std;
int main(){
    PhysicalLayer layer1;
    cout << layer1.layer_id << endl;
    cout << layer1.non_linear_func << endl;
    cout << layer1.device.BW_NoC << endl;
    return 0;
}