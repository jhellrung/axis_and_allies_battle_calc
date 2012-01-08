/*******************************************************************************
 * axis_and_allies_battle_calc/scenarios/spring_1942.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_SCENARIOS_SPRING_1942_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_SCENARIOS_SPRING_1942_HPP

#include <cassert>

#include <algorithm>

#include <boost/array.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include "combat_profile_page.hpp"
#include "size_type.hpp"
#include "unit_combat_value.hpp"
#include "unit_count.hpp"
#include "unit_tag.hpp"

namespace axis_and_allies_battle_calc
{

struct spring_1942
{
    template< e_battle_id Id >
    struct battle_traits
    {
        typedef void attacker_type;
        typedef void defender_type;
    };
};

template<>
struct spring_1942::battle_traits< e_battle_id_land >
{
    struct attacker;
    struct defender;
    typedef attacker attacker_type;
    typedef defender defender_type;
};

template<>
struct spring_1942::battle_traits< e_battle_id_amphibious_assault >
{
    struct attacker;
    typedef attacker attacker_type;
    typedef battle_traits< e_battle_id_land >::defender_type defender_type;
};

template<>
struct spring_1942::battle_traits< e_battle_id_sea >
{
    struct attacker;
    struct defender;
    typedef attacker attacker_type;
    typedef defender defender_type;
};

/*******************************************************************************
 * struct spring_1942::battle_traits< e_battle_id_land >::attacker
 ******************************************************************************/

template<>
struct spring_1942::battle_traits< e_battle_id_land >::attacker
{
    typedef attacker this_type;

    typedef boost::mpl::vector5<
        unit_tags::infantry,
        unit_tags::artillery,
        unit_tags::tank,
        unit_tags::fighter,
        unit_tags::bomber
    > tags;

    size_type n_infantry;
    size_type n_artillery;
    size_type n_tank;
    size_type n_fighter;
    size_type n_bomber;

    static this_type _()
    {
        static this_type const this_ = {0,0,0,0,0};
        return this_;
    }

    size_type& operator[](unsigned int const i)
    {
        assert(i < 5);
        return (&n_infantry)[i];
    }

    size_type operator[](unsigned int const i) const
    {
        assert(i < 5);
        return (&n_infantry)[i];
    }

    static boost::array< char const *, 5 >
    str_ids()
    {
        static boost::array< char const *, 5 > const result = {{
            "infantry",
            "artillery",
            "tank",
            "fighter",
            "bomber"
        }};
        return result;
    }

    template< class Opponent >
    boost::array< unsigned char, 5 >
    marginal_combat_values(unsigned int /*round*/, Opponent const & /*opponent*/) const
    {
        boost::array< unsigned char, 5 > const result =
            {{ (n_infantry > n_artillery ? 1 : 2), 2, 3, 3, 4 }};
        return result;
    }

    typedef boost::array< combat_profile_page, 1 > combat_profile_result_type;
    template< class Opponent >
    combat_profile_result_type
    combat_profile(unsigned int const round, Opponent const & /*opponent*/) const
    {
        combat_profile_result_type result =
            {{ combat_profile_page::_() }};
        if(round != 0) {
            result[0].unit_ids = land_unit_ids | air_unit_ids;
            size_type const n_supported_infantry = std::min(n_infantry, n_artillery);
            result[0].n[0] = n_infantry - n_supported_infantry;
            result[0].n[1] = n_artillery + n_supported_infantry;
            result[0].n[2] = n_tank + n_fighter;
            result[0].n[3] = n_bomber;
        }
        return result;
    }
};

/*******************************************************************************
 * struct spring_1942::battle_traits< e_battle_id_land >::defender
 ******************************************************************************/

template<>
struct spring_1942::battle_traits< e_battle_id_land >::defender
{
    typedef defender this_type;

    typedef boost::mpl::vector6<
        unit_tags::infantry,
        unit_tags::artillery,
        unit_tags::tank,
        unit_tags::antiaircraft_gun,
        unit_tags::fighter,
        unit_tags::bomber
    > tags;

    size_type n_infantry;
    size_type n_artillery;
    size_type n_tank;
    size_type n_antiaircraft_gun;
    size_type n_fighter;
    size_type n_bomber;

    static this_type _()
    {
        static this_type const this_ = {0,0,0,0,0,0};
        return this_;
    }

