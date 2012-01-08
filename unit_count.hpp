/*******************************************************************************
 * axis_and_allies_battle_calc/unit_count.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_UNIT_COUNT_HPP
#define AXIS_AND_ALLIES_BATTLE_UNIT_COUNT_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>

#include "size_type.hpp"

namespace axis_and_allies_battle_calc
{

template< unsigned int BitMask, class Combatant >
inline size_type
unit_count(Combatant const & combatant);

namespace unit_count_private
{

template<
    class Combatant, unsigned int BitMask,
    int I = 0,
    int N = boost::mpl::size< typename Combatant::tags_type >::value
>
struct iterate;

template<
    class Combatant, unsigned int BitMask, int I,
    bool = (0 != (BitMask & (1 << boost::mpl::at_c< typename Combatant::tags_type, I >::type::value)))
>
struct iterate_0;

template< class Combatant, unsigned int BitMask, int I, int N >
struct iterate
    : iterate_0< Combatant, BitMask, I >
{ };

template< class Combatant, unsigned int BitMask, int N >
struct iterate< Combatant, BitMask, N, N >
{
    static size_type apply(Combatant const & /*combatant*/)
    { return static_cast< size_type >(0); }
};

template< unsigned int BitMask, int N, int I = 0 >
struct iterate
{
    template< class Combatant >
    typename boost::enable_if_c<
        (1 << boost::mpl::at_c< typename Combatant::tags_type, I >::type::type) &
        BitMask
    size_type apply(Combatant const & combatant)
};

template< class Combatant, unsigned int BitMask, int I >
struct iterate_0< Combatant, BitMask, I, false >
    : iterate< Combatant, BitMask, I+1 >
{ };

template< class Combatant, unsigned int BitMask, int I >
struct iterate_0< Combatant, BitMask, I, true >
{
    static size_type apply(Combatant const & combatant)
    { return combatant[I] + iterate< Combatant, BitMask, I+1 >::apply(combatant); }
};

} // namespace unit_count_private

template< unsigned int BitMask, class Combatant >
inline size_type
unit_count(Combatant const & combatant)
{ return unit_count_private::iterate< Combatant, BitMask >::apply(combatant); }

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_UNIT_COUNT_HPP
