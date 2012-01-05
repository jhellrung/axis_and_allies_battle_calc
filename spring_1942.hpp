/*******************************************************************************
 * axis_and_allies_battle_calc/spring_1942.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_SPRING_1942_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_SPRING_1942_HPP

#include <cassert>

#include <algorithm>

#include <boost/array.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "combat_unit.hpp"
#include "combat_value_with_type_mask.hpp"
#include "n_type.hpp"

namespace axis_and_allies_battle_calc
{

namespace spring_1942
{

AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( infantry, 3 );
AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( artillery, 4 );
AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( tank, 5 );
AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( antiaircraft_gun, 6 );

AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( fighter, 10 );
AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( bomber, 12 );

AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( submarine, 6 );
AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( destroyer, 8 );
AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( cruiser, 12 );
AXIS_AND_ALLIES_BATTLE_CALC_TYPEDEF_PROFILE( aircraft_carrier, 14 );

typedef combat_unit::profile< combat_unit::e_type_battleship, 0, true > battleship0_profile_type;
typedef combat_unit::profile< combat_unit::e_type_battleship, 20, true > battleship1_profile_type;

typedef combat_unit::profile_tagged< n_type, infantry_profile_type > n_infantry_type;
typedef combat_unit::profile_tagged< n_type, artillery_profile_type > n_artillery_type;
typedef combat_unit::profile_tagged< n_type, tank_profile_type > n_tank_type;
typedef combat_unit::profile_tagged< n_type, antiaircraft_gun_profile_type > n_antiaircraft_gun_type;

typedef combat_unit::profile_tagged< n_type, fighter_profile_type > n_fighter_type;
typedef combat_unit::profile_tagged< n_type, bomber_profile_type > n_bomber_type;

typedef combat_unit::profile_tagged< n_type, submarine_profile_type > n_submarine_type;
typedef combat_unit::profile_tagged< n_type, destroyer_profile_type > n_destroyer_type;
typedef combat_unit::profile_tagged< n_type, cruiser_profile_type > n_cruiser_type;
typedef combat_unit::profile_tagged< n_type, aircraft_carrier_profile_type > n_aircraft_carrier_type;
typedef combat_unit::profile_tagged< n_type, battleship0_profile_type > n_battleship0_type;
typedef combat_unit::profile_tagged< n_type, battleship1_profile_type > n_battleship1_type;

namespace land_battle
{

/*******************************************************************************
 * struct spring_1942::land_battle::attacker
 ******************************************************************************/

struct attacker
{
    n_infantry_type n_infantry;
    n_artillery_type n_artillery;
    n_tank_type n_tank;
    n_fighter_type n_fighter;
    n_bomber_type n_bomber;

    static attacker _()
    {
        static attacker const result = { {0}, {0}, {0}, {0}, {0} };
        return result;
    }

    typedef boost::array< combat_value_with_type_mask, 1 > combat_value_result_type;
    template< class Opponent >
    combat_value_result_type
    combat_value(unsigned int const round, Opponent const & /*opponent*/) const
    {
        combat_value_result_type result = {{ combat_value_with_type_mask::_() }};
        if(round != 0) {
            n_type const n_supported_infantry = std::min(n_infantry._, n_artillery._);
            result[0].n[0] = n_infantry._ - n_supported_infantry;
            result[0].n[1] = n_artillery._ + n_supported_infantry;
            result[0].n[2] = n_tank._ + n_fighter._;
            result[0].n[3] = n_bomber._;
            result[0].type_mask =
                (1 << combat_unit::e_type_infantry)
              | (1 << combat_unit::e_type_artillery)
              | (1 << combat_unit::e_type_tank)
              | (1 << combat_unit::e_type_fighter)
              | (1 << combat_unit::e_type_bomber);
        }
        return result;
    }

    boost::array< unsigned char, 5 >
    simplified_marginal_combat_value() const
    {
        boost::array< n_type, 5 > const result =
            {{ (n_infantry._ > n_artillery._ ? 1 : 2), 2, 3, 3, 4 }};
        return result;
    }

