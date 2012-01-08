/*******************************************************************************
 * axis_and_allies_battle_calc/combat_value.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_VALUE_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_VALUE_HPP

#include "size_type.hpp"

namespace axis_and_allies_battle_calc
{

struct unit_combat_value
{
    typedef unit_combat_value this_type;

    unsigned int unit_ids;
    unsigned char value;

    static this_type _()
    {
        static this_type const this_ = {0,0};
        return this_;
    }
};

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_VALUE_HPP
