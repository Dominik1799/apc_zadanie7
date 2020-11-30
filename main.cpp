#include <iostream>
#include "Graph.h"

int main() {
    Edge e{0,1,4};
    Edge e1{0,3,3};
    Edge e2{2,1,1};
    Edge e3{3,2,2};
    Edge e4{2,4,3};
    Edge e5{2,5,4};
    Edge e6{4,5,5};
    std::vector<Edge> p;
    p.push_back(e);
    p.push_back(e1);
    p.push_back(e2);
    p.push_back(e3);
    p.push_back(e4);
    p.push_back(e5);
    p.push_back(e6);
    Graph varname(p);
    std::vector<int> penis = varname.Path(2,2).value();
    for (int i = 0; i < penis.size(); ++i) {
        std::cout << penis[i] << "\n";
    }
    return 0;
}