    void decrement_n(unsigned int const i)
    {
        assert(static_cast< void* >(this) == static_cast< void* >(&n_infantry._));
        assert(i < 5);
        n_type& n = (&n_infantry._)[i];
        assert(n != 0);
        --n;
    }
};

/*******************************************************************************
 * struct spring_1942::land_battle::defender
 ******************************************************************************/

struct defender
{
    n_infantry_type n_infantry;
    n_artillery_type n_artillery;
    n_tank_type n_tank;
    n_antiaircraft_gun_type n_antiaircraft_gun;
    n_fighter_type n_fighter;
    n_bomber_type n_bomber;

    static defender _()
    {
        static defender const result = { {0}, {0}, {0}, {0}, {0}, {0} };
        return result;
    }

    typedef boost::array< combat_value_with_type_mask, 2 > combat_value_result_type;
    template< class Oppponent >
    combat_value_result_type
    combat_value(unsigned int const round, Oppponent const & opponent) const
    {
        assert(n_antiaircraft_gun._ <= 1);
        combat_value_result_type result = {{
            combat_value_with_type_mask::_(),
            combat_value_with_type_mask::_()
        }};
        if(round == 0) {
            if(n_antiaircraft_gun._ != 0) {
                result[0].n[0] = opponent.n_fighter._;
                result[0].type_mask = (1 << combat_unit::e_type_fighter);
                result[1].n[0] = opponent.n_bomber._;
                result[1].type_mask = (1 << combat_unit::e_type_bomber);
            }
        }
        else {
            result[0].n[0] = n_bomber;
            result[0].n[1] = n_infantry + n_artillery;
            result[0].n[2] = n_tank;
            result[0].n[3] = n_fighter;
            result[0].type_mask =
                (1 << combat_unit::e_type_infantry)
              | (1 << combat_unit::e_type_artillery)
              | (1 << combat_unit::e_type_tank)
              | (1 << combat_unit::e_type_fighter)
              | (1 << combat_unit::e_type_bomber);
        }
        return result;
    }

    boost::array< unsigned char, 6 >
    simplified_marginal_combat_value() const
    {
        boost::array< unsigned char, 6 > const result =
            {{ 2, 2, 3, 0, 4, 1 }};
        return result;
    }

    void decrement_n(unsigned int const i)
    {
        assert(static_cast< void* >(this) == static_cast< void* >(&n_infantry._));
        assert(i < 6);
        n_type& n = (&n_infantry._)[i];
        assert(n != 0);
        --n;
    }
};

typedef attacker attacker_type;
typedef defender defender_type;

} // namespace land_battle

namespace sea_battle
{

/*******************************************************************************
 * struct spring_1942::sea_battle::attacker
 ******************************************************************************/

struct attacker
{
    n_submarine_type n_submarine;
    n_destroyer_type n_destroyer;
    n_cruiser_type n_cruiser;
    n_aircraft_carrier_type n_aircraft_carrier;
    n_battleship0_type n_battleship0;
    n_battleship1_type n_battleship1;
    n_fighter_type n_fighter;
    n_bomber_type n_bomber;

    static attacker _()
    {
        static attacker const result = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };
        return result;
    }

    typedef boost::array< combat_value_with_type_mask, 3 > combat_value_result_type;

