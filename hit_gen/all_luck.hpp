/*******************************************************************************
 * axis_and_allies_battle_calc/hit_gen/all_luck.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_HIT_GEN_ALL_LUCK_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_HIT_GEN_ALL_LUCK_HPP

#include <cstddef>

#include "size_type.hpp"

namespace axis_and_allies_battle_calc
{

namespace hit_gen
{

template< class Rand >
struct all_luck
{
    Rand& rand;

    all_luck(Rand& rand_)
        : rand(rand_)
    { }

    typedef size_type result_type;
    template< std::size_t N >
    result_type operator()(size_type (&combat_profile)[N]) const
    {
        size_type n = 0;
        for(std::size_t i = 0; i != N; ++i)
            for(size_type j = 0; j != combat_profile[i]; ++j)
                if(rand() <= i)
                    ++n;
        return n;
    }
};

} // namespace hit_gen

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_HIT_GEN_ALL_LUCK_HPP
