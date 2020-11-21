/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2018 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#ifndef NDN_CXX_LP_TAGS_HPP
#define NDN_CXX_LP_TAGS_HPP

#include "ndn-cxx/lp/cache-policy.hpp"
#include "ndn-cxx/lp/empty-value.hpp"
#include "ndn-cxx/lp/geo-tag.hpp"
#include "ndn-cxx/lp/prefix-announcement-header.hpp"
#include "ndn-cxx/tag.hpp"

namespace ndn {
namespace lp {

/** \class IncomingFaceIdTag
 *  \brief a packet tag for IncomingFaceId field
 *
 *  This tag can be attached to Interest, Data, Nack.
 */
typedef SimpleTag<uint64_t, 10> IncomingFaceIdTag;

/** \class NextHopFaceIdTag
 *  \brief a packet tag for NextHopFaceId field
 *
 *  This tag can be attached to Interest.
 */
typedef SimpleTag<uint64_t, 11> NextHopFaceIdTag;

/** \class CachePolicyTag
 *  \brief a packet tag for CachePolicy field
 *
 *  This tag can be attached to Data.
 */
typedef SimpleTag<CachePolicy, 12> CachePolicyTag;

/** \class CongestionMarkTag
 *  \brief a packet tag for CongestionMark field
 *
 *  This tag can be attached to Interest, Data, Nack.
 */
typedef SimpleTag<uint64_t, 13> CongestionMarkTag;

/** \class NonDiscoveryTag
 *  \brief a packet tag for NonDiscovery field
 *
 *  This tag can be attached to Interest.
 */
typedef SimpleTag<EmptyValue, 14> NonDiscoveryTag;

/** \class PrefixAnnouncementTag
 *  \brief a packet tag for PrefixAnnouncement field
 *
 *  This tag can be attached to Data.
 */
typedef SimpleTag<PrefixAnnouncementHeader, 15> PrefixAnnouncementTag;

/** \class HopCountTag
 *  \brief a packet tag for HopCount field
 *
 * This tag can be attached to Interest, Data, Nack.
 */
typedef SimpleTag<uint64_t, 0x60000000> HopCountTag;

/** \class GeoTag
 *  \brief a packet tag for GeoTag field
 *
 * This tag can be attached to Interest, Data, Nack.
 */
class GeoTag; // 0x60000001, defined directly in geo-tag.hpp
/**
 * \class FuntionTag
 * \brief a packet tag for FunctionExecutedTag
 *
 *  This tag can be attached to Interest, Data, Nack.
 */
typedef SimpleTag<uint64_t, 0x60000010> FunctionTag;

/**
 * \class SessionTag
 * \brief a packet tag for session support.
 * 
 * This tag can be attached to Interest, Data, Nack.
 * 
 */
typedef SimpleTag<uint64_t, 0x60000011> SessionTag;

/**
 * \class DataPushTag
 * \brief a packet tag for pusing data.
 * 
 * This tag can be attached to Interest, Data, Nack.
 * 
 */
typedef SimpleTag<uint64_t, 0x60000012> DataPushTag;

/**
 * \class ProcessingTimeTag
 * \brief a packet tag for the time of executing function.
 * 
 * This tag can be attached to Data.
 * issue #25
 */
typedef SimpleTag<uint64_t, 0x60000013> ProcessingTimeTag;

/**
 * \brief a packet tag send to the client before transmitting data.
 * 
 * \class MinCostMarkerTag
 * 
 * This tag can be attached to Interest, Data, and marking the best function
 * executing node.
 * 
 */
typedef SimpleTag<uint64_t, 0x60000014> MinCostMarkerTag;
typedef SimpleTag<uint64_t, 0x60000015> MinCostTag;
/**
 * \brief a packet tag represent the Data just contains MetaData
 * \class MetaDataTag
 * 
 * This tag can be attached to Data.
 */
typedef SimpleTag<uint64_t, 0x60000016> MetaDataTag;

typedef SimpleTag<uint64_t,  0x60000017> HopDelayTag;
} // namespace lp
} // namespace ndn

#endif // NDN_CXX_LP_TAGS_HPP
