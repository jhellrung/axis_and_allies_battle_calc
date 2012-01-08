/*******************************************************************************
 * axis_and_allies_battle_calc/unit_tags.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef AXIS_AND_ALLIES_BATTLE_CALC_UNIT_TAGS_HPP
#define AXIS_AND_ALLIES_BATTLE_CALC_UNIT_TAGS_HPP

namespace axis_and_allies_battle_calc
{

enum e_unit_id
{
    e_unit_id_infantry,
    e_unit_id_marine,
    e_unit_id_mechanized_infantry,
    e_unit_id_artillery,
    e_unit_id_armor,
    e_unit_id_tank = e_unit_id_armor,
    e_unit_id_antiaircraft_gun,
    e_unit_id_fighter,
    e_unit_id_tactical_bomber,
    e_unit_id_bomber,
    e_unit_id_strategic_bomber = e_unit_id_bomber,
    e_unit_id_submarine,
    e_unit_id_transport,
    e_unit_id_destroyer,
    e_unit_id_cruiser,
    e_unit_id_aircraft_carrier,
    e_unit_id_battleship
};

unsigned int const land_unit_ids =
    (1 << e_unit_id_infantry)
  | (1 << e_unit_id_marine)
  | (1 << e_unit_id_mechanized_infantry)
  | (1 << e_unit_id_artillery)
  | (1 << e_unit_id_armor);
unsigned int const air_unit_ids =
    (1 << e_unit_id_fighter)
  | (1 << e_unit_id_tactical_bomber);
  | (1 << e_unit_id_bomber);
unsigned int const surface_warship_ids =
    (1 << e_unit_id_destroyer)
  | (1 << e_unit_id_cruiser)
  | (1 << e_unit_id_aircraft_carrier)
  | (1 << e_unit_id_battleship);
unsigned int const surface_ship_ids =
    (1 << e_unit_id_transport)
  | surface_warship_ids;
unsigned int const warship_ids =
    (1 << e_unit_id_submarine)
  | surface_warship_ids;
unsigned int const sea_unit_ids =
    surface_ship_ids
  | warship_ids;

enum e_unit_modifier
{
    e_unit_modifier_no_retreat,
    e_unit_modifier_capital_damage,
    e_unit_modifier_last_casualty
};

template< e_unit_id Id, unsigned int Modifiers = 0 >
struct unit_tag
{
    static e_unit_id const id = Id;
    static unsigned int const modifiers = Modifiers;
}

namespace unit_tags
{

#define AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( id ) \
typedef unit_tag< e_unit_id_ ## id > id
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( infantry );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( marine );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( mechanized_infantry );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( artillery );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( armor );
typedef armor tank;
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( antiaircraft_gun );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( fighter );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( tactical_bomber );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( bomber );
typedef bomber strategic_bomber;
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( submarine );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( transport );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( destroyer );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( cruiser );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( aircraft_carrier );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( battleship );
#undef AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG

namespace no_retreat
{

#define AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( id ) \
typedef unit_tag< e_unit_id_ ## id, (1 << e_unit_modifier_no_retreat) > id
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( infantry );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( marine );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( mechanized_infantry );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( artillery );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( armor );
typedef armor tank;
#undef AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG

} // namespace no_retreat

namespace capital_damage
{

#define AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( id ) \
typedef unit_tag< e_unit_id_ ## id, (1 << e_unit_modifier_capital_damage) > id
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( aircraft_carrier );
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( battleship );
#undef AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG

} // namespace capital_damage

namespace last_casualty
{

#define AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( id ) \
typedef unit_tag< e_unit_id_ ## id, (1 << e_unit_modifier_last_casualty) > id
AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG( transport );
#undef AXIS_AND_ALLIES_BATTLE_CALC_DEFINE_UNIT_TAG

} // namespace last_casualty

} // namespace unit_tags

} // namespace axis_and_allies_battle_calc

#endif // #ifndef AXIS_AND_ALLIES_BATTLE_CALC_UNIT_TAGS_HPP
