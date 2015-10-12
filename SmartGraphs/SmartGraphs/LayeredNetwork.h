//
//  LayeredNetwork.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 12.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef LayeredNetwork_h
#define LayeredNetwork_h
#include "NetworkGraph.h"

class LayeredNetwork: NetworkGraph {
public:
    LayeredNetwork(std::shared_ptr<NetworkGraph>  network): NetworkGraph(network->vertexCapacity(), network->source, network->sink) {
        _networkGraph = network;
        createLayeredNetwork(network);
    }
    
    bool sinkReachable() {
        return _sinkReachable;
    }
private:
    bool _sinkReachable;
    void createLayeredNetwork(std::shared_ptr<NetworkGraph> network) {
        distance = std::vector<VertexType>(network->vertexCount(), NOT_VISITED);
        incPotential = std::vector<VertexType>(network->vertexCount(), 0);
        outPotential = std::vector<VertexType>(network->vertexCount(), 0);
        potential = std::vector<CapacityType>(network->vertexCount(), NOT_VISITED);
        extraFlow = std::vector<FlowType>(network->vertexCount(), 0);
        std::vector<VertexType> used(network->vertexCount());
        std::queue<VertexType>  vertexQueue;
        
        incPotential[source] = NOT_VISITED;
        outPotential[sink] = NOT_VISITED;
        
        vertexQueue.push(source);
        distance[source] = 0;
        while (!vertexQueue.empty()) {
            VertexType currentVertex = vertexQueue.front();
            vertexQueue.pop();
            if (!used[currentVertex]) {
                used[currentVertex] = 1;
                auto nextEdges = network->getNextEdges(currentVertex);
                for (auto it = nextEdges.begin(); it != nextEdges.end(); ++it) {
                    if ((*it)->info()->residualCapacity() > 0) {
                        if (distance[(*it)->to()] == NOT_VISITED) {
                            distance[(*it)->to()] = distance[currentVertex] + 1;
                        }
                        if (distance[(*it)->to()] == distance[currentVertex] + 1) {
                            addEdge(*it);
                            incPotential[(*it)->to()] += (*it)->info()->residualCapacity();
                            outPotential[(*it)->from()] += (*it)->info()->residualCapacity();
                            
                        }
                        vertexQueue.push((*it)->to());
                    }
                }
                
                potential[currentVertex] = std::min(incPotential[currentVertex], outPotential[currentVertex]);
                
#ifdef DEBUG
                std::cout<<"Potential for "<<currentVertex <<" is "<<potential[currentVertex] <<std::endl;
#endif
            }
        }
        _sinkReachable = (distance[sink] != NOT_VISITED);
    }
    std::shared_ptr<NetworkGraph> _networkGraph;
    std::vector<VertexType> distance;
    std::vector<VertexType> incPotential;
    std::vector<VertexType> outPotential;
    std::vector<CapacityType> potential;
    std::vector<FlowType> extraFlow;
};

#endif /* LayeredNetwork_h */
