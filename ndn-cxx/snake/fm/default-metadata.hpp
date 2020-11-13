/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NDN_SNAKE_DEFAULT_SNAKE_METADATA_HPP
#define NDN_SNAKE_DEFAULT_SNAKE_METADATA_HPP
#include "snake-metadata.hpp"
namespace ndn{
namespace snake{
class DefaultSnakeMetadata : public SnakeMetadata 
{
public:
  DefaultSnakeMetadata(const char* str):SnakeMetadata(str)
  {
    // std::cout<< "DefaultSnakeMetadata(const char* str)" << std::endl;
  }
  DefaultSnakeMetadata():SnakeMetadata(){
    // std::cout<< "DefaultSnakeMetadata()" << std::endl;
  }
  ~DefaultSnakeMetadata(){}
  uint32_t getFileType()
  {
    return FileType::TXT;
  }
};
}//namespace snake
}//namespace ndn
#endif