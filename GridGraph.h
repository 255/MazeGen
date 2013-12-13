#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H

#include <iostream>
#include <chrono>
#include <random>
#include <set>
#include <unordered_set>
#include <list>
using std::set;
using std::multiset;
using std::unordered_set;

// Nodes and edges are not defined as inner classes because edges will be used 
// elsewhere, and it just makes things easier.

class Node;

/** An undirected edge. */
class Edge {
    long weight;
    Node *n1, *n2;
  
  public:
    Edge(Node *node1, Node *node2, long w) : weight(w) {
        // sort by address
        if (node1 < node2) {
            n1 = node1;
            n2 = node2;
        } else {
            n1 = node2;
            n2 = node1;
        }
    };
    
    struct EdgePtrComp {
        bool operator() (const Edge *e1, const Edge *e2) {
            return *e1 < *e2;
        }
    };

    bool pointsTo(const Node* node) const { return node == n1 || node == n2; }

    long getWeight() const { return weight; }
    Node* getNode1() const { return n1; }
    Node* getNode2() const { return n2; }
    
    bool operator< (const Edge& other) const {        
        return weight < other.weight;
    }
};

/** An Node */
class Node {
    set<const Edge*> edges; // this is way more general than it needs to be
  
  public:
    void addEdge(const Edge* edge) {
        edges.insert(edge);
    }

    const set<const Edge*>& getEdges() const {
        return edges;
    }
    
};

// The UTF-8 full block character
static const char BLOCK[3] = {char(0xE2), char(0x96), char(0x88)};

class GridGraph {
    Node ***grid;
    multiset<Edge> edges; // keep these sorted by weight (don't actually need to...) and allow duplicate weights
    size_t height, width;
    size_t count;

    /** Is there an edge btwn these two nodes? */
    bool static edgeBtwn(const Node* node1, const Node* node2, const unordered_set<const Edge*>& edges);
    
    /** Is there an edge btwn these two nodes in the graph? */
    bool edgeBtwn(const Node* node1, const Node* node2) const;
    
    /** Add edge to graph */
    void addEdge(Node* node1, Node* node2, long weight);

  public:
    /** Construct a graph with random weight edges */
    GridGraph(size_t h, size_t w);

    virtual ~GridGraph() {
        for (size_t r = 0; r < height; ++r)
        for (size_t c = 0; c < width; ++c)
            delete grid[r][c];

        for (size_t i = 0; i < height; ++i)
            delete[] grid[i];
        
        delete[] grid;
    }

    /** A naive implementation of Prim's Algorithm */
    void prims(unordered_set<const Edge*>& result);

    /** Print the grid with the given set of edges. */
    size_t printWithEdges(std::ostream& os, const unordered_set<const Edge*>& edges, const char wall[] = BLOCK, int char_len = 3);
};

#endif