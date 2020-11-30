#pragma once

#include <vector>
#include <optional>
#include <algorithm>

// define 1 if you want to implement it
#define BONUS 0

// Edge can be constructed with syntax
// Edge e{ 1, 5, 1.89 };
struct Edge
{
    int v;
    int u;
    double weight;
};

class GraphException : public std::exception {
public:
    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override {
        return "Invalid graph operation!";
    }
};

/* Graph represents one abstract graph.
 *
 * Vertices are always numbered from 0.
 *
 * Between two vertices,  there can only be one edge.   If edges is in
 * form of  (v, v), so  both ends are  the same, e.g.   AddEdge({1, 1,
 * 0.5}) throw an exception.
 *
 * Also throw excepton if one or both vertices are out of bounds.
 *
 * Graph is not directed so (u, v) is the same edge as (v, u).
 *
 * Weights are  limited to positive  values, so 0 and  negative values
 * are forbidden.  If you encounter such weights throw an exception.
 */

class Graph
{
    // Do not modify public interface
public:
    // Construct graph with n vertices and no edges
    explicit Graph(size_t n);

    /* Will construct graph with given  edges, the vertices are from 0
     * to whatever is the highest number in the vector of edges.
     */
    explicit Graph(const std::vector<Edge>& edges);

    /* Add  edge to  graph. If  the edge  already exists,  replace the
     * weight.
     */
    void AddEdge(const Edge& edge);

    /* Same as AddEdge, but can insert  multiple edges. If one edge is
     * there more  than once,  use later one  (edge with  higher index
     * overwrites edge with lower index)
     */
    void AddEdges(const std::vector<Edge>& edges);

    /* Return  weight between  vertices  u  and v.  If  edge does  not
     * exists, behaviour is undefined.
     */
    double operator()(int u, int v) const;

    /* Return weight between vertices u and v, if edge does not exists,
     * throw an exception.
     */
    double At(int u, int v) const;

    /* Return true  if there  is an  edge between  u and  v, otherwise
     * false.
     */
    bool Connected(int u, int v) const noexcept;

    /* Return shortest path  from u to v (path with  minimal cost). If
     * there is no path return nullopt.   You can use dijkstra, or any
     * other algorithm. Path should start with u and ends with v.
     *
     * https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
     *
     * 1 BONUS point will be for  speed of this function.
     * First 5 implementations will be awarded with the bonus point.
     */
    std::optional<std::vector<int>> Path(int u, int v) const;

#if BONUS == 1
    /* Returns  minimum spanning  tree  for this  graph.  You can  use
     * kruskal's algorithm
     * https://en.wikipedia.org/wiki/Kruskal%27s_algorithm
     */
    Graph SpannigTree() const;
#endif

private:
    std::vector<std::vector<double>> graph;
    int getNextVertex(std::vector<double> distances, std::vector<bool> vertMap) const;
};

Graph::Graph(size_t n) {
    graph = std::vector<std::vector<double>>(n,std::vector<double>(n,0));
//    for (const auto& l : graph){
//        for (auto d : l)
//            std::cout << d << " ";
//        std::cout << std::endl;
//    }
}

Graph::Graph(const std::vector<Edge> &edges) {
    size_t maxVert = 0;
    for (auto& e : edges) {
        if (e.u == e.v || e.weight <= 0)
            throw GraphException();
        maxVert = e.u > maxVert ? e.u : maxVert;
        maxVert = e.v > maxVert ? e.v : maxVert;
    }
    graph = std::vector<std::vector<double>>(maxVert+1,std::vector<double>(maxVert+1,0));
    for (auto& e : edges) {
        graph[e.u][e.v] = e.weight;
        graph[e.v][e.u] = e.weight;
    }
//    for (const auto& l : graph){
//        for (auto d : l)
//            std::cout << d << " ";
//        std::cout << std::endl;
//    }

}

void Graph::AddEdge(const Edge &edge) {
    if (edge.u == edge.v || edge.weight <= 0)
        throw GraphException();
    graph[edge.u][edge.v] = edge.weight;
    graph[edge.v][edge.u] = edge.weight;
}

void Graph::AddEdges(const std::vector<Edge> &edges) {
    for (auto& e : edges){
        if (e.u == e.v || e.weight <= 0 || e.u >= graph.size() || e.v >= graph.size())
            throw GraphException();
        graph[e.u][e.v] = e.weight;
        graph[e.v][e.u] = e.weight;
    }
}

double Graph::At(int u, int v) const {
    if (u >= graph.size() || v >= graph.size() || graph[u][v] == 0)
        throw GraphException();
    return graph[u][v];
}

double Graph::operator()(int u, int v) const {
    return graph[u][v];
}

bool Graph::Connected(int u, int v) const noexcept {
    if (u >= graph.size() || v >= graph.size() || graph[u][v] == 0)
        return false;
    else
        return true;
}

std::optional<std::vector<int>> Graph::Path(int u, int v) const {
    if (u >= graph.size() || v >= graph.size())
        return std::nullopt;
    std::vector<bool> vertMap(graph.size(), false);
    std::vector<double> vertDistance(graph.size(),std::numeric_limits<double>::max());
    vertDistance[u] = 0;
    std::vector<int> backTrack(graph.size(),-1);
    std::vector<int> result;
    backTrack[u] = u;
    while (true) {
        int currVert = getNextVertex(vertDistance, vertMap);
        if (currVert == v)
            break;
        vertMap[currVert] = true;
        for (size_t i = 0; i < graph.size(); i++){
            if (graph[currVert][i] != 0 && !vertMap[i] && i != currVert
                && vertDistance[i] > vertDistance[currVert] + graph[currVert][i]){
                vertDistance[i] = vertDistance[currVert] + graph[currVert][i];
                backTrack[i] = currVert;
            }
        }
    }
    int endPoint = v;
    while (true) {
        result.push_back(endPoint);
        if (endPoint == u)
            break;
        endPoint = backTrack[endPoint];
    }
    std::reverse(result.begin(),result.end());
    return result;
}

int Graph::getNextVertex(std::vector<double> distances, std::vector<bool> vertMap) const {
    double currentMin = std::numeric_limits<double>::max();
    int minIndex;
    for(int i = 0; i < distances.size(); i++){
        if (currentMin > distances[i] && !vertMap[i]){
            currentMin = distances[i];
            minIndex = i;
        }
    }
    return minIndex;
}

