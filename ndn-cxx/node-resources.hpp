/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file node-resources.hpp
 * @author QI-Jianpeng
 * @brief Node resource
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
 * Usage:
 * \code{.cpp}
 *   Resource se(100,200);
 *   std::string str = se.Serialize();
 *   Resource der;
 *   der.DeSerialize(str);
 * \endcode
 */

#include <rapidjson/rapidjson.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <stdexcept>

namespace ndn{
namespace snake{
  using namespace rapidjson;
//TODO(qjp): Add a static method to get the 
//           current system avaliable resources.
class Resource{
  public:
    static const uint64_t DEFAULT_AVALIABLE_CPU = 10000l;
    static const uint64_t DEFAULT_AVALIABLE_MEM = 10000l;

    Resource(const uint64_t cpu, const uint64_t mem):_cpu(cpu), _mem(mem){}
    Resource(const Resource& res):_cpu(res._cpu), _mem(res._mem){}

    ~Resource(){}
    /**
     * @brief Get resources from the current node.
     * 
     * @param res 
     * @return Resource 
     */
    Resource operator-(const Resource& res){
      Resource tmp;
      if(this->_cpu >= res._cpu && this->_mem >= res._mem){
        tmp._cpu = this->_cpu - res._cpu;
        tmp._mem = this->_mem - res._mem;
        return tmp;
      } else {
        throw std::invalid_argument( "Resource on current node not avaliable");
      }
    }
    /**
     * @brief Restore the resources
     * 
     * @param res 
     * @return Resource& 
     */
    Resource operator+(const Resource& res){
      Resource tmp;
      tmp._cpu = this->_cpu + res._cpu;
      tmp._mem = this->_mem + res._mem;
      if(isValidate()){
        return tmp;
      } else {
        throw std::invalid_argument("Your resources putting back are t0o high!");
      }
      
    }
    bool isCPUAvaliable(){
      return _cpu >=0;
    }
    bool isMemAvaliable(){
      return _mem >= 0;
    }
    bool isValidate(){
      return _cpu <= DEFAULT_AVALIABLE_CPU && _mem <= DEFAULT_AVALIABLE_MEM;
    }


    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();
        writer.String("cpu");
        writer.Uint64(_cpu);
        writer.String("mem");
        writer.Uint64(_mem);
        writer.EndObject();
    }
    std::string Serialize(){
      StringBuffer sb;
      PrettyWriter<StringBuffer> writer(sb);
      this->Serialize(writer);
      return sb.GetString();
    }
    void DeSerialize(const rapidjson::Value & obj){
      this->_cpu = obj["cpu"].GetUint64();
      this->_mem = obj["mem"].GetUint64();
    }
    void DeSerialize(const std::string &str){
      rapidjson::Document doc;
       doc.Parse(str.c_str());
       this->DeSerialize(doc);
    }

    Resource(){};
    uint64_t getCPU(){
      return _cpu;
    }
    uint64_t getMem(){
      return _mem;
    }
  private:
    uint64_t _cpu;
    uint64_t _mem;
};
}//namespace snake
}//namespace ndn