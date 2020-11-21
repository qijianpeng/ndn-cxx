#include "snake-utils.hpp"
#include "snake-parameters.hpp"
#include <string.h>
#include <ndn-cxx/lp/tags.hpp>
#include <ndn-cxx/encoding/nfd-constants.hpp>
#include "common/logger.hpp"
#include "ns3/simulator.h"
#include "ns3/hash.h"
#include "ns3/computation-module.h"
#include "ndn-cxx/snake/fm/snake-metadata.hpp"
#include "ndn-cxx/snake/fm/metadata-wrapper.hpp"
#include "ndn-cxx/snake/compute/calculator-container.hpp"
#include "ndn-cxx/snake/compute/jointly-calc.hpp"
#include <tuple>
#include <thread>
#include <chrono>

namespace ndn{
namespace snake{
namespace util{
const std::uint16_t INVALID = 0;
  bool containsFunctionTag(const TagHost& tagHost)
	{
		auto ft = tagHost.getTag<lp::FunctionTag>();
		return ft != nullptr;
	}

	bool containsSnakeMarker(const Name& name)
	{
		return name.toUri().find(ndn::nfd::SNAKE_SEPARATOR, 0) != std::string::npos;
	}

	bool isBelong2SnakeSystem(const Interest& interest)
	{
	  if(containsSnakeMarker(interest.getName())){
			return true;
		}
	  if(containsFunctionTag(interest)) {
	    return true;
	  }
	  return false;
	}

	bool isBelong2SnakeSystem(const Data& data){
	  if(containsSnakeMarker(data.getName())){
			return true;
		}
		if(containsFunctionTag(data)) {
	    return true;
	  }
		return false;
	}

	bool isFunctionExecuted(const Interest& interest){
	  auto tag = interest.getTag<lp::FunctionTag>();
	  if (tag == nullptr) {
		  // no tag
		  return false;
	  } else {
			uint64_t tagValue = tag->get();
			return tagValue == nfd::SnakeComputing::FUNCTION_EXECUTED;
	  }
	  return false;
	}

  template<typename T>
  void cloneTag(const Data& src, const Data& dst){
		shared_ptr<T> srcTag = src.getTag<T>();
		if(nullptr != srcTag){
			dst.setTag(srcTag);
		}
	}

	template<typename T>
	void setTag(const TagHost& tagHost, shared_ptr<T> tag){
		tag.setTag(make_shared<T>(tag));
	}

  template<typename T>
	void setTag(const TagHost& tagHost, uint64_t value){
		tagHost.setTag(make_shared<T>(value));
	}

	template<typename T>
	void unsetTag(const TagHost& tagHost){
		auto tag = tagHost.getTag<T>();
		if(tag != nullptr){
			tagHost.removeTag<T>();
		}
	}

	//\sa ndn-cxx/lp/tags.hpp
  void cloneTags(const Data&src, const Data& dst){

		cloneTag<lp::FunctionTag>(src, dst);
		cloneTag<lp::IncomingFaceIdTag>(src, dst);
		cloneTag<lp::NextHopFaceIdTag>(src, dst);
		cloneTag<lp::CachePolicyTag>(src, dst);
		cloneTag<lp::CongestionMarkTag>(src, dst);
		cloneTag<lp::NonDiscoveryTag>(src, dst);
		cloneTag<lp::PrefixAnnouncementTag>(src, dst);
		cloneTag<lp::HopCountTag>(src, dst);
		cloneTag<lp::SessionTag>(src, dst);
		cloneTag<lp::DataPushTag>(src, dst);
		cloneTag<lp::MetaDataTag>(src, dst);
		cloneTag<lp::MinCostTag>(src, dst);
		cloneTag<lp::MinCostMarkerTag>(src, dst);
		cloneTag<lp::ProcessingTimeTag>(src, dst);
		cloneTag<lp::HopDelayTag>(src, dst);
	}
  shared_ptr<Data> cloneData(const Data& data){
		auto newData = make_shared<Data>(data.getName());
		newData->wireDecode(data.wireEncode());
		//We need to clone all the tags, since tags have been lost after `wireEncode`.
		cloneTags(data, *newData);
		return newData;
	}

