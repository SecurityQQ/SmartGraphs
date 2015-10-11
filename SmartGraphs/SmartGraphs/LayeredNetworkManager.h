//
//  LayeredNetworkManager.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 07.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef LayeredNetworkManager_h
#define LayeredNetworkManager_h
#include "ResidualNetworkGraph.h"
#include <set>

#define USED_VERTEX (INT_MAX - 1)

class LayeredNetworkManager {
public:
    static LayeredNetworkManager& sharedInstance();
    std::shared_ptr<ResidualNetworkGraph> getLayeredGraph(std::shared_ptr<NetworkGraph> network, const VertexType source,
                                                        const VertexType sink);
    FlowType maxFlowWithMPMAlgorithm(std::shared_ptr<NetworkGraph> graph, const VertexType source, const VertexType sink);
    void getBlockingFlow(std::shared_ptr<ResidualNetworkGraph> network, const VertexType source, const VertexType sink);
private:
    void updatePotentials(std::shared_ptr<ResidualNetworkGraph> layeredNetwork, std::set<VertexType> &vertexesToDelete, const VertexType source, const VertexType sink);
    LayeredNetworkManager() {}
    std::vector<VertexType> distance;
    std::vector<VertexType> incPotential;
    std::vector<VertexType> outPotential;
    std::vector<CapacityType> potential;
    std::vector<FlowType> extraFlow;
};

LayeredNetworkManager& LayeredNetworkManager:: sharedInstance() {
    static LayeredNetworkManager instance;
    return instance;
}


FlowType LayeredNetworkManager:: maxFlowWithMPMAlgorithm(std::shared_ptr<NetworkGraph> graph, const VertexType source, const VertexType sink) {
    auto residualNetwork = std::shared_ptr<NetworkGraph>(new NetworkGraph(graph->vertexCount()));    
    while (true) {
        auto layeredGraph = getLayeredGraph(graph, source, sink);
#ifdef DEBUG
        std::cout<<"layered graph created"<<std::endl;
        layeredGraph->BFS(0, [](const std::shared_ptr<Edge<NetworkEdgeInfoWithID>> edge) {
            std::cout<<edge->from() <<"->"<<edge->to() <<"("<<edge->info()->flow()<<"/"<<edge->info()->capacity()<<")"<<std::endl;
        });
#endif
         
        if (distance[sink] == NOT_VISITED) {
            break;
        }
        
        getBlockingFlow(layeredGraph, source, sink);
#ifdef DEBUG
        std::cout<<"Blocking path added"<<std::endl;
        graph->BFS(0, [](const std::shared_ptr<Edge<NetworkEdgeInfoWithID>> edge) {
            std::cout<<edge->from() <<"->"<<edge->to() <<"("<<edge->info()->flow()<<"/"<<edge->info()->capacity()<<")"<<std::endl;
        });
        
#endif
    }
    FlowType maxFlow = 0;
    auto sourceAdjEdges = graph->getNextEdges(source);
    for (auto it = sourceAdjEdges.begin(); it != sourceAdjEdges.end(); ++it) {
        maxFlow += (*it)->info()->flow();
    }
    return maxFlow;
}

std::shared_ptr<ResidualNetworkGraph> LayeredNetworkManager:: getLayeredGraph(std::shared_ptr<NetworkGraph> network, const VertexType source, const VertexType sink) {
    distance = std::vector<VertexType>(network->vertexCount(), NOT_VISITED);
    incPotential = std::vector<VertexType>(network->vertexCount(), 0);
    outPotential = std::vector<VertexType>(network->vertexCount(), 0);
    potential = std::vector<CapacityType>(network->vertexCount(), NOT_VISITED);
    extraFlow = std::vector<FlowType>(network->vertexCount(), 0);
    auto layeredGraph = std::shared_ptr<ResidualNetworkGraph>(new ResidualNetworkGraph(network->vertexCount()));
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
                        layeredGraph->addEdge(*it);
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
    
    return layeredGraph;
}

