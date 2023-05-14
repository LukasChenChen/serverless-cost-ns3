#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

#include "ns3/myAlgorithm.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("test-my");



int
main (int argc, char *argv[])
{

   CommandLine cmd;
   //this will rewrite the values in cfg files.
   float beta = 0.5;

   float reduFactor = 10;

   bool enable_simulation = false;

   bool verbose = false;

   float alpha = 0.05;

   cmd.AddValue ("beta", "zipf beta", beta);

   cmd.AddValue ("reduFactor", "reduce the number of requests", reduFactor);

   cmd.AddValue ("simu", "do simulation", enable_simulation);

   cmd.AddValue ("verbose", "verbose, show the detailed cost", verbose);

   cmd.Parse (argc, argv);

   LogComponentEnable ("MyAlgorithm", LOG_LEVEL_ERROR);
   LogComponentEnable ("Zipf", LOG_LEVEL_ERROR);
   LogComponentEnable ("network_controller", LOG_LEVEL_ERROR);
   LogComponentEnable ("serverless_server", LOG_LEVEL_ERROR);

   MyAlgorithm ma;

   ma.scheduleRequests(beta, reduFactor, verbose, alpha);
   

   if(enable_simulation){

   //length of slot, number of time slot
   ma.genTraffic(1, 30);
   
   Simulator::Stop (Seconds (30 * 1));

   Simulator::Run ();

   Simulator::Destroy ();
   }
}