//
// Created by Josh Szczesniak on 5/1/21.
//

#ifndef NS_3SIMULATION_MANAGERAPP_H
#define NS_3SIMULATION_MANAGERAPP_H
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"

class managerApp: public ns3::Application {
public:
    void StartApplication (void);
};


#endif //NS_3SIMULATION_MANAGERAPP_H
