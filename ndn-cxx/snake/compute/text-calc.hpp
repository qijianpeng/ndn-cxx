/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NDN_SNAKE_TEXT_JOINTLY_CALCULATOR_HPP
#define NDN_SNAKE_TEXT_JOINTLY_CALCULATOR_HPP
#include "jointly-calc.hpp"
#include "ndn-cxx/snake/fm/default-metadata.hpp"

namespace ndn{
namespace snake{
class TextJointlyCalculator : public JointlyCalculator
{
public:
  TextJointlyCalculator(){}
  ~TextJointlyCalculator(){}
  /**
   * \brief Jointly calculate cost.
   * 
   * \param sysinfo [in] Available resources of computing node.
   * \param metadata [in] Metadata of the needed data.
   * \param functionName [in] Function name that will be invoking.
   * \return uint64_t 
   */
  uint64_t costCalculate(ns3::SysInfo& sysinfo, SnakeMetadata& metadata,
                                 Parameters& paras) override
  {
    // double cputime = (1.0 * paras.cpuUint64PropertyValue()) / sysinfo.getCPU();//seconds
    uint64_t filesize = metadata.getValue<uint64_t>("size");
    // std::cout<< "filesize: " << filesize << std::endl;
    // std::cout<< "node cpu: " << sysinfo.getCPU() << std::endl;
    std::string timeComplexity = paras.OStringPropertyValue();
		std::string spaceComplexity = paras.SStringPropertyValue();
    uint64_t cost = 0;
    if("n" == timeComplexity){//O(n)
      //assume processing 1000b under 2GHz need 1000ms.
      cost += uint64_t(filesize / (1.0 * sysinfo.getCPU()/(2.0 * 1024 * 1024 * 1024))); 
      // std::cout << "Filesize: " << filesize << ", CPU ms: " << cost << ", CPU F(GHz): " << (1.0 * sysinfo.getCPU()/(2.0 * 1024 * 1024 * 1024))<< std::endl;
		} else if("1" == timeComplexity){//O(1)
      cost += 1;
    }
    if("n" == spaceComplexity){//S(n)
		  //Assume 3,000 IOPS,  I/O size = 256KiB
			//\sa https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/ebs-io-characteristics.html
			cost += uint64_t(1.0 * filesize * 1000 / (3000 * 256 * 1024 * 8));
		}
	  return cost;
  }
};
}//namespace snake
}//namespace ndn

#endif