    size_type& operator[](unsigned int const i)
    {
        assert(i < 6);
        return (&n_infantry)[i];
    }

    size_type operator[](unsigned int const i) const
    {
        assert(i < 6);
        return (&n_infantry)[i];
    }

    static boost::array< char const *, 6 >
    str_ids()
    {
        static boost::array< char const *, 6 > const result = {{
            "infantry",
            "artillery",
            "tank",
            "antiaircraft gun",
            "fighter",
            "bomber"
        }};
        return result;
    }

    template< class Opponent >
    static boost::array< unsigned char, 6 >
    marginal_combat_values(unsigned int const /*round*/, Opponent const & /*opponent*/)
    {
        static boost::array< unsigned char, 6 > const result =
            {{ 2, 2, 3, 0, 4, 1 }};
        return result;
    }

    typedef boost::array< combat_profile_page, 2 > combat_profile_result_type;
    template< class Oppponent >
    combat_profile_result_type
    combat_profile(unsigned int const round, Oppponent const & opponent) const
    {
        assert(n_antiaircraft_gun <= 1);
        combat_profile_result_type result = {{
            combat_profile_page::_(),
            combat_profile_page::_()
        }};
        if(round == 0) {
            if(n_antiaircraft_gun != 0) {
                result[0].unit_ids = (1 << unit_traits::e_type_fighter);
                result[0].n[0] = opponent.n_fighter;
                result[1].unit_ids = (1 << unit_traits::e_type_bomber);
                result[1].n[0] = opponent.n_bomber;
            }
        }
        else {
            result[0].unit_ids = land_unit_ids | air_unit_ids;
            result[0].n[0] = n_bomber;
            result[0].n[1] = n_infantry + n_artillery;
            result[0].n[2] = n_tank;
            result[0].n[3] = n_fighter;
        }
        return result;
    }
};

/*******************************************************************************
 * struct spring_1942::battle_traits< e_battle_id_amphibious_assault >::attacker
 ******************************************************************************/

template<>
struct spring_1942::battle_traits< e_battle_id_amphibious_assault >::attacker
{
    typedef attacker this_type;

    typedef boost::mpl::vector10<
        unit_tags::no_retreat::infantry,
        unit_tags::no_retreat::artillery,
        unit_tags::no_retreat::tank,
        unit_tags::infantry,
        unit_tags::artillery,
        unit_tags::tank,
        unit_tags::fighter,
        unit_tags::bomber,
        unit_tags::cruiser,
        unit_tags::battleship
    > tags;

    struct seaborne_
    {
        size_type n_infantry;
        size_type n_artillery;
        size_type n_tank;
    } seaborne;
    struct overland_
    {
        size_type n_infantry;
        size_type n_artillery;
        size_type n_tank;
    } overland;
    n_fighter_type n_fighter;
    n_bomber_type n_bomber;
    struct offshore_bombardment_
    {
        size_type n_cruiser;
        size_type n_battleship;
    } offshore_bombardment;

    static this_type _()
    {
        static this_type const this_ = {{0,0,0},{0,0,0},0,0,{0,0}};
        return this_;
    }

    size_type& operator[](unsigned int const i)
    {
        assert(i < 10);
        return (&seaborne.n_infantry)[i];
    }

    size_type operator[](unsigned int const i) const
    {
        assert(i < 10);
        return (&seaborne.n_infantry)[i];
    }

    static boost::array< char const *, 10 >
    str_ids()
    {
        static boost::array< char const *, 10 > const result = {{
            "infantry (seaborne)",
            "artillery (seaborne)",
            "tank (seaborne)",
            "infantry (overland)",
            "artillery (overland)",
            "tank (overland)",
            "fighter",
            "bomber",
            "cruiser (offshore bombardment)",
            "battleship (offshore bombardment)"
        }};
        return result;
    }

    template< class Opponent >
    boost::array< unsigned char, 10 >
    marginal_combat_values(unsigned int /*round*/, Opponent const & /*opponent*/) const
    {
        boost::array< unsigned char, 10 > result =
            {{ 1, 2, 3, 1, 2, 3, 3, 4, 0, 0 }};
        size_type const n_infantry = overload.n_infantry + seaborne.n_infantry;
        size_type const n_artillery = overload.n_artillery + seaborne.n_artillery;
        if(n_infantry <= n_artillery)
            result[3] = result[0] = 2;
        return result;
    }

