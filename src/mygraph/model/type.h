#ifndef TYPE_H
#define TYPE_H

#include "ns3/core-module.h"
#include <string>
#include <algorithm>
using namespace ns3;

namespace ns3{

struct FunctionFreq{
    std::map<int, int> m; //<funcType, freq>

    int get(int funcType){

        auto it = m.find(funcType);
        if(it == m.end()){
            return 0;
        }else{
            return it->second;
        }
    }

    void add(int funcType){
        auto it = m.find(funcType);
        if(it == m.end()){
            return;
        }else{
            it->second = it->second + 1;
        }

    }

    void minus(int funcType){
         auto it = m.find(funcType);
        if(it == m.end()){
            return;
        }else{
            it->second = it->second - 1;
        }
    }
};

//tag put in the packet.
struct requestTag{
    int      id;  
    int      ingress;  
    int      type; //function type      
    int      isColdStart;             
    int      served;
    int      hostNode;
    int      arriveTime;
};

struct PhyNode{
    int     id;
    float   latitude;
    float   longitude;
    float   mem;
    float   cpuFreq;
    std::map<int, float> funcFreq;  //<functype, freq>
    std::map<int, float> recency;  //<functype, time>

    float getFreq(int funcType){
        if(funcFreq.find(funcType) == funcFreq.end()){
            funcFreq.insert({funcType, 1});
            return 1;
        }else{
            float freq = funcFreq[funcType];
            if(!(freq >0)){
                funcFreq.insert({funcType, 1});
                return 1;
            }
            return freq;
        }
    }

     float getRecency(int funcType){
        if(recency.find(funcType) == recency.end()){
            recency.insert({funcType,1});
            return 1;
        }else{
            float recen = recency[funcType];
             if(!(recen > 0)){
                recency.insert({funcType,1});
                return 1;
            }
            return recen;
        }
    }

    void setRecency(int funcType, float recen){
        if(recency.find(funcType) == recency.end()){
            recency.insert({funcType, recen});
            return;
        }else{
            recency[funcType] = recen;
            return;
        }
    }

    void setFreq(int funcType, float freq){
        if(funcFreq.find(funcType) == funcFreq.end()){
            funcFreq.insert({funcType, freq});
            return;
        }else{
            funcFreq[funcType] = freq;
            return;
        }
    }

    void addFreq(int funcType){
        if(funcFreq.find(funcType) == funcFreq.end()){
            funcFreq.insert({funcType, 1});
            return;
        }else{
            funcFreq[funcType] += 1;
            return;
        }
    }

    void minusFreq(int funcType){
        if(funcFreq.find(funcType) == funcFreq.end()){
            return;
        }else{
            funcFreq[funcType] -= 1;
            return;
        }
    }
};

struct Function{
    int     name; //name is a number, request will be associated with this. request-> function->phynode
    int     type;
    int     port;
    float   size;
    float   coldStartTime;
    float   processingTime;
    
    float   clock =0;
    float   priority = 0;
    PhyNode phyNode; //deployed node
    int     lifeTime;
    int     lastUseTime = 0; // the function last time being used.

    // void activePriority(float m_clock, FunctionFreq &m_functionfreq){
    //     int freq = m_functionfreq.get(type);
    //     clock = m_clock;
    //     priority = clock + ((float)freq + coldStartTime)/(size/1000);
    // }

    // void cachePriority(FunctionFreq m_functionfreq){
    //     int freq = m_functionfreq.get(type);
    //     priority = clock + ((float)freq + coldStartTime)/(size/1000);
    // }

    void minusLife(){
        lifeTime = lifeTime - 1;
    }
    //renew the lifetime
    void activeLifeTime(int lifeTime_input){
        lifeTime = lifeTime_input;
    }

    void activeLastUsed( int timeSlot){
        lastUseTime = timeSlot;
    }

};

struct FunctionInfo{
    float processingTime; //ms
    float coldStartTime; //s
    int   size ; //MB
    int lifeTime; //minute
};

struct FunctionInfoMap{
    std::map<int, FunctionInfo> funcMap; //<funcType, FunctionInfo.

    void add(int funcType, FunctionInfo fi){
        funcMap.insert({funcType, fi});
    }

    bool get(int funcType, FunctionInfo &fi){

        if(funcMap.find(funcType) == funcMap.end()){
            return false;
        }else{
            fi = funcMap[funcType];
            return true;
        }
    }

