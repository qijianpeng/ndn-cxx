
#ifndef BOOST_MPL_CLEAR_HPP_INCLUDED
#define BOOST_MPL_CLEAR_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: clear.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <ndnboost/mpl/clear_fwd.hpp>
#include <ndnboost/mpl/aux_/clear_impl.hpp>
#include <ndnboost/mpl/sequence_tag.hpp>
#include <ndnboost/mpl/aux_/na_spec.hpp>
#include <ndnboost/mpl/aux_/lambda_support.hpp>

namespace ndnboost { namespace mpl {

template<
      typename BOOST_MPL_AUX_NA_PARAM(Sequence)
    >
struct clear
    : clear_impl< typename sequence_tag<Sequence>::type >
        ::template apply< Sequence >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,clear,(Sequence))
};

BOOST_MPL_AUX_NA_SPEC(1, clear)

}}

#endif // BOOST_MPL_CLEAR_HPP_INCLUDED