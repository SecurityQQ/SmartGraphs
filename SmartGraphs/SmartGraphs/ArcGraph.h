//
//  ArcGraph.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 04.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef ArcGraph_h
#define ArcGraph_h

#include "IGraph.h"
#include <forward_list>
#include <assert.h>

template <class EdgeInfo>
class ArcGraph {
    
public:

    typedef std::forward_list<std::shared_ptr<Edge<EdgeInfo>>> EdgesCollection;

    ArcGraph(const VertexType numberOfVertexes);
    virtual ~ArcGraph() {}
    
    virtual void addEdge(const VertexType from, const VertexType to, const std::shared_ptr<EdgeInfo> info = nullptr);
    virtual void addEdge(const std::shared_ptr<Edge<EdgeInfo>> edge);
    virtual void removeEdge(const VertexType from, const VertexType to);
    virtual void removeEdge(const std::shared_ptr<Edge<EdgeInfo>> edge);
    virtual void changeEdge(const VertexType from, const VertexType to, std::shared_ptr<EdgeInfo> info = nullptr);
    virtual const std::shared_ptr<Edge<EdgeInfo>> getEdge(const VertexType from, const VertexType to) const;
    
    virtual const VertexType vertexCount() const;
    virtual const VertexType vertexCapacity() const;
    virtual const EdgesCollection getNextEdges(const VertexType vertex) const;
    virtual const EdgesCollection getPrevEdges(const VertexType vertex) const;
    virtual std::shared_ptr<const std::vector<const VertexType>> getAllVertexes() const;
    
    virtual void BFS(const VertexType start, const std::function<void(const std::shared_ptr<Edge<EdgeInfo>> edge)>);
    
    
protected:
    
    ArcGraph() {} 
    std::vector<std::forward_list<std::shared_ptr<Edge<EdgeInfo>>>> incomingEdgesToVertex;
    std::vector<std::forward_list<std::shared_ptr<Edge<EdgeInfo>>>> outcomingEdgesFromVertex;
    VertexType _vertexCount;
};

template <class EdgeInfo>
ArcGraph<EdgeInfo>:: ArcGraph(const VertexType numberOfVertexes) {
    
    incomingEdgesToVertex = std::move(std::vector<std::forward_list<std::shared_ptr<Edge<EdgeInfo>>>>(numberOfVertexes, std::forward_list<std::shared_ptr<Edge<EdgeInfo>>>()));
    outcomingEdgesFromVertex = std::move(std::vector<std::forward_list<std::shared_ptr<Edge<EdgeInfo>>>>(numberOfVertexes, std::forward_list<std::shared_ptr<Edge<EdgeInfo>>>()));
    _vertexCount = numberOfVertexes;
}

template <class EdgeInfo>
void ArcGraph<EdgeInfo>:: addEdge(const VertexType from, const VertexType to, const std::shared_ptr<EdgeInfo> info) {
    assert((from < incomingEdgesToVertex.size()) && (to < outcomingEdgesFromVertex.size()));
    incomingEdgesToVertex[to].push_front(std::shared_ptr<Edge<EdgeInfo>>(new Edge<EdgeInfo>(from, to, *info)));
    outcomingEdgesFromVertex[from].push_front(std::shared_ptr<Edge<EdgeInfo>>(new Edge<EdgeInfo>(from, to, *info)));
}

template <class EdgeInfo>
void ArcGraph<EdgeInfo>::addEdge(const std::shared_ptr<Edge<EdgeInfo>> edge) {
    auto from = edge->from();
    auto to = edge->to();
    assert((from < incomingEdgesToVertex.size()) && (to < outcomingEdgesFromVertex.size()));
    incomingEdgesToVertex[to].push_front(edge);
    outcomingEdgesFromVertex[from].push_front(edge);
}

template <class EdgeInfo>
void ArcGraph<EdgeInfo>:: removeEdge(const VertexType from, const VertexType to) {
    assert((from < incomingEdgesToVertex.size()) && (to < outcomingEdgesFromVertex.size()));
    incomingEdgesToVertex[to].remove_if([&from](std::shared_ptr<Edge<EdgeInfo>> edge) {
        return edge->from() == from;
    });
    outcomingEdgesFromVertex[from].remove_if([&to](std::shared_ptr<Edge<EdgeInfo>> edge) {
        return edge->to() == to;
    });
}

template <class EdgeInfo>
void ArcGraph<EdgeInfo>:: removeEdge(const std::shared_ptr<Edge<EdgeInfo>> edge) {
    incomingEdgesToVertex[edge->to()].remove(edge);
    outcomingEdgesFromVertex[edge->from()].remove(edge);
}

template <class EdgeInfo>
void ArcGraph<EdgeInfo>:: changeEdge(const VertexType from, const VertexType to, std::shared_ptr<EdgeInfo> info) {
    assert(false);
}


template <class EdgeInfo>
const std::shared_ptr<Edge<EdgeInfo>> ArcGraph<EdgeInfo>:: getEdge(const VertexType from, const VertexType to) const {
    for (auto it = incomingEdgesToVertex[to].begin(); it != incomingEdgesToVertex[to].end(); ++it) {
        if ((*it)->from() == from) {
            return *it;
        }
    }
    return nullptr;
}

template <class EdgeInfo>
const VertexType ArcGraph<EdgeInfo>:: vertexCount() const {
    return _vertexCount;
}

template <class EdgeInfo>
const VertexType ArcGraph<EdgeInfo>:: vertexCapacity() const {
    return _vertexCount;
}

template <class EdgeInfo>
const typename ArcGraph<EdgeInfo>::EdgesCollection ArcGraph<EdgeInfo>:: getNextEdges(const VertexType vertex) const {
    return outcomingEdgesFromVertex[vertex];
}

template <class EdgeInfo>
const typename ArcGraph<EdgeInfo>::EdgesCollection ArcGraph<EdgeInfo>:: getPrevEdges(const VertexType vertex) const {
    return incomingEdgesToVertex[vertex];
}

template <class EdgeInfo>
std::shared_ptr<const std::vector<const VertexType>> ArcGraph<EdgeInfo>:: getAllVertexes() const {
    assert(false);
    return std::shared_ptr<const std::vector<const VertexType>>();
}

template <class EdgeInfo>
void ArcGraph<EdgeInfo>:: BFS(const VertexType start, const std::function<void(const std::shared_ptr<Edge<EdgeInfo>> edge)> completionBlock) {
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


#endif /* ArcGraph_h */
