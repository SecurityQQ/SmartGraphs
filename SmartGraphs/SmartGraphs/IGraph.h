//
//  IGraph.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 02.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef IGraph_h
#define IGraph_h

#include <vector>
#include <queue>
#include "Edge.h"

template <class EdgeInfo>
class IGraph {
    
public:
    
    typedef std::vector<std::shared_ptr<Edge<EdgeInfo>>> EdgesCollection;
    
    virtual ~IGraph() {}
    
    virtual void addEdge(const VertexType from, const VertexType to, std::shared_ptr<EdgeInfo> info = nullptr) = 0;
    virtual void removeEdge(const VertexType from, const VertexType to) = 0;
    virtual void changeEdge(const VertexType from, const VertexType to, std::shared_ptr<EdgeInfo> info = nullptr) = 0;
    virtual const std::shared_ptr<Edge<EdgeInfo>> getEdge(const VertexType from, const VertexType to) const = 0; // should return nullptr if edge is not exists
    virtual const bool edgeExists(const VertexType from, const VertexType to) const { return getEdge(from, to) != nullptr; }
    virtual const VertexType vertexCount() const = 0;
    virtual const VertexType vertexCapacity() const = 0;
    virtual const EdgesCollection getNextEdges(const VertexType vertex) const = 0;
    virtual const EdgesCollection getPrevEdges(const VertexType vertex) const = 0;
    virtual std::shared_ptr<const std::vector<const VertexType>> getAllVertexes() const = 0;
    
    void BFS(const VertexType start, const std::function<void(const std::shared_ptr<Edge<EdgeInfo>> edge)> completionBlock) const;
};


template <class EdgeInfo>
void IGraph<EdgeInfo>:: BFS(const VertexType start, const std::function<void(const std::shared_ptr<Edge<EdgeInfo>> edge)> completionBlock) const {
    std::queue<VertexType> vertexQueue;
    std::vector<VertexType> used(vertexCount());
    vertexQueue.push(start);
    
    while (!vertexQueue.empty()) {
        VertexType currentVertex = vertexQueue.front();
        vertexQueue.pop();
        if (!used[currentVertex]) {
            used[currentVertex] = 1;
            auto nextEdges = getNextEdges(currentVertex);
            for (auto it = nextEdges.begin(); it != nextEdges.end(); ++it) {
                vertexQueue.push((*it)->to());
                completionBlock(*it);
            }
        }
    }
}



#endif /* IGraph_h */