    int getSize(int funcType){

        if(funcMap.find(funcType) == funcMap.end()){
            return 0;
        }else{
            auto fi = funcMap[funcType];
            return fi.size;
        }

    }
};


struct ServerlessConfig {
    std::string TopoName;
    std::string RequestFile;
    float       CommCostPara; //parameter for communication cost
    float       MemCap;
    int         NodeNum;
    float       Beta;
    int         SlotNum;
    float       ReduFactor;
    float       Alpha;  // alpha parameter for running cost.
    float       cpuFreq;
};

struct Location{
    float latitude;
    float longitude;

    void init(float lat1, float long1){
        latitude = lat1;
        longitude = long1;
    }
};

typedef std::map<int, int> ReqNumMap; //<time_slot, req num>

struct ReqOnNodes{
    std::vector<int> numVector; // for index 0, 1 ... thats nodes 1,2...125

    void add(int num){
        numVector.push_back(num);
    }

    void clear(){
        numVector.clear();
    }
};

//each time slot the number of requests
struct ReqOnNodesTime{
    std::vector<ReqOnNodes> numVector; //for index 0,...N, it is time slot 1....N+1

    void add(ReqOnNodes ron){
        numVector.push_back(ron);
    }

    void clear(){
        numVector.clear();
    }
};

struct ReqOnNodesTimeType{
    std::map<int,ReqOnNodesTime> numMap; //<funcType, reqOnNodesTime>

    void add(int i, ReqOnNodesTime ront){
        numMap[i] = ront;
    }
};

struct Request{
    int      id;
    Function function;
    PhyNode  ingress;
    int      arriveTime;
    bool     served;
    PhyNode  deployNode;
    bool     isColdStart;
    float    linkDelay;

    void update(Function f, PhyNode p, bool coldStart){
       function = f;
       deployNode = p;
       served = true;
       isColdStart = coldStart;
    }

    void calcLinkDelay(float ld){
        linkDelay = ld;
    }

};

struct Requests {
    std::vector<Request> rs;

    void add(Request r){
        rs.push_back(r); 
    }

    int size(){
        return rs.size();
    }

    bool find(int requestID, Request &r){

        for(auto it = rs.begin(); it != rs.end(); it++){
            if((*it).id == requestID){
                r = *it;
                return true;
            }
            else{
                r.id = -1;
                return false;
            }
            
        }
    }
};


struct RequestsMap{
    std::map<int,Requests> rsMap; //<time_slot, requests>

    void add(int timeSlot, Request r){
        //create an entry if cannot find the key
        if(rsMap.find(timeSlot) == rsMap.end()){
            Requests rs; 
            rs.add(r);
            rsMap.insert({timeSlot, rs});
        }
        else{
            auto it = rsMap.find(timeSlot);
            Requests rs = it->second;
            rs.add(r);
            it->second = rs;
        }
    }

    int size(){
        return rsMap.size();
    }

    int getTotalNum(){
        int num = 0;
        for(auto it = rsMap.begin(); it != rsMap.end(); it++){
            num += it->second.size();
        }

        return num;
    }

    void insert(int timeSlot, Requests rs){
        rsMap.insert({timeSlot, rs});
    }

    Request find(int timeSlot, int requestID){

        Request r;
        for(auto it = rsMap.begin(); it != rsMap.end(); it++){
            if(it->second.find(requestID,r )){
                return r;
            }
        }

        r.id = -1;
        return r;


    }
};

struct Distance{
    int   phyNodeID;
    float distance;

    bool operator< (const Distance& val) const {
        return distance < val.distance;
    }

    int getID(){
        return phyNodeID;
    }
};

struct DistSlice{
    std::vector<Distance> vec;

    std::vector<Distance> get(){
        return vec;
    }

    int size(){
        return vec.size();
    }

    void add(Distance d){
        vec.push_back(d);
    }

    void show(){
        std::cout << " show DistSlice " << std::endl;
        for(auto it = vec.begin(); it != vec.end(); it++){
            std::cout << (*it).getID();
            std::cout << ",";
        }
        std::cout << std::endl;
    }
};

struct Topology {
   std::map<int, PhyNode> m; //<nodeid, phynode>

   PhyNode get(int phyNodeID){
       auto it = m.find(phyNodeID);
       PhyNode p;
       p.id = 0;
       if(it == m.end()){
           //cannot find the node
           return p;
       }else{
           return it->second;
       }  
   }

