/*
 * snake-utils.hpp
 *
 *  Created on: Sep 4, 2020
 *      Author: qijianpeng
 */

#ifndef NDN_UTIL_SNAKE_UTILS_HPP
#define NDN_UTIL_SNAKE_UTILS_HPP

#include "ndn-cxx/interest.hpp"

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
void setFunctionExecuted(const Interest& interest);
/**
 * Remove/Reset the function flag in the \c interest.
 * This function can be invoked on the Interest Processing
 * Path.
 */
void removeFunctionTag(const Interest& interest);
/**
 * Check the function in the \c data is executed or not.
 * This function can be invoked on the Data Processing
 * Path.
 */
bool isFunctionExecuted(const Data& data);
/**
 * Set the function in the \c data as executed.
 * This function can be invoked on the Data Processing
 * Path.
 */
void setFunctionExecuted(const Data& data);
/**
 * Remove/Reset the function flag in the \c data.
 * This function can be invoked on the Data Processing
 * Path.
 */
void removeFunctionTag(const Data& data);
}
}//namespace snake
} //namespace ndn

#endif /* NDN_UTIL_SNAKE_UTILS_HPP */
