/*******************************************************************************
 * axis_and_allies_battle_calc/hit_app/min_cost.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_HIT_APP_MIN_COST_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_HIT_APP_MIN_COST_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>

#include "decrement_unit_count_c.hpp"

namespace axis_and_allies_battle_calc
{

namespace hit_app
{

namespace min_cost_private
{

template<
    class Cost, class Combatant,
    int I = 0,
    int N = boost::mpl::size< typename Combatant::tags_type >::value
>
struct iterate
{
    static void apply(
        Combatant& combatant,
        boost::array< unsigned char, N > const & simple_combat_value,
        unsigned int current_min_cost,
        unsigned int current_min_cost_index,
        void (*p_decrement_unit_count)( Combatant& ))
    {
        typedef typename boost::mpl::at_c< typename Combatant::tags_type, I >::type combat_unit_tag_type;
        static combat_unit::e_type const combat_unit_type = combat_unit_tag_type::type;
        if(0 != (combat_unit_bitmask & (1 << combat_unit_type))) {
            static unsigned int const cost = boost::mpl::apply< Cost, combat_unit_tag_type >::type::value;
            if(
                cost < current_min_cost
             || (cost == current_min_cost && simple_combat_value[I] < simple_combat_value[current_min_cost_index])
            ) {
                current_min_cost = cost;
                current_min_cost_index = I;
                p_decrement_unit_count = &decrement_unit_count_c<I>;
            }
        }
    }
};

} // namespace min_cost_private

template< class Cost >
struct min_cost
{
    typedef void result_type;
    template< class Combatant, class Opponent >
    void operator()(
        unsigned int const round,
        Combatant& combatant,
        Opponent const & opponent,
        unsigned int combat_unit_bitmask) const
    {
        
    }
};

} // namespace hit_app

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_HIT_APP_MIN_COST_HPP