    typedef boost::array< combat_profile_page, 1 > combat_profile_result_type;
    template< class Opponent >
    combat_profile_result_type
    combat_profile(unsigned int const round, Opponent const & opponent) const
    {
        combat_profile_result_type result = {{ combat_profile_page::_() }};
        if(round != 0) {
            size_type const n_infantry = seaborne.n_infantry + overload.n_infantry;
            size_type const n_artillery = seaborne.n_artillery + overload.n_artillery;
            size_type const n_tank = seaborne.n_tank + overland.n_tank;
            size_type const n_supported_infantry = std::min(n_infantry, n_artillery);
            result[0].unit_ids = land_unit_ids | air_unit_ids;
            result[0].n[0] = n_infantry - n_supported_infantry;
            result[0].n[1] = n_artillery + n_supported_infantry;
            result[0].n[2] = n_tank + n_fighter;
            result[0].n[3] = n_bomber;
            if(round == 1) {
                size_type max_n_bombardment =
                    seaborne.n_infantry + seaborne.n_artillery + seaborne.n_tank;
                size_type const n_battleship_bombardment =
                    std::min(offshore_bombardment.n_battleship._, max_n_bombardment);
                max_n_bombardment -= n_battleship_bombardment;
                size_type const n_cruiser_bomardment =
                    std::min(offshore_bombardment.n_cruiser, max_n_bombardment);
                result[0].n[2] += n_cruiser_bomardment;
                result[0].n[3] += n_battleship_bombardment;
            }
        }
        return result;
    }
};

/*******************************************************************************
 * struct spring_1942::sea_battle::attacker
 ******************************************************************************/

struct attacker
{
    typedef attacker this_type;

    typedef boost::mpl::vector9<
        unit_tags::submarine,
        unit_tags::transport,
        unit_tags::destroyer,
        unit_tags::cruiser,
        unit_tags::aircraft_carrier,
        unit_tags::battleship,
        unit_tags::capital_damage::battleship,
        unit_tags::fighter,
        unit_tags::bomber
    > tags;

    size_type n_submarine;
    size_type n_transport;
    size_type n_destroyer;
    size_type n_cruiser;
    size_type n_aircraft_carrier;
    size_type n_battleship;
    size_type n_battleship_damage;
    size_type n_fighter;
    size_type n_bomber;

    static this_type _()
    {
        static this_type const this_ = {0,0,0,0,0,0,0,0,0};
        return this_;
    }

    size_type& operator[](unsigned int const i)
    {
        assert(i < 9);
        return (&n_submarine)[i];
    }

    size_type operator[](unsigned int const i) const
    {
        assert(i < 9);
        return (&n_submarine)[i];
    }

    static boost::array< char const *, 9 >
    str_ids()
    {
        static boost::array< char const *, 9 > const result = {{
            "submarine",
            "transport",
            "destroyer",
            "cruiser",
            "aircraft carrier",
            "battleship",
            "battleship damage",
            "fighter",
            "bomber"
        }};
        return result;
    }

    template< class Opponent >
    boost::array< unsigned char, 9 >
    marginal_combat_values(unsigned int const /*round*/, Opponent const & opponent) const
    {
        boost::array< unsigned char, 9 > result =
            {{ 2, 0, 2, 3, 1, 0, 4, 3, 4 }};
        if(unit_count< sea_unit_ids >(opponent) == 0)
            result[0] = 0;
        if(n_destroyer == 0 && unit_count< surface_ship_bitmask | air_unit_ids >(opponent) == 0)
            result[8] = result[7] = 0;
        return result;
    }

