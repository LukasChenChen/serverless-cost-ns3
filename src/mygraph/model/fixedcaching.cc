/*
When add to activefunctions, change the freq.
When leaving the cache map, reduce the freq.
Everything in m_afs will be moved into cache because since it can be created,
memory is enough.

Everything add to m_afs, the priority will be refreshed, the m_clock and m_functionfreq will be refreshed.
*/


#include "ns3/core-module.h"
#include "fixedcaching.h"
#include "ns3/log.h"
#include <boost/algorithm/string/split.hpp> // boost:split()
#include <boost/algorithm/string.hpp> // boost::is_any_of()
#include "ns3/zipf.h"
#include "ns3/operation.h"
#include "ns3/network_controller.h"

namespace ns3 {
    //the lifetime left for function
   int lifeTime_G = 10;

  NS_LOG_COMPONENT_DEFINE ("FixedCaching");

  FixedCaching::FixedCaching (){
    NS_LOG_FUNCTION (this);
  }

  FixedCaching::~FixedCaching(){
    NS_LOG_FUNCTION (this);
  }

  bool FixedCaching::loadConfig(std::string filename){

    std::string line;
    std::ifstream myfile (filename);

    // int inter_latency, intra_latency, inter_bw, intra_bw, domain_num, processing_cap;

    std::string TopoName, RequestFile;
    float CommCostPara, MemCap, Beta, ReduFactor, Alpha, cpuFreq;
    int NodeNum, SlotNum;

    NS_LOG_INFO("-----read config-----");
    
    if (myfile.is_open()) {
        
        while (getline (myfile, line)) {
            
            size_t pos = line.find("=");
            if (pos == std::string::npos){

                std::cout << "cant find =" <<std::endl;
                return false;
            }
                

            std::string tmp_key = line.substr(0, pos);

            
            if (tmp_key == "TopoName")
            {
                TopoName = line.substr(pos+1);
                // std::cout<< "TopoName " << TopoName << std::endl;
            }
            else if (tmp_key == "RequestFile")
            {
                RequestFile = line.substr(pos+1);
                // std::cout<< "RequestFile " << RequestFile << std::endl;
            }
            else if (tmp_key == "CommCostPara")
            {
                CommCostPara = stof(line.substr(pos+1));
                // std::cout<< "CommCostPara " << CommCostPara << std::endl;
            }
            else if (tmp_key == "MemCap")
            {
                MemCap = stof(line.substr(pos+1));
                // std::cout<< "MemCap " << MemCap << std::endl;
            }
            else if (tmp_key == "NodeNum")
            {
                NodeNum = stoi(line.substr(pos+1));
                // std::cout<< "NodeNum " << NodeNum << std::endl;
            }
            else if (tmp_key == "Beta")
            {
                Beta = stof(line.substr(pos+1));
                // std::cout<< "Beta " << Beta << std::endl;
            }
            else if (tmp_key == "SlotNum")
            {
                SlotNum = stoi(line.substr(pos+1));
                // std::cout<< "SlotNum " << SlotNum << std::endl;
            }
            else if (tmp_key == "ReduFactor")
            {
                ReduFactor = stof(line.substr(pos+1));
                // std::cout<< "ReduFactor " << ReduFactor << std::endl;
            } 
            else if (tmp_key == "Alpha")
            {
                Alpha = stof(line.substr(pos+1));
                // std::cout<< "Alpha " << Alpha << std::endl;
            }
            else if (tmp_key == "cpuFreq")
            {
                cpuFreq = stof(line.substr(pos+1));
                // std::cout<< "cpuFreq " << cpuFreq << std::endl;
                break;
            }
           
        
        
            
        }


        myfile.close();
    } 
    else 
    { 
        NS_LOG_ERROR("Unable to open file Scaling::read_config"); 
        return false;
    }

    
    
    m_cfg.TopoName = TopoName;
    m_cfg.RequestFile = RequestFile;
    m_cfg.CommCostPara = CommCostPara;
    m_cfg.MemCap = MemCap;
    m_cfg.NodeNum = NodeNum;
    m_cfg.Beta =Beta;
    m_cfg.SlotNum =SlotNum;
    m_cfg.ReduFactor =ReduFactor;
    m_cfg.Alpha =Alpha;
    m_cfg.cpuFreq = cpuFreq;
    
    return true;

}

void FixedCaching::initFuncMap(int lifeTime){
   
    FunctionInfo fi = {35, 5.31, 55, lifeTime};
    m_funcInfoMap.add(1,fi);

    fi = {63, 5.33, 158, lifeTime};
    m_funcInfoMap.add(2,fi);

    fi = {20, 5.34, 332, lifeTime};
    m_funcInfoMap.add(3,fi);

    fi = {2076, 4.89, 92, lifeTime};
    m_funcInfoMap.add(4,fi);
}
  
void FixedCaching::loadTopo(){
    NS_LOG_FUNCTION("this");

    std::string line;

    std::ifstream myfile(m_cfg.TopoName);

    NS_LOG_INFO("-----read topo file-----");

    int nodeID = 0;
        
    if (myfile.is_open()) {
        
        while (getline (myfile, line)) 
        {
           
            std::string node_s = line;

            std::vector<std::string> split_node_v;

            boost::split(split_node_v, node_s, boost::is_any_of(","), boost::token_compress_on);
            
            int count = 1;

            std::string LATITUDE_S, LONGITUDE_S;
            
            //skip first row, it is the title
            if(nodeID == 0){
                nodeID++;
                continue;
            }

            for (std::vector<std::string>::iterator it = split_node_v.begin(); it  != split_node_v.end(); it++)
            {

                int i = count;
             
                if(i == 2)
                {
                    LATITUDE_S = *it;

                    NS_LOG_LOGIC(" LATITUDE_S " << LATITUDE_S);
                    
                }else if(i == 3)
                {
                    LONGITUDE_S = *it;

                    NS_LOG_LOGIC("LONGITUDE_S " << LONGITUDE_S);
                    
                }
                count ++;
            }

            float latitude = stof(LATITUDE_S); 

            float longitude = stof(LONGITUDE_S);

            Location loc;
            loc.init(latitude, longitude);

            m_map.insert({nodeID, loc});

            PhyNode f = {
                nodeID, //id
                latitude, //latitude
                longitude, //longitude
                m_cfg.MemCap, //mem
                m_cfg.cpuFreq,
            };

            m_topo.add(nodeID, f);
             

            NS_LOG_LOGIC("add node " << nodeID << "lat " << loc.latitude << "long " << loc.latitude);
            nodeID++; 
        }
    }

    myfile.close();

}

//notice  the request file is row 2, 4, 6, 8...
void FixedCaching::loadRequest(){
    NS_LOG_FUNCTION("this");

    std::string line;

    std::ifstream myfile(m_cfg.RequestFile);

    NS_LOG_INFO("-----read Request file-----");

    int req_num = 0;

    int rowNum = 1;
        
    if (myfile.is_open()) {
        
        while (getline (myfile, line)) 
        {   
            //get rid of odd row
            if(rowNum % 2 != 0 ){
               rowNum++;
               continue;
            }

            std::string req_num_s = line;

            std::vector<std::string> split_req_v;
            
            //split req_num_s into several strings
            boost::split(split_req_v, req_num_s, boost::is_any_of(","), boost::token_compress_on);
            
            int count = 1;

            ReqNumMap reqNumMap;

            for (std::vector<std::string>::iterator it = split_req_v.begin(); it  != split_req_v.end(); it++)
            {

                int i = count;
             
                req_num = stoi(*it);

                NS_LOG_LOGIC("time slot " << i << "req_num: " << req_num);

                reqNumMap.insert({i, req_num});
                    
                count ++;
            }
            NS_LOG_LOGIC("insert function type " << rowNum/2);
            m_req_num.insert({ rowNum/2 ,reqNumMap});

            rowNum++;
           
        }
    }

    myfile.close();
}

//given the num_values -> node num, aplha, and max_num -> num of requests/num of nodes
std::map<int, int> FixedCaching::genZipfNum(int num_values, float alpha, int max_num){
  NS_LOG_FUNCTION("this");
//   NS_LOG_INFO("num_values " << num_values);
  int zipf_rv = 0;
  //for
  Zipf_generator zipf;
  int seed = 1;
  zipf.rand_val(seed);
  
  std::map<int, int> req_map;// <node id, request_num>;
  for (int i=1; i<= num_values; i++)
  { 
    NS_LOG_LOGIC("num_values " << num_values);
    zipf_rv = zipf.zipf(alpha, max_num);
    NS_LOG_LOGIC("zipf value " << zipf_rv);
    req_map.insert({i, zipf_rv});
  }

  return req_map;

}

bool FixedCaching::createRequest(int timeSlot, int funcType, int ingressID, Request &r){
    NS_LOG_FUNCTION(this);

    auto it = m_map.find(ingressID);

    if (it != m_map.end()){
    
        PhyNode pNode = {
            ingressID,            //int     id;
            it->second.latitude,  //float   latitude;
            it->second.longitude, //float   longitude;
            m_cfg.MemCap,         //float   mem;
        };

        PhyNode pn;//empty

        FunctionInfo fInfo;

        Function f;
        f.type = funcType;
        
        FunctionInfo fi;
        if (m_funcInfoMap.get(funcType, fi)){
           f.processingTime   = fi.processingTime;
           f.coldStartTime   = fi.coldStartTime;
           f.size   = fi.size;
           f.lifeTime = fi.lifeTime;

           Request request = {
           m_req_count, //id
           f,           //Function function;
           pNode,       //PhyNode  ingress;
           timeSlot,           //arriveTime;
           false,       // served
           pn,           // deployNode
           false,       //isColdStart
           };

           r = request;

           m_req_count++;
           return true;
        }
        else{
            NS_LOG_ERROR("cannot find funcType :" << funcType);
            return false;
        }

    }//end it != m_map.end()
    else{
        NS_LOG_ERROR("cannot find ingress id: " << ingressID);
        return false;
    }
}

//create the requests for a single time slot
void FixedCaching::createRequestInSlot(int timeSlot, int funcType, ReqOnNodes ron){
    NS_LOG_FUNCTION(this);
    //order does not matter
    //create for different node
    for(int i = 0; i < ron.numVector.size(); i++){
        //node id starts from 1
        int nodeID = i+1;
        int rNum = ron.numVector[i];

        rNum = (int)(rNum/m_cfg.ReduFactor);
         //create rNum requests
        for (int i = 0; i < rNum; i++){

            Request r;
            createRequest(timeSlot, funcType, nodeID, r);

            m_request_map.add(timeSlot, r);
        }
    }

}

//create all requests from m_rontt
void FixedCaching::createRequests(){

    for (auto it = m_rontt.numMap.begin(); it != m_rontt.numMap.end(); it++){
        int funcType = it->first;
        ReqOnNodesTime ront = it->second;
        //iterate over all time slot
        for(int i = 0; i < ront.numVector.size(); i++){
            //time slot start from 1
            createRequestInSlot(i+1, funcType, ront.numVector[i]);
        }
    }

}


//read the request from the file, reduce by the factor
void FixedCaching::readRequests(){
    // std::map<int, int> req_map = genZipfNum(125, 0.5, 519);

    NS_LOG_FUNCTION("this");

    std::string line;

    std::ifstream myfile("config/requests-" + std::to_string(m_cfg.Beta) +".csv");

    NS_LOG_INFO("-----read zipf requests file-----");

    int req_num = 0;

    int funcType = 0;

    ReqOnNodesTime ront;

    if (myfile.is_open()) {
        
        while (getline (myfile, line)) 
        {   
            //get rid of odd row
            if(line.find("funcType") != std::string::npos ){
               NS_LOG_INFO("found " << line);
               if(funcType == 0){
                   funcType++;
                   continue; //skip the first line of the file
               }

               NS_LOG_LOGIC("add functype " << funcType);
               //add to this member variable
               m_rontt.add(funcType, ront);
               ront.clear();
               funcType++;
               continue; //go to the data line
            }

            std::string req_num_s = line;

            std::vector<std::string> split_req_v;
            
            //split req_num_s into several strings
            boost::split(split_req_v, req_num_s, boost::is_any_of(","), boost::token_compress_on);
            
            int count = 1;


            ReqOnNodes ron;

            //get the number of each nodes
            for (std::vector<std::string>::iterator it = split_req_v.begin(); it  != split_req_v.end(); it++)
            {
                NS_LOG_LOGIC("node id " << count << "req_num: " << req_num);
                
                req_num = stoi(*it);

                ron.add(req_num);
                count++;
            }
           
            ront.add(ron);
           
        }//end while
         //when all finish add type 4
        if (!getline (myfile, line)){
            NS_LOG_LOGIC("add functype " << funcType);
            m_rontt.add(funcType, ront);
        }
    }//end if

    myfile.close();

    return;        
}

//return meter
float FixedCaching::distance(int node_1, int node_2){
    NS_LOG_FUNCTION(this);
    auto it_1 = m_map.find(node_1);
    auto it_2 = m_map.find(node_2);

    if(it_1 == m_map.end() || it_2 == m_map.end()){
        NS_LOG_ERROR(" cannot find node id : " << node_1 << " or " << node_2);
        return 0;
    }

    Location loc1 = it_1->second;
    Location loc2 = it_2->second;

    return CalcGPSDistance(loc1.latitude, loc1.longitude, loc2.latitude, loc2.longitude)/1000;

}

//sort in ascending order of distance
DistSlice FixedCaching::sortPhyNodes(Request r){
    NS_LOG_FUNCTION(this);
    NS_LOG_LOGIC("Calculate distance -------");

    if (r.ingress.id > m_map.size()){
        NS_LOG_ERROR( "Wrong ingress id "<< r.ingress.id);
    }
    DistSlice ds;
    for(auto it = m_map.begin(); it != m_map.end(); it++){
        int nodeId = it->first;
        //in kilometers
        float dist = distance(r.ingress.id, nodeId);
        NS_LOG_LOGIC("distance is " << dist);

        Distance dis;
        dis.phyNodeID = it->first;
        dis.distance = dist;
        ds.add(dis);
        NS_LOG_LOGIC("dsitance ->>> " << dist);
    }
    NS_LOG_LOGIC("displaying nodes -------");
    // ds.show();
    NS_LOG_LOGIC("Sorting nodes -------");
    //sort ds.
    std::sort(ds.vec.begin(), ds.vec.end());

    // ds.show();

    return ds;
}

//index = -1 means not successful
Function FixedCaching::getIdleFunction(int phynodeID, int funcType, int &index){
    NS_LOG_FUNCTION(this);

    Function f = m_cm.getIdleFunction(phynodeID, funcType, index);

    return f;

}


//place to neighbour with cached container
void FixedCaching::placeToNeighbour(Request &r, Function function, int index, int phyNodeID){
    NS_LOG_FUNCTION(this);

    // function.activeLifeTime(lifeTime_G);
    
    //put the function in active list
    m_afs.add(function, phyNodeID, m_functionfreq, m_clock);

    //remove from cacheMap
    m_cm.remove(phyNodeID, index, m_functionfreq);

    PhyNode p = m_topo.get(phyNodeID);

    if(p.id == 0){
        return;
    }

    r.update(function, p, false);

}

//get 
int FixedCaching::getContainerSize(int funcType){
    return m_funcInfoMap.getSize(funcType);
}

//get the cpu frequency of certain physical node
float FixedCaching::getCPU(int phyNodeID){
    PhyNode p = m_topo.get(phyNodeID);
    if(p.id == 0){
        //cannot find this one
        return 0;
    }else{
        return p.cpuFreq;
    }
}


//get the instantion cost of a function
float FixedCaching::getInstanCost(int phyNodeID, int funcType){
    float cpuFreq = getCPU(phyNodeID);

    int size = getContainerSize(funcType);

    float instanCost = (float)size/cpuFreq;

    if(cpuFreq == 0){
        return 0;
    }

    return instanCost;
}

bool FixedCaching::deployToNeighbour(DistSlice ds, Request &r){
    NS_LOG_FUNCTION(this);
    
    bool succFlag = false;
   
    //ingore the first node, it is the current node.
    for(int j = 1; j < ds.size(); j++){

        int nodeID = ds.vec[j].getID();

        float instanCost = getInstanCost( nodeID , r.function.type);

        if(ds.vec[j].distance * m_cfg.CommCostPara < instanCost){

            int index = -1;

            Function f = getIdleFunction(ds.vec[j].phyNodeID, r.function.type, index);

            if (index == -1){
                continue;
            }else{
                //place to this node
                placeToNeighbour(r, f, index, ds.vec[j].phyNodeID);

                succFlag = true;
            }
        }

    }
    
    return succFlag;
}

//no need to change because it always kill the one with lowest lifetime,
//just modify the method getlowestprioirty in types.h
void FixedCaching::createToCurrent(Request &r){
    NS_LOG_FUNCTION(this);
    bool succFlag = true;
    int count = 0; //just incase infinitely loop
    Function f;
    //if memory not sufficient and this request priority is higher than the cached one
    if(r.function.size > m_topo.get(r.ingress.id).mem){
        //clear the cache map
        while (count < 1000) {
            count++;

            int l2 = m_cm.getLowestLifeTime(r.ingress.id);

            succFlag = m_cm.deleteLowestPriority(r.ingress.id, f, m_functionfreq);

            if(succFlag == true){
                //if success terminate the container
                m_topo.update("add", r.ingress.id, f.size);
            }
                
            //keep check the memory, if sufficient
            if (r.function.size <= m_topo.get(r.ingress.id).mem){

                
                r.function.phyNode = r.ingress;

                m_afs.add(r.function, r.ingress.id, m_functionfreq, m_clock);

                m_topo.update("minus", r.ingress.id, f.size);

                r.update(r.function, r.ingress, true);

                return;

            }
            //no space to delete
            if (succFlag == false){

                return;
            }
           

        }//end while
    }//end if
    else{
        //space sufficient
    
        r.function.phyNode = r.ingress;
        
        m_afs.add(r.function, r.ingress.id, m_functionfreq, m_clock);

        m_topo.update("minus", r.ingress.id, r.function.size);

        r.update(r.function, r.ingress, true);

        return;
    }
}

void FixedCaching::placeToCurrent(Request &r, Function f, int index){
    NS_LOG_FUNCTION(this);
    //renew the lifetime
    // f.activeLifeTime(lifeTime_G);

    m_afs.add(f, r.ingress.id, m_functionfreq, m_clock);

    m_cm.remove(r.ingress.id, index, m_functionfreq);

    r.update(f, r.ingress, false);

}

void FixedCaching::deployRequest(Request &r){
    NS_LOG_FUNCTION(this);

    DistSlice ds = sortPhyNodes(r);
    
    int index = -1;
    Function f = getIdleFunction(r.ingress.id, r.function.type, index);

    if( index == -1){
        //try cache container on neighbour
        if(deployToNeighbour(ds, r) == false){
            //create a new one on local
            createToCurrent(r);
        }
    }else{
        NS_LOG_LOGIC("use cache function");
        // place to local cache container
        placeToCurrent(r, f, index);
    }

}


void FixedCaching::updateCache(){
    NS_LOG_FUNCTION(this);

      //iterate over active function, add to cachemap, delete the active functions, because we assume every container finish in one slot
    //it_afs nodefunctions
    for(auto it_afs = m_afs.m.begin(); it_afs != m_afs.m.end(); it_afs++){
        //it_nfs std::map <int, Functions>
        for(auto it_nfs = it_afs->second.functions.begin(); it_nfs != it_afs->second.functions.end(); it_nfs++){
            for(int i = 0; i < it_nfs->second.size(); i++){
                Function f = it_nfs->second.get(i);
                m_cm.add(f, m_functionfreq);//add to cache map
            }//end for

        }// end for
    }//end for

    //clear the activemaop
    m_afs.clear();

    //iterate over cache map and check the lifetime is 0, remove it from caching map then.
    for(auto it = m_cm.caches.begin(); it != m_cm.caches.end(); it++){
        for(auto it_fl = it->second.functionList.begin(); it_fl != it->second.functionList.end(); ){
            //reduce the lifetime by 1 first
            (*it_fl).minusLife();

            if((*it_fl).lifeTime < 1){
                m_topo.update("add", it->second.phyNodeID, (*it_fl).size);
                it_fl = it->second.functionList.erase(it_fl);
            }
            else{
                ++it_fl;
            }
        }
    }


    m_cm.sortLifeTime();



}

void FixedCaching::deployRequests(){
    NS_LOG_FUNCTION(this);

    //iterate over time slot
    for(int i=1; i<= m_request_map.size(); i++){
        if(m_request_map.rsMap.find(i) != m_request_map.rsMap.end()){
            auto it_map = m_request_map.rsMap.find(i);
            // Requests rs = it_map->second;

            for(auto it = it_map->second.rs.begin(); it != it_map->second.rs.end(); it++){
                deployRequest(*it);

                m_total_req_num += 1;
            }
        }

        m_clock += 1;

        updateCache();

        NS_LOG_INFO("Cache size " << m_cm.size());


    }//end for

   
}


void FixedCaching::init(){

    m_clock = 1;

    m_cold_req_num = 0;

    m_total_req_num = 0;

    m_served_req_num = 0;

    m_req_count = 0; //count to create request
}

//get the running cost
float FixedCaching::getRunCost(int phyNodeID, int funcType){
    float cpuFreq = getCPU(phyNodeID);

    int size = getContainerSize(funcType);

    float runCost = (float)size*cpuFreq*m_cfg.Alpha;
    
    if(size == 0){
        NS_LOG_INFO("run cost size is 0");
    }
    else if(cpuFreq == 0){
        NS_LOG_INFO("run cost cpuFreq is 0");
    }
    else if(m_cfg.Alpha == 0){
        NS_LOG_INFO("run cost Alpha is 0");
    }
    return runCost;
}

void FixedCaching::printResult(std::string filename, bool verbose){
    NS_LOG_FUNCTION(this);
    //result [time slot, id, type, linkdelay, processingdelay, coldstartdelay, iscoldstart, total delay]
    std::vector<float> result;
    float singleCost = 0; // total cost for a request
    float avg_cost = 0;

    float instanCost = 0;
    float runCost = 0;
    float commCost = 0;
    float totalCommCost = 0;
    float totalInstanCost = 0;
    float totalRunCost = 0;


    for(int i=1; i<= m_request_map.size(); i++){
        if(m_request_map.rsMap.find(i) != m_request_map.rsMap.end()){
            auto it_map = m_request_map.rsMap.find(i);
            // Requests rs = it_map->second;

            for(auto it = it_map->second.rs.begin(); it != it_map->second.rs.end(); it++){
                //skip if unserved
                if((*it).served == false){
                    continue;
                }

                NS_LOG_LOGIC("request served");

                m_served_req_num += 1;
                
                result.push_back(i);
                result.push_back(float((*it).id));

                result.push_back(float((*it).function.type));

                float dist = distance((*it).ingress.id, (*it).deployNode.id);

                commCost = dist * m_cfg.CommCostPara;

                // NS_LOG_ERROR(" dist " << dist);

                totalCommCost += commCost;

                result.push_back(commCost);

                singleCost += commCost;

                runCost = getRunCost((*it).deployNode.id, (*it).function.type);
                result.push_back(runCost);
                singleCost += runCost;

                totalRunCost += runCost;

                if((*it).isColdStart == true){
                    instanCost = getInstanCost((*it).deployNode.id, (*it).function.type);
                    result.push_back(instanCost);
                    result.push_back(1.0);
                    singleCost += instanCost;

                    totalInstanCost += instanCost;

                    m_cold_req_num++;

                }else{
                    
                    result.push_back(0.0);
                    result.push_back(0.0);
                }
                result.push_back(singleCost);
                avg_cost += singleCost;
                if(verbose == true){
                    write_vector_file(filename, result);
                }
            
                result.clear();
                singleCost = 0;//total cost for one request
            }
        }
    }//end for

    //log total numbers:
    write_result_title(filename);
    std::vector<float> numbers;
    numbers.push_back(float(m_cold_req_num));
    numbers.push_back(float(m_served_req_num));
    numbers.push_back(float(m_total_req_num));
    float coldstartfreq = float(m_cold_req_num)/float(m_served_req_num);
    numbers.push_back(coldstartfreq);
    avg_cost = float(avg_cost)/float(m_served_req_num);
    
    numbers.push_back(totalCommCost/float(m_served_req_num));
    numbers.push_back(totalInstanCost/float(m_served_req_num));
    numbers.push_back((totalInstanCost+totalCommCost)/float(m_served_req_num));
    numbers.push_back(totalRunCost/float(m_served_req_num));

    numbers.push_back(avg_cost);
    write_vector_file(filename, numbers);
    

}


//print result without slot 1
void FixedCaching::printResult_no_1(std::string filename){
    NS_LOG_FUNCTION(this);
    //result [time slot, id, type, linkdelay, processingdelay, coldstartdelay, iscoldstart, total delay]
    std::vector<float> result;
    float totalDelay = 0;

    float cold_req_num_no_1 = 0;
    float served_req_num_no_1 = 0;
    float total_req_num_no_1 = 0;
    float avg_delay = 0;


    for(int i=2; i<= m_request_map.size(); i++){
        if(m_request_map.rsMap.find(i) != m_request_map.rsMap.end()){
            auto it_map = m_request_map.rsMap.find(i);
            // Requests rs = it_map->second;

            for(auto it = it_map->second.rs.begin(); it != it_map->second.rs.end(); it++){

                total_req_num_no_1 += 1;
                //skip if unserved
                if((*it).served == false){
                    continue;
                }

                NS_LOG_LOGIC("request served");

                
                served_req_num_no_1 += 1;

                
                result.push_back(i);
                result.push_back(float((*it).id));

                result.push_back(float((*it).function.type));

                float linkDelay = distance((*it).ingress.id, (*it).deployNode.id)/1000;

                result.push_back(linkDelay);

                (*it).calcLinkDelay(linkDelay);

                totalDelay += totalDelay;
                //conver ms to s
                result.push_back((*it).function.processingTime/1000);
                totalDelay += (*it).function.processingTime/1000;

                if((*it).isColdStart == true){
                    result.push_back((*it).function.coldStartTime);
                    result.push_back(1.0);
                    totalDelay += (*it).function.coldStartTime;

                    
                    cold_req_num_no_1 += 1;
                    
                    
                }else{
                    
                    result.push_back(0.0);
                    result.push_back(0.0);
                }
                result.push_back(totalDelay);
                avg_delay += totalDelay;
                write_vector_file(filename, result);
                result.clear();
                totalDelay = 0;
            }
        }
    }//end for

    //log total numbers:
    write_result_title(filename);
    std::vector<float> numbers;
    numbers.push_back(cold_req_num_no_1);
    numbers.push_back(served_req_num_no_1);
    numbers.push_back(total_req_num_no_1);
    float coldstartfreq = cold_req_num_no_1/served_req_num_no_1;
    numbers.push_back(coldstartfreq);
    avg_delay = avg_delay/served_req_num_no_1;
    numbers.push_back(avg_delay);
    write_vector_file(filename, numbers);
    
    

}


void FixedCaching::genTraffic(int time_slot, int time_slot_num){
    NS_LOG_FUNCTION(this);

    Network_controller nc = Network_controller();
    //data rate can not be too small otherwise the packet will not arrive
    nc.set_map(m_topo, m_cfg, "1kBps", "1kbps");

    nc.set_max_slot(time_slot, time_slot_num);

    nc.create_network();


    //for loop request

    //iterate over time slot
    for(int i=1; i<= m_request_map.size(); i++){
        if(m_request_map.rsMap.find(i) != m_request_map.rsMap.end()){
            auto it_map = m_request_map.rsMap.find(i);
            // Requests rs = it_map->second;

            for(auto it = it_map->second.rs.begin(); it != it_map->second.rs.end(); it++){
                nc.create_request_traffic(*it);

            }
        }


    }//end for
    NS_LOG_DEBUG("create sink m_request_map size " << m_request_map.size());
    nc.create_sink(m_request_map);

}

void FixedCaching::scheduleRequests(float beta_input, float reduFactor_input, bool verbose, float alpha){
    loadConfig("config/config.txt");

    m_cfg.Beta = beta_input;
    m_cfg.ReduFactor = reduFactor_input;
    m_cfg.Alpha = alpha;

    print_parameter(m_cfg);

    loadTopo();
    init();
    initFuncMap(lifeTime_G);
    readRequests();
    createRequests();

    deployRequests();

    //output data
    std::string beta, reducefactor;
    beta = std::to_string(m_cfg.Beta);
    reducefactor = std::to_string(m_cfg.ReduFactor);
    beta.erase(beta.find_last_not_of('0')+1, std::string::npos);
    beta.erase(beta.find_last_not_of('.')+1, std::string::npos);
    reducefactor.erase(beta.find_last_not_of('0')+1,  std::string::npos);
    reducefactor.erase(beta.find_last_not_of('.')+ 1, std::string::npos);
    std::string filename = "result/fc-result-"+ beta +"-" + reducefactor+ ".csv";
    write_time(filename);
    write_parameter(filename, m_cfg);
    // printResult_no_1(filename);
    printResult(filename, verbose);
    //end output data
    

    
    NS_LOG_INFO("Total Cold start Num served<< " << m_cold_req_num);

    NS_LOG_INFO("Total Request Num served<< " << m_served_req_num);

    NS_LOG_INFO("Total Request Num << " << m_total_req_num);

    // genTraffic(1, 30);
}

}//end ns3