    template< class Opponent >
    combat_value_result_type
    combat_value(unsigned int const round, Opponent const & opponent) const
    {
        combat_value_result_type result = {{
            combat_value_with_type_mask::_(),
            combat_value_with_type_mask::_(),
            combat_value_with_type_mask::_()
        }};
        if(round % 2 == 0) {
            if(n_submarine._ != 0 && opponent.n_destroyer == 0) {
                result[0].n[1] = n_submarine._;
                result[0].type_mask =
                    (1 << combat_unit::e_type_submarine)
                  | (1 << combat_unit::e_type_destroyer)
                  | (1 << combat_unit::e_type_cruiser)
                  | (1 << combat_unit::e_type_aircraft_carrier)
                  | (1 << combat_unit::e_type_battleship);
            }
        }
        else {
            result[0].n[0] = n_aircraft_carrier._;
            result[0].n[1] = n_destroyer._;
            result[0].n[2] = n_cruiser._;
            result[0].n[3] = n_battleship0._ + n_battleship1._;
            if(n_fighter._ != 0 || n_bomber._ != 0) {
                if(n_destroyer._ != 0) {
                    result[0].n[2] += n_fighter._;
                    result[0].n[3] += n_bomber._;
                }
                else {
                    result[1].n[2] = n_fighter._;
                    result[1].n[3] = n_bomber._;
                    result[1].type_mask =
                        (1 << combat_unit::e_type_destroyer)
                      | (1 << combat_unit::e_type_cruiser)
                      | (1 << combat_unit::e_type_aircraft_carrier)
                      | (1 << combat_unit::e_type_battleship)
                      | (1 << combat_unit::e_type_fighter);
                }
            }
            if(n_submarine._ != 0 && opponent.n_destroyer != 0) {
                result[2].n[1] = n_submarine._;
                result[2].type_mask =
                    (1 << combat_unit::e_type_submarine)
                  | (1 << combat_unit::e_type_destroyer)
                  | (1 << combat_unit::e_type_cruiser)
                  | (1 << combat_unit::e_type_aircraft_carrier)
                  | (1 << combat_unit::e_type_battleship);
            }
            result[0].type_mask =
                (1 << combat_unit::e_type_submarine)
              | (1 << combat_unit::e_type_destroyer)
              | (1 << combat_unit::e_type_cruiser)
              | (1 << combat_unit::e_type_aircraft_carrier)
              | (1 << combat_unit::e_type_battleship)
              | (1 << combat_unit::e_type_fighter);
        }
        return result;
    }

    boost::array< unsigned char, 8 >
    simplified_marginal_combat_value() const
    {
        boost::array< unsigned char, 8 > const result =
            {{ 2, 2, 3, 1, 0, 4, 3, 4 }};
        return result;
    }

    void decrement_n(unsigned int const i)
    {
        assert(static_cast< void* >(this) == static_cast< void* >(&n_submarine._));
        assert(i < 8);
        n_type& n = (&n_submarine._)[i];
        assert(n != 0);
        --n;
        if(i == 4) {
            assert(&n == &n_battleship0._);
            ++n_battleship1._;
        }
    }
};

/*******************************************************************************
 * struct spring_1942::sea_battle::defender
 ******************************************************************************/

struct defender
{
    n_submarine_type n_submarine;
    n_destroyer_type n_destroyer;
    n_cruiser_type n_cruiser;
    n_aircraft_carrier_type n_aircraft_carrier;
    n_battleship0_type n_battleship0;
    n_battleship1_type n_battleship1;
    n_fighter_type n_fighter;

    static defender _()
    {
        static defender const result = { {0}, {0}, {0}, {0}, {0}, {0}, {0} };
        return result;
    }

    typedef boost::array< combat_value_with_type_mask, 3 > combat_value_result_type;

