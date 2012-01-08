/*******************************************************************************
 * axis_and_allies_battle_calc/decrement_unit_count_c.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_DECREMENT_UNIT_COUNT_C_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_DECREMENT_UNIT_COUNT_C_HPP

#include <cassert>

#include <boost/mpl/at.hpp>
#include <boost/static_assert.hpp>

namespace axis_and_allies_battle_calc
{

template< int I, class Combatant >
inline void
decrement_unit_count_c(Combatant& combatant);

namespace decrement_unit_count_c_private
{

template<
    class Combatant, int I,
    class Tag = typename boost::mpl::at_c< typename Combatant::tags_type, I >::type::tag
>
struct dispatch
    : dispatch< Combatant, I, typename Tag::tag >
{ };

template< class Combatant, int I >
struct dispatch< Combatant, I, void >
{
    static void apply(Combatant& combatant)
    {
        assert(combatant[I] != 0);
        --combatant[I];
    }
};

template< class Combatant, int I >
struct dispatch< Combatant, I, unit_traits::capital_tag<0> >
    : dispatch< Combatant, I, void >
{ };

template< class Combatant, int I, unsigned int N >
struct dispatch< Combatant, I, unit_traits::capital_tag<N> >
{
    static void apply(Combatant& combatant)
    {
        assert(combatant[I] != 0);
        --combatant[I];
        BOOST_STATIC_ASSERT((
            boost::mpl::at_c< typename Combatant::tags_type, I >::type::value
         == boost::mpl::at_c< typename Combatant::tags_type, I+1 >::type::value
        ));
        BOOST_STATIC_ASSERT((N-1 == boost::mpl::at_c< typename Combatant::tags_type, I+1 >::type::tag::value));
        ++combatant[I+1];
    }
};

} // namespace decrement_unit_count_c_private

template< int I, class Combatant >
inline void
decrement_unit_count_c(Combatant& combatant)
{ decrement_unit_count_c_private::dispatch< Combatant, I >::apply(combatant); }

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_DECREMENT_UNIT_COUNT_C_HPP
