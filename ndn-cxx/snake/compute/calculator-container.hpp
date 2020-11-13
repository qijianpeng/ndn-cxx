/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NDN_SNAKE_CALCULATOR_CONTAINER_HPP
#define NDN_SNAKE_CALCULATOR_CONTAINER_HPP
#include "jointly-calc.hpp"
#include "text-calc.hpp"
#include "ndn-cxx/snake/fm/file-type.hpp"
#include "ndn-cxx/snake/fm/snake-metadata.hpp"
#include "ndn-cxx/snake/fm/default-metadata.hpp"

namespace ndn{
namespace snake{

class CalculatorContainer 
{
private:
    /* Here will be the instance stored. */
    static TextJointlyCalculator* textJointlyCalculator;
    /* Private constructor to prevent instancing. */
    CalculatorContainer();

public:
    /* Static access method. */
    static JointlyCalculator* getInstance(uint64_t fileType);
};

/* Null, because instance will be initialized on demand. */
TextJointlyCalculator* CalculatorContainer::textJointlyCalculator =  new TextJointlyCalculator();

JointlyCalculator* CalculatorContainer::getInstance(uint64_t fileType)
{
  JointlyCalculator* calculator;
  switch (fileType)
  {
  case FileType::TXT :
    calculator = textJointlyCalculator;
    break;
  case FileType::MP3 ://
  case FileType::MP4 ://
  default:
    calculator = textJointlyCalculator;//< Use Text as default.
    break;
  }
  return calculator;
}

CalculatorContainer::CalculatorContainer()
{}
}//namespace ndn
}//namespace snake
#endif