//
//  GraphManager.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 12.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef GraphManager_h
#define GraphManager_h
#include "ResidualNetworkGraph.h"
#include <set>

class GraphManager {
public:
    FlowType maxFlowWithMPMAlgorithm(std::shared_ptr<NetworkGraph> graph, const VertexType source, const VertexType sink);
    void getAndPushBlockingFlowToNetwork(std::shared_ptr<ResidualNetworkGraph> network);
private:
    void updatePotentials(std::shared_ptr<ResidualNetworkGraph> layeredNetwork, std::set<VertexType> &vertexesToDelete, const VertexType source, const VertexType sink);
    GraphManager() {}
};

FlowType maxFlowWithMPMAlgorithm(std::shared_ptr<NetworkGraph> graph, const VertexType source, const VertexType sink);
void getAndPushBlockingFlowToNetwork(std::shared_ptr<ResidualNetworkGraph> network);



#endif /* GraphManager_h */
