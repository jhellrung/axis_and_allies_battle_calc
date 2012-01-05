/*******************************************************************************
 * axis_and_allies_battle_calc/combat_unit.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_UNIT_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_UNIT_HPP

#include <limits>

#include <boost/static_assert.hpp>

namespace axis_and_allies_battle_calc
{

namespace combat_unit
{

enum e_type
{
    e_type_infantry,
    e_type_marine,
    e_type_mechanized_infantry,
    e_type_artillery,
    e_type_armor,
    e_type_tank = e_type_armor,
    e_type_antiaircraft_gun,
    e_type_fighter,
    e_type_tactical_bomber,
    e_type_bomber,
    e_type_strategic_bomber = e_type_bomber,
    e_type_submarine,
    e_type_destroyer,
    e_type_cruiser,
    e_type_aircraft_carrier,
    e_type_battleship,
    n_e_type
};

BOOST_STATIC_ASSERT((n_e_type <= sizeof( unsigned int ) * std::numeric_limits< unsigned char >::digits));

template< e_type Type = n_e_type, unsigned int Cost = 0, bool Retreatable = true >
struct profile
{
    static e_type const type = Type;
    static unsigned int const cost = Cost;
    static bool const retreatable = Retreatable;
};

#define AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( type, cost ) \
typedef ::axis_and_allies_battle_calc::combat_unit::profile< \
    ::axis_and_allies_battle_calc::combat_unit::e_type_ ## type, cost \
> type ## _profile_type

template<>
struct profile< n_e_type, 0 >
{
    e_type type;
    unsigned int cost;

    static profile _()
    {
        static profile const result = { n_e_type, 0 };
        return result;
    }
};

template< class T, class Profile = profile<> >
struct profile_tagged
{
    T _;

    typedef Profile profile_type;
    static e_type const type = Profile::type;
    static unsigned int const cost = Profile::cost;
};

template< class T >
struct profile_tagged< T, profile<> >
{
    T _;
    e_type type;
    unsigned int cost;
};

} // namespace combat_unit

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_COMBAT_UNIT_HPP
