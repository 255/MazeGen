#include "GridGraph.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    unsigned long height, width;
    if (argc == 3)
        height = width = strtoul(argv[2], nullptr, 10);
    else if (argc == 4) {
        height = strtoul(argv[2], nullptr, 10);
        width  = strtoul(argv[3], nullptr, 10);
    } else {
        cout << "USAGE:\n\t" << argv[0] << " filename height width" << endl;        
        return 1;
    }
    
    unordered_set<const Edge*> edges;
    GridGraph graph(height, width);
    graph.prims(edges);
    
    std::ofstream file(argv[1], std::ofstream::binary);

    graph.printWithEdges(file, edges);
        
    file.close();
    
    return 0;
}