	bool isFunctionExecuted(const Data& data){
	  shared_ptr<lp::FunctionTag> tag = data.getTag<lp::FunctionTag>();
	  return (tag != nullptr) && (*tag == nfd::SnakeComputing::FUNCTION_EXECUTED);
	}

	void markFunctionAsExecuted(Interest& interest){
		setTag<lp::FunctionTag>(interest, nfd::SnakeComputing::FUNCTION_EXECUTED);
	}

	void markFunctionAsUnexecuted(Interest& interest){
		setTag<lp::FunctionTag>(interest, nfd::SnakeComputing::FUNCTION_UNEXECUTED);
	}

 	void markFunctionAsExecuted(Data& data){
		 setTag<lp::FunctionTag>(data, nfd::SnakeComputing::FUNCTION_EXECUTED);
	}

  
	void tryToMarkAsFunction(Interest& interest){
		if(isBelong2SnakeSystem(interest)){
      markFunctionAsUnexecuted(interest);
			/**Make this interest as Long-Lived*/
		  interest.setLongLived(true);
		}
	}
	void removeFunctionTag(const Interest& interest){
		unsetTag<lp::FunctionTag>(interest);
	}
	void removeFunctionTag(const Data& data){
    unsetTag<lp::FunctionTag>(data);
	}

	/**
	 * Extract function name and its parameters.
	 * \c str format:  <tt> ndn:://dataName/snake/functionName/ApplicationParameters </tt>
	 */
	std::string extractFunctionName(std::string& str){
      int functionStartPos = str.find(ndn::nfd::SNAKE_SEPARATOR, 0) + ndn::nfd::SNAKE_SEPARATOR.length() + 1;
      std::string functionAndBehind = str.substr(functionStartPos);
      int functionEndPos = functionAndBehind.find_first_of("/");
      std::string functionName(functionAndBehind, 0, functionEndPos);
      return functionName;
	}


  /**
	 * FIXME(qjp): now we just sleep a specefic time dual.
	 * 
	 * TODO 1. for concurrent execution, schedule method should be employed.
	 * @param functionParameters a long number.
	 * @return process delay with ms.
	 */
	uint64_t functionInvoke(ns3::Ptr<ns3::Node> &node, Data& data, std::string functionName, std::string functionParameters){
		
		Parameters paras;
		paras.DeSerialize(functionParameters);
		uint64_t millisecond = paras.cpuUint64PropertyValue();
		ns3::Ptr<ns3::ComputationModel> cm = node->GetObject<ns3::ComputationModel>();
		data.setTag(make_shared<lp::ProcessingTimeTag>(millisecond));
		afterFunctionInvoke(data);
		return (uint64_t)millisecond;
	}

	void afterFunctionInvoke(Data& newData){
        std::string results = "results";
        newData.setContent(::ndn::encoding::makeStringBlock(::ndn::tlv::Content, results));
    	  markFunctionAsExecuted(newData);
	}

	bool canExecuteFunction(const Data &data){
		return true;
	}