    template< class Opponent >
    combat_value_result_type
    combat_value(unsigned int const round, Opponent const & opponent) const
    {
        assert(n_fighter._ <= 2 * n_aircraft_carrier._);
        combat_value_result_type result = {{
            combat_value_with_type_mask::_(),
            combat_value_with_type_mask::_(),
            combat_value_with_type_mask::_()
        }};
        if(round % 2 == 0) {
            if(n_submarine._ != 0 && opponent.n_destroyer == 0) {
                result[0].n[0] = n_submarine._;
                result[0].type_mask =
                    (1 << combat_unit::e_type_submarine)
                  | (1 << combat_unit::e_type_destroyer)
                  | (1 << combat_unit::e_type_cruiser)
                  | (1 << combat_unit::e_type_aircraft_carrier)
                  | (1 << combat_unit::e_type_battleship);
            }
        }
        else {
            result[0].n[1] = n_destroyer._ + n_aircraft_carrier._;
            result[0].n[2] = n_cruiser._;
            result[0].n[3] = n_battleship0._ + n_battleship1._;
            if(n_fighter._ != 0) {
                if(n_destroyer._ != 0)
                    result[0].n[3] += n_fighter._;
                else {
                    result[1].n[3] = n_fighter._;
                    result[1].type_mask =
                        (1 << combat_unit::e_type_destroyer)
                      | (1 << combat_unit::e_type_cruiser)
                      | (1 << combat_unit::e_type_aircraft_carrier)
                      | (1 << combat_unit::e_type_battleship)
                      | (1 << combat_unit::e_type_fighter)
                      | (1 << combat_unit::e_type_bomber);
                }
            }
            if(n_submarine._ != 0 && opponent.n_destroyer != 0) {
                result[2].n[0] = n_submarine._;
                result[2].type_mask =
                    (1 << combat_unit::e_type_submarine)
                  | (1 << combat_unit::e_type_destroyer)
                  | (1 << combat_unit::e_type_cruiser)
                  | (1 << combat_unit::e_type_aircraft_carrier)
                  | (1 << combat_unit::e_type_battleship);
            }
            result[0].type_mask =
                (1 << combat_unit::e_type_submarine)
              | (1 << combat_unit::e_type_destroyer)
              | (1 << combat_unit::e_type_cruiser)
              | (1 << combat_unit::e_type_aircraft_carrier)
              | (1 << combat_unit::e_type_battleship)
              | (1 << combat_unit::e_type_fighter)
              | (1 << combat_unit::e_type_bomber);
        }
        return result;
    }

    boost::array< unsigned char, 7 >
    simplified_marginal_combat_value() const
    {
        boost::array< unsigned char, 7 > const result =
            {{ 1, 2, 3, 2, 0, 4, 4 }};
        return result;
    }

    void decrement_n(unsigned int const i)
    {
        assert(static_cast< void* >(this) == static_cast< void* >(&n_submarine._));
        assert(i < 7);
        n_type& n = (&n_infantry._)[i];
        assert(n != 0);
        --n;
        if(i == 4) {
            assert(&n == &n_battleship0._);
            ++n_battleship1._;
        }
    }
};


typedef attacker attacker_type;
typedef defender defender_type;

} // namespace sea_battle

namespace amphibious_assault
{

/*******************************************************************************
 * struct spring_1942::amphibious_assault::attacker
 ******************************************************************************/

struct attacker
{
    struct seaborne_type
    {
        typedef combat_unit::profile< combat_unit::e_type_infantry, 3, false > infantry_profile_type;
        typedef combat_unit::profile< combat_unit::e_type_artillery, 4, false > artillery_profile_type;
        typedef combat_unit::profile< combat_unit::e_type_tank, 5, false > tank_profile_type;
        typedef combat_unit::profile_tagged< n_type, infantry_profile_type > n_infantry_type;
        typedef combat_unit::profile_tagged< n_type, artillery_profile_type > n_artillery_type;
        typedef combat_unit::profile_tagged< n_type, tank_profile_type > n_tank_type;
        n_infantry_type n_infantry;
        n_artillery_type n_artillery;
        n_tank_type n_tank;
    } seaborne;
    struct overland_type
    {
        n_infantry_type n_infantry;
        n_artillery_type n_artillery;
        n_tank_type n_tank;
    } overland;
    n_fighter_type n_fighter;
    n_bomber_type n_bomber;
    struct offshore_bombardment_type
    {
        typedef combat_unit::profile< combat_unit::e_type_battleship, 20 > battleship_profile_type;
        typedef combat_unit::profile_tagged< n_type, battleship_profile_type > n_battleship_type;
        n_cruiser_type n_cruiser;
        n_battleship_type n_battleship;
    } offshore_bombardment;

