/*
 * snake-utils.hpp
 *
 *  Created on: Sep 4, 2020
 *      Author: qijianpeng
 */

#ifndef NDN_UTIL_SNAKE_UTILS_HPP
#define NDN_UTIL_SNAKE_UTILS_HPP

#include "ndn-cxx/interest.hpp"
#include "ns3/computation-module.h"
#include "ns3/core-module.h"
#include "ns3/node-list.h"

#include "ns3/node.h"
#include "ns3/ptr.h"

namespace ndn{
namespace snake{
namespace util{
/**
 * Check the function in the \c interest is executed or not.
 * This function can be invoked on the Interest Processing
 * Path.
 */
bool isFunctionExecuted(const Interest& interest);
/**
 * Set the function in the \c interest as executed.
 * This function can be invoked on the Interest Processing
 * Path.
 */
void setFunctionExecuted(Interest& interest);
/**
 * Remove/Reset the function flag in the \c interest.
 * This function can be invoked on the Interest Processing
 * Path.
 */
void markFunctionAsUnexecuted(Interest& interest);
void removeFunctionTag(const Interest& interest);
/**
 * Check the function in the \c data is executed or not.
 * This function can be invoked on the Data Processing
 * Path.
 */
bool isFunctionExecuted(const Data& data);

/**
 * Remove/Reset the function flag in the \c data.
 * This function can be invoked on the Data Processing
 * Path.
 */
void removeFunctionTag(const Data& data);
std::string extractFunctionName(std::string& str);

uint64_t functionInvoke(ns3::Ptr<ns3::Node> &node, Data& data, std::string functionName, std::string functionParameters);
bool canExecuteFunction(const Data &data);
void afterFunctionInvoke(Data& newData);
bool isBelong2SnakeSystem(const Interest& interest);
bool isBelong2SnakeSystem(const Data& data);
void tryToMarkAsFunction(Interest& interest);
Name recombineNameWithSessionId(const Name &nameWithAllComponents, const uint64_t sessionId);
shared_ptr<Data> cloneData(const Data& data);
void cloneTags(const TagHost&src, const TagHost& dst);
/**
 * \brief Inject a ssesion id into `Interest`, `Data`, `Nack`
 * 
 * \param tagHost 
 * \param sessionId 
 */
void injectSessionId(const TagHost & tagHost, const uint64_t sessionId,
                    uint64_t (*accumulationFunc)(uint64_t, uint64_t));
uint64_t xorOperator(uint64_t a, uint64_t b);
ns3::SysInfo* getCurrentNodeSysInfo(void);
std::string unescape( std::string str);


uint64_t costEstimator(const Interest &interest, const Data& data, ns3::Ptr<ns3::Node> & currentNode);

uint64_t
hashing(const std::string function, const std::string functionParas, const uint64_t nodeId) ;

}
}//namespace snake
} //namespace ndn

#endif /* NDN_UTIL_SNAKE_UTILS_HPP */