   void add(int phyNodeID, PhyNode p){

       m.insert({phyNodeID, p});

   }

   void update(std::string operation, int &phyNodeID, float size){

       if (operation == "add"){
           m[phyNodeID].mem = m[phyNodeID].mem + size;
       }else if(operation == "minus"){
           m[phyNodeID].mem = m[phyNodeID].mem - size;
       }

   }

   int size(){
       return m.size();
   }

   void addFreq(int phyNodeID, int funcType){

       PhyNode p = get(phyNodeID);
       p.addFreq(funcType);

       m[phyNodeID] = p;
   }

   void minusFreq(int phyNodeID, int funcType){

       PhyNode p = get(phyNodeID);
       p.minusFreq(funcType);

       m[phyNodeID] = p;
   }

   void setRecency(int phyNodeID, int funcType, float recen){

       PhyNode p = get(phyNodeID);
       p.setRecency(funcType, recen);

       m[phyNodeID] = p;
   }
   //add freq tp all phynodes
   void addFreqAll(int funcType){ 
        for(auto it = m.begin(); it != m.end(); it++){
            it->second.addFreq(funcType);
        }

   }

   void setRecencyAll( int funcType, float recen){

       for(auto it = m.begin(); it != m.end(); it++){
            it->second.setRecency(funcType, recen);
        }
   }
};

struct Cache{
    int phyNodeID;
    std::vector<Function> functionList; //a list of functions, actually containers
    //remove a function by index
    void remove(int index, FunctionFreq m_functionfreq){

        m_functionfreq.minus(functionList[index].type);

        if(functionList.size()==0){
            return;
        }
        functionList.erase(functionList.begin() + index);
        
    }

    void add(Function f, FunctionFreq m_functionfreq){
        // m_functionfreq.minus(f.type);

       
        functionList.push_back(f);
        sortlist();

    }

    int size(){
        return functionList.size();
    }
   
    void sortlist(){
        std::sort(functionList.begin(), functionList.end(), [](Function &a, Function &b){return a.priority<b.priority;});
    }

    void sortlistLifeTime(){
        std::sort(functionList.begin(), functionList.end(), [](Function &a, Function &b){return a.lifeTime<b.lifeTime;});
    }
    //ascending order
    void sortlistUseTime(){
        std::sort(functionList.begin(), functionList.end(), [](Function &a, Function &b){return a.lastUseTime<b.lastUseTime;});
    }
    //remove a particular type 
    bool removeType(int funcType){

        for(auto it = functionList.begin(); it != functionList.end(); it++){
            if((*it).type == funcType){
                functionList.erase(it);
                return true;
            }
        }
        return false;

    }
};


struct CacheMap {
    std::map<int, Cache> caches; // <phynodeid, cache>

    void add(Function f, FunctionFreq m_functionfreq){
        Cache c = get(f.phyNode.id);
        if(c.phyNodeID == 0){
            //not found, then we need to create on
            c.phyNodeID = f.phyNode.id;
            c.add(f, m_functionfreq);
            caches[f.phyNode.id] = c;
            return;
        }

        c.add(f, m_functionfreq);

        caches[f.phyNode.id] = c;
    }

    Cache get(int phyNodeID){
        auto it = caches.find(phyNodeID);

        Cache c;
        c.phyNodeID = 0; // use this to validate

        if (it == caches.end()){
            return c;
        }else{
            return it->second;
        }
    }
    //remove a container from cache
    void remove(int phyNodeID, int index, FunctionFreq m_functionfreq){
         auto it = caches.find(phyNodeID);

         if (it == caches.end()){
            //no such node, return
            return;
        }else{
            it->second.remove(index, m_functionfreq);

        }
       
        //change mem

    }

    Function getIdleFunction(int phyNodeID, int funcType, int &index){

        Cache c = get(phyNodeID);
        Function f;
        if(c.phyNodeID == 0){
            //cannot find this id or cahce under this id
            // std::cout<< "cannot find cached function on node ! " << phyNodeID << std::endl;
            index = -1;
            return f;
        }
        
        int i = 0;
        for(auto it = c.functionList.begin(); it != c.functionList.end(); it++){
            if((*it).type == funcType){
                // std::cout<< "find the cached function!" <<std::endl;
                index = i;
                return *it;
            }
            i++;
        }

        index = -1;
        return f;
    }