  void injectSessionId(const TagHost & tagHost, const uint64_t sessionId,
	  uint64_t (*accumulationFunc)(uint64_t, uint64_t)){

		uint64_t accumulation = sessionId;
		auto tag = tagHost.getTag<lp::SessionTag>();
		if(tag != nullptr){
      accumulation = accumulationFunc(*tag, sessionId);
		}
    setTag<lp::SessionTag>(tagHost, accumulation);
	}
	uint64_t xorOperator(uint64_t a, uint64_t b){
		return a xor b;
	}
  ns3::SysInfo* getCurrentNodeSysInfo()
  {
		int contextVal = ns3::Simulator::GetContext();
    ns3::Ptr<ns3::Node> node = ns3::NodeList::GetNode(contextVal);
		ns3::Ptr<ns3::ComputationModel> cm = node->GetObject<ns3::ComputationModel>();
	  
		ns3::SysInfoValue siv;
		cm->GetAttribute("SystemStateInfo", siv);
	  ns3::SysInfo sysinfo = siv.Get();

		return &sysinfo;
	}
  /**
 * Convert the hex character to an integer from 0 to 15, or -1 if not a hex character.
 * @param c
 * @return
 */
static int
fromHexChar(uint8_t c)
{
  if (c >= '0' && c <= '9')
    return (int)c - (int)'0';
  else if (c >= 'A' && c <= 'F')
    return (int)c - (int)'A' + 10;
  else if (c >= 'a' && c <= 'f')
    return (int)c - (int)'a' + 10;
  else
    return -1;
}

/**
 * Return a copy of str, converting each escaped "%XX" to the char value.
 * @param str
 */
 std::string unescape(const std::string str){
  std::ostringstream result;

  for (size_t i = 0; i < str.size(); ++i) {
    if (str[i] == '%' && i + 2 < str.size()) {
      int hi = fromHexChar(str[i + 1]);
      int lo = fromHexChar(str[i + 2]);

      if (hi < 0 || lo < 0)
        // Invalid hex characters, so just keep the escaped string.
        result << str[i] << str[i + 1] << str[i + 2];
      else
        result << (uint8_t)(16 * hi + lo);

      // Skip ahead past the escaped value.
      i += 2;
    }
    else
      // Just copy through.
      result << str[i];
  }

  return result.str();
}
Name recombineNameWithSessionId(const Name &nameWithAllComponents, const uint64_t sessionId)
{
	Name name("");
	name.append(nameWithAllComponents.getPrefix(3));//< /dataName/snake/functionName
	name.appendNumber(sessionId);
	name.append(nameWithAllComponents.get(nameWithAllComponents.size()-1));//< application parameters sha256
	return name;
}

uint64_t costCalculateStrategy(ns3::SysInfo& sysInfo, SnakeMetadata& metadata, Parameters& paras)
{
  uint64_t cost = std::numeric_limits<uint64_t>::max();
  ndn::snake::JointlyCalculator* calc = CalculatorContainer::getInstance(metadata.getFileType());
  cost = calc->costCalculate(sysInfo, metadata, paras);
	return cost;
}

uint64_t
costEstimator(const Interest &interest, const Data& data, ns3::Ptr<ns3::Node> & currentNode){
  uint64_t costEta = 0;
	// paras
  Block block = interest.getApplicationParameters();
  std::string functionParameters = ndn::encoding::readString(block);
	Parameters paras;
	paras.Initialize(functionParameters);
	// metadata
	const ndn::Block* snakeMetaDataBlock = data.getMetaInfo().findAppMetaInfo(lp::tlv::MetaData);
	std::string snakeMetaDataStr = ndn::encoding::readString(*snakeMetaDataBlock);
  MetadataWrapper metadataWrapper(snakeMetaDataStr.c_str());
	std::cout << metadataWrapper.Serialize() << std::endl;
	// avaliable node resources
	ns3::Ptr<ns3::ComputationModel> cm = currentNode->GetObject<ns3::ComputationModel>();
	ns3::SysInfoValue siv;
	cm->GetAttribute("SystemStateInfo", siv);
  // ns3::SysInfo delta(10,10);
	// siv.Set(siv.Get() - delta);
	// cm->SetAttribute("SystemStateInfo", siv);
	ns3::SysInfo sysinfo = siv.Get();
	//TODO considering bandwidth cost.
	//note I: original data passing the bottleneck cost size/bandwidth
	//note II: bandwidth is different between uplink and downlink.
  costEta = costCalculateStrategy(sysinfo, *metadataWrapper.getMetadata(), paras);
	auto sentTimeP = data.getTag<lp::HopDelayTag>();
	if(nullptr != sentTimeP){
		boost::uint64_t now = time::duration_cast<time::microseconds>(time::steady_clock::now().time_since_epoch()).count();
		now = now & 0x00ffffffffffffff;
		boost::uint64_t delay = (now - (*sentTimeP) & 0x00ffffffffffffff) / 1000;//miliseconds
    costEta += delay; //TODO avg statistics.
		// boost::uint64_t nodeId = (*delayP) >> 56;
		data.removeTag<lp::HopDelayTag>();
	}
	return costEta;
}

uint64_t
hashing(const std::string function, const std::string functionParas, const uint64_t nodeId)
{
	std::string hashingContents = function + functionParas + std::to_string(nodeId);
  // return std::hash<std::string>{}(hashingContents);
	return ns3::Hash64(hashingContents);
}

}//namespace util
}//namespace snake
}//namespace ndn
