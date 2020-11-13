/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @author QI-Jianpeng
 * @version 0.1
 * @date 2020-09-08
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
#ifndef NDN_SNAKE_PARAMETERS_HPP
#define NDN_SNAKE_PARAMETERS_HPP
#include <rapidjson/rapidjson.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <stdexcept>
#include <map>
typedef std::map<std::string, std::string> StringMap;

namespace ndn{
namespace snake{
/**
 * \brief Macro for steady property access.
 * 
 */
#define PROPERTY_ACCESSOR_DEFINE(type, propertyName, repidAtomType) \
  type propertyName ## repidAtomType ## PropertyValue(void) {                \
    if(doc.HasMember(#propertyName)){                               \
        return doc[#propertyName].Get##repidAtomType();                      \
      } else return 0;                                              \
  }

using namespace rapidjson;
//TODO(qjp): Add a static method to get the 
//           current system avaliable resources.
class Parameters{
  public:
    // static StringMap TYPE_MAPPING;
    Parameters(){};
    ~Parameters(){};
    const rapidjson::Value & DeSerialize(const std::string &str)
    {
      doc.Parse(str.c_str());
      return doc;
    }

    void Initialize(const std::string &str){
      DeSerialize(str);
    }
    
    const std::string Serialize()
    {
      StringBuffer buffer;
      Writer<StringBuffer> writer(buffer);
      doc.Accept(writer);
      return buffer.GetString();
    }

    PROPERTY_ACCESSOR_DEFINE(uint64_t, cpu, Uint64);
    PROPERTY_ACCESSOR_DEFINE(uint64_t, mem, Uint64);

    // template<typename T>
    // T getProperty(std::string & propertyName)
    // {
    //   if(doc.HasMember(propertyName)){
    //     return 
    //   }
    // }
  private:
    rapidjson::Document doc;
  //   static StringMap _TYPE_MAPPING(){
  //     StringMap tmpMap ;
  //     tmpMap["uint64_t"] = "Uint64";
  //     tmpMap["std::string"] = "String";
  //     return tmpMap;
  // }
};
// StringMap Parameters::TYPE_MAPPING =  Parameters::_TYPE_MAPPING();

}//namespace snake
}//namespace ndn
#endif