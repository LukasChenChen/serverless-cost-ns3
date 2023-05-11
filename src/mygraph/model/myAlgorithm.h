#ifndef MYALGORITHM_H
#define MYALGORITHM_H

#include "ns3/core-module.h"
#include "type.h"
using namespace ns3;

namespace ns3{

class MyAlgorithm
{
public:
  MyAlgorithm ();

  virtual ~MyAlgorithm();

  bool loadConfig(std::string filename);

  void loadTopo();

  void loadRequest();

  std::map<int, int> genZipfNum(int num_values, float alpha, int max_num);

  void init();

  void initFuncMap();

  bool createRequest(int timeSlot, int funcType, int ingressID, Request &r);

  void createRequestInSlot(int timeSlot, int funcType, ReqOnNodes ron);

  void createRequests();

  void readRequests();

  float distance(int node_1, int node_2);

  DistSlice sortPhyNodes(Request r);

  void placeToNeighbour(Request &r, Function function, int index, int phynodeID);

  bool deployToNeighbour(DistSlice ds, Request &r);

  void createToCurrent(Request &r);

  void placeToCurrent(Request &r, Function f, int index);

  void deployRequest(Request &r);

  void deployRequests();

  void scheduleRequests(float beta_input, float reduFactor_input);

  Function getIdleFunction(int phynodeID, int funcType, int &index);

  void updateCache();

  void printResult(std::string filename);

  void printResult_no_1(std::string filename);

  void genTraffic(int time_slot, int time_slot_num);

  int getContainerSize(int funcType);

  float getProb(int nodeID, int funcType);

  int getEvictedContainer(int nodeID, int reqFuncType);

  float getCPU(int phyNodeID);

  float getInstanCost(int phyNodeID, int funcType);

  float getRunCost(int phyNodeID, int funcType);

  ServerlessConfig m_cfg;

  
  
  //the map for nodes <nodeID, Location>
  std::map<int, Location> m_map;
  
  // <function type, reqNumMap>
  std::map<int, ReqNumMap> m_req_num;

  RequestsMap m_request_map; //all the request here

  int m_req_count;
  
  //function map stores function info
  FunctionInfoMap m_funcInfoMap;

  ReqOnNodesTimeType m_rontt;

  CacheMap m_cm;

  ActiveFunctions m_afs;

  Topology m_topo;

  int m_clock;

  FunctionFreq m_functionfreq;

  int m_cold_req_num;

  int m_total_req_num;

  int m_served_req_num;


};//END class myAlgorithm


}// end ns3 namespace

#endif /* MYALGORITHM_H */