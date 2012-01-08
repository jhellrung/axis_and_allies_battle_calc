/*******************************************************************************
 * axis_and_allies_battle_calc/hit_gen/min_luck.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_HIT_GEN_MIN_LUCK_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_HIT_GEN_MIN_LUCK_HPP

#include <cstddef>

#include "size_type.hpp"

namespace axis_and_allies_battle_calc
{

namespace hit_gen
{

template< class Rand >
struct min_luck
{
    Rand& rand;

    min_luck(Rand& rand_)
        : rand(rand_)
    { }

    typedef size_type result_type;
    template< std::size_t N >
    result_type operator()(size_type (&combat_profile)[N]) const
    {
        size_type sum = 0;
        for(std::size_t i = 0; i != N; ++i)
            sum += (i + 1) * combat_profile[i];
        return sum / 6 + (sum % 6 != 0 && rand() < sum % 6);
    }
};

} // namespace hit_gen

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_HIT_GEN_MIN_LUCK_HPP
