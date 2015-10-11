//
//  ResidualNetworkGraph.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 07.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef ResidualNetworkGraph_h
#define ResidualNetworkGraph_h
#include "assert.h"
#include "ArcGraph.h"
#include "NetworkEdgeInfoWithID.h"

#define NOT_VISITED INT_MAX


class ResidualNetworkGraph: public NetworkGraph {
    
public:
    ResidualNetworkGraph(const VertexType numberOfVertexes): NetworkGraph(numberOfVertexes) {};
    CapacityType getCapacity(std::shared_ptr<Edge<NetworkEdgeInfoWithID>> edge) {
        return edge->info()->capacity() - edge->info()->flow();
    }
    void addFlow(std::shared_ptr<Edge<NetworkEdgeInfoWithID>> edge, const FlowType flow) {
        auto oldFlow = edge->info()->flow();
        auto to = edge->to();
        auto from = edge->from();
        assert(getEdge(from, to) != nullptr);
        edge->info()->setFlow(oldFlow + flow);
        auto reversedEdge = getEdge(to, from);
        if (reversedEdge == nullptr) {
            auto info = std::shared_ptr<NetworkEdgeInfoWithID>(new NetworkEdgeInfoWithID(0, edge->info()->flow(), edge->info()->id()));
            addEdge(to, from, info);
        } else {
            reversedEdge->info()->setCapacity(reversedEdge->info()->capacity() + flow);
        }
// TODO: implement this
        //remade capacities
    }

private:
    
};

#endif /* ResidualNetworkGraph_h */
