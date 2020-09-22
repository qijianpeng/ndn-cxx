#include "snake-utils.hpp"
#include <string.h>
#include <ndn-cxx/lp/tags.hpp>
#include <ndn-cxx/encoding/nfd-constants.hpp>
#include <node-resources.hpp>
#include "common/logger.hpp"

#include <tuple>
#include <thread>
#include <chrono>

namespace ndn{
namespace snake{
namespace util{
const std::uint16_t INVALID = 0;
	bool isBelong2SnakeSystem(const Interest& interest){
	  if(interest.getName().toUri().find(ndn::nfd::SNAKE_SEPARATOR, 0) != -1){
			return true;
		}
		auto tag = interest.getTag<lp::FunctionTag>();
	  if (tag == nullptr) {
	    // no tag
	    return false;
	  }
	  return true;
	}
	bool isBelong2SnakeSystem(const Data& data){
	  if(data.getName().toUri().find(ndn::nfd::SNAKE_SEPARATOR, 0) != -1){
			return true;
		}
		auto tag = data.getTag<lp::FunctionTag>();
	  if (tag == nullptr) {
	    // no tag
	    return false;
	  }
	  return true;
	}
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
  shared_ptr<Data> cloneData(const Data& data){
		auto newData = make_shared<Data>(data.getName());
		newData->wireDecode(data.wireEncode());
		return newData;
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

	void markFunctionAsExecuted(Interest& interest){
	  interest.setTag(make_shared<lp::FunctionTag>(nfd::SnakeComputing::FUNCTION_EXECUTED));
	}

	void markFunctionAsUnexecuted(Interest& interest){
	  interest.setTag(make_shared<lp::FunctionTag>(nfd::SnakeComputing::FUNCTION_UNEXECUTED));
	}

 	void markFunctionAsExecuted(Data& data){
	  data.setTag(make_shared<lp::FunctionTag>(nfd::SnakeComputing::FUNCTION_EXECUTED));

	}
	void tryToMarkAsFunction(Interest& interest){
		if(isBelong2SnakeSystem(interest)){
      markFunctionAsUnexecuted(interest);
		}
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


	/**
	 * Extract function name and its parameters.
	 * \c str format:  <tt> ndn:://dataName/snake/functionName/{\"cpu\":100, \"mem\":20} </tt>
	 * 
	 */
	std::tuple<std::string, std::string> extractFunctionNameAndParameters(std::string& str){
			//std::cout<<str <<std::endl;
      if(&str == nullptr) return std::make_tuple(nullptr, nullptr);
      //
      int functionStartPos = str.find(ndn::nfd::SNAKE_SEPARATOR, 0) + ndn::nfd::SNAKE_SEPARATOR.length() + 1;
      std::string functionAndBehind = str.substr(functionStartPos);
      int functionEndPos = functionAndBehind.find_first_of("/");
      std::string functionName(functionAndBehind, 0, functionEndPos);
			//TODO use JSON object instead.
			std::string parametersAndBehind = functionAndBehind.substr(functionEndPos + 1);
			int paramentersEndPos = parametersAndBehind.find_first_of("/");
			std::string parameters(parametersAndBehind, 0, paramentersEndPos);
      return std::make_tuple(functionName, parameters);
	}


  /**
	 * FIXME(qjp): now we just sleep a specefic time dual.
	 * 
	 * TODO 1. try reflect mechanism.
	 *      2. for concurrent execution, schedule method should be employed.
	 * @param functionParameters a long number.
	 */
	void functionInvoke(const Data& data, std::string functionName, std::string functionParameters){
		
		//NFD_LOG_DEBUG("Begin invoking function: " << functionName);
		Resource der;
		der.DeSerialize(functionParameters);
		long millisecond = der.getCPU();
		//std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
	}

	void afterFunctionInvoke(Data& newData){
        //newData.wireDecode(data.wireEncode());
        //newData.resetWire();
        std::string results = "results";
        newData.setContent(::ndn::encoding::makeStringBlock(::ndn::tlv::Content, results));
    	  markFunctionAsExecuted(newData);
        //std::cout<<"<< Wire data: " << newData.getContent().wire() << std::endl;
	}

	bool canExecuteFunction(const Data &data){
		return true;
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
}
}//namespace snake
}//namespace ndn
