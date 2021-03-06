//
//  NetworkGraph.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 06.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef NetworkGraph_h
#define NetworkGraph_h
#include "assert.h"
#include "ArcGraph.h"
#include "NetworkEdgeInfoWithID.h"

#define NOT_VISITED INT_MAX


class NetworkGraph: public ArcGraph<NetworkEdgeInfoWithID> {
    
public:
    NetworkGraph(const VertexType numberOfVertexes, const VertexType source, const VertexType sink): ArcGraph<NetworkEdgeInfoWithID>(numberOfVertexes), source(source), sink(sink) {};
    virtual CapacityType getCapacity(std::shared_ptr<Edge<NetworkEdgeInfoWithID>> edge) {
        return edge->info()->capacity();
    }
    const VertexType source;
    const VertexType sink;
private:

};


#endif /* NetworkGraph_h */
