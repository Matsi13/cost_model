#include "route_helper.h"
#include <vector>
#include <iostream>
using namespace std;

int main(){
   
   
    bool size1, size2;
    bool vertice11, vertice12, vertice13, vertice14, vertice15, vertice16, vertice17, vertice18, vertice21, vertice22, vertice23, vertice24, vertice25, vertice26, vertice27, vertice28;
    bool success;

    // case 1

    CoreLoc coreloc1 = {0,1,1};
    CoreLoc coreloc2 = {0,0,6};

    CoreRange corerange1 = {coreloc1, 5, 5};
    CoreRange corerange2 = {coreloc2, 8, 2};

    bool rows_split1 = true;
    bool rows_split2 = true;

    vector<CoreLoc> layer1_vertice;
    vector<CoreLoc> layer2_vertice;

    find_vertice(corerange1, corerange2, rows_split1, rows_split2, layer1_vertice, layer2_vertice);

    size1 = layer1_vertice.size() == 4;
    size2 = layer2_vertice.size() == 8;

    vertice11 = is_same_point(layer1_vertice[0], {0,1,1});
    vertice12 = is_same_point(layer1_vertice[1], {0,1,5});
    vertice13 = is_same_point(layer1_vertice[2], {0,5,1});
    vertice14 = is_same_point(layer1_vertice[3], {0,5,5});

    vertice21 = is_same_point(layer2_vertice[0], {0,0,6});
    vertice22 = is_same_point(layer2_vertice[1], {0,0,7});
    vertice23 = is_same_point(layer2_vertice[2], {0,7,6});
    vertice24 = is_same_point(layer2_vertice[3], {0,7,7});
    vertice25 = is_same_point(layer2_vertice[4], {0,1,6});
    vertice26 = is_same_point(layer2_vertice[5], {0,1,7});
    vertice27 = is_same_point(layer2_vertice[6], {0,6,6});
    vertice28 = is_same_point(layer2_vertice[7], {0,6,7});

    success = size1 && size2 && vertice11 && vertice12 && vertice13 && vertice14 && vertice21 && vertice22 & vertice23 && vertice24 && vertice25 && vertice26 && vertice27 && vertice28;

    if (!success) {
        cout << "fail in case 1" << endl;
        return -1;
    }

    // case 2

    coreloc1 = {0,1,1};
    coreloc2 = {0,0,6};

    corerange1 = {coreloc1, 5, 5};
    corerange2 = {coreloc2, 8, 2};

    rows_split1 = true;
    rows_split2 = false;

    layer1_vertice.clear();
    layer2_vertice.clear();

    find_vertice(corerange1, corerange2, rows_split1, rows_split2, layer1_vertice, layer2_vertice);

    size1 = layer1_vertice.size() == 8;
    size2 = layer2_vertice.size() == 4;

    vertice11 = is_same_point(layer1_vertice[0], {0,1,1});
    vertice12 = is_same_point(layer1_vertice[1], {0,1,5});
    vertice13 = is_same_point(layer1_vertice[2], {0,5,1});
    vertice14 = is_same_point(layer1_vertice[3], {0,5,5});
    vertice15 = is_same_point(layer1_vertice[4], {0,2,1});
    vertice16 = is_same_point(layer1_vertice[5], {0,2,5});
    vertice17 = is_same_point(layer1_vertice[6], {0,4,1});
    vertice18 = is_same_point(layer1_vertice[7], {0,4,5});

    vertice21 = is_same_point(layer2_vertice[0], {0,0,6});
    vertice22 = is_same_point(layer2_vertice[1], {0,0,7});
    vertice23 = is_same_point(layer2_vertice[2], {0,7,6});
    vertice24 = is_same_point(layer2_vertice[3], {0,7,7});
    

    success = size1 && size2 && vertice11 && vertice12 && vertice13 && vertice14 && vertice21 && vertice22 & vertice23 && vertice24 && vertice15 && vertice16 && vertice17 && vertice18;

    if (!success) {
        cout << "fail in case 2" << endl;
        return -1;
    }

    // case 3

    coreloc1 = {0,1,1};
    coreloc2 = {0,0,6};

    corerange1 = {coreloc1, 5, 5};
    corerange2 = {coreloc2, 8, 2};

    rows_split1 = false;
    rows_split2 = false;

    layer1_vertice.clear();
    layer2_vertice.clear();

    find_vertice(corerange1, corerange2, rows_split1, rows_split2, layer1_vertice, layer2_vertice);

    size1 = layer1_vertice.size() == 8;
    size2 = layer2_vertice.size() == 4;

    vertice11 = is_same_point(layer1_vertice[0], {0,1,1});
    vertice12 = is_same_point(layer1_vertice[1], {0,5,1});
    vertice13 = is_same_point(layer1_vertice[2], {0,1,5});
    vertice14 = is_same_point(layer1_vertice[3], {0,5,5});
    vertice15 = is_same_point(layer1_vertice[4], {0,1,2});
    vertice16 = is_same_point(layer1_vertice[5], {0,5,2});
    vertice17 = is_same_point(layer1_vertice[6], {0,1,5});
    vertice18 = is_same_point(layer1_vertice[7], {0,5,4});

    vertice21 = is_same_point(layer2_vertice[0], {0,0,6});
    vertice22 = is_same_point(layer2_vertice[1], {0,0,7});
    vertice23 = is_same_point(layer2_vertice[2], {0,7,6});
    vertice24 = is_same_point(layer2_vertice[3], {0,7,7});
    

    success = size1 && size2 && vertice11 && vertice12 && vertice13 && vertice14 && vertice21 && vertice22 & vertice23 && vertice24 && vertice15 && vertice16 && vertice17 && vertice18;

    if (!success) {
        cout << "fail in case 3" << endl;
        return -1;
    }

    // case 4

    coreloc1 = {0,1,1};
    coreloc2 = {0,0,6};

    corerange1 = {coreloc1, 5, 5};
    corerange2 = {coreloc2, 8, 2};

    rows_split1 = false;
    rows_split2 = true;

    layer1_vertice.clear();
    layer2_vertice.clear();

    find_vertice(corerange1, corerange2, rows_split1, rows_split2, layer1_vertice, layer2_vertice);

    size1 = layer1_vertice.size() == 4;
    size2 = layer2_vertice.size() == 8;

    vertice11 = is_same_point(layer1_vertice[0], {0,1,1});
    vertice12 = is_same_point(layer1_vertice[1], {0,5,1});
    vertice13 = is_same_point(layer1_vertice[2], {0,1,5});
    vertice14 = is_same_point(layer1_vertice[3], {0,5,5});

    vertice21 = is_same_point(layer2_vertice[0], {0,0,6});
    vertice22 = is_same_point(layer2_vertice[1], {0,0,7});
    vertice23 = is_same_point(layer2_vertice[2], {0,7,6});
    vertice24 = is_same_point(layer2_vertice[3], {0,7,7});
    vertice25 = is_same_point(layer2_vertice[4], {0,1,6});
    vertice26 = is_same_point(layer2_vertice[5], {0,1,7});
    vertice27 = is_same_point(layer2_vertice[6], {0,6,6});
    vertice28 = is_same_point(layer2_vertice[7], {0,6,7});

    success = size1 && size2 && vertice11 && vertice12 && vertice13 && vertice14 && vertice21 && vertice22 & vertice23 && vertice24 && vertice25 && vertice26 && vertice27 && vertice28;

    if (!success) {
        cout << "fail in case 4" << endl;
        return -1;
    }

    return 0;
}