    static attacker _()
    {
        static attacker const result = { {{0},{0},{0}}, {{0},{0},{0}}, {0}, {0}, {{0},{0}} };
        return result;
    }

    typedef boost::array< combat_value_with_type_mask, 1 > combat_value_result_type;

    template< class Opponent >
    combat_value_result_type
    combat_value(unsigned int const round, Opponent const & opponent) const
    {
        combat_value_result_type result = {{ combat_value_with_type_mask::_() }};
        if(round != 0) {
            n_type const n_infantry = seaborne.n_infantry._ + overload.n_infantry._;
            n_type const n_artillery = seaborne.n_artillery._ + overload.n_artillery._;
            n_type const n_tank = seaborne.n_tank._ + overland.n_tank._;
            n_type const n_supported_infantry = std::min(n_infantry, n_artillery);
            result[0].n[0] = n_infantry - n_supported_infantry;
            result[0].n[1] = n_artillery + n_supported_infantry;
            result[0].n[2] = n_tank + n_fighter;
            result[0].n[3] = n_bomber;
            if(round == 1) {
                n_type max_n_bombardment =
                    seaborne.n_infantry._ + seaborne.n_artillery._ + seaborne.n_tank._;
                n_type const n_battleship_bombardment =
                    std::min(offshore_bombardment.n_battleship._, max_n_bombardment);
                max_n_bombardment -= n_battleship_bombardment;
                n_type const n_cruiser_bomardment =
                    std::min(offshore_bombardment.n_cruiser._, max_n_bombardment);
                result[0].n[2] += n_cruiser_bomardment;
                result[0].n[3] += n_battleship_bombardment;
            }
            result[0].type_mask =
                (1 << combat_unit::e_type_infantry)
              | (1 << combat_unit::e_type_artillery)
              | (1 << combat_unit::e_type_tank)
              | (1 << combat_unit::e_type_fighter)
              | (1 << combat_unit::e_type_bomber);
        }
        return result;
    }

    boost::array< unsigned char, 10 >
    simplified_marginal_combat_value() const
    {
        n_type const n_infantry = overload.n_infantry._ + seaborne.n_infantry._;
        n_type const n_artillery = overload.n_artillery._ + seaborne.n_artillery._;
        boost::array< unsigned char, 10 > const result = {{
            (n_infantry > n_artillery ? 1 : 2), 2, 3,
            (n_infantry > n_artillery ? 1 : 2), 2, 3,
            3, 4, 3, 4
        }};
        return result;
    }

    void decrement_n(unsigned int const i)
    {
        assert(static_cast< void* >(this) == static_cast< void* >(&seaborne.n_infantry._));
        assert(i < 10);
        n_type& n = (&seaborne.n_infantry._)[i];
        assert(n != 0);
        --n;
    }
};

typedef attacker attacker_type;
typedef land_battle::defender_type defender_type;

} // namespace amphibious_assault

} // namespace spring_1942

} // namespace axis_and_allies_battle_calc

BOOST_FUSION_ADAPT_STRUCT(
    axis_and_allies_battle_calc::spring_1942::land_battle::attacker,
    ( axis_and_allies_battle_calc::spring_1942::n_infantry_type, n_infantry )
    ( axis_and_allies_battle_calc::spring_1942::n_artillery_type, n_artillery )
    ( axis_and_allies_battle_calc::spring_1942::n_tank_type, n_tank )
    ( axis_and_allies_battle_calc::spring_1942::n_fighter_type, n_fighter )
    ( axis_and_allies_battle_calc::spring_1942::n_bomber_type, n_bomber )
)

