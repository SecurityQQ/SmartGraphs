//
//  NetworkEdgeInfoWithID.h
//  SmartGraphs
//
//  Created by Alexander Malyshev on 06.10.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef NetworkEdgeInfoWithID_h
#define NetworkEdgeInfoWithID_h

typedef long FlowType;
typedef unsigned long CapacityType;
typedef unsigned long IDType;

class NetworkEdgeInfoWithID {
    
public:
    NetworkEdgeInfoWithID(CapacityType capacity, IDType id): _flow(0), _capacity(capacity), _id(id) {};
    NetworkEdgeInfoWithID(FlowType flow, CapacityType capacity, IDType id): _flow(flow), _capacity(capacity), _id(id) {};
    virtual FlowType flow() { return _flow; }
    virtual CapacityType capacity() { return _capacity; }
    virtual IDType id() { return _id; }
    virtual CapacityType residualCapacity() { return _capacity - _flow; }
    virtual void setFlow(FlowType flow) {
        assert(flow <= capacity());
        _flow = flow;
    }
    virtual void setCapacity(CapacityType capacity) {
        assert(flow() <= capacity);
        _capacity = capacity;
    }
protected:
    FlowType _flow;
    CapacityType _capacity;
    IDType _id;
};





#endif /* NetworkEdgeInfoWithID_h */
