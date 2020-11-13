/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @author QI-Jianpeng
 * @version 0.1
 * 
 * @copyright Copyright (c) 2020
 * 
 * Requirement: Install the RapidJSON(https://github.com/Tencent/rapidjson) into your `include` lib.
 * Steps:
 * \code{.sh}
     git clone https://github.com/Tencent/rapidjson.git 
     cd rapidjson/
     git submodule update --init
     mkdir build
     cd build
     cmake ..
     make
     make install
 * \endcode
 */
#ifndef NDN_SNAKE_SNAKE_METADATA_HPP
#define NDN_SNAKE_SNAKE_METADATA_HPP
#include <rapidjson/rapidjson.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <stdexcept>
#include <map>

#include "ndn-cxx/encoding/block-helpers.hpp"
#include "file-type.hpp"

namespace ndn{
namespace snake{

using namespace rapidjson;

class SnakeMetadata{
  public:
    SnakeMetadata(){
      doc.SetObject();
    };
    SnakeMetadata(const char* str){
      doc.SetObject();
      Initialize(str);
    };

    ~SnakeMetadata(){};
    uint32_t getFileType(){
      return FileType::UNRECOGNIZED;
    }
    const rapidjson::Value & DeSerialize(const char* str)
    {
      doc.Parse(str);
      return doc;
    }

    void Initialize(const char* str){
      doc.SetObject();
      DeSerialize(str);
    }
    
    const std::string Serialize()
    {
      StringBuffer buffer;
      Writer<StringBuffer> writer(buffer);
      doc.Accept(writer);
      return buffer.GetString();
    }
    
    template<typename T>
    T getValue(const char* propertyName)
    {
      if(doc.HasMember(propertyName)){
        T value = doc[propertyName].Get<T>();
        return value;
      } else return NULL;

    }
    template<typename T>
    T getValue(const std::string propertyName)
    {
      return getValue<T>(propertyName.c_str());
    }

    std::string getStringValue(const char* propertyName)
    { // FIXME(QJP) This is hard coding.
      if(doc[propertyName].IsBool())   return std::to_string(doc[propertyName].GetBool());
      if(doc[propertyName].IsInt())   return std::to_string(doc[propertyName].GetInt());
      if(doc[propertyName].IsUint())   return std::to_string(doc[propertyName].GetUint());
      if(doc[propertyName].IsInt64())   return std::to_string(doc[propertyName].GetInt64());
      if(doc[propertyName].IsUint64())   return std::to_string(doc[propertyName].GetUint64());
      if(doc[propertyName].IsDouble())   return std::to_string(doc[propertyName].GetDouble());
      return doc[propertyName].GetString();
    }
    
    std::string getStringValue(std::string propertyName)
    {
      return getStringValue(propertyName.c_str());
    }

   void removeProperty(const char* propertyName)
    {
      if(doc.HasMember(propertyName)){
        doc.RemoveMember(propertyName);
      }
    }
    template<typename T>
    void addProperty(const char* propertyName, T propertyValue)
    {
      removeProperty(propertyName);
      rapidjson::Value v(propertyValue);
      doc.AddMember(rapidjson::StringRef(propertyName), 
        v, doc.GetAllocator());
    }
    void addStringProperty(const char* propertyName, const char* propertyValue)
    {
      removeProperty(propertyName);
      doc.AddMember(rapidjson::StringRef(propertyName),
        rapidjson::StringRef(propertyValue), doc.GetAllocator());
    }
    const ndn::Block getMetadataBlock()
    {
      return ::ndn::encoding::makeStringBlock(::ndn::tlv::MetaInfo, Serialize());
    }
  protected:
    rapidjson::Document doc;
};

}//namespace snake
}//namespace ndn
#endif