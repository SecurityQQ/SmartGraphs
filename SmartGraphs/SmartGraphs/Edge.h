//
//  Edge.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 02.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef Edge_h
#define Edge_h

#include "GraphConstants.h"
typedef unsigned int DefaultEdgeInfo;


template <class EdgeInfo = DefaultEdgeInfo>
class Edge {
    
public:
    Edge(const VertexType from, const VertexType to, const EdgeInfo info): _from(from), _to(to), _info(std::shared_ptr<EdgeInfo>(new EdgeInfo(info))) {};
    Edge(const VertexType from, const VertexType to, std::shared_ptr<EdgeInfo> info): _from(from), _to(to), _info(info) {};
    
    VertexType from() { return _from; }
    VertexType to() { return _to; }
    std::shared_ptr<EdgeInfo> info() { return _info; }
    
private:
    VertexType _from;
    VertexType _to;
    std::shared_ptr<EdgeInfo> _info;
};

#endif /* Edge_h */
