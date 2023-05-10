#include "operation.h"

#define PI 3.14159265358979323846
#define RADIO_TERRESTRE 6372797.56085
#define GRADOS_RADIANES PI / 180

#include "ns3/core-module.h"
#include <math.h>
#include <algorithm>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <iomanip> // put_time
#include <time.h>
// #include "type.h"
namespace ns3{

float CalcGPSDistance(float latitud1, float longitud1, float latitud2, float longitud2){
    double haversine;
    double temp;
    double distancia_puntos;

    latitud1  = latitud1  * GRADOS_RADIANES;
    longitud1 = longitud1 * GRADOS_RADIANES;
    latitud2  = latitud2  * GRADOS_RADIANES;
    longitud2 = longitud2 * GRADOS_RADIANES;

    haversine = (pow(sin((1.0 / 2) * (latitud2 - latitud1)), 2)) + ((cos(latitud1)) * (cos(latitud2)) * (pow(sin((1.0 / 2) * (longitud2 - longitud1)), 2)));
    temp = 2 * asin(std::min(1.0, sqrt(haversine)));
    distancia_puntos = RADIO_TERRESTRE * temp;

   return distancia_puntos;
}

void write_time(std::string filename){

    // std::string filename = "result/my_result.csv";

    std::ofstream outfile;

    outfile.open(filename, std::ofstream::app);//append mode
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss; 

    outfile <<std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << std::endl;
     //result [id, linkdelay, processingdelay, coldstartdelay, iscoldstart, total delay]
    outfile <<"timeslot" << ","<< "id"<<","<<"type" <<"," <<"commCost"<<","<<"runCost"<<","<<"InstanCost"<<","<<"iscold"<<","<<"singleCost"<<std::endl;

    outfile.close();
}

void write_result_title(std::string filename){
    std::ofstream outfile;

    outfile.open(filename, std::ofstream::app);//append mode


    outfile <<"coldNum"<< ","<< "servedNum"<< ","<< "totalNum"<< "," <<"coldfreq"<< ","<<"avgcommcost"<< "," << "avginstancost"<< "," << "avgruncost"<<","<<"avgcost"<<std::endl;

    outfile.close();

}

void write_vector_file(std::string filename, std::vector<float> data = std::vector<float>())
{
    // std::string filename = "result/my_result.csv";
    std::ofstream outfile;

    outfile.open(filename, std::ofstream::app);//append mode

    
    if (outfile.is_open())

    {  

        for(auto it = data.begin(); it != data.end(); it++)
        {   
            outfile << *it;
            if(it != data.end()-1){
                outfile << ",";
            }
        }
        outfile<<std::endl;
        
        outfile.close();
        
    }
    else
    {
       std::cout<< "failes to open file " << filename << std::endl;
    }
}

//write the parameters of config
void write_parameter(std::string filename, ns3::ServerlessConfig cfg)
{
    // std::string filename = "result/my_result.csv";
    std::ofstream outfile;

    outfile.open(filename, std::ofstream::app);//append mode

    std::vector<float> data;

    data.push_back(cfg.CommCostPara);
    data.push_back(cfg.MemCap);
    data.push_back(float(cfg.NodeNum));
    data.push_back(cfg.Beta);
    data.push_back(float(cfg.SlotNum));
    data.push_back(cfg.ReduFactor);
    data.push_back(cfg.Alpha);
    data.push_back(cfg.cpuFreq);

    if (outfile.is_open())

    {  
        outfile <<"Topo name " <<"," <<cfg.TopoName << std::endl;
        outfile <<"Request file " << "," <<cfg.RequestFile << std::endl;

        int count = 1;
        for(auto it = data.begin(); it != data.end(); it++)
        {   

            switch(count){
                case 1:
                     outfile << "CommCostPara" << ",";
                     break;
                case 2:
                     outfile << "MemCap" << ",";
                     break;
                case 3:
                     outfile << "NodeNum" << ",";
                     break;
                case 4:
                     outfile << "Beta" << ",";
                     break;
                case 5:
                     outfile << "SlotNum" << ",";
                     break;
                case 6:
                     outfile << "ReduFactor" << ",";
                     break;
                case 7:
                     outfile << "Alpha" << ",";
                     break;
                case 8:
                     outfile << "cpuFreq" << ",";
                     break;
                     
            }

            outfile << *it <<std::endl;
            count++;
        }
        // outfile<<std::endl;
        
        outfile.close();
        
    }
    else
    {
       std::cout<< "failes to open file " << filename << std::endl;
    }
}


void print_parameter(ns3::ServerlessConfig cfg){
   std::cout <<"------Print config----" << std::endl;

   std::cout << "TopoName " << cfg.TopoName << std::endl;

   std::cout << "RequestFile "<<cfg.RequestFile << std::endl;

   std::cout << "CommCostPara "<<cfg.CommCostPara << std::endl;

   std::cout << "MemCap "<< cfg.MemCap << std::endl;
   std::cout << "NodeNum "<<cfg.NodeNum << std::endl;
   std::cout << "Beta " << cfg.Beta << std::endl;
   std::cout << "SlotNum "<< cfg.SlotNum << std::endl;

   std::cout << "ReduFactor " << cfg.ReduFactor << std::endl;

   std::cout << "Alpha " << cfg.Alpha << std::endl;
   std::cout << "cpuFreq " << cfg.cpuFreq << std::endl;



}

}//end ns3 namespace