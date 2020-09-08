#include "snake-utils.hpp"
#include <ndn-cxx/lp/tags.hpp>
#include <ndn-cxx/encoding/nfd-constants.hpp>

#include <tuple>
#include <thread>
#include <chrono>

namespace ndn{
namespace util{
namespace snake{

const std::uint16_t INVALID = 0;
	bool isFunctionExecuted(const Interest& interest){
	  if (& interest == nullptr){
		return false;
	  }
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

	bool isFunctionExecuted(const Data& data){
	  if (& data == nullptr){
		return false;
	  }
	  auto tag = data.getTag<lp::FunctionTag>();
	  if (tag == nullptr) {
		// no tag
		return false;
	  } else {
		uint64_t tagValue = tag->get();
		return tagValue == nfd::SnakeComputing::FUNCTION_EXECUTED;
	  }
	  return false;
	}

	void markFunctionAsExecuted(const Interest& interest){
	  interest.setTag(make_shared<lp::FunctionTag>(nfd::SnakeComputing::FUNCTION_EXECUTED));
	}

	void removeFunctionTag(const Interest& interest){
	  auto tag = interest.getTag<lp::FunctionTag>();
	  if (tag != nullptr) {
		interest.removeTag<lp::FunctionTag>();
	  }
	}
	void removeFunctionTag(const Data& data){
	  auto tag = data.getTag<lp::FunctionTag>();
	  if (tag != nullptr) {
	  data.removeTag<lp::FunctionTag>();
	  }
    }
	bool isBelong2SnakeSystem(const Interest& interest){
	  auto tag = interest.getTag<lp::FunctionTag>();
	  if (tag == nullptr) {
	    // no tag
	    return false;
	  }
	  return true;
	}

	/**
	 * Extract function name and its parameters.
	 * \c str format:  <tt> ndn:://dataName/functionName/cpu=100,m=20 </tt>
	 */
	std::tuple<std::string, std::string> extractFunctionNameAndParameters(std::string& str){
      if(&str == nullptr) return std::make_tuple(nullptr, nullptr);
      //
      int functionStartPos = str.find(ndn::nfd::SNAKE_SEPARATOR, 0) + 6;
      std::string functionAndBehind = str.substr(functionStartPos);
      int functionEndPos = functionAndBehind.find_first_of("/");
      std::string functionName(functionAndBehind, 0, functionEndPos);
			//TODO use JSON object instead.
			std::string parameters = functionAndBehind.substr(functionEndPos);
			parameters
      return std::make_tuple(functionName, parameters);
	}


  /**
	 * FIXME(qjp): now we just sleep a specefic time dual.
	 * 
	 * TODO 1. use Object Serialization and transfer method.
	 *      2. try reflect mechanism.
	 * @param functionParameters a long number.
	 */
	void invoke(Data& data, std::string functionName, std::string functionParameters){
		long millisecond = std::stoi(functionParameters);
		std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
	}

	void afterFunctionInvoke(Data& data, Data& newData){
        newData.wireDecode(data.wireEncode());
        //newData.resetWire();
        std::string results = "results";
        newData.setContent(::ndn::encoding::makeStringBlock(::ndn::tlv::Content, results));
    	  setFunctionExecuted(newData);
	}
}
}//namespace snake
}//namespace ndn