    typedef boost::array< combat_profile_page, 3 > combat_profile_result_type;
    template< class Opponent >
    combat_profile_result_type
    combat_profile(unsigned int const round, Opponent const & opponent) const
    {
        combat_profile_result_type result =
            {{ combat_profile_page::_(), combat_profile_page::_(), combat_profile_page::_() }};
        if(round % 2 == 0) {
            if(n_submarine != 0 && opponent.n_destroyer == 0) {
                result[0].unit_ids = sea_unit_ids;
                result[0].n[1] = n_submarine;
            }
        }
        else {
            result[0].unit_ids = sea_unit_ids | air_unit_ids;
            result[0].n[0] = n_aircraft_carrier;
            result[0].n[1] = n_destroyer;
            result[0].n[2] = n_cruiser;
            result[0].n[3] = n_battleship;
            if(n_fighter != 0 || n_bomber != 0) {
                if(n_destroyer != 0) {
                    result[0].n[2] += n_fighter;
                    result[0].n[3] += n_bomber;
                }
                else {
                    result[1].unit_ids = surface_ship_ids | air_unit_ids;
                    result[1].n[2] = n_fighter;
                    result[1].n[3] = n_bomber;
                }
            }
            if(n_submarine != 0 && opponent.n_destroyer != 0) {
                result[2].unit_ids = sea_unit_ids;
                result[2].n[1] = n_submarine;
            }
        }
        return result;
    }
};

/*******************************************************************************
 * struct spring_1942::sea_battle::defender
 ******************************************************************************/

struct defender
{
    typedef defender this_type;

    typedef boost::mpl::vector8<
        unit_tags::submarine,
        unit_tags::transport,
        unit_tags::destroyer,
        unit_tags::cruiser,
        unit_tags::aircraft_carrier,
        unit_tags::battleship,
        unit_tags::capital_damage::battleship,
        unit_tags::fighter
    > tags;

    size_type n_submarine;
    size_type n_transport;
    size_type n_destroyer;
    size_type n_cruiser;
    size_type n_aircraft_carrier;
    size_type n_battleship;
    size_type n_battleship_damage;
    size_type n_fighter;

    static this_type _()
    {
        static this_type const this_ = {0,0,0,0,0,0,0,0};
        return this_type;
    }

    size_type& operator[](unsigned int const i)
    {
        assert(i < 8);
        return (&n_submarine)[i];
    }

    size_type operator[](unsigned int const i) const
    {
        assert(i < 8);
        return (&n_submarine)[i];
    }

    static boost::array< char const *, 8 >
    str_ids()
    {
        static boost::array< char const *, 8 > const result = {{
            "submarine",
            "transport",
            "destroyer",
            "cruiser",
            "aircraft carrier",
            "battleship",
            "battleship damage",
            "fighter",
            "bomber"
        }};
        return result;
    }

    template< class Opponent >
    boost::array< unsigned char, 8 >
    marginal_combat_values(unsigned int const /*round*/, Opponent const & opponent) const
    {
        boost::array< unsigned char, 8 > result =
            {{ 1, 0, 2, 3, 2, 0, 4, 4 }};
        if(unit_count< sea_unit_ids >(opponent) == 0)
            result[0] = 0;
        if(n_destroyer == 0 && unit_count< surface_ship_bitmask | air_unit_ids >(opponent) == 0)
            result[7] = 0;
        return result;
    }

    typedef boost::array< combat_profile_page, 3 > combat_profile_result_type;
    template< class Opponent >
    combat_profile_result_type
    combat_profile(unsigned int const round, Opponent const & opponent) const
    {
        assert(n_fighter <= 2 * n_aircraft_carrier);
        combat_profile_result_type result = {{
            combat_profile_page::_(),
            combat_profile_page::_(),
            combat_profile_page::_()
        }};
        if(round % 2 == 0) {
            if(n_submarine != 0 && opponent.n_destroyer == 0) {
                result[0].unit_ids = sea_unit_ids;
                result[0].n[0] = n_submarine;
            }
        }
        else {
            result[0].unit_ids = sea_unit_ids | air_unit_ids;
            result[0].n[1] = n_destroyer + n_aircraft_carrier;
            result[0].n[2] = n_cruiser;
            result[0].n[3] = n_battleship_0 + n_battleship_1;
            if(n_fighter != 0) {
                if(n_destroyer != 0)
                    result[0].n[3] += n_fighter;
                else {
                    result[1].unit_ids = surface_ship_bitmask | air_unit_ids;
                    result[1].n[3] = n_fighter;
                }
            }
            if(n_submarine != 0 && opponent.n_destroyer != 0) {
                result[2].unit_ids = sea_unit_ids;
                result[2].n[0] = n_submarine;
            }
        }
        return result;
    }
};

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_SCENARIOS_SPRING_1942_HPP