void LayeredNetworkManager:: getBlockingFlow(std::shared_ptr<ResidualNetworkGraph> layeredNetwork, const VertexType source, const VertexType sink) {
    while (potential[source] != USED_VERTEX && potential[sink] != USED_VERTEX) {
        std::set<VertexType> vertexesToDelete;
        std::queue<VertexType> pushingQueue;
        std::vector<char> used(layeredNetwork->vertexCount(), 0);
        auto getIndexOfMinElement = [](const std::vector<CapacityType>& v) -> unsigned long long {
            CapacityType min = v[0];
            unsigned long long minIndex = 0;
            for (unsigned long long i = 1; i < v.size(); ++i) {
                if (min > v[i]) {
                    min = v[i];
                    minIndex = i;
                }
            }
            return minIndex;
        };
        
        VertexType vertexWithMinPotential = getIndexOfMinElement(potential);
        extraFlow[vertexWithMinPotential] = potential[vertexWithMinPotential];
        pushingQueue.push(vertexWithMinPotential);
        
        //forward:
        while (!pushingQueue.empty()) {
            auto currentVertex = pushingQueue.front();
            pushingQueue.pop();
            if (!used[currentVertex]) {
                used[currentVertex] = 1;
                auto nextEdges = layeredNetwork->getNextEdges(currentVertex);
                for (auto edge: nextEdges) {
                    auto residualCapacity = edge->info()->residualCapacity();
                    auto oldFlow = edge->info()->flow();
                    auto to = edge->to();
                    FlowType pushingFlow;
                    if (residualCapacity <= extraFlow[currentVertex]) {
                        pushingFlow = residualCapacity;
                    } else {
                        pushingFlow = extraFlow[currentVertex];
                    }
                    edge->info()->setFlow(oldFlow + pushingFlow);
                    outPotential[currentVertex] -= pushingFlow;
                    incPotential[to] -= pushingFlow;
                    extraFlow[to] += pushingFlow;
                    extraFlow[currentVertex] -= pushingFlow;
                    pushingQueue.push(to);
#ifdef DEBUG
                    std::cout<<"from "<<edge->from() << "to "<<edge->to()<<" pushed: "<<pushingFlow<<std::endl;
#endif
                    if (extraFlow[currentVertex] == 0) {
                        break;
                    }
                }
                vertexesToDelete.insert(currentVertex);
            }
        }
        
        //backward:
        extraFlow[vertexWithMinPotential] = potential[vertexWithMinPotential];
        pushingQueue.push(vertexWithMinPotential);
        used = std::vector<char>(layeredNetwork->vertexCount(), 0);
        
        while (!pushingQueue.empty()) {
            auto currentVertex = pushingQueue.front();
            pushingQueue.pop();
            if (!used[currentVertex]) {
                used[currentVertex] = 1;
                auto nextEdges = layeredNetwork->getPrevEdges(currentVertex);
                for (auto edge: nextEdges) {
                    auto residualCapacity = edge->info()->residualCapacity();
                    auto oldFlow = edge->info()->flow();
                    auto from = edge->from();
                    FlowType pushingFlow;
                    if (residualCapacity <= extraFlow[currentVertex]) {
                        pushingFlow = residualCapacity;
                    } else {
                        pushingFlow = extraFlow[currentVertex];
                    }
                    edge->info()->setFlow(oldFlow + pushingFlow);
                    incPotential[currentVertex] -= pushingFlow;
                    outPotential[from] -= pushingFlow;
                    potential[currentVertex] -= pushingFlow;
                    extraFlow[from] += pushingFlow;
                    extraFlow[currentVertex] -= pushingFlow;
                    pushingQueue.push(from);
#ifdef DEBUG
                    std::cout<<"(backward) from "<<edge->from() << "to "<<edge->to()<<" pushed: "<<pushingFlow<<std::endl;
#endif
                    if (extraFlow[currentVertex] == 0) {
                        break;
                    }
                }
                vertexesToDelete.insert(currentVertex);
            }
        }
        
        updatePotentials(layeredNetwork, vertexesToDelete, source, sink);
    }
}

void LayeredNetworkManager:: updatePotentials(std::shared_ptr<ResidualNetworkGraph> layeredNetwork, std::set<VertexType> &vertexesToDelete, const VertexType source, const VertexType sink) {
    while (!vertexesToDelete.empty()) {
        auto currentVertex = *vertexesToDelete.begin();
        vertexesToDelete.erase(currentVertex);
        potential[currentVertex] = std::min(incPotential[currentVertex], outPotential[currentVertex]);
        if (potential[currentVertex] <= 0) {
            potential[currentVertex] = USED_VERTEX;
            auto nextEdges = layeredNetwork->getNextEdges(currentVertex);
            auto prevEdges = layeredNetwork->getPrevEdges(currentVertex);
            for (auto it = nextEdges.begin(); it != nextEdges.end(); ++it) {
                layeredNetwork->removeEdge(*it);
                auto to = (*it)->to();
                vertexesToDelete.insert(to);
                incPotential[to] -= (*it)->info()->residualCapacity();
            }
            for (auto it = prevEdges.begin(); it != prevEdges.end(); ++it) {
                layeredNetwork->removeEdge(*it);
                auto from = (*it)->from();
                vertexesToDelete.insert(from);
                outPotential[from] -= (*it)->info()->residualCapacity();
            }
        }
    }
    
}

#endif /* LayeredNetworkManager_h */
