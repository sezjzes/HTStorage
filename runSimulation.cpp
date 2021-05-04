//
// Created by Josh Szczesniak on 4/25/21.
//
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-address-helper.h"
#include "clientApp.h"
#include "managerApp.h"

using namespace ns3;
int main(){
    NodeContainer nodes;
    nodes.Create (2);

    InternetStackHelper internet;
    internet.Install (nodes);


    CsmaHelper csma;
    NetDeviceContainer ndc = csma.Install (nodes);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
    NetDeviceContainer d0d2 = p2p.Install (nodes);

    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0i2 = ipv4.Assign (d0d2);

    std::cout<< i0i2.GetAddress(0)<<"," <<i0i2.GetAddress(1)<< std::endl;


    managerApp manager = managerApp();
    std::cout<<"HI"<<std::endl;
    manager.SetStartTime(Seconds(0));
    //client.SetNode(nodes.Get(0));
    //manager.Initialize();
    nodes.Get(0)->AddApplication(&manager);

    clientApp client = clientApp();
    client.SetStartTime(Seconds(1));
    //client.SetNode(nodes.Get(0));
    //client.Initialize();
    nodes.Get(1)->AddApplication(&client);

    std::cout<<"HI"<<std::endl;
    Simulator::Stop (Seconds (11));
    Simulator::Run();
    while(true);


}