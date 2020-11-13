/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef FILE_TYPE_HPP
#define FILE_TYPE_HPP
namespace ndn {
namespace snake {
enum FileType : uint32_t { // uint16_t not supported in RapidJSON
  UNRECOGNIZED = 0, //Default
  TXT = 1, // Text formates 1-99
  MP4 = 101, // Video formates 100-199
  MP3 = 201, //Audio formates 200 - 299
  OTHER = 300 // Other formates 300+
};

} //namespace lp
} //namespace ndn

#endif