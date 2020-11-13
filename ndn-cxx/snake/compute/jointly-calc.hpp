/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NDN_SNAKE_JOINTLY_CALCULATOR_HPP
#define NDN_SNAKE_JOINTLY_CALCULATOR_HPP

#include "ndn-cxx/snake/fm/snake-metadata.hpp"
#include "ndn-cxx/snake-parameters.hpp"
#include "ns3/node-info.h"

namespace ndn{
namespace snake{
class JointlyCalculator
{
public:
  JointlyCalculator(){};
  ~JointlyCalculator(){}
  /**
   * \brief Jointly calculate cost.
   * 
   * \param sysinfo [in] Available resources of computing node.
   * \param metadata [in] Metadata of the needed data.
   * \param paras [in] Function parameters that will be invoking.
   * \return uint64_t 
   */
  virtual uint64_t costCalculate(ns3::SysInfo& sysinfo, SnakeMetadata& metadata,
                                  Parameters& paras) = 0;
};
}//namespace snake
}//namespace ndn

#endif