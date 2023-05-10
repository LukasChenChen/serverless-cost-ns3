#ifndef OPERATION_H
#define OPERATION_H

#include "ns3/core-module.h"
#include "type.h"
using namespace ns3;

namespace ns3{
float CalcGPSDistance(float latitud1, float longitud1, float latitud2, float longitud2);

void write_vector_file(std::string filename, std::vector<float> data);

void write_time(std::string filename);

void write_result_title(std::string filename);

void write_parameter(std::string filename, ServerlessConfig cfg);

void print_parameter(ns3::ServerlessConfig cfg);
} //end namespace ns3

#endif /* OPERATION_H */