    float getLowestPriority(int phyNodeID){
        Cache c = get(phyNodeID);

        if (c.size() == 0){
            return 0;
        }else{
            return c.functionList[0].priority; 
        }
    }
    //delete the first element in cache
    bool deleteLowestPriority(int phyNodeID, Function &function, FunctionFreq m_functionfreq){
        Cache c = get(phyNodeID);

        if(c.size() > 0){
            function = c.functionList[0];
            remove(phyNodeID, 0, m_functionfreq);
            return true;
        }
        else{
            return false;
        }

    }

    void sort(){

        for(auto it = caches.begin(); it != caches.end(); it++){
            it->second.sortlist();
        }
        
    }

    int size(){
        int size = 0;
        for(auto it = caches.begin(); it != caches.end(); it++){
            size += it->second.size();
        }
        return size;
    }

    void sortLifeTime(){

        for(auto it = caches.begin(); it != caches.end(); it++){
            it->second.sortlistLifeTime();
        }
        
    }

    //sort the cache by least recent use time
    void sortUseTime(){

        for(auto it = caches.begin(); it != caches.end(); it++){
            it->second.sortlistUseTime();
        }
        
    }
    //hist and cast
    float getLowestLifeTime(int phyNodeID){
        Cache c = get(phyNodeID);

        if (c.size() == 0){
            return 0;
        }else{
            return c.functionList[0].lifeTime; 
        }
    }
    //Lru
    int getLowestUseTime(int phyNodeID){
        Cache c = get(phyNodeID);

        if (c.size() == 0){
            return 0;
        }else{
            return c.functionList[0].lastUseTime; 
        }
    }

    //delete the least recently used container
    bool deleteLeastUse(int phyNodeID, Function &function, FunctionFreq m_functionfreq){
        Cache c = get(phyNodeID);

        if(c.size() > 0){
            function = c.functionList[0];
            remove(phyNodeID, 0, m_functionfreq);
            return true;
        }
        else{
            return false;
        }

    }

    //delete a container based on the probability
    bool deleteProb(int phyNodeID, int funcType, Topology &m_topo){
        Cache c = get(phyNodeID);

        if(c.size() > 0){
            //cannot find the type
            if (c.removeType(funcType) ==false){
                return false;
            }

            caches[phyNodeID] = c;

            //reduce the freq in topo
            m_topo.minusFreq(phyNodeID, funcType);
            
            return true;
        }
        else{
            return false;
        }

    }

};

struct Functions{
    int type;
    std::vector<Function> slice;

    void add(Function f){
        slice.push_back(f);
    }

    int size(){
        return slice.size();
    }

    Function get(int index){
        Function f;
        f.type = 0;
        if(index < slice.size()){
            return slice[index];
        }else{
            return f;
        }
    }
};

struct NodeFunctions {
    int phynodeID;
    std::map<int, Functions> functions; //<funcType, functions>

    void add(Function f){
        int funcType = f.type;

        auto it = functions.find(funcType);

        if(it == functions.end()){
            Functions fs;
            fs.add(f);
            functions.insert({funcType, fs});
        }else{
            it->second.add(f);
        }
    }

    int size(){
        return functions.size();
    }
};

struct ActiveFunctions {
    std::map<int, NodeFunctions> m; //<phynodeid, functionsmap>
    //update the frequency and recency.
    void add(Function f, int phynodeID, FunctionFreq &m_functionfreq, int m_clock){

        auto it = m.find(phynodeID);

        m_functionfreq.add(f.type);
        
        
        //add to the function
        if (it == m.end()){
            NodeFunctions nfs;
            nfs.add(f);
            m.insert({phynodeID, nfs});
        }
        else{
            it->second.add(f);
        }
        
        
    }

    int size(){
        return m.size();
    }

    void clear(){
        m.clear();
    }

};


typedef std::pair<int, float> ProbPair; //<funcType, probability>


struct ProbPairVec{
    std::vector<ProbPair> probPair_v;
    //ascending order
    void sortVec(){
        std::sort(probPair_v.begin(), probPair_v.end(), [](ProbPair &a, ProbPair &b){return a.second<b.second;});
    }

    void push_back(ProbPair pp){
        probPair_v.push_back(pp);
    }
};


} //end ns3

#endif /* TYPE_H */