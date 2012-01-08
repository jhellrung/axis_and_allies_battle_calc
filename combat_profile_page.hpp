/*******************************************************************************
 * axis_and_allies_battle_calc/combat_profile_page.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_PROFILE_PAGE_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_PROFILE_PAGE_HPP

#include "size_type.hpp"

namespace axis_and_allies_battle_calc
{

struct combat_profile_page
{
    typedef combat_profile_page this_type;

    unsigned int unit_ids;
    size_type n[5];

    static this_type _()
    {
        static this_type const this_ = {0,{0,0,0,0,0}};
        return this_;
    }
};

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_PROFILE_PAGE_HPP