BOOST_FUSION_ADAPT_STRUCT(
    axis_and_allies_battle_calc::spring_1942::land_battle::defender,
    ( axis_and_allies_battle_calc::spring_1942::n_infantry_type, n_infantry )
    ( axis_and_allies_battle_calc::spring_1942::n_artillery_type, n_artillery )
    ( axis_and_allies_battle_calc::spring_1942::n_tank_type, n_tank )
    ( axis_and_allies_battle_calc::spring_1942::n_antiaircraft_gun_type, n_antiaircraft_gun )
    ( axis_and_allies_battle_calc::spring_1942::n_fighter_type, n_fighter )
    ( axis_and_allies_battle_calc::spring_1942::n_bomber_type, n_bomber )
)

BOOST_FUSION_ADAPT_STRUCT(
    axis_and_allies_battle_calc::spring_1942::sea_battle::attacker,
    ( axis_and_allies_battle_calc::spring_1942::n_submarine_type, n_submarine )
    ( axis_and_allies_battle_calc::spring_1942::n_destroyer_type, n_destroyer )
    ( axis_and_allies_battle_calc::spring_1942::n_cruiser_type, n_cruiser )
    ( axis_and_allies_battle_calc::spring_1942::n_aircraft_carrier_type, n_aircraft_carrier )
    ( axis_and_allies_battle_calc::spring_1942::n_battleship0_type, n_battleship0 )
    ( axis_and_allies_battle_calc::spring_1942::n_battleship1_type, n_battleship1 )
    ( axis_and_allies_battle_calc::spring_1942::n_fighter_type, n_fighter )
    ( axis_and_allies_battle_calc::spring_1942::n_bomber_type, n_bomber )
)

BOOST_FUSION_ADAPT_STRUCT(
    axis_and_allies_battle_calc::spring_1942::sea_battle::defender,
    ( axis_and_allies_battle_calc::spring_1942::n_submarine_type, n_submarine )
    ( axis_and_allies_battle_calc::spring_1942::n_destroyer_type, n_destroyer )
    ( axis_and_allies_battle_calc::spring_1942::n_cruiser_type, n_cruiser )
    ( axis_and_allies_battle_calc::spring_1942::n_aircraft_carrier_type, n_aircraft_carrier )
    ( axis_and_allies_battle_calc::spring_1942::n_battleship0_type, n_battleship0 )
    ( axis_and_allies_battle_calc::spring_1942::n_battleship1_type, n_battleship1 )
    ( axis_and_allies_battle_calc::spring_1942::n_fighter_type, n_fighter )
)

BOOST_FUSION_ADAPT_STRUCT(
    axis_and_allies_battle_calc::spring_1942::amphibious_assault::attacker,
    ( axis_and_allies_battle_calc::spring_1942::amphibious_assault::attacker::seaborne_type::n_infantry_type, seaborne.n_infantry )
    ( axis_and_allies_battle_calc::spring_1942::amphibious_assault::attacker::seaborne_type::n_artillery_type, seaborne.n_artillery )
    ( axis_and_allies_battle_calc::spring_1942::amphibious_assault::attacker::seaborne_type::n_tank_type, seaborne.n_tank )
    ( axis_and_allies_battle_calc::spring_1942::n_infantry_type, overland.n_infantry )
    ( axis_and_allies_battle_calc::spring_1942::n_artillery_type, overland.n_artillery )
    ( axis_and_allies_battle_calc::spring_1942::n_tank_type, overland.n_tank )
    ( axis_and_allies_battle_calc::spring_1942::n_fighter_type, n_fighter )
    ( axis_and_allies_battle_calc::spring_1942::n_bomber_type, n_bomber )
    ( axis_and_allies_battle_calc::spring_1942::n_bomber_type, n_bomber )
    ( axis_and_allies_battle_calc::spring_1942::amphibious_assault::attacker::offshore_bombardment_type::n_battleship_type, offshore_bombardment.n_battleship )
    ( axis_and_allies_battle_calc::spring_1942::n_cruiser_type, offshore_bombardment.n_cruiser )
)

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_SPRING_1942_HPP
