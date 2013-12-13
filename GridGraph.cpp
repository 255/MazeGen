#include "GridGraph.h"

GridGraph::GridGraph(size_t h, size_t w) : height(h), width(w) {
    count = h * w;
    
    // make a 2D matrix of Node*
    grid = new Node**[height];
    
    for (size_t i = 0; i < height; ++i)
        grid[i] = new Node*[width];
    
    for (size_t r = 0; r < height; ++r)
    for (size_t c = 0; c < width; ++c)
        grid[r][c] = new Node();

    // Add edges: THIS IS VERY INEFFICIENT! Attempts to add bunches of duplicate edges.
    std::minstd_rand0 randy((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    for (size_t r = 0; r < height; ++r)
    for (size_t c = 0; c < width; ++c) {
        if (r > 0)
            addEdge(grid[r][c], grid[r-1][c], randy());
        if (r < height-1)                            
            addEdge(grid[r][c], grid[r+1][c], randy());
        if (c > 0)                                   
            addEdge(grid[r][c], grid[r][c-1], randy());
        if (c < width-1)                             
            addEdge(grid[r][c], grid[r][c+1], randy());
    }
}

void GridGraph::addEdge(Node* node1, Node* node2, long weight) {
    // add if edge doesn't exist
    if (!edgeBtwn(node1, node2)) {
        auto new_edge(edges.emplace(node1, node2, weight));
        node1->addEdge(&(*new_edge));
        node2->addEdge(&(*new_edge));
    }
}

void GridGraph::prims(std::unordered_set<const Edge*>& result) {
    // initialize to grid[0][0]
    unordered_set<const Node*> added;
    added.insert(grid[0][0]);
    multiset<const Edge*, Edge::EdgePtrComp> edges_pool; // can have duplicate weights
    edges_pool.insert(grid[0][0]->getEdges().cbegin(), grid[0][0]->getEdges().cend());

    std::list<multiset<const Edge*, Edge::EdgePtrComp>::iterator> delete_these;

    while (added.size() < count) {
        // run through available edges smallest to biggest, try in that order
        for (auto edge_iter = edges_pool.begin(); edge_iter != edges_pool.end(); ++edge_iter) {
            const Edge* edge = *edge_iter;
            // make sure both nodes haven't been added
            if (!(added.count(edge->getNode1()) && added.count(edge->getNode2()))) {
                const Node* latest_node;
                if (added.count(edge->getNode1()))
                    latest_node = edge->getNode2();
                else
                    latest_node = edge->getNode1();
                
                // add the node we found
                added.insert(latest_node);
                edges_pool.insert(latest_node->getEdges().cbegin(), latest_node->getEdges().cend());

                // add the edge to the tree
                result.insert(edge);

                // remove the edge from the pool: it's used now
                edges_pool.erase(edge_iter);
                break;
            } else // remove edges that connect two nodes in the tree already
               delete_these.push_back(edge_iter);
        }
        
        for (auto bad_edge_iter : delete_these)
            edges_pool.erase(bad_edge_iter);

        delete_these.clear();
        
        //std::cout << "Added " << added.size() << " nodes. Currently have " << edges_pool.size() << " edges in pool\n";
    }
}

bool GridGraph::edgeBtwn(const Node* node1, const Node* node2, const unordered_set<const Edge*>& edges) {
    // Assumes these are NOT directed edges of course
    for (auto edge : node1->getEdges())
        if (edge->getNode1() == node2 || edge->getNode2() == node2)
            return (bool)edges.count(edge);

    return false;
}

bool GridGraph::edgeBtwn(const Node* node1, const Node* node2) const {
    // Assumes these are NOT directed edges of course
    for (auto edge : node1->getEdges())
        if (edge->getNode1() == node2 || edge->getNode2() == node2)
            return true;

    return false;
}

size_t GridGraph::printWithEdges(std::ostream& os, const unordered_set<const Edge*>& edges, const char wall[], int char_len) {
    // alternate btwn nodes (' ') and edges (walls or ' ')
    bool row_node = true, col_node = true;
    size_t count = 0;
    
    for (size_t i = 0; i < width*2+1; ++i)
        os.write(wall, char_len); // enclose the maze in walls
    
    os << '\n';
    
    for (size_t i = 0; i < height; i += row_node) {
        os.write(wall, char_len);
        
        for (size_t j = 0; j < width; j += col_node) {
            if (col_node && row_node) {
                os << ' '; // Nodes are always ' '
            } else {
                bool edge_btwn = false;

                if (row_node && j < width-1)
                    edge_btwn |= edgeBtwn(grid[i][j], grid[i][j+1], edges);
                else if (col_node && i < height-1)
                    edge_btwn |= edgeBtwn(grid[i][j], grid[i+1][j], edges);

                if (edge_btwn) {
                    count++;
                    os << ' ';
                } else
                    os.write(wall, char_len); //os << '#';
            }

            col_node = !col_node;
        }

        row_node = !row_node;
        os << '\n';
    }

    return count;
}