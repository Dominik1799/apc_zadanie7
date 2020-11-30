#include <iostream>
#include "Graph.h"

int main() {
    Edge e{0,1,5};
    Edge e1{4,8,9};
    Edge e2{2,1,3};
    Edge e3{10,5,2};
    Edge e4{6,9,1};
    std::vector<Edge> p;
    p.push_back(e);
    p.push_back(e1);
    p.push_back(e2);
    p.push_back(e3);
    p.push_back(e4);
    Graph varname(p);
    Graph(5);
    varname.At(5,4);
    return 0;
}
