/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NDN_SNAKE_METADATA_WRAPPER
#define NDN_SNAKE_METADATA_WRAPPER

#include "snake-metadata.hpp"
#include "default-metadata.hpp"
namespace ndn{
namespace snake{
class MetadataWrapper{
public :
  MetadataWrapper(const char* str){
    SnakeMetadata* tmpMetadata = new SnakeMetadata(str);
    uint32_t filetype = tmpMetadata->getValue<uint32_t>("filetype");
    switch (filetype) {
      case FileType::TXT :
        snakeMetadata = new DefaultSnakeMetadata(str);
        break;
      case FileType::MP3:
      case FileType::MP4:
      default:
         snakeMetadata = new DefaultSnakeMetadata(str);
    }
    // DefaultSnakeMetadata dsnakeMetadata(str);
    this->snakeMetadata->Initialize(str);
    // std::cout << "MetadataWrapper constructor: " << this->snakeMetadata->Serialize() << std::endl;
    //  delete tmpMetadata;
  }
  
  SnakeMetadata* getMetadata()
  {
    return snakeMetadata;
  }
  std::string Serialize(){
    return snakeMetadata->Serialize();
  }
private:
  SnakeMetadata* snakeMetadata = 0;
  // DefaultSnakeMetadata snakeMetadata;
};
}//namespace snake
}//namespace ndn
#endif