//
//  main.cpp
//  SmartGraphs
//
//  Created by Alexander Malyshev on 02.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#include <iostream>
#include "ArcGraph.h"
#include <vector>
#include "NetworkEdgeInfoWithID.h"
#include "NetworkGraph.h"
#include "LayeredNetworkManager.h"
#include "LayeredNetwork.h"
#include "GraphManager.h"
#undef DEBUG

int main(int argc, const char * argv[]) {
    
    int n, m;
    std::cin>>n >>m;
    std::shared_ptr<ResidualNetworkGraph> network(new ResidualNetworkGraph(n));
    for (int i = 0; i < m; ++i) {
        int from, to, capacity;
        std::cin>>from >> to >> capacity;
        network->addEdge(from - 1, to - 1, std::shared_ptr<NetworkEdgeInfoWithID>(new NetworkEdgeInfoWithID(0, capacity, i)));
    }
    std::vector<FlowType> flows(m, 0);
//    auto printGraph = [](const std::shared_ptr<Edge<NetworkEdgeInfoWithID>> edge) {
//        std::cout<<edge->from() <<"->"<<edge->to() <<"("<<edge->info()->flow()<<"/"<<edge->info()->capacity()<<")"<<std::endl;
//    };
//    
//    network->BFS(0, printGraph);
    
//    auto layered = LayeredNetworkManager::sharedInstance().getLayeredGraph(network, 0, n - 1);
//    layered->BFS(0, printGraph);
//    auto maxFlow = LayeredNetworkManager::sharedInstance().getBlockingFlow(network, 0, n - 1);
    
    std::cout<<LayeredNetworkManager::sharedInstance().maxFlowWithMPMAlgorithm(network, 0, n - 1) <<std::endl;
    network->BFS(0, [&flows, &network](const std::shared_ptr<Edge<NetworkEdgeInfoWithID>> edge) {
        flows[edge->info()->id()] = edge->info()->flow();
    });
    for (auto it = flows.begin(); it != flows.end(); ++it) {
        std::cout<<*it<<std::endl;
    }

    
    return 0;
}
