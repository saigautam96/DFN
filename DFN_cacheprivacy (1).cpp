#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/netanim-module.h"

namespace ns3 {

int 
main(int argc, char* argv[])
{
 CommandLine cmd;
 cmd.Parse(argc, argv);
 AnnotatedTopologyReader topologyReader("", 1);
 topologyReader.SetFileName("src/ndnSIM/examples/topologies/DFN_cacheprivacy.txt");
 topologyReader.Read();
 
 ndn::StackHelper ndnHelper;
 ndnHelper.SetDefaultRoutes(true);
 //ndnHelper.SetOldContentStore("ns3::ndn::cs::lru", "MaxSize","100");
 //ndnHelper.SetPit("ns3::ndn::pit::Persistent", "MaxSize", "100");
 ndnHelper.setPolicy("nfd::cs::lru");
 ndnHelper.setCsSize(1000);
 ndnHelper.InstallAll();
 
 ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/startegy/best-route");
 
 ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
 ndnGlobalRoutingHelper.InstallAll();
 
 Ptr<Node> consumers[6] = { Names::Find<Node>("C1"),
                            Names::Find<Node>("C2"),Names::Find<Node>("C3"),Names::Find<Node>("C4"),Names::Find<Node>("C5"),Names::Find<Node>("C6")};
 Ptr<Node> Attackers[3]   = {Names::Find<Node>("A1"),Names::Find<Node>("A2"), Names::Find<Node>("A3")}; 
 Ptr<Node> Adversary_1    = {Names::Find<Node>("A4")};
 Ptr<Node> Adversary_2    = {Names::Find<Node>("A5")};
 Ptr<Node> producers[2]   = {Names::Find<Node>("P1"),Names::Find<Node>("P2")};
 for(int k=0; k<6; k++){
 ndn::AppHelper consumerHelper1("ns3::ndn::ConsumerZipfMandelbrot");
 consumerHelper1.SetAttribute("Frequency", StringValue("100"));
 consumerHelper1.SetAttribute("Randomize", StringValue("uniform"));
 consumerHelper1.SetAttribute("Randomize", StringValue("exponential"));
 consumerHelper1.SetPrefix("/google.com/L");
 ApplicationContainer consumer = consumerHelper1.Install(consumers[k]);
 }
 for(int k=0;k<3;k++)
 {
 ndn::AppHelper consumerHelper3("ns3::ndn::ConsumerZipfMandelbrot");
 consumerHelper3.SetAttribute("Frequency", StringValue("100"));
 consumerHelper3.SetAttribute("Randomize", StringValue("uniform"));
 consumerHelper3.SetPrefix("/google.com/m1");
 //consumerHelper1.Install(Attacker);
 ApplicationContainer Attacker = consumerHelper3.Install(Attackers[k]);
 //Attacker.Start(Seconds(5.0));
 //Attacker.Stop(Seconds(15.0));
 }
 
 ndn::AppHelper consumerHelper4("ns3::ndn::ConsumerZipfMandelbrot");
 consumerHelper4.SetAttribute("Frequency", StringValue("100"));
 consumerHelper4.SetAttribute("Randomize", StringValue("uniform"));
 consumerHelper4.SetPrefix("/google.com/m2");
 //consumerHelper1.Install(Attacker);
 consumerHelper4.Install(Adversary_1);
 //Attacker.Start(Seconds(5.0));
 //Attacker.Stop(Seconds(15.0));
 
 
 ndn::AppHelper consumerHelper5("ns3::ndn::ConsumerZipfMandelbrot");
 consumerHelper5.SetAttribute("Frequency", StringValue("100"));
 consumerHelper5.SetAttribute("Randomize", StringValue("uniform"));
 consumerHelper5.SetPrefix("/google.com/m2");
 //consumerHelper1.Install(Attacker);
 consumerHelper5.Install(Adversary_2);
 //Attacker.Start(Seconds(5.0));
 //Attacker.Stop(Seconds(15.0));
 
 for(int k=0;k<2;k++){
  
 ndn::AppHelper producerHelper("ns3::ndn::Producer");
 producerHelper.SetPrefix("/google.com/L");
 producerHelper.SetPrefix("/google.com/m1");
 producerHelper.SetPrefix("/google.com/m2");
 producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
 producerHelper.Install(producers[k]);
 //ndnGlobalRoutingHelper.AddOrigins("/google.com/L", producers[k]);
 //ndnGlobalRoutingHelper.AddOrigins("/google.com/m1", producers[k]);
 //ndnGlobalRoutingHelper.AddOrigins("/google.com/m2", producers[k]);
 
 }
 
 //ndn::GlobalRoutingHelper::CalculateRoutes();
 
 ndn::FibHelper::AddRoute("A1", "/google.com/m1", "R1",1);
 ndn::FibHelper::AddRoute("C1", "/google.com/L", "R1",1);
 
 ndn::FibHelper::AddRoute("A2", "/google.com/m1", "R3",1);
 ndn::FibHelper::AddRoute("C2", "/google.com/L", "R3",1);
 
 ndn::FibHelper::AddRoute("A3", "/google.com/m1", "R7",1);
 ndn::FibHelper::AddRoute("C3", "/google.com/L", "R7",1);
 
 ndn::FibHelper::AddRoute("A4", "/google.com/m2", "R10",1);
 ndn::FibHelper::AddRoute("C4", "/google.com/L", "R10",1);
 
 ndn::FibHelper::AddRoute("A5", "/google.com/m2", "R9",1);
 ndn::FibHelper::AddRoute("C5", "/google.com/L", "R9",1);
 
 ndn::FibHelper::AddRoute("C6", "/google.com/L", "R3",1);
 
 //Router1
 ndn::FibHelper::AddRoute("R1", "/google.com/L", "R2",1);
 ndn::FibHelper::AddRoute("R1", "/google.com/m1", "R2",1);
 
 ndn::FibHelper::AddRoute("R1", "/google.com/L", "R8",1);
 ndn::FibHelper::AddRoute("R1", "/google.com/m1", "R8",1);
 
 ndn::FibHelper::AddRoute("R1", "/google.com/L", "R9",1); 
 ndn::FibHelper::AddRoute("R1", "/google.com/m1", "R9",1);
 
 //Router2
 ndn::FibHelper::AddRoute("R2", "/google.com/m1", "R4",1);
 ndn::FibHelper::AddRoute("R2", "/google.com/L", "R4",1);
 
 ndn::FibHelper::AddRoute("R2", "/google.com/m1", "R5",1);
 ndn::FibHelper::AddRoute("R2", "/google.com/L", "R5",1);
 
 //Router3
 ndn::FibHelper::AddRoute("R3", "/google.com/m1", "R6",1);
 ndn::FibHelper::AddRoute("R3", "/google.com/L", "R6",1);
 
 ndn::FibHelper::AddRoute("R3", "/google.com/m1", "R5",1);
 ndn::FibHelper::AddRoute("R3", "/google.com/L", "R5",1);
 
 ndn::FibHelper::AddRoute("R3", "/google.com/m1", "R10",1);
 ndn::FibHelper::AddRoute("R3", "/google.com/L", "R10",1);
 
 //Router7
 ndn::FibHelper::AddRoute("R7", "/google.com/L", "R5",1);
 ndn::FibHelper::AddRoute("R7", "/google.com/m1", "R5",1);
 
 //Router10
 ndn::FibHelper::AddRoute("R10", "/google.com/m2", "R6",1);
 ndn::FibHelper::AddRoute("R10", "/google.com/L", "R6",1);
 ndn::FibHelper::AddRoute("R10", "/google.com/m2", "R5",1);
 ndn::FibHelper::AddRoute("R10", "/google.com/L", "R5",1);
 ndn::FibHelper::AddRoute("R10", "/google.com/L", "R11",1);
 ndn::FibHelper::AddRoute("R10", "/google.com/m2", "R11",1);
 
 ndn::FibHelper::AddRoute("R9", "/google.com/m2", "R6",1);
 ndn::FibHelper::AddRoute("R9", "/google.com/L", "R6",1);
 ndn::FibHelper::AddRoute("R9", "/google.com/m2", "R4",1);
 ndn::FibHelper::AddRoute("R9", "/google.com/L", "R4",1);
 
 ndn::FibHelper::AddRoute("R5", "/google.com/m1", "R6",1);
 ndn::FibHelper::AddRoute("R5", "/google.com/L", "R6",1);
 ndn::FibHelper::AddRoute("R5", "/google.com/m2", "R6",1);
 
 ndn::FibHelper::AddRoute("R6", "/google.com/m1", "R4",1);
 ndn::FibHelper::AddRoute("R6", "/google.com/L", "R4",1);
 ndn::FibHelper::AddRoute("R6", "/google.com/m2", "R4",1);
 
 ndn::FibHelper::AddRoute("R11", "/google.com/m2", "R8",1);
 ndn::FibHelper::AddRoute("R11", "/google.com/L", "R8",1);
 
 ndn::FibHelper::AddRoute("R8", "/google.com/m1", "R4",1);
 ndn::FibHelper::AddRoute("R8", "/google.com/L", "R4",1);
 ndn::FibHelper::AddRoute("R8", "/google.com/m2", "R4",1);
  
 ndn::FibHelper::AddRoute("R6", "/google.com/L/P2", "P2",1);
 ndn::FibHelper::AddRoute("R6", "/google.com/m1/P2", "P2",1);
 ndn::FibHelper::AddRoute("R6", "/google.com/m2/P2", "P2",1);
 
 ndn::FibHelper::AddRoute("R4", "/google.com/m1/P1", "P1",1);
 ndn::FibHelper::AddRoute("R4", "/google.com/L/P1", "P1",1);
 ndn::FibHelper::AddRoute("R4", "/google.com/m2/P1", "P1",1);
 
 Simulator::Stop(Seconds(300.0));
 
 ndn::CsTracer::InstallAll("timing_DFN_.txt", Seconds(1.0));
 
 Simulator::Run();
 Simulator::Destroy();
 
 return 0;
 }
 
 }
 
 int 
 main(int argc, char* argv[])
 {
 return ns3::main(argc, argv);
 }

