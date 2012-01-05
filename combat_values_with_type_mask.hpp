/*******************************************************************************
 * axis_and_allies_battle_calc/combat_value_with_type_mask.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_VALUE_WITH_TYPE_MASK_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_VALUE_WITH_TYPE_MASK_HPP

#include "n_type.hpp"

namespace axis_and_allies_battle_calc
{

struct combat_value_with_type_mask
{
    n_type n[4];
    unsigned int type_mask;

    static combat_value_with_type_mask _()
    {
        static combat_value_with_type_mask const result = { {0,0,0,0}, 0 };
        return result;
    }
};

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_VALUE_WITH_TYPE_MASK_HPP
