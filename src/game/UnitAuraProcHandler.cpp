/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "Unit.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "Totem.h"
#include "Creature.h"
#include "Formulas.h"
#include "CreatureAI.h"
#include "Util.h"

pAuraProcHandler AuraProcHandler[TOTAL_AURAS]=
{
    &Unit::HandleNULLProc,                                  //  0 SPELL_AURA_NONE
    &Unit::HandleNULLProc,                                  //  1 SPELL_AURA_BIND_SIGHT
    &Unit::HandleNULLProc,                                  //  2 SPELL_AURA_MOD_POSSESS
    &Unit::HandleNULLProc,                                  //  3 SPELL_AURA_PERIODIC_DAMAGE
    &Unit::HandleDummyAuraProc,                             //  4 SPELL_AURA_DUMMY
    &Unit::HandleRemoveByDamageChanceProc,                  //  5 SPELL_AURA_MOD_CONFUSE
    &Unit::HandleNULLProc,                                  //  6 SPELL_AURA_MOD_CHARM
    &Unit::HandleRemoveByDamageChanceProc,                  //  7 SPELL_AURA_MOD_FEAR
    &Unit::HandleNULLProc,                                  //  8 SPELL_AURA_PERIODIC_HEAL
    &Unit::HandleNULLProc,                                  //  9 SPELL_AURA_MOD_ATTACKSPEED
    &Unit::HandleNULLProc,                                  // 10 SPELL_AURA_MOD_THREAT
    &Unit::HandleNULLProc,                                  // 11 SPELL_AURA_MOD_TAUNT
    &Unit::HandleRemoveByDamageChanceProc,                  // 12 SPELL_AURA_MOD_STUN
    &Unit::HandleNULLProc,                                  // 13 SPELL_AURA_MOD_DAMAGE_DONE
    &Unit::HandleNULLProc,                                  // 14 SPELL_AURA_MOD_DAMAGE_TAKEN
    &Unit::HandleDamageShieldAuraProc,                      // 15 SPELL_AURA_DAMAGE_SHIELD
    &Unit::HandleRemoveByDamageProc,                        // 16 SPELL_AURA_MOD_STEALTH
    &Unit::HandleNULLProc,                                  // 17 SPELL_AURA_MOD_STEALTH_DETECT
    &Unit::HandleRemoveByDamageProc,                        // 18 SPELL_AURA_MOD_INVISIBILITY
    &Unit::HandleNULLProc,                                  // 19 SPELL_AURA_MOD_INVISIBILITY_DETECTION
    &Unit::HandleNULLProc,                                  // 20 SPELL_AURA_OBS_MOD_HEALTH
    &Unit::HandleNULLProc,                                  // 21 SPELL_AURA_OBS_MOD_MANA
    &Unit::HandleModResistanceAuraProc,                     // 22 SPELL_AURA_MOD_RESISTANCE
    &Unit::HandleNULLProc,                                  // 23 SPELL_AURA_PERIODIC_TRIGGER_SPELL
    &Unit::HandleNULLProc,                                  // 24 SPELL_AURA_PERIODIC_ENERGIZE
    &Unit::HandleNULLProc,                                  // 25 SPELL_AURA_MOD_PACIFY
    &Unit::HandleRemoveByDamageChanceProc,                  // 26 SPELL_AURA_MOD_ROOT
    &Unit::HandleNULLProc,                                  // 27 SPELL_AURA_MOD_SILENCE
    &Unit::HandleNULLProc,                                  // 28 SPELL_AURA_REFLECT_SPELLS
    &Unit::HandleNULLProc,                                  // 29 SPELL_AURA_MOD_STAT
    &Unit::HandleNULLProc,                                  // 30 SPELL_AURA_MOD_SKILL
    &Unit::HandleIncreaseSpeedAuraProc,                     // 31 SPELL_AURA_MOD_INCREASE_SPEED
    &Unit::HandleNULLProc,                                  // 32 SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED
    &Unit::HandleNULLProc,                                  // 33 SPELL_AURA_MOD_DECREASE_SPEED
    &Unit::HandleNULLProc,                                  // 34 SPELL_AURA_MOD_INCREASE_HEALTH
    &Unit::HandleNULLProc,                                  // 35 SPELL_AURA_MOD_INCREASE_ENERGY
    &Unit::HandleNULLProc,                                  // 36 SPELL_AURA_MOD_SHAPESHIFT
    &Unit::HandleNULLProc,                                  // 37 SPELL_AURA_EFFECT_IMMUNITY
    &Unit::HandleNULLProc,                                  // 38 SPELL_AURA_STATE_IMMUNITY
    &Unit::HandleNULLProc,                                  // 39 SPELL_AURA_SCHOOL_IMMUNITY
    &Unit::HandleNULLProc,                                  // 40 SPELL_AURA_DAMAGE_IMMUNITY
    &Unit::HandleNULLProc,                                  // 41 SPELL_AURA_DISPEL_IMMUNITY
    &Unit::HandleProcTriggerSpellAuraProc,                  // 42 SPELL_AURA_PROC_TRIGGER_SPELL
    &Unit::HandleProcTriggerDamageAuraProc,                 // 43 SPELL_AURA_PROC_TRIGGER_DAMAGE
    &Unit::HandleNULLProc,                                  // 44 SPELL_AURA_TRACK_CREATURES
    &Unit::HandleNULLProc,                                  // 45 SPELL_AURA_TRACK_RESOURCES
    &Unit::HandleNULLProc,                                  // 46 SPELL_AURA_46 4 spells in 4.3.4
    &Unit::HandleNULLProc,                                  // 47 SPELL_AURA_MOD_PARRY_PERCENT
    &Unit::HandleNULLProc,                                  // 48 SPELL_AURA_PERIODIC_TRIGGER_BY_CLIENT (Client periodic trigger spell by self (3 spells in 3.3.5a))
    &Unit::HandleNULLProc,                                  // 49 SPELL_AURA_MOD_DODGE_PERCENT
    &Unit::HandleNULLProc,                                  // 50 SPELL_AURA_MOD_CRITICAL_HEALING_AMOUNT
    &Unit::HandleNULLProc,                                  // 51 SPELL_AURA_MOD_BLOCK_CHANCE_PERCENT
    &Unit::HandleNULLProc,                                  // 52 SPELL_AURA_MOD_CRIT_PERCENT
    &Unit::HandleNULLProc,                                  // 53 SPELL_AURA_PERIODIC_LEECH
    &Unit::HandleNULLProc,                                  // 54 SPELL_AURA_MOD_HIT_CHANCE
    &Unit::HandleNULLProc,                                  // 55 SPELL_AURA_MOD_SPELL_HIT_CHANCE
    &Unit::HandleRemoveByDamageChanceProc,                  // 56 SPELL_AURA_TRANSFORM
    &Unit::HandleSpellCritChanceAuraProc,                   // 57 SPELL_AURA_MOD_SPELL_CRIT_CHANCE
    &Unit::HandleNULLProc,                                  // 58 SPELL_AURA_MOD_INCREASE_SWIM_SPEED
    &Unit::HandleNULLProc,                                  // 59 SPELL_AURA_MOD_DAMAGE_DONE_CREATURE
    &Unit::HandleRemoveByDamageChanceProc,                  // 60 SPELL_AURA_MOD_PACIFY_SILENCE
    &Unit::HandleNULLProc,                                  // 61 SPELL_AURA_MOD_SCALE
    &Unit::HandleNULLProc,                                  // 62 SPELL_AURA_PERIODIC_HEALTH_FUNNEL
    &Unit::HandleNULLProc,                                  // 63 unused (3.0.8a-4.3.4) old SPELL_AURA_PERIODIC_MANA_FUNNEL
    &Unit::HandleNULLProc,                                  // 64 SPELL_AURA_PERIODIC_MANA_LEECH
    &Unit::HandleModCastingSpeedNotStackAuraProc,           // 65 SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK
    &Unit::HandleRemoveByDamageProc,                        // 66 SPELL_AURA_FEIGN_DEATH
    &Unit::HandleNULLProc,                                  // 67 SPELL_AURA_MOD_DISARM
    &Unit::HandleNULLProc,                                  // 68 SPELL_AURA_MOD_STALKED
    &Unit::HandleNULLProc,                                  // 69 SPELL_AURA_SCHOOL_ABSORB
    &Unit::HandleNULLProc,                                  // 70 SPELL_AURA_EXTRA_ATTACKS      Useless, used by only one spell 41560 that has only visual effect (3.2.2a)
    &Unit::HandleNULLProc,                                  // 71 SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL
    &Unit::HandleModPowerCostSchoolAuraProc,                // 72 SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT
    &Unit::HandleModPowerCostSchoolAuraProc,                // 73 SPELL_AURA_MOD_POWER_COST_SCHOOL
    &Unit::HandleReflectSpellsSchoolAuraProc,               // 74 SPELL_AURA_REFLECT_SPELLS_SCHOOL
    &Unit::HandleNULLProc,                                  // 75 SPELL_AURA_MOD_LANGUAGE
    &Unit::HandleNULLProc,                                  // 76 SPELL_AURA_FAR_SIGHT
    &Unit::HandleMechanicImmuneResistanceAuraProc,          // 77 SPELL_AURA_MECHANIC_IMMUNITY
    &Unit::HandleNULLProc,                                  // 78 SPELL_AURA_MOUNTED
    &Unit::HandleModDamagePercentDoneAuraProc,              // 79 SPELL_AURA_MOD_DAMAGE_PERCENT_DONE
    &Unit::HandleNULLProc,                                  // 80 SPELL_AURA_MOD_PERCENT_STAT
    &Unit::HandleNULLProc,                                  // 81 SPELL_AURA_SPLIT_DAMAGE_PCT
    &Unit::HandleNULLProc,                                  // 82 SPELL_AURA_WATER_BREATHING
    &Unit::HandleNULLProc,                                  // 83 SPELL_AURA_MOD_BASE_RESISTANCE
    &Unit::HandleNULLProc,                                  // 84 SPELL_AURA_MOD_REGEN
    &Unit::HandleCantTrigger,                               // 85 SPELL_AURA_MOD_POWER_REGEN
    &Unit::HandleNULLProc,                                  // 86 SPELL_AURA_CHANNEL_DEATH_ITEM
    &Unit::HandleNULLProc,                                  // 87 SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN
    &Unit::HandleNULLProc,                                  // 88 SPELL_AURA_MOD_HEALTH_REGEN_PERCENT
    &Unit::HandleNULLProc,                                  // 89 SPELL_AURA_PERIODIC_DAMAGE_PERCENT
    &Unit::HandleNULLProc,                                  // 90 unused (3.0.8a-4.3.4) old SPELL_AURA_MOD_RESIST_CHANCE
    &Unit::HandleNULLProc,                                  // 91 SPELL_AURA_MOD_DETECT_RANGE
    &Unit::HandleNULLProc,                                  // 92 SPELL_AURA_PREVENTS_FLEEING
    &Unit::HandleNULLProc,                                  // 93 SPELL_AURA_MOD_UNATTACKABLE
    &Unit::HandleNULLProc,                                  // 94 SPELL_AURA_INTERRUPT_REGEN
    &Unit::HandleNULLProc,                                  // 95 SPELL_AURA_GHOST
    &Unit::HandleSpellMagnetAuraProc,                       // 96 SPELL_AURA_SPELL_MAGNET
    &Unit::HandleManaShieldAuraProc,                        // 97 SPELL_AURA_MANA_SHIELD
    &Unit::HandleNULLProc,                                  // 98 SPELL_AURA_MOD_SKILL_TALENT
    &Unit::HandleNULLProc,                                  // 99 SPELL_AURA_MOD_ATTACK_POWER
    &Unit::HandleNULLProc,                                  //100 SPELL_AURA_AURAS_VISIBLE obsolete 3.x? all player can see all auras now, but still have 2 spells including GM-spell (1852,2855)
    &Unit::HandleNULLProc,                                  //101 SPELL_AURA_MOD_RESISTANCE_PCT
    &Unit::HandleNULLProc,                                  //102 SPELL_AURA_MOD_MELEE_ATTACK_POWER_VERSUS
    &Unit::HandleNULLProc,                                  //103 SPELL_AURA_MOD_TOTAL_THREAT
    &Unit::HandleRemoveByDamageProc,                        //104 SPELL_AURA_WATER_WALK
    &Unit::HandleNULLProc,                                  //105 SPELL_AURA_FEATHER_FALL
    &Unit::HandleNULLProc,                                  //106 SPELL_AURA_HOVER
    &Unit::HandleAddFlatModifierAuraProc,                   //107 SPELL_AURA_ADD_FLAT_MODIFIER
    &Unit::HandleAddPctModifierAuraProc,                    //108 SPELL_AURA_ADD_PCT_MODIFIER
    &Unit::HandleNULLProc,                                  //109 SPELL_AURA_ADD_TARGET_TRIGGER
    &Unit::HandleNULLProc,                                  //110 SPELL_AURA_MOD_POWER_REGEN_PERCENT
    &Unit::HandleDropChargeByDamageProc,                    //111 SPELL_AURA_ADD_CASTER_HIT_TRIGGER
    &Unit::HandleOverrideClassScriptAuraProc,               //112 SPELL_AURA_OVERRIDE_CLASS_SCRIPTS
    &Unit::HandleNULLProc,                                  //113 SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN
    &Unit::HandleNULLProc,                                  //114 SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT
    &Unit::HandleNULLProc,                                  //115 SPELL_AURA_MOD_HEALING
    &Unit::HandleNULLProc,                                  //116 SPELL_AURA_MOD_REGEN_DURING_COMBAT
    &Unit::HandleMechanicImmuneResistanceAuraProc,          //117 SPELL_AURA_MOD_MECHANIC_RESISTANCE
    &Unit::HandleNULLProc,                                  //118 SPELL_AURA_MOD_HEALING_PCT
    &Unit::HandleNULLProc,                                  //119 unused (3.0.8a-4.3.4) old SPELL_AURA_SHARE_PET_TRACKING
    &Unit::HandleNULLProc,                                  //120 SPELL_AURA_UNTRACKABLE
    &Unit::HandleNULLProc,                                  //121 SPELL_AURA_EMPATHY
    &Unit::HandleNULLProc,                                  //122 SPELL_AURA_MOD_OFFHAND_DAMAGE_PCT
    &Unit::HandleNULLProc,                                  //123 SPELL_AURA_MOD_TARGET_RESISTANCE
    &Unit::HandleNULLProc,                                  //124 SPELL_AURA_MOD_RANGED_ATTACK_POWER
    &Unit::HandleNULLProc,                                  //125 SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN
    &Unit::HandleNULLProc,                                  //126 SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT
    &Unit::HandleNULLProc,                                  //127 SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS
    &Unit::HandleNULLProc,                                  //128 SPELL_AURA_MOD_POSSESS_PET
    &Unit::HandleNULLProc,                                  //129 SPELL_AURA_MOD_SPEED_ALWAYS
    &Unit::HandleNULLProc,                                  //130 SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS
    &Unit::HandleNULLProc,                                  //131 SPELL_AURA_MOD_RANGED_ATTACK_POWER_VERSUS
    &Unit::HandleNULLProc,                                  //132 SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT
    &Unit::HandleNULLProc,                                  //133 SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT
    &Unit::HandleNULLProc,                                  //134 SPELL_AURA_MOD_MANA_REGEN_INTERRUPT
    &Unit::HandleNULLProc,                                  //135 SPELL_AURA_MOD_HEALING_DONE
    &Unit::HandleNULLProc,                                  //136 SPELL_AURA_MOD_HEALING_DONE_PERCENT
    &Unit::HandleNULLProc,                                  //137 SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE
    &Unit::HandleHasteAuraProc,                             //138 SPELL_AURA_MOD_MELEE_HASTE
    &Unit::HandleNULLProc,                                  //139 SPELL_AURA_FORCE_REACTION
    &Unit::HandleNULLProc,                                  //140 SPELL_AURA_MOD_RANGED_HASTE
    &Unit::HandleNULLProc,                                  //141 SPELL_AURA_MOD_RANGED_AMMO_HASTE
    &Unit::HandleNULLProc,                                  //142 SPELL_AURA_MOD_BASE_RESISTANCE_PCT
    &Unit::HandleNULLProc,                                  //143 SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE
    &Unit::HandleNULLProc,                                  //144 SPELL_AURA_SAFE_FALL
    &Unit::HandleNULLProc,                                  //145 SPELL_AURA_MOD_PET_TALENT_POINTS
    &Unit::HandleNULLProc,                                  //146 SPELL_AURA_ALLOW_TAME_PET_TYPE
    &Unit::HandleNULLProc,                                  //147 SPELL_AURA_MECHANIC_IMMUNITY_MASK
    &Unit::HandleNULLProc,                                  //148 SPELL_AURA_RETAIN_COMBO_POINTS
    &Unit::HandleCantTrigger,                               //149 SPELL_AURA_REDUCE_PUSHBACK
    &Unit::HandleNULLProc,                                  //150 SPELL_AURA_MOD_SHIELD_BLOCKDAMAGE
    &Unit::HandleNULLProc,                                  //151 SPELL_AURA_TRACK_STEALTHED
    &Unit::HandleNULLProc,                                  //152 SPELL_AURA_MOD_DETECTED_RANGE
    &Unit::HandleNULLProc,                                  //153 old SPELL_AURA_SPLIT_DAMAGE_FLAT
    &Unit::HandleNULLProc,                                  //154 SPELL_AURA_MOD_STEALTH_LEVEL
    &Unit::HandleNULLProc,                                  //155 SPELL_AURA_MOD_WATER_BREATHING
    &Unit::HandleNULLProc,                                  //156 SPELL_AURA_MOD_REPUTATION_GAIN
    &Unit::HandleNULLProc,                                  //157 SPELL_AURA_PET_DAMAGE_MULTI (single test like spell 20782, also single for 214 aura)
    &Unit::HandleNULLProc,                                  //158 SPELL_AURA_MOD_SHIELD_BLOCKVALUE
    &Unit::HandleNULLProc,                                  //159 SPELL_AURA_NO_PVP_CREDIT
    &Unit::HandleNULLProc,                                  //160 old SPELL_AURA_MOD_AOE_AVOIDANCE
    &Unit::HandleNULLProc,                                  //161 SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT
    &Unit::HandleNULLProc,                                  //162 SPELL_AURA_POWER_BURN_MANA
    &Unit::HandleNULLProc,                                  //163 SPELL_AURA_MOD_CRIT_DAMAGE_BONUS
    &Unit::HandleNULLProc,                                  //164 2 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //165 SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS
    &Unit::HandleNULLProc,                                  //166 SPELL_AURA_MOD_ATTACK_POWER_PCT
    &Unit::HandleNULLProc,                                  //167 SPELL_AURA_MOD_RANGED_ATTACK_POWER_PCT
    &Unit::HandleNULLProc,                                  //168 SPELL_AURA_MOD_DAMAGE_DONE_VERSUS
    &Unit::HandleNULLProc,                                  //169 old SPELL_AURA_MOD_CRIT_PERCENT_VERSUS
    &Unit::HandleNULLProc,                                  //170 SPELL_AURA_DETECT_AMORE       different spells that ignore transformation effects
    &Unit::HandleNULLProc,                                  //171 SPELL_AURA_MOD_SPEED_NOT_STACK
    &Unit::HandleNULLProc,                                  //172 SPELL_AURA_MOD_MOUNTED_SPEED_NOT_STACK
    &Unit::HandleNULLProc,                                  //173 unused (3.0.8a-4.3.4) no spells, old SPELL_AURA_ALLOW_CHAMPION_SPELLS  only for Proclaim Champion spell
    &Unit::HandleNULLProc,                                  //174 SPELL_AURA_MOD_SPELL_DAMAGE_OF_STAT_PERCENT
    &Unit::HandleNULLProc,                                  //175 SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT
    &Unit::HandleNULLProc,                                  //176 SPELL_AURA_SPIRIT_OF_REDEMPTION   only for Spirit of Redemption spell, die at aura end
    &Unit::HandleNULLProc,                                  //177 SPELL_AURA_AOE_CHARM (22 spells)
    &Unit::HandleNULLProc,                                  //178 old SPELL_AURA_MOD_DEBUFF_RESISTANCE
    &Unit::HandleNULLProc,                                  //179 SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_CHANCE
    &Unit::HandleNULLProc,                                  //180 SPELL_AURA_MOD_FLAT_SPELL_DAMAGE_VERSUS
    &Unit::HandleNULLProc,                                  //181 unused (3.0.8a-4.3.4) old SPELL_AURA_MOD_FLAT_SPELL_CRIT_DAMAGE_VERSUS
    &Unit::HandleNULLProc,                                  //182 SPELL_AURA_MOD_RESISTANCE_OF_STAT_PERCENT
    &Unit::HandleNULLProc,                                  //183 SPELL_AURA_MOD_CRITICAL_THREAT only used in 28746
    &Unit::HandleNULLProc,                                  //184 SPELL_AURA_MOD_ATTACKER_MELEE_HIT_CHANCE
    &Unit::HandleNULLProc,                                  //185 SPELL_AURA_MOD_ATTACKER_RANGED_HIT_CHANCE
    &Unit::HandleNULLProc,                                  //186 SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE
    &Unit::HandleNULLProc,                                  //187 SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_CHANCE
    &Unit::HandleNULLProc,                                  //188 SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_CHANCE
    &Unit::HandleModRating,                                 //189 SPELL_AURA_MOD_RATING
    &Unit::HandleNULLProc,                                  //190 SPELL_AURA_MOD_FACTION_REPUTATION_GAIN
    &Unit::HandleNULLProc,                                  //191 SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED
    &Unit::HandleNULLProc,                                  //192 SPELL_AURA_HASTE_MELEE
    &Unit::HandleHasteAllProc,                              //193 SPELL_AURA_HASTE_ALL (in fact combat (any type attack) speed pct)
    &Unit::HandleNULLProc,                                  //194 SPELL_AURA_MOD_IGNORE_ABSORB_SCHOOL
    &Unit::HandleNULLProc,                                  //195 SPELL_AURA_MOD_IGNORE_ABSORB_FOR_SPELL
    &Unit::HandleNULLProc,                                  //196 SPELL_AURA_MOD_COOLDOWN (single spell 24818 in 3.2.2a)
    &Unit::HandleNULLProc,                                  //197 SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCEe
    &Unit::HandleNULLProc,                                  //198 unused (3.0.8a-4.3.4) old SPELL_AURA_MOD_ALL_WEAPON_SKILLS
    &Unit::HandleNULLProc,                                  //199 old SPELL_AURA_MOD_INCREASES_SPELL_PCT_TO_HIT
    &Unit::HandleNULLProc,                                  //200 SPELL_AURA_MOD_KILL_XP_PCT
    &Unit::HandleNULLProc,                                  //201 SPELL_AURA_FLY                             this aura enable flight mode...
    &Unit::HandleNULLProc,                                  //202 SPELL_AURA_CANNOT_BE_DODGED
    &Unit::HandleNULLProc,                                  //203 SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_DAMAGE
    &Unit::HandleNULLProc,                                  //204 SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_DAMAGE
    &Unit::HandleNULLProc,                                  //205 SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_DAMAGE
    &Unit::HandleNULLProc,                                  //206 SPELL_AURA_MOD_FLIGHT_SPEED
    &Unit::HandleNULLProc,                                  //207 SPELL_AURA_MOD_FLIGHT_SPEED_MOUNTED
    &Unit::HandleNULLProc,                                  //208 SPELL_AURA_MOD_FLIGHT_SPEED_STACKING
    &Unit::HandleNULLProc,                                  //209 SPELL_AURA_MOD_FLIGHT_SPEED_MOUNTED_STACKING
    &Unit::HandleNULLProc,                                  //210 SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACKING
    &Unit::HandleNULLProc,                                  //211 SPELL_AURA_MOD_FLIGHT_SPEED_MOUNTED_NOT_STACKING
    &Unit::HandleNULLProc,                                  //212 old SPELL_AURA_MOD_RANGED_ATTACK_POWER_OF_STAT_PERCENT
    &Unit::HandleNULLProc,                                  //213 SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT implemented in Player::RewardRage
    &Unit::HandleNULLProc,                                  //214 Tamed Pet Passive (single test like spell 20782, also single for 157 aura)
    &Unit::HandleNULLProc,                                  //215 SPELL_AURA_ARENA_PREPARATION
    &Unit::HandleNULLProc,                                  //216 SPELL_AURA_HASTE_SPELLS
    &Unit::HandleNULLProc,                                  //217 8 spells in 4.3.4 melee haste related
    &Unit::HandleNULLProc,                                  //218 SPELL_AURA_HASTE_RANGED
    &Unit::HandleNULLProc,                                  //219 SPELL_AURA_MOD_MANA_REGEN_FROM_STAT
    &Unit::HandleNULLProc,                                  //220 SPELL_AURA_MOD_RATING_FROM_STAT
    &Unit::HandleNULLProc,                                  //221 SPELL_AURA_IGNORED 5 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //222 2 spells in 4.3.4 Prayer of Mending
    &Unit::HandleNULLProc,                                  //223 dummy code (cast damage spell to attacker) and another dummy (jump to another nearby raid member)
    &Unit::HandleNULLProc,                                  //224 unused (3.0.8a-4.3.4)
    &Unit::HandleMendingAuraProc,                           //225 SPELL_AURA_PRAYER_OF_MENDING
    &Unit::HandlePeriodicDummyAuraProc,                     //226 SPELL_AURA_PERIODIC_DUMMY
    &Unit::HandleNULLProc,                                  //227 SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE
    &Unit::HandleNULLProc,                                  //228 SPELL_AURA_DETECT_STEALTH
    &Unit::HandleNULLProc,                                  //229 SPELL_AURA_MOD_AOE_DAMAGE_AVOIDANCE
    &Unit::HandleNULLProc,                                  //230 Commanding Shout
    &Unit::HandleProcTriggerSpellAuraProc,                  //231 SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE
    &Unit::HandleNULLProc,                                  //232 SPELL_AURA_MECHANIC_DURATION_MOD
    &Unit::HandleNULLProc,                                  //233 set model id to the one of the creature with id m_modifier.m_miscvalue
    &Unit::HandleNULLProc,                                  //234 SPELL_AURA_MECHANIC_DURATION_MOD_NOT_STACK
    &Unit::HandleNULLProc,                                  //235 SPELL_AURA_MOD_DISPEL_RESIST
    &Unit::HandleNULLProc,                                  //236 SPELL_AURA_CONTROL_VEHICLE
    &Unit::HandleNULLProc,                                  //237 SPELL_AURA_MOD_SPELL_DAMAGE_OF_ATTACK_POWER
    &Unit::HandleNULLProc,                                  //238 SPELL_AURA_MOD_SPELL_HEALING_OF_ATTACK_POWER
    &Unit::HandleNULLProc,                                  //239 SPELL_AURA_MOD_SCALE_2 only in Noggenfogger Elixir (16595) before 2.3.0 aura 61
    &Unit::HandleNULLProc,                                  //240 SPELL_AURA_MOD_EXPERTISE
    &Unit::HandleNULLProc,                                  //241 Forces the player to move forward
    &Unit::HandleNULLProc,                                  //242 SPELL_AURA_MOD_SPELL_DAMAGE_FROM_HEALING (only 2 test spels in 3.2.2a)
    &Unit::HandleNULLProc,                                  //243 faction reaction override spells
    &Unit::HandleNULLProc,                                  //244 SPELL_AURA_COMPREHEND_LANGUAGE
    &Unit::HandleNULLProc,                                  //245 SPELL_AURA_MOD_DURATION_OF_MAGIC_EFFECTS
    &Unit::HandleNULLProc,                                  //246 SPELL_AURA_MOD_DURATION_OF_EFFECTS_BY_DISPEL
    &Unit::HandleNULLProc,                                  //247 target to become a clone of the caster
    &Unit::HandleNULLProc,                                  //248 SPELL_AURA_MOD_COMBAT_RESULT_CHANCE
    &Unit::HandleNULLProc,                                  //249 SPELL_AURA_CONVERT_RUNE
    &Unit::HandleNULLProc,                                  //250 SPELL_AURA_MOD_INCREASE_HEALTH_2
    &Unit::HandleNULLProc,                                  //251 SPELL_AURA_MOD_ENEMY_DODGE
    &Unit::HandleNULLProc,                                  //252 SPELL_AURA_SLOW_ALL
    &Unit::HandleNULLProc,                                  //253 SPELL_AURA_MOD_BLOCK_CRIT_CHANCE
    &Unit::HandleNULLProc,                                  //254 SPELL_AURA_MOD_DISARM_SHIELD disarm Shield
    &Unit::HandleNULLProc,                                  //255 SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT
    &Unit::HandleNULLProc,                                  //256 SPELL_AURA_NO_REAGENT_USE Use SpellClassMask for spell select
    &Unit::HandleNULLProc,                                  //257 SPELL_AURA_MOD_TARGET_RESIST_BY_SPELL_CLASS Use SpellClassMask for spell select
    &Unit::HandleNULLProc,                                  //258 SPELL_AURA_MOD_SPELL_VISUAL
    &Unit::HandleNULLProc,                                  //259 old SPELL_AURA_MOD_PERIODIC_HEAL
    &Unit::HandleNULLProc,                                  //260 SPELL_AURA_SCREEN_EFFECT (miscvalue = id in ScreenEffect.dbc) not required any code
    &Unit::HandleNULLProc,                                  //261 SPELL_AURA_PHASE undetectable invisibility?
    &Unit::HandleIgnoreUnitStateAuraProc,                   //262 SPELL_AURA_IGNORE_UNIT_STATE
    &Unit::HandleNULLProc,                                  //263 SPELL_AURA_ALLOW_ONLY_ABILITY player can use only abilities set in SpellClassMask
    &Unit::HandleNULLProc,                                  //264 1 spell in 4.3.4 Deterrence
    &Unit::HandleNULLProc,                                  //265 unused (3.0.8a-4.3.4)
    &Unit::HandleNULLProc,                                  //266 unused (3.0.8a-4.3.4)
    &Unit::HandleNULLProc,                                  //267 SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL
    &Unit::HandleNULLProc,                                  //268 old SPELL_AURA_MOD_ATTACK_POWER_OF_STAT_PERCENT
    &Unit::HandleNULLProc,                                  //269 SPELL_AURA_MOD_IGNORE_DAMAGE_REDUCTION_SCHOOL
    &Unit::HandleNULLProc,                                  //270 old SPELL_AURA_MOD_IGNORE_TARGET_RESIST (unused in 3.2.2a)
    &Unit::HandleModDamageFromCasterAuraProc,               //271 SPELL_AURA_MOD_DAMAGE_FROM_CASTER
    &Unit::HandleNULLProc,                                  //272 SPELL_AURA_MAELSTROM_WEAPON (unclear use for aura, it used in (3.2.2a...3.3.0) in single spell 53817 that spellmode stacked and charged spell expected to be drop as stack
    &Unit::HandleNULLProc,                                  //273 SPELL_AURA_X_RAY (client side implementation)
    &Unit::HandleNULLProc,                                  //274 proc free shot?
    &Unit::HandleNULLProc,                                  //275 SPELL_AURA_MOD_IGNORE_SHAPESHIFT Use SpellClassMask for spell select
    &Unit::HandleNULLProc,                                  //276 SPELL_AURA_MOD_DAMAGE_DONE_BY_MECHANIC
    &Unit::HandleNULLProc,                                  //277 old SPELL_AURA_MOD_MAX_AFFECTED_TARGETS
    &Unit::HandleNULLProc,                                  //278 SPELL_AURA_MOD_DISARM_RANGED disarm ranged weapon
    &Unit::HandleNULLProc,                                  //279 SPELL_AURA_INITIALIZE_IMAGES 9 spells in 4.3.4 visual effects?
    &Unit::HandleNULLProc,                                  //280 SPELL_AURA_MOD_TARGET_ARMOR_PCT
    &Unit::HandleNULLProc,                                  //281 SPELL_AURA_MOD_GUILD_REPUTATION_GAIN
    &Unit::HandleNULLProc,                                  //282 SPELL_AURA_INCREASE_BASE_HEALTH_PERCENT
    &Unit::HandleNULLProc,                                  //283 SPELL_AURA_MOD_HEALING_RECEIVED
    &Unit::HandleNULLProc,                                  //284 146 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //285 SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR
    &Unit::HandleNULLProc,                                  //286 SPELL_AURA_ABILITY_PERIODIC_CRIT
    &Unit::HandleNULLProc,                                  //287 SPELL_AURA_DEFLECT_SPELLS
    &Unit::HandleNULLProc,                                  //288 increase parry/deflect, prevent attack (single spell used 67801)
    &Unit::HandleNULLProc,                                  //289 1 spell in 4.3.4 Mortality
    &Unit::HandleNULLProc,                                  //290 SPELL_AURA_MOD_ALL_CRIT_CHANCE
    &Unit::HandleNULLProc,                                  //291 SPELL_AURA_MOD_QUEST_XP_PCT
    &Unit::HandleNULLProc,                                  //292 call stabled pet
    &Unit::HandleNULLProc,                                  //293 3 spells
    &Unit::HandleNULLProc,                                  //294 2 spells, possible prevent mana regen
    &Unit::HandleNULLProc,                                  //295 unused (4.3.4)
    &Unit::HandleNULLProc,                                  //296 62 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //297 14 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //298 6 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //299 unused (3.2.2a-4.3.4)
    &Unit::HandleNULLProc,                                  //300 21 spells (share damage?)
    &Unit::HandleNULLProc,                                  //301 SPELL_AURA_HEAL_ABSORB 31 spells
    &Unit::HandleNULLProc,                                  //302 unused (3.2.2a-4.3.4)
    &Unit::HandleNULLProc,                                  //303 AURA_STATE_BLEEDING 35 spells increases damage done vs. aurastate
    &Unit::HandleNULLProc,                                  //304 SPELL_AURA_FAKE_INEBRIATE
    &Unit::HandleNULLProc,                                  //305 SPELL_AURA_MOD_MINIMUM_SPEED
    &Unit::HandleNULLProc,                                  //306 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //307 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //308 new aura for hunter traps
    &Unit::HandleNULLProc,                                  //309 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //310 SPELL_AURA_MOD_PET_AOE_DAMAGE_AVOIDANCE
    &Unit::HandleNULLProc,                                  //311 2 spells in 4.3.4 some kind of stun effect
    &Unit::HandleNULLProc,                                  //312 37 spells in 4.3.4 some cosmetic auras
    &Unit::HandleNULLProc,                                  //313  0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //314  SPELL_AURA_PREVENT_RESURRECTION 2 spells int 4.3.4 prevents ressurection ?
    &Unit::HandleNULLProc,                                  //315 SPELL_AURA_UNDERWATER_WALKING 4 spells in 4.3.4 underwater walking
    &Unit::HandleNULLProc,                                  //316 old SPELL_AURA_MOD_PERIODIC_HASTE 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //317 SPELL_AURA_MOD_INCREASE_SPELL_POWER_PCT 13 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //318 SPELL_AURA_MASTERY 12 spells in 4.3
    &Unit::HandleNULLProc,                                  //319 SPELL_AURA_MOD_MELEE_ATTACK_SPEED 47 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //320 SPELL_AURA_MOD_RANGED_ATTACK_SPEED 5 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //321 1 spells in 4.3 Hex
    &Unit::HandleNULLProc,                                  //322 SPELL_AURA_INTERFERE_TARGETING 6 spells in 4.3
    &Unit::HandleNULLProc,                                  //323 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //324 2 spells in 4.3.4 test spells
    &Unit::HandleNULLProc,                                  //325 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //326 SPELL_AURA_PHASE_2 24 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //327 0 spells in 4.3.4
    &Unit::HandleAuraProcOnPowerAmount,                     //328 SPELL_AURA_PROC_ON_TARGET_AMOUNT 2 spells in 4.3.4 Eclipse Mastery Driver Passive
    &Unit::HandleNULLProc,                                  //329 SPELL_AURA_MOD_RUNIC_POWER_GAIN 3 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //330 SPELL_AURA_ALLOW_CAST_WHILE_MOVING 16 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //331 SPELL_AURA_MOD_WEATHER 10 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //332 SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS 16 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //333 SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2 10 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //334 SPELL_AURA_BLIND_SIGHT 2 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //335 SPELL_AURA_SEE_WHILE_INVISIBLE invisibility-related 5 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //336 SPELL_AURA_FLIGHT_RESTRICTIONS 8 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //337 SPELL_AURA_MOD_VENDOR_PRICE 1 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //338 SPELL_AURA_MOD_DURABILITY_LOSS 3 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //339 SPELL_AURA_MOD_SKILLCHANCE 1 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //340 SPELL_AURA_MOD_RESSURECTED_HEALTH_PCT 1 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //341 SPELL_AURA_MOD_SPELL_CATEGORY_COOLDOWN 2 spells in 4.3.4 modifies cooldown of hearthstone and cast time of archaeology mining
    &Unit::HandleNULLProc,                                  //342 SPELL_AURA_MOD_ATTACKSPEED_2 17 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //343 SPELL_AURA_MOD_ALL_DAMAGE_FROM_CASTER 2 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //344 SPELL_AURA_MOD_AUTOATTACK_DAMAGE 6 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //345 SPELL_AURA_MOD_IGNORE_ARMOR_PCT 3 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //346 SPELL_AURA_ALT_POWER_INDICATOR 32 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //347 SPELL_AURA_MOD_CD_FROM_HASTE 2 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //348 SPELL_AURA_MOD_MONEY_TO_GUILD_BANK 2 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //349 SPELL_AURA_MOD_CURRENCY_GAIN 16 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //350 SPELL_AURA_MOD_ITEM_LOOT 1 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //351 SPELL_AURA_MOD_CURRENCY_LOOT 8 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //352 SPELL_AURA_ALLOW_WORGEN_TRANSFORM 1 spells in 4.3.4 enables worgen<>human form switches
    &Unit::HandleNULLProc,                                  //353 SPELL_AURA_MOD_CAMOUFLAGE 3 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //354 SPELL_AURA_MOD_HEALING_DONE_FROM_PCT_HEALTH 2 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //355 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //356 SPELL_AURA_MOD_DAMAGE_DONE_FROM_PCT_POWER 2 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //357 1 spells in 4.3.4 Enable Tower Assault Unit Frame
    &Unit::HandleNULLProc,                                  //358 SPELL_AURA_WORGEN_TRANSFORM 5 spells in 4.3.4 forces client-side transformation to worgen form
    &Unit::HandleNULLProc,                                  //359 1 spells in 4.3.4 healing done vs aurastate?
    &Unit::HandleNULLProc,                                  //360 SPELL_AURA_PROC_DUPLICATE_SPELLS 2 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //361 SPELL_AURA_PROC_TRIGGER_SPELL_2 4 spells in 4.3.4 some damage spells
    &Unit::HandleNULLProc,                                  //362 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //363 1 spells in 4.3.4 Throw Totem
    &Unit::HandleNULLProc,                                  //364 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //365 1 spells in 4.3.4 Max Far Clip Plane
    &Unit::HandleNULLProc,                                  //366 SPELL_AURA_OVERRIDE_SPELL_POWER_BY_AP_PCT 1 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //367 2 spells in 4.3.4 test spells
    &Unit::HandleNULLProc,                                  //368 0 spells in 4.3.4
    &Unit::HandleNULLProc,                                  //369 5 spells in 4.3.4 darkmoon faire related
    &Unit::HandleNULLProc                                   //370 1 spells in 4.3.4 Fair Far Clip
};

bool Unit::IsTriggeredAtSpellProcEvent(Unit *pVictim, SpellAuraHolderPtr holder, SpellEntry const* procSpell, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, bool isVictim, SpellProcEventEntry const*& spellProcEvent )
{
    if (!holder || holder->IsDeleted())
        return false;

    SpellEntry const* spellProto = holder->GetSpellProto();

    if (!spellProto)
        return false;

    switch (IsTriggeredAtCustomProcEvent(pVictim, holder, procSpell, procFlag, procExtra, attType, isVictim, spellProcEvent))
    {
        case SPELL_AURA_PROC_OK:
            return true;
        case SPELL_AURA_PROC_CANT_TRIGGER:
            return false;
        case SPELL_AURA_PROC_FAILED:
        default:
            break;
    }

    // Get EventProcFlag
    uint32 EventProcFlag = GetProcFlag(spellProto);

    if (spellProcEvent && spellProcEvent->procFlags)        // if exist get custom spellProcEvent->procFlags
        EventProcFlag = spellProcEvent->procFlags;
    else
        EventProcFlag = spellProto->GetProcFlags();         // else get from spell proto
    // Continue if no trigger exist
    if (!EventProcFlag)
        return false;

    // Check spellProcEvent data requirements
    if (!SpellMgr::IsSpellProcEventCanTriggeredBy(spellProcEvent, EventProcFlag, procSpell, procFlag, procExtra))
        return false;

    // In most cases req get honor or XP from kill
    if ((EventProcFlag & PROC_FLAG_KILL) && GetTypeId() == TYPEID_PLAYER)
    {
        bool allow = ((Player*)this)->isHonorOrXPTarget(pVictim);
        // Shadow Word: Death - can trigger from every kill
        if (holder->GetId() == 32409)
            allow = true;
        if (!allow)
            return false;
    }
    // Aura added by spell can`t trigger from self (prevent drop charges/do triggers)
    // But except periodic triggers (can triggered from self)
    if (procSpell && procSpell->Id == spellProto->Id && !(EventProcFlag & PROC_FLAG_ON_TAKE_PERIODIC))
        return false;

    // Check if current equipment allows aura to proc
    if (!isVictim && GetTypeId() == TYPEID_PLAYER)
    {
        SpellEquippedItemsEntry const* eqItems = spellProto->GetSpellEquippedItems();

        if(eqItems && eqItems->EquippedItemClass == ITEM_CLASS_WEAPON)
        {
            Item *item = NULL;
            if (attType == BASE_ATTACK)
                item = ((Player*)this)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            else if (attType == OFF_ATTACK)
                item = ((Player*)this)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            else
                item = ((Player*)this)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);

            if(!item || item->IsBroken() || item->GetProto()->Class != ITEM_CLASS_WEAPON || !((1<<item->GetProto()->SubClass) & eqItems->EquippedItemSubClassMask))
                return false;
        }
        else if(eqItems && eqItems->EquippedItemClass == ITEM_CLASS_ARMOR)
        {
            // Check if player is wearing shield
            Item *item = ((Player*)this)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if(!item || item->IsBroken() || !CanUseEquippedWeapon(OFF_ATTACK) || item->GetProto()->Class != ITEM_CLASS_ARMOR || !((1<<item->GetProto()->SubClass) & eqItems->EquippedItemSubClassMask))
                return false;
        }
    }
    // Get chance from spell
    float chance = (float)spellProto->GetProcChance();
    // If in spellProcEvent exist custom chance, chance = spellProcEvent->customChance;
    if (spellProcEvent && spellProcEvent->customChance)
        chance = spellProcEvent->customChance;
    // If PPM exist calculate chance from PPM
    if (!isVictim && spellProcEvent && spellProcEvent->ppmRate != 0)
    {
        uint32 WeaponSpeed = GetAttackTime(attType);
        chance = GetPPMProcChance(WeaponSpeed, spellProcEvent->ppmRate);
    }

    // some spells has direct chance modifiers in dummy auras
    switch(spellProto->GetSpellFamilyName())
    {
        case SPELLFAMILY_SHAMAN:
        {
            if (spellProto->GetSpellFamilyFlags().test<CF_SHAMAN_EARTHLIVING_WEAPON_PASSIVE>())
            {
                // only if target has < 35% health
                if (!pVictim || !pVictim->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT))
                    break;

                // Blessing of the Eternals
                if (Aura* modAura = GetAuraByEffectMask(SPELL_AURA_DUMMY,SPELLFAMILY_SHAMAN,spellProto->GetSpellFamilyFlags(),GetObjectGuid()))
                    chance += (float)modAura->GetModifier()->m_amount;
                break;
            }
        }
        break;
    }

    // Apply cumulative chance modifier auras
    if (Player* modOwner = GetSpellModOwner())
    {
        modOwner->ApplySpellMod(spellProto->Id,SPELLMOD_CHANCE_OF_SUCCESS,chance);
        modOwner->ApplySpellMod(spellProto->Id,SPELLMOD_FREQUENCY_OF_SUCCESS,chance);
    }

    // Earthliving procs more often with Blessing of the Eternals on low hp targets
    if (spellProto->Id == 52007 && pVictim->GetHealthPercent() < 35.0f)
    {
        // Search Blessing of the Eternals
        Unit::AuraList const& dummyAuras = GetAurasByType(SPELL_AURA_DUMMY);
        for (Unit::AuraList::const_iterator itr = dummyAuras.begin(); itr != dummyAuras.end(); ++itr)
            if ((*itr)->GetSpellProto()->SpellIconID == 3157 && (*itr)->GetSpellProto()->GetSpellFamilyName() == SPELLFAMILY_SHAMAN)
            {
                chance += (*itr)->GetModifier()->m_amount;
                break;
            }
    }

    return roll_chance_f(chance);
}

SpellAuraProcResult Unit::HandleHasteAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const * /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 cooldown)
{
    SpellEntry const *hasteSpell = triggeredByAura->GetSpellProto();

    Item* castItem = triggeredByAura->GetCastItemGuid() && GetTypeId()==TYPEID_PLAYER
        ? ((Player*)this)->GetItemByGuid(triggeredByAura->GetCastItemGuid()) : NULL;

    uint32 triggered_spell_id = 0;
    Unit* target = pVictim;
    int32 basepoints0 = 0;

    switch(hasteSpell->GetSpellFamilyName())
    {
        case SPELLFAMILY_ROGUE:
        {
            switch(hasteSpell->Id)
            {
                // Blade Flurry
                case 13877:
                case 33735:
                {
                    target = SelectRandomUnfriendlyTarget(pVictim);
                    if(!target)
                        return SPELL_AURA_PROC_FAILED;

                    basepoints0 = int32(damageInfo->damage);
                    triggered_spell_id = 22482;
                    break;
                }
            }
            break;
        }
    }

    // processed charge only counting case
    if (!triggered_spell_id)
        return SPELL_AURA_PROC_OK;

    SpellEntry const* triggerEntry = sSpellStore.LookupEntry(triggered_spell_id);

    if (!triggerEntry)
    {
        sLog.outError("Unit::HandleHasteAuraProc: Spell %u have nonexistent triggered spell %u",hasteSpell->Id,triggered_spell_id);
        return SPELL_AURA_PROC_FAILED;
    }

    // default case
    if (!target || (target != this && !target->isAlive()))
        return SPELL_AURA_PROC_FAILED;

    if (cooldown && HasSpellCooldown(triggerEntry))
        return SPELL_AURA_PROC_FAILED;

    if (basepoints0)
        CastCustomSpell(target,triggered_spell_id,&basepoints0,NULL,NULL,true,castItem,triggeredByAura);
    else
        CastSpell(target,triggered_spell_id,true,castItem,triggeredByAura);

    if (cooldown)
        AddSpellCooldown(triggered_spell_id,0,time(NULL) + cooldown);

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleSpellCritChanceAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const * procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 cooldown)
{
    if (!procSpell)
        return SPELL_AURA_PROC_FAILED;

    SpellEntry const* triggeredByAuraSpell = triggeredByAura->GetSpellProto();

    Item* castItem = triggeredByAura->GetCastItemGuid() && GetTypeId()==TYPEID_PLAYER
        ? ((Player*)this)->GetItemByGuid(triggeredByAura->GetCastItemGuid()) : NULL;

    uint32 triggered_spell_id = 0;
    Unit* target = pVictim;
    int32 basepoints0 = 0;

    switch(triggeredByAuraSpell->GetSpellFamilyName())
    {
        case SPELLFAMILY_MAGE:
        {
            switch(triggeredByAuraSpell->Id)
            {
                // Focus Magic
                case 54646:
                {
                    Unit* caster = triggeredByAura->GetCaster();
                    if (!caster)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 54648;
                    target = caster;
                    break;
                }
            }
            break;
        }
        default:
            break;
    }

    // processed charge only counting case
    if (!triggered_spell_id)
        return SPELL_AURA_PROC_OK;

    SpellEntry const* triggerEntry = sSpellStore.LookupEntry(triggered_spell_id);

    if (!triggerEntry)
    {
        sLog.outError("Unit::HandleHasteAuraProc: Spell %u have nonexistent triggered spell %u",triggeredByAuraSpell->Id,triggered_spell_id);
        return SPELL_AURA_PROC_FAILED;
    }

    // default case
    if (!target || (target != this && !target->isAlive()))
        return SPELL_AURA_PROC_FAILED;

    if (cooldown && HasSpellCooldown(triggered_spell_id))
        return SPELL_AURA_PROC_FAILED;

    if (basepoints0)
        CastCustomSpell(target,triggered_spell_id,&basepoints0,NULL,NULL,true,castItem,triggeredByAura);
    else
        CastSpell(target,triggered_spell_id,true,castItem,triggeredByAura);

    if (cooldown)
        AddSpellCooldown(triggered_spell_id,0,time(NULL) + cooldown);

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleDummyAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const * procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown)
{
    SpellEntry const* dummySpell = triggeredByAura->GetSpellProto();
    SpellEffectIndex effIndex = triggeredByAura->GetEffIndex();
    SpellEffectEntry const* dummySpellEffect = dummySpell->GetSpellEffect(effIndex);
    SpellClassOptionsEntry const* dummyClassOptions = dummySpell->GetSpellClassOptions();
    SpellClassOptionsEntry const* procClassOptions = procSpell ? procSpell->GetSpellClassOptions() : NULL;
    int32  triggerAmount = triggeredByAura->GetModifier()->m_amount;
    uint32 damage = damageInfo->damage;

    Item* castItem = triggeredByAura->GetCastItemGuid() && GetTypeId()==TYPEID_PLAYER
        ? ((Player*)this)->GetItemByGuid(triggeredByAura->GetCastItemGuid()) : NULL;

    // some dummy spells have trigger spell in spell data already (from 3.0.3)
    uint32 triggered_spell_id = dummySpellEffect->EffectApplyAuraName == SPELL_AURA_DUMMY ? dummySpellEffect->EffectTriggerSpell : 0;
    Unit* target = pVictim;
    int32  basepoints[MAX_EFFECT_INDEX] = {0, 0, 0};
    ObjectGuid originalCaster = ObjectGuid();

    switch (dummySpell->GetSpellFamilyName())
    {
        case SPELLFAMILY_GENERIC:
        {
            switch (dummySpell->Id)
            {
                // Eye for an Eye, Rank 1
                case 9799:
                {
                    // return damage % to attacker but < 50% own total health
                    basepoints[0] = triggerAmount * int32(damage) / 100;
                    if (basepoints[0] > int32(GetMaxHealth() / 2))
                        basepoints[0] = int32(GetMaxHealth() / 2);

                    triggered_spell_id = 25997;
                    break;
                }
                // Sweeping Strikes (NPC spells may be)
                case 18765:
                case 35429:
                {
                    // prevent chain of triggered spell from same triggered spell
                    if (procSpell && procSpell->Id == 26654)
                        return SPELL_AURA_PROC_FAILED;

                    target = SelectRandomUnfriendlyTarget(pVictim);
                    if (!target)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 26654;
                    break;
                }
                // Twisted Reflection (boss spell)
                case 21063:
                    triggered_spell_id = 21064;
                    break;
                // Unstable Power
                case 24658:
                {
                    if (!procSpell || procSpell->Id == 24659)
                        return SPELL_AURA_PROC_FAILED;
                    // Need remove one 24659 aura
                    RemoveAuraHolderFromStack(24659);
                    return SPELL_AURA_PROC_OK;
                }
                // Restless Strength
                case 24661:
                {
                    // Need remove one 24662 aura
                    RemoveAuraHolderFromStack(24662);
                    return SPELL_AURA_PROC_OK;
                }
                // Adaptive Warding (Frostfire Regalia set)
                case 28764:
                {
                    if (!procSpell)
                        return SPELL_AURA_PROC_FAILED;

                    // find Mage Armor
                    bool found = false;
                    AuraList const& mRegenInterrupt = GetAurasByType(SPELL_AURA_MOD_MANA_REGEN_INTERRUPT);
                    for (AuraList::const_iterator iter = mRegenInterrupt.begin(); iter != mRegenInterrupt.end(); ++iter)
                    {
                        if (SpellEntry const* iterSpellProto = (*iter)->GetSpellProto())
                        {
                            SpellClassOptionsEntry const* iterClassOptions = iterSpellProto->GetSpellClassOptions();
                            if(iterClassOptions && iterClassOptions->SpellFamilyName == SPELLFAMILY_MAGE && (iterClassOptions->SpellFamilyFlags & UI64LIT(0x10000000)))
                            {
                                found=true;
                                break;
                            }
                        }
                    }
                    if (!found)
                        return SPELL_AURA_PROC_FAILED;

                    switch(GetFirstSchoolInMask(GetSpellSchoolMask(procSpell)))
                    {
                        case SPELL_SCHOOL_NORMAL:
                        case SPELL_SCHOOL_HOLY:
                            return SPELL_AURA_PROC_FAILED;                   // ignored
                        case SPELL_SCHOOL_FIRE:   triggered_spell_id = 28765; break;
                        case SPELL_SCHOOL_NATURE: triggered_spell_id = 28768; break;
                        case SPELL_SCHOOL_FROST:  triggered_spell_id = 28766; break;
                        case SPELL_SCHOOL_SHADOW: triggered_spell_id = 28769; break;
                        case SPELL_SCHOOL_ARCANE: triggered_spell_id = 28770; break;
                        default:
                            return SPELL_AURA_PROC_FAILED;
                    }

                    target = this;
                    break;
                }
                // Obsidian Armor (Justice Bearer`s Pauldrons shoulder)
                case 27539:
                {
                    if (!procSpell)
                        return SPELL_AURA_PROC_FAILED;

                    switch(GetFirstSchoolInMask(GetSpellSchoolMask(procSpell)))
                    {
                        case SPELL_SCHOOL_NORMAL:
                            return SPELL_AURA_PROC_FAILED;                   // ignore
                        case SPELL_SCHOOL_HOLY:   triggered_spell_id = 27536; break;
                        case SPELL_SCHOOL_FIRE:   triggered_spell_id = 27533; break;
                        case SPELL_SCHOOL_NATURE: triggered_spell_id = 27538; break;
                        case SPELL_SCHOOL_FROST:  triggered_spell_id = 27534; break;
                        case SPELL_SCHOOL_SHADOW: triggered_spell_id = 27535; break;
                        case SPELL_SCHOOL_ARCANE: triggered_spell_id = 27540; break;
                        default:
                            return SPELL_AURA_PROC_FAILED;
                    }

                    target = this;
                    break;
                }
                // Mana Leech (Passive) (Priest Pet Aura)
                case 28305:
                {
                    // Cast on owner
                    target = GetOwner();
                    if(!target)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 34650;
                    break;
                }
                // Mark of Malice
                case 33493:
                {
                    // Cast finish spell at last charge
                    if (triggeredByAura->GetHolder()->GetAuraCharges() > 1)
                        return SPELL_AURA_PROC_FAILED;

                    target = this;
                    triggered_spell_id = 33494;
                    break;
                }
                // Vampiric Aura (boss spell)
                case 38196:
                {
                    basepoints[0] = 3 * damage;               // 300%
                    if (basepoints[0] < 0)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 31285;
                    target = this;
                    break;
                }
                // Aura of Madness (Darkmoon Card: Madness trinket)
                //=====================================================
                // 39511 Sociopath: +35 strength (Paladin, Rogue, Druid, Warrior)
                // 40997 Delusional: +70 attack power (Rogue, Hunter, Paladin, Warrior, Druid)
                // 40998 Kleptomania: +35 agility (Warrior, Rogue, Paladin, Hunter, Druid)
                // 40999 Megalomania: +41 damage/healing (Druid, Shaman, Priest, Warlock, Mage, Paladin)
                // 41002 Paranoia: +35 spell/melee/ranged crit strike rating (All classes)
                // 41005 Manic: +35 haste (spell, melee and ranged) (All classes)
                // 41009 Narcissism: +35 intellect (Druid, Shaman, Priest, Warlock, Mage, Paladin, Hunter)
                // 41011 Martyr Complex: +35 stamina (All classes)
                // 41406 Dementia: Every 5 seconds either gives you +5% damage/healing. (Druid, Shaman, Priest, Warlock, Mage, Paladin)
                // 41409 Dementia: Every 5 seconds either gives you -5% damage/healing. (Druid, Shaman, Priest, Warlock, Mage, Paladin)
                case 39446:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    // Select class defined buff
                    switch (getClass())
                    {
                        case CLASS_PALADIN:                 // 39511,40997,40998,40999,41002,41005,41009,41011,41409
                        case CLASS_DRUID:                   // 39511,40997,40998,40999,41002,41005,41009,41011,41409
                        {
                            uint32 RandomSpell[]={39511,40997,40998,40999,41002,41005,41009,41011,41409};
                            triggered_spell_id = RandomSpell[urand(0, countof(RandomSpell)-1)];
                            break;
                        }
                        case CLASS_ROGUE:                   // 39511,40997,40998,41002,41005,41011
                        case CLASS_WARRIOR:                 // 39511,40997,40998,41002,41005,41011
                        {
                            uint32 RandomSpell[]={39511,40997,40998,41002,41005,41011};
                            triggered_spell_id = RandomSpell[urand(0, countof(RandomSpell)-1)];
                            break;
                        }
                        case CLASS_PRIEST:                  // 40999,41002,41005,41009,41011,41406,41409
                        case CLASS_SHAMAN:                  // 40999,41002,41005,41009,41011,41406,41409
                        case CLASS_MAGE:                    // 40999,41002,41005,41009,41011,41406,41409
                        case CLASS_WARLOCK:                 // 40999,41002,41005,41009,41011,41406,41409
                        {
                            uint32 RandomSpell[]={40999,41002,41005,41009,41011,41406,41409};
                            triggered_spell_id = RandomSpell[urand(0, countof(RandomSpell)-1)];
                            break;
                        }
                        case CLASS_HUNTER:                  // 40997,40999,41002,41005,41009,41011,41406,41409
                        {
                            uint32 RandomSpell[]={40997,40999,41002,41005,41009,41011,41406,41409};
                            triggered_spell_id = RandomSpell[urand(0, countof(RandomSpell)-1)];
                            break;
                        }
                        default:
                            return SPELL_AURA_PROC_FAILED;
                    }

                    target = this;
                    if (roll_chance_i(10))
                        ((Player*)this)->Say("This is Madness!", LANG_UNIVERSAL);
                    break;
                }
                // Sunwell Exalted Caster Neck (Shattered Sun Pendant of Acumen neck)
                // cast 45479 Light's Wrath if Exalted by Aldor
                // cast 45429 Arcane Bolt if Exalted by Scryers
                case 45481:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    // Get Aldor reputation rank
                    if (((Player *)this)->GetReputationRank(932) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45479;
                        break;
                    }
                    // Get Scryers reputation rank
                    if (((Player *)this)->GetReputationRank(934) == REP_EXALTED)
                    {
                        // triggered at positive/self casts also, current attack target used then
                        if (IsFriendlyTo(target))
                        {
                            target = getVictim();
                            if (!target)
                            {
                                target = ObjectAccessor::GetUnit(*this,((Player *)this)->GetSelectionGuid());
                                if (!target)
                                    return SPELL_AURA_PROC_FAILED;
                            }
                            if (IsFriendlyTo(target))
                                return SPELL_AURA_PROC_FAILED;
                        }

                        triggered_spell_id = 45429;
                        break;
                    }
                    return SPELL_AURA_PROC_FAILED;
                }
                // Sunwell Exalted Melee Neck (Shattered Sun Pendant of Might neck)
                // cast 45480 Light's Strength if Exalted by Aldor
                // cast 45428 Arcane Strike if Exalted by Scryers
                case 45482:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    // Get Aldor reputation rank
                    if (((Player *)this)->GetReputationRank(932) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45480;
                        break;
                    }
                    // Get Scryers reputation rank
                    if (((Player *)this)->GetReputationRank(934) == REP_EXALTED)
                    {
                        triggered_spell_id = 45428;
                        break;
                    }
                    return SPELL_AURA_PROC_FAILED;
                }
                // Sunwell Exalted Tank Neck (Shattered Sun Pendant of Resolve neck)
                // cast 45431 Arcane Insight if Exalted by Aldor
                // cast 45432 Light's Ward if Exalted by Scryers
                case 45483:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    // Get Aldor reputation rank
                    if (((Player *)this)->GetReputationRank(932) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45432;
                        break;
                    }
                    // Get Scryers reputation rank
                    if (((Player *)this)->GetReputationRank(934) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45431;
                        break;
                    }
                    return SPELL_AURA_PROC_FAILED;
                }
                // Sunwell Exalted Healer Neck (Shattered Sun Pendant of Restoration neck)
                // cast 45478 Light's Salvation if Exalted by Aldor
                // cast 45430 Arcane Surge if Exalted by Scryers
                case 45484:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    // Get Aldor reputation rank
                    if (((Player *)this)->GetReputationRank(932) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45478;
                        break;
                    }
                    // Get Scryers reputation rank
                    if (((Player *)this)->GetReputationRank(934) == REP_EXALTED)
                    {
                        triggered_spell_id = 45430;
                        break;
                    }
                    return SPELL_AURA_PROC_FAILED;
                }
                /*
                // Sunwell Exalted Caster Neck (??? neck)
                // cast ??? Light's Wrath if Exalted by Aldor
                // cast ??? Arcane Bolt if Exalted by Scryers*/
                case 46569:
                    return SPELL_AURA_PROC_FAILED;                           // old unused version
                // Living Seed
                case 48504:
                {
                    triggered_spell_id = 48503;
                    basepoints[0] = triggerAmount;
                    target = this;
                    break;
                }
                // Health Leech (used by Bloodworms)
                case 50453:
                {
                    Unit *owner = GetOwner();
                    if (!owner)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 50454;
                    basepoints[0] = int32(damage*1.69);
                    target = owner;
                    break;
                }
                // Vampiric Touch (generic, used by some boss)
                case 52723:
                case 60501:
                {
                    triggered_spell_id = 52724;
                    basepoints[0] = damage / 2;
                    target = this;
                    break;
                }
                // Shadowfiend Death (Gain mana if pet dies with Glyph of Shadowfiend)
                case 57989:
                {
                    Unit *owner = GetOwner();
                    if (!owner || owner->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    // Glyph of Shadowfiend (need cast as self cast for owner, no hidden cooldown)
                    owner->CastSpell(owner,58227,true,castItem,triggeredByAura);
                    return SPELL_AURA_PROC_OK;
                }
                // Kill Command, pet aura
                case 58914:
                {
                    // also decrease owner buff stack
                    Unit* owner = GetOwner();
                    if (!owner)
                        return SPELL_AURA_PROC_FAILED;

                    owner->RemoveAuraHolderFromStack(34027);

                    // Remove only single aura from stack
                    SpellAuraHolderPtr holder = triggeredByAura->GetHolder();
                    if (holder && !holder->IsDeleted())
                    {
                        if (holder->ModStackAmount(-1))
                        {
                            owner->RemoveAurasDueToSpell(34026);
                            return SPELL_AURA_PROC_OK;
                        }
                        else
                            return SPELL_AURA_PROC_CANT_TRIGGER;
                    }
                    else
                        return SPELL_AURA_PROC_FAILED;
                    break;
                }
                // Swift Hand of Justice
                case 59906:
                {
                    triggered_spell_id = 59913;
                    basepoints[0] = GetMaxHealth()/50;
                    break;
                }
                // Discerning Eye of the Beast
                case 59915:
                {
                    if (getPowerType() != POWER_MANA)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 59914;
                    break;
                }
                // Petrified Bark
                case 62337:
                 {
                    triggered_spell_id = 62379;
                    basepoints[0] = damage;
                    // this == pVictim, why? :/ temp. workaround
                    target = SelectRandomUnfriendlyTarget(getVictim());
                    break;
                 }
                // Glyph of Life Tap
                case 63320:
                    triggered_spell_id = 63321;
                    break;
                // Meteor Fists
                case 66725:
                case 68161:
                    triggered_spell_id = 66765;
                    break;
                // Meteor Fists
                case 66808:
                case 68160:
                    triggered_spell_id = 66809;
                    break;
                // Dark Hunger (Lich King)
                case 69383:
                    basepoints[0] = 0.5f * damage;
                    target = this;
                    triggered_spell_id = 69384;
                    break;
                // Shiny Shard of the Scale - Equip Effect
                case 69739:
                    // Cauterizing Heal or Searing Flame
                    triggered_spell_id = (procFlag & PROC_FLAG_SUCCESSFUL_POSITIVE_SPELL) ? 69734 : 69730;
                    break;
                // Purified Shard of the Scale - Equip Effect
                case 69755:
                    // Cauterizing Heal or Searing Flame
                    triggered_spell_id = (procFlag & PROC_FLAG_SUCCESSFUL_POSITIVE_SPELL) ? 69733 : 69729;
                    break;
                // Vampiric Might (Cult Fanatic, Icecrown Citadel, Lady Deathwhisper encounter)
                case 70674:
                {
                    if (damage <= 0)
                        return SPELL_AURA_PROC_FAILED;

                    basepoints[0] = 3 * damage;
                    triggered_spell_id = 70677;
                    target = this;
                    break;
                }
                // Essence of the Blood Queen
                case 70871:
                {
                    triggered_spell_id = 70872;
                    target = this;
                    basepoints[0] = int32((damage * triggerAmount) / 100.0f);
                    if (basepoints[0] < 0)
                        return SPELL_AURA_PROC_FAILED;
                    break;
                }
                case 71169:
                {
                    // Shadow's Fate
                    if (GetTypeId() != TYPEID_UNIT)
                        return SPELL_AURA_PROC_FAILED;

                    switch (((Creature*)this)->GetCreatureInfo()->Entry)
                    {
                        case 38431:  // Puthricide 25
                        case 38586:
                            CastSpell(this, 71518, true);
                            break;
                        case 38434:  // Lanathel 25
                        case 38436:
                            CastSpell(this, 72934, true);
                            break;
                        case 38265:  // Sindragosa 25
                        case 38267:
                            CastSpell(this, 72289, true);
                            break;
                        default:
                            break;
                    }
                    CastSpell(triggeredByAura->GetCaster(), 71203, true);
                    return SPELL_AURA_PROC_OK;
                }
                // Item - Deathbringer's Will
                case 71519:
                case 71562:
                {
                    uint32 const normal_spells[MAX_CLASSES*3] =
                    {
                        0, 0, 0,                // (unused)
                        71491, 71484, 71492,    // Warrior
                        71491, 71484, 71492,    // Paladin
                        71485, 71491, 71486,    // Hunter
                        71485, 71486, 71492,    // Rogue
                        71492, 71492, 71492,    // Priest
                        71491, 71484, 71492,    // Death Knight
                        71485, 71486, 71492,    // Shaman
                        71492, 71492, 71492,    // Mage
                        71492, 71492, 71492,    // Warlock
                        0, 0, 0,                // (unused)
                        71485, 71484, 71492     // Druid
                    };
                    uint32 const heroic_spells[MAX_CLASSES*3] =
                    {
                        0, 0, 0,                // (unused)
                        71559, 71561, 71560,    // Warrior
                        71559, 71561, 71560,    // Paladin
                        71556, 71559, 71558,    // Hunter
                        71556, 71558, 71560,    // Rogue
                        71560, 71560, 71560,    // Priest
                        71559, 71561, 71560,    // Death Knight
                        71556, 71558, 71560,    // Shaman
                        71560, 71560, 71560,    // Mage
                        71560, 71560, 71560,    // Warlock
                        0, 0, 0,                // (unused)
                        71556, 71561, 71560     // Druid
                    };

                    if (cooldown && HasSpellCooldown(dummySpell->Id))
                        return SPELL_AURA_PROC_FAILED;

                    uint32 const *proc_spells = NULL;
                    switch (dummySpell->Id)
                    {
                        case 71519: proc_spells = normal_spells; break;
                        case 71562: proc_spells = heroic_spells; break;
                        default: return SPELL_AURA_PROC_FAILED;
                    }

                    CastSpell(this, proc_spells[getClass()*3 + urand(0,2)], true, castItem, triggeredByAura);

                    if (cooldown)
                        AddSpellCooldown(dummySpell->Id, 0, time(NULL) + cooldown);

                    return SPELL_AURA_PROC_OK;
                }
                // Item - Shadowmourne Legendary
                case 71903:
                {
                    if (!roll_chance_i(triggerAmount))
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 71905;             // Soul Fragment

                    SpellAuraHolderPtr aurHolder = GetSpellAuraHolder(triggered_spell_id);

                    // will added first to stack
                    if (!aurHolder)
                        CastSpell(this, 72521, true);       // Shadowmourne Visual Low
                    // half stack
                    else if (aurHolder->GetStackAmount() + 1 == 6)
                        CastSpell(this, 72523, true);       // Shadowmourne Visual High
                    // full stack
                    else if (aurHolder->GetStackAmount() + 1 >= aurHolder->GetSpellProto()->GetStackAmount())
                    {
                        RemoveAurasDueToSpell(triggered_spell_id);
                        CastSpell(this, 71904, true);       // Chaos Bane
                        return SPELL_AURA_PROC_OK;
                    }
                    break;
                }
                // Necrotic Touch item 50692
                case 71875:
                case 71877:
                {
                    basepoints[0] = damage * triggerAmount / 100;
                    target = pVictim;
                    triggered_spell_id = 71879;
                    break;
                }
                case 85466:                                 // Bane of Havoc
                case 85468:
                {
                    triggered_spell_id = 85455;
                    // search bane debuff
                    target = GetSingleCastSpellTarget(80240);
                    if (SpellEntry const* havoc = sSpellStore.LookupEntry(80240))
                        basepoints[0] = int32(damage * havoc->CalculateSimpleValue(EFFECT_INDEX_0) / 100.0f);

                    if (!target || target == pVictim)
                        return SPELL_AURA_PROC_FAILED;
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_MAGE:
        {
            // Magic Absorption
            if (dummySpell->GetSpellIconID() == 459)             // only this spell have SpellIconID == 459 and dummy aura
            {
                if (getPowerType() != POWER_MANA)
                    return SPELL_AURA_PROC_FAILED;

                // mana reward
                basepoints[0] = (triggerAmount * GetMaxPower(POWER_MANA) / 100);
                target = this;
                triggered_spell_id = 29442;
                break;
            }
            // Master of Elements
            if (dummySpell->GetSpellIconID() == 1920)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                // mana cost save
                int32 cost = procSpell->GetManaCost() + procSpell->GetManaCostPercentage() * GetCreateMana() / 100;
                basepoints[0] = cost * triggerAmount/100;
                if (basepoints[0] <=0)
                    return SPELL_AURA_PROC_FAILED;

                target = this;
                triggered_spell_id = 29077;
                break;
            }

            // Arcane Potency
            if (dummySpell->GetSpellIconID() == 2120)
            {
                if (!procSpell || procSpell->Id == 44401)
                    return SPELL_AURA_PROC_FAILED;

                target = this;
                switch (dummySpell->Id)
                {
                    case 31571:
                        triggered_spell_id = 57529;
                        break;
                    case 31572:
                        triggered_spell_id = 57531;
                        break;
                    default:
                        sLog.outError("Unit::HandleDummyAuraProc: non handled spell id: %u",dummySpell->Id);
                        return SPELL_AURA_PROC_FAILED;
                }
                break;
            }

            // Hot Streak
            if (dummySpell->GetSpellIconID() == 2999)
            {
                if (effIndex != EFFECT_INDEX_0)
                    return SPELL_AURA_PROC_OK;
                Aura *counter = GetAura(triggeredByAura->GetId(), EFFECT_INDEX_1);
                if (!counter)
                    return SPELL_AURA_PROC_OK;

                // Count spell criticals in a row in second aura
                Modifier *mod = counter->GetModifier();
                if (procEx & PROC_EX_CRITICAL_HIT)
                {
                    mod->m_amount *=2;
                    if (mod->m_amount < 100) // not enough
                        return SPELL_AURA_PROC_OK;
                    // Critical counted -> roll chance
                    if (roll_chance_i(triggerAmount))
                        CastSpell(this, 48108, true, castItem, triggeredByAura);
                }
                mod->m_amount = 25;
                return SPELL_AURA_PROC_OK;
            }
            // Burnout
            if (dummySpell->GetSpellIconID() == 2998)
            {
                if(!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                int32 cost = procSpell->GetManaCost() + procSpell->GetManaCostPercentage() * GetCreateMana() / 100;
                basepoints[0] = cost * triggerAmount/100;
                if (basepoints[0] <=0)
                    return SPELL_AURA_PROC_FAILED;

                triggered_spell_id = 44450;
                target = this;
                break;
            }
            switch(dummySpell->Id)
            {
                // Ignite
                case 11119:
                case 11120:
                case 12846:
                case 12847:
                case 12848:
                {
                    switch (dummySpell->Id)
                    {
                        case 11119: basepoints[0] = int32(0.04f*damage); break;
                        case 11120: basepoints[0] = int32(0.08f*damage); break;
                        case 12846: basepoints[0] = int32(0.12f*damage); break;
                        case 12847: basepoints[0] = int32(0.16f*damage); break;
                        case 12848: basepoints[0] = int32(0.20f*damage); break;
                        default:
                            sLog.outError("Unit::HandleDummyAuraProc: non handled spell id: %u (IG)",dummySpell->Id);
                            return SPELL_AURA_PROC_FAILED;
                    }

                    triggered_spell_id = 12654;
                    break;
                }
                // Empowered Fire (mana regen)
                case 12654:
                {
                    Unit* caster = triggeredByAura->GetCaster();
                    // it should not be triggered from other ignites
                    if (caster && pVictim && caster->GetObjectGuid() == pVictim->GetObjectGuid())
                    {
                        Unit::AuraList const& auras = caster->GetAurasByType(SPELL_AURA_ADD_FLAT_MODIFIER);
                        for (Unit::AuraList::const_iterator i = auras.begin(); i != auras.end(); i++)
                        {
                            switch((*i)->GetId())
                            {
                                case 31656:
                                case 31657:
                                case 31658:
                                {
                                    if (roll_chance_i(int32((*i)->GetSpellProto()->GetProcChance())))
                                    {
                                        caster->CastSpell(caster, 67545, true);
                                        return SPELL_AURA_PROC_OK;
                                    }
                                    else
                                        return SPELL_AURA_PROC_FAILED;
                                }
                            }
                        }
                    }
                    return SPELL_AURA_PROC_FAILED;
                }
                // Glyph of Ice Block
                case 56372:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    // not 100% safe with client version switches but for 3.1.3 no spells with cooldown that can have mage player except Frost Nova.
                    RemoveSpellCategoryCooldown(35, true);
                    return SPELL_AURA_PROC_OK;
                }
                // Glyph of Icy Veins
                case 56374:
                {
                    Unit::AuraList const& hasteAuras = GetAurasByType(SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK);
                    for(Unit::AuraList::const_iterator i = hasteAuras.begin(); i != hasteAuras.end();)
                    {
                        if (!IsPositiveSpell((*i)->GetId()))
                        {
                            RemoveAurasDueToSpell((*i)->GetId());
                            i = hasteAuras.begin();
                        }
                        else
                            ++i;
                    }
                    RemoveSpellsCausingAura(SPELL_AURA_HASTE_SPELLS);
                    RemoveSpellsCausingAura(SPELL_AURA_MOD_DECREASE_SPEED);
                    return SPELL_AURA_PROC_OK;
                }
                // Glyph of Polymorph
                case 56375:
                {
                    if (!pVictim || !pVictim->isAlive())
                        return SPELL_AURA_PROC_FAILED;

                    pVictim->RemoveSpellsCausingAura(SPELL_AURA_PERIODIC_DAMAGE);
                    pVictim->RemoveSpellsCausingAura(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
                    pVictim->RemoveSpellsCausingAura(SPELL_AURA_PERIODIC_LEECH);
                    return SPELL_AURA_PROC_OK;
                }
                // Blessing of Ancient Kings
                case 64411:
                {
                    // for DOT procs
                    if (!IsPositiveSpell(procSpell->Id))
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 64413;
                    basepoints[0] = damage * 15 / 100;
                    break;
                }
                // Item - Mage T8 4P Bonus
                case 64869:
                {
                    if (!roll_chance_i(triggeredByAura->GetModifier()->m_amount))
                        return SPELL_AURA_PROC_FAILED;

                    SpellAuraHolderPtr holder;
                    // Missile Barrage
                    if (SpellAuraHolderPtr _holder = GetSpellAuraHolder(44401, GetObjectGuid()))
                        holder = _holder;
                    // Hot Streak
                    else if (SpellAuraHolderPtr _holder = GetSpellAuraHolder(48108, GetObjectGuid()))
                        holder = _holder;
                    // Brain Freeze
                    else if (SpellAuraHolderPtr _holder = GetSpellAuraHolder(57761, GetObjectGuid()))
                        holder = _holder;

                    if (!holder || holder->GetAuraCharges() > 1)
                        return SPELL_AURA_PROC_FAILED;

                    holder->SetAuraCharges(holder->GetAuraCharges() + 1, false);
                    return SPELL_AURA_PROC_OK;
                }
                // Fingers of Frost
                case 74396:
                {
                    // Remove only single aura from stack
                    SpellAuraHolderPtr holder = triggeredByAura->GetHolder();
                    if (holder && !holder->IsDeleted())
                    {
                        if (holder->ModStackAmount(-1))
                        {
                            RemoveSpellAuraHolder(holder);
                            RemoveAurasDueToSpell(44544);
                        }
                        return SPELL_AURA_PROC_OK;
                    }
                    else
                        return SPELL_AURA_PROC_FAILED;
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            // Retaliation
            if (dummySpell->GetSpellFamilyFlags().test<CF_WARRIOR_RETALIATION2>())
            {
                // check attack comes not from behind
                if (!HasInArc(M_PI_F, pVictim))
                    return SPELL_AURA_PROC_FAILED;

                triggered_spell_id = 22858;
                break;
            }
            // Second Wind
            if (dummySpell->GetSpellIconID() == 1697)
            {
                // only for spells and hit/crit (trigger start always) and not start from self casted spells (5530 Mace Stun Effect for example)
                if (procSpell == 0 || !(procEx & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) || this == pVictim)
                    return SPELL_AURA_PROC_FAILED;
                // Need stun or root mechanic
                if (!(GetAllSpellMechanicMask(procSpell) & IMMUNE_TO_ROOT_AND_STUN_MASK))
                    return SPELL_AURA_PROC_FAILED;

                switch (dummySpell->Id)
                {
                    case 29838:
                        triggered_spell_id=29842;
                        break;
                    case 29834:
                        triggered_spell_id=29841;
                        break;
                    case 42770:
                        triggered_spell_id=42771;
                        break;
                    default:
                        sLog.outError("Unit::HandleDummyAuraProc: non handled spell id: %u (SW)",dummySpell->Id);
                    return SPELL_AURA_PROC_FAILED;
                }

                target = this;
                break;
            }
/*
            // Damage Shield
            if (dummySpell->GetSpellIconID() == 3214)
            {
                triggered_spell_id = 59653;
                basepoints[0] = GetShieldBlockValue() * triggerAmount / 100;
                break;
            }
*/
            // Sweeping Strikes
            if (dummySpell->Id == 12328)
            {
                // prevent chain of triggered spell from same triggered spell
                if (procSpell && procSpell->Id == 26654)
                    return SPELL_AURA_PROC_FAILED;

                target = SelectRandomUnfriendlyTarget(pVictim);
                if (!target)
                    return SPELL_AURA_PROC_FAILED;

                triggered_spell_id = 26654;
                break;
            }
            // Vengeance (warrior)
            else if (dummySpell->Id == 93098)
                return HandleVengeanceProc(pVictim, damage, triggerAmount);
            // Vengeance (death knight)
            else if (dummySpell->Id == 93099)
                return HandleVengeanceProc(pVictim, damage, triggerAmount);
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Seed of Corruption
            if (dummySpell->GetSpellFamilyFlags().test<CF_WARLOCK_SEED_OF_CORRUPTION1>())
            {
                Modifier const* mod = triggeredByAura->GetModifier();
                // if damage is more than need or target die from damage deal finish spell
                if (mod->m_amount <= (int32)damage || GetHealth() <= damage)
                {
                    // remember guid before aura delete
                    ObjectGuid casterGuid = triggeredByAura->GetCasterGuid();

                    // Remove aura (before cast for prevent infinite loop handlers)
                    RemoveAurasDueToSpell(triggeredByAura->GetId());

                    // Cast finish spell (triggeredByAura already not exist!)
                    int32 bp = 0;
                    if (mod->m_miscvalue)               // marked as affected by soulburn
                    {
                        bp = 1;
                        CastSpell(this, 87388, true);   // Soul Shard energize
                    }

                    CastCustomSpell(this, 27285, NULL, &bp, NULL, true, castItem, NULL, casterGuid);

                    return SPELL_AURA_PROC_OK;                            // no hidden cooldown
                }

                // Damage counting
                mod->m_amount-=damage;
                return SPELL_AURA_PROC_OK;
            }
            // Seed of Corruption (Mobs cast) - no die req
            if (dummyClassOptions && dummyClassOptions->SpellFamilyFlags == UI64LIT(0x0) && dummySpell->GetSpellIconID() == 1932)
            {
                Modifier const* mod = triggeredByAura->GetModifier();
                // if damage is more than need deal finish spell
                if ( mod->m_amount <= (int32)damage )
                {
                    // remember guid before aura delete
                    ObjectGuid casterGuid = triggeredByAura->GetCasterGuid();

                    // Remove aura (before cast for prevent infinite loop handlers)
                    RemoveAurasDueToSpell(triggeredByAura->GetId());

                    // Cast finish spell (triggeredByAura already not exist!)
                    CastSpell(this, 32865, true, castItem, NULL, casterGuid);
                    return SPELL_AURA_PROC_OK;                            // no hidden cooldown
                }
                // Damage counting
                mod->m_amount-=damage;
                return SPELL_AURA_PROC_OK;
            }
            // Nether Protection
            if (dummySpell->GetSpellIconID() == 1985)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                basepoints[0] = -triggerAmount;
                switch (GetFirstSchoolInMask(GetSpellSchoolMask(procSpell)))
                {
                    case SPELL_SCHOOL_NORMAL:
                        return SPELL_AURA_PROC_FAILED;                   // ignore
                    case SPELL_SCHOOL_HOLY:   triggered_spell_id = 54370; break;
                    case SPELL_SCHOOL_FIRE:   triggered_spell_id = 54371; break;
                    case SPELL_SCHOOL_NATURE: triggered_spell_id = 54375; break;
                    case SPELL_SCHOOL_FROST:  triggered_spell_id = 54372; break;
                    case SPELL_SCHOOL_SHADOW: triggered_spell_id = 54374; break;
                    case SPELL_SCHOOL_ARCANE: triggered_spell_id = 54373; break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
            }
            // Fel Synergy
            else if (dummySpell->GetSpellIconID() == 3222)
            {
                target = GetPet();

                if (!target)
                    return SPELL_AURA_PROC_FAILED;

                basepoints[0] = damage * triggerAmount / 100;
                triggered_spell_id = 54181;
                break;
            }
            // Jinx
            else if (dummySpell->GetSpellIconID() == 5002)
            {
                if (!target || effIndex != EFFECT_INDEX_1)
                    return SPELL_AURA_PROC_FAILED;

                basepoints[0] = triggerAmount;

                switch (target->getClass())
                {
                    case CLASS_WARRIOR:
                        triggered_spell_id = 85539;
                        break;
                    case CLASS_ROGUE:
                        triggered_spell_id = 85540;
                        break;
                    case CLASS_DEATH_KNIGHT:
                        triggered_spell_id = 85541;
                        break;
                    case CLASS_HUNTER:
                        triggered_spell_id = 85542;
                        break;
                    case CLASS_DRUID:
                    {
                        if (target->GetShapeshiftForm() == FORM_BEAR)
                            triggered_spell_id = 85539;
                        else if (target->GetShapeshiftForm() == FORM_CAT)
                            triggered_spell_id = 85540;
                        break;
                    }
                }

                if (!triggered_spell_id)
                    return SPELL_AURA_PROC_FAILED;

                break;
            }
            // Burning Embers
            else if (dummySpell->SpellIconID == 5116)
            {
                if (effIndex != EFFECT_INDEX_0)
                    return SPELL_AURA_PROC_FAILED;

                triggered_spell_id = 85421;
                basepoints[0] = int32(triggerAmount * damage / 100.0f / 7);
                break;
            }

            switch (dummySpell->Id)
            {
                // Nightfall & Glyph of Corruption
                case 18094:
                case 18095:
                case 56218:
                {
                    target = this;
                    triggered_spell_id = 17941;
                    break;
                }
                // Improved Soul Fire
                case 18119:
                case 18120:
                {
                    basepoints[0] = triggerAmount;
                    triggered_spell_id = 85383;
                    break;
                }
                // Shadowflame (Voidheart Raiment set bonus)
                case 37377:
                {
                    triggered_spell_id = 37379;
                    break;
                }
                // Pet Healing (Corruptor Raiment or Rift Stalker Armor)
                case 37381:
                {
                    target = GetPet();
                    if (!target)
                        return SPELL_AURA_PROC_FAILED;

                    // heal amount
                    basepoints[0] = damage * triggerAmount/100;
                    triggered_spell_id = 37382;
                    break;
                }
                // Shadowflame Hellfire (Voidheart Raiment set bonus)
                case 39437:
                {
                    triggered_spell_id = 37378;
                    break;
                }
                // Improved Felhunter (bad description in DBC for triggered aura!)
                case 56249:
                {
                    target = this;
                    triggered_spell_id = 54425;
                    // mana gain amount (normal for aura 54037 and doubled for 54038)
                    if (GetOwner() && GetOwner()->HasAura(54038))
                        basepoints[0] = 8;
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            // Vampiric Touch
            if (dummySpell->GetSpellFamilyFlags().test<CF_PRIEST_VAMPIRIC_TOUCH>())
            {
                if (!pVictim || !pVictim->isAlive())
                    return SPELL_AURA_PROC_FAILED;

                // pVictim is caster of aura
                if (triggeredByAura->GetCasterGuid() != pVictim->GetObjectGuid())
                    return SPELL_AURA_PROC_FAILED;

                // Energize 1% of max. mana per 10 secs
                pVictim->CastSpell(pVictim, 57669, true, castItem, triggeredByAura);
                return SPELL_AURA_PROC_OK;                                // no hidden cooldown
            }

            switch (dummySpell->GetSpellIconID())
            {
                // Sin and Punishment
                case 1869:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    // modify Shadowfiend cooldown
                    ((Player*)this)->SendModifyCooldown(34433, -triggerAmount * IN_MILLISECONDS);
                    return SPELL_AURA_PROC_OK;
                }
                // Evangelism
                case 2138:
                {
                    // Mind Flay
                    bool dark = procSpell && procSpell->Id == 15407;

                    // Don't allow proc from Holy Fire periodic and Mind Flay apply
                    if (dark && (procFlag & PROC_FLAG_ON_DO_PERIODIC) == 0)
                        return SPELL_AURA_PROC_FAILED;
                    else if (!dark && (procFlag & PROC_FLAG_ON_DO_PERIODIC) != 0)
                        return SPELL_AURA_PROC_FAILED;

                    // Rank 1
                    if (dummySpell->Id == 81659)
                        triggered_spell_id = dark ? 87117 : 81660;
                    // Rank 2
                    else
                        triggered_spell_id = dark ? 87118 : 81661;
                    break;
                }
                // Body and Soul
                case 2218:
                {
                    // Body and Soul speed aura
                    if (effIndex == EFFECT_INDEX_0)
                        triggered_spell_id = dummySpell->Id == 64127 ? 64128 : 65081;
                    // Body and Soul dispel poison part
                    else if (pVictim == this && roll_chance_i(triggerAmount))
                        triggered_spell_id = 64136;
                    else
                        return SPELL_AURA_PROC_FAILED;
                    break;
                }
                // Divine Aegis
                case 2820:
                {
                    if (!pVictim || !pVictim->isAlive())
                        return SPELL_AURA_PROC_FAILED;

                    // Heal should be crit if it's not Prayer of Healing
                    if ((!procSpell || procSpell->Id != 596) && (procEx & PROC_EX_CRITICAL_HIT) == 0)
                        return SPELL_AURA_PROC_FAILED;

                    // find Divine Aegis on the target and get absorb amount
                    Aura* DivineAegis = pVictim->GetAura(47753,EFFECT_INDEX_0);
                    if (DivineAegis)
                        basepoints[0] = DivineAegis->GetModifier()->m_amount;
                    basepoints[0] += damage * triggerAmount/100;

                    // limit absorb amount
                    int32 levelbonus = pVictim->getLevel()*125;
                    if (basepoints[0] > levelbonus)
                        basepoints[0] = levelbonus;
                    triggered_spell_id = 47753;
                    break;
                }
                // Rapture
                case 2894:
                {
                    if (!procSpell)
                        return SPELL_AURA_PROC_FAILED;

                    // Proc only on first effect
                    if (triggeredByAura->GetEffIndex() != EFFECT_INDEX_1)
                        return SPELL_AURA_PROC_CANT_TRIGGER;

                    Unit* pCaster = triggeredByAura->GetCaster();
                    if (!pCaster || !pCaster->IsInWorld())
                        return SPELL_AURA_PROC_FAILED;

                    // only Power Word: Shield returns Mana to Priest
                    if (procSpell->Id  != 47753)
                    {
                        // energize caster
                        int32 manapct1000 = 5 * (sSpellMgr.GetSpellRank(triggeredByAura->GetId()) + 2);
                        int32 bp0 = pCaster->GetMaxPower(POWER_MANA) * manapct1000 / 1000;
                        pCaster->CastCustomSpell(pCaster, 47755, &bp0, NULL, NULL, true);
                    }

                    if (!roll_chance_i(triggeredByAura->GetModifier()->m_amount) || pCaster->HasAura(63853))
                        return SPELL_AURA_PROC_FAILED;
                    else
                        //cooldown aura
                        pCaster->CastSpell(pCaster, 63853, true);

                    switch(pVictim->getPowerType())
                    {
                        case POWER_RUNIC_POWER:
                            triggered_spell_id = 63652;
                            break;
                        case POWER_RAGE:
                            triggered_spell_id = 63653;
                            break;
                        case POWER_MANA:
                        {
                            basepoints[0] = pVictim->GetMaxPower(POWER_MANA) * 2 / 100;
                            triggered_spell_id = 63654;
                            break;
                        }
                        case POWER_ENERGY:
                            triggered_spell_id = 63655;
                            break;
                        default:
                            break;
                    }
                    target = pVictim;
                    break;
                }
                // Phantasm
                case 2901:
                {
                    if(!roll_chance_i(triggerAmount))
                        return SPELL_AURA_PROC_FAILED;

                    RemoveSpellsCausingAura(SPELL_AURA_MOD_ROOT);
                    RemoveSpellsCausingAura(SPELL_AURA_MOD_DECREASE_SPEED);
                    break;
                }
                // Train of Thought
                case 2942:
                {
                    if (!procSpell || GetTypeId() != TYPEID_PLAYER || dummySpell->Id == 92295 && !roll_chance_i(50))
                        return SPELL_AURA_PROC_FAILED;

                    // Greater Heal
                    if (procSpell->Id == 2060)
                        // Inner Focus
                        ((Player*)this)->SendModifyCooldown(89485, -triggerAmount * IN_MILLISECONDS);
                    else
                        // Penance
                        ((Player*)this)->SendModifyCooldown(47540, -triggerAmount * IN_MILLISECONDS / 10);

                    return SPELL_AURA_PROC_OK;
                }
                // Divine Touch
                case 3021:
                {
                    if (!procSpell)
                        return SPELL_AURA_PROC_FAILED;

                    // Renew
                    Aura* healingAura = pVictim->GetAura<SPELL_AURA_PERIODIC_HEAL, SPELLFAMILY_PRIEST, CF_PRIEST_RENEW>(GetObjectGuid());
                    if (!healingAura)
                        return SPELL_AURA_PROC_FAILED;

                    int32 healingfromticks = healingAura->GetModifier()->m_amount * GetSpellAuraMaxTicks(procSpell);

                    basepoints[0] = healingfromticks * triggerAmount / 100;
                    triggered_spell_id = 63544;
                    break;
                }
                // Improved Devouring Plague
                case 3790:
                {
                    if (!procSpell)
                        return SPELL_AURA_PROC_FAILED;

                    Aura* leachAura = pVictim->GetAura<SPELL_AURA_PERIODIC_LEECH, SPELLFAMILY_PRIEST, CF_PRIEST_DEVOURING_PLAGUE>(GetObjectGuid());
                    if (!leachAura)
                        return SPELL_AURA_PROC_FAILED;

                    int32 damagefromticks = leachAura->GetModifier()->m_amount * GetSpellAuraMaxTicks(procSpell);
                    basepoints[0] = damagefromticks * triggerAmount / 100;
                    triggered_spell_id = 63675;
                    break;
                }
                // Atonement
                case 4938:
                {
                    basepoints[0] = damage * triggerAmount / 100;
                    triggered_spell_id = 94472;
                    target = pVictim;
                    break;
                }
            }

            switch(dummySpell->Id)
            {
                // Vampiric Embrace
                case 15286:
                {
                    // Return if self damage
                    if (this == pVictim)
                        return SPELL_AURA_PROC_FAILED;

                    // Heal amount - Self/Team
                    int32 team = triggerAmount * damage / 200;
                    int32 self = triggerAmount * damage / 100;
                    CastCustomSpell(this, 15290, &team, &self, NULL, true, castItem, triggeredByAura);
                    return SPELL_AURA_PROC_OK;                                // no hidden cooldown
                }
                // Priest Tier 6 Trinket (Ashtongue Talisman of Acumen)
                case 40438:
                {
                    // Shadow Word: Pain
                    if (procSpell->GetSpellFamilyFlags().test<CF_PRIEST_SHADOW_WORD_PAIN>())
                        triggered_spell_id = 40441;
                    // Renew
                    else if (procSpell->GetSpellFamilyFlags().test<CF_PRIEST_RENEW>())
                        triggered_spell_id = 40440;
                    else
                        return SPELL_AURA_PROC_FAILED;

                    target = this;
                    break;
                }
                // Oracle Healing Bonus ("Garments of the Oracle" set)
                case 26169:
                {
                    // heal amount
                    basepoints[0] = int32(damage * 10/100);
                    target = this;
                    triggered_spell_id = 26170;
                    break;
                }
                // Frozen Shadoweave (Shadow's Embrace set) warning! its not only priest set
                case 39372:
                {
                    if (!procSpell || (GetSpellSchoolMask(procSpell) & (SPELL_SCHOOL_MASK_FROST | SPELL_SCHOOL_MASK_SHADOW))==0)
                        return SPELL_AURA_PROC_FAILED;

                    // heal amount
                    basepoints[0] = damage * triggerAmount/100;
                    target = this;
                    triggered_spell_id = 39373;
                    break;
                }
                // Greater Heal (Vestments of Faith (Priest Tier 3) - 4 pieces bonus)
                case 28809:
                {
                    triggered_spell_id = 28810;
                    break;
                }
                // Glyph of Dispel Magic
                case 55677:
                {
                    if (!target->IsFriendlyTo(this))
                        return SPELL_AURA_PROC_FAILED;

                    if (target->GetTypeId() == TYPEID_PLAYER)
                        basepoints[0] = int32(target->GetMaxHealth() * triggerAmount / 100);
                    else if (Unit* caster = triggeredByAura->GetCaster())
                        basepoints[0] = int32(caster->GetMaxHealth() * triggerAmount / 100);
                    // triggered_spell_id in spell data
                    break;
                }
                // Glyph of Prayer of Healing
                case 55680:
                {
                    basepoints[0] = int32(damage * triggerAmount  / 200);   // 10% each tick
                    triggered_spell_id = 56161;             // Glyph of Prayer of Healing
                    break;
                }
                // Priest T10 Healer 2P Bonus
                case 70770:
                {
                    triggered_spell_id = 70772;             // Blessed Healing
                    basepoints[0] = int32(triggerAmount * damage / 100) / GetSpellAuraMaxTicks(triggered_spell_id);
                    break;
                }
                // Echo of Light
                case 77485:
                {
                    if (effIndex != EFFECT_INDEX_0)
                        return SPELL_AURA_PROC_FAILED;

                    if (effIndex != EFFECT_INDEX_0)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 77489;
                    basepoints[0] = int32(damage * triggerAmount / 100 / GetSpellAuraMaxTicks(triggered_spell_id));
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            switch(dummySpell->Id)
            {
                // Leader of the Pack
                case 24932:
                {
                    // dummy m_amount store health percent (!=0 if Improved Leader of the Pack applied)
                    int32 heal_percent = triggeredByAura->GetModifier()->m_amount;
                    if (!heal_percent)
                        return SPELL_AURA_PROC_FAILED;

                    // check explicitly only to prevent mana cast when halth cast cooldown
                    if (cooldown && HasSpellCooldown(34299))
                        return SPELL_AURA_PROC_FAILED;

                    // health
                    triggered_spell_id = 34299;
                    basepoints[0] = GetMaxHealth() * heal_percent / 100;
                    target = this;

                    // mana to caster
                    if (triggeredByAura->GetCasterGuid() == GetObjectGuid())
                    {
                        if (SpellEntry const* manaCastEntry = sSpellStore.LookupEntry(60889))
                        {
                            int32 mana_percent = manaCastEntry->CalculateSimpleValue(EFFECT_INDEX_0) * heal_percent;
                            CastCustomSpell(this, manaCastEntry, &mana_percent, NULL, NULL, true, castItem, triggeredByAura);
                        }
                    }
                    break;
                }
                // Healing Touch (Dreamwalker Raiment set)
                case 28719:
                {
                    // mana back
                    basepoints[0] = int32(procSpell->GetManaCost() * 30 / 100);
                    target = this;
                    triggered_spell_id = 28742;
                    break;
                }
                // Healing Touch Refund (Idol of Longevity trinket)
                case 28847:
                {
                    target = this;
                    triggered_spell_id = 28848;
                    break;
                }
                // Mana Restore (Malorne Raiment set / Malorne Regalia set)
                case 37288:
                case 37295:
                {
                    target = this;
                    triggered_spell_id = 37238;
                    break;
                }
                // Druid Tier 6 Trinket
                case 40442:
                {
                    float  chance;

                    // Starfire
                    if (procSpell->GetSpellFamilyFlags().test<CF_DRUID_STARFIRE>())
                    {
                        triggered_spell_id = 40445;
                        chance = 25.0f;
                    }
                    // Rejuvenation
                    else if (procSpell->GetSpellFamilyFlags().test<CF_DRUID_REJUVENATION>())
                    {
                        triggered_spell_id = 40446;
                        chance = 25.0f;
                    }
                    // Mangle (Bear) and Mangle (Cat)
                    else if (procSpell->GetSpellFamilyFlags().test<CF_DRUID_MANGLE_BEAR, CF_DRUID_MANGLE_CAT>())
                    {
                        triggered_spell_id = 40452;
                        chance = 40.0f;
                    }
                    else
                        return SPELL_AURA_PROC_FAILED;

                    if (!roll_chance_f(chance))
                        return SPELL_AURA_PROC_FAILED;

                    target = this;
                    break;
                }
                // Maim Interrupt
                case 44835:
                {
                    // Deadly Interrupt Effect
                    triggered_spell_id = 32747;
                    break;
                }
                // Glyph of Rejuvenation
                case 54754:
                {
                   if (!pVictim || pVictim->GetHealthPercent() >= 50.0f)
                        return SPELL_AURA_PROC_FAILED;

                    target = pVictim;
                    triggered_spell_id = 54755;
                    basepoints[0] = int32(damage * triggerAmount  / 100);
                    break;
                }
                // Glyph of Shred
                case 54815:
                {
                    basepoints[1] = triggerAmount;
                    triggered_spell_id = 63974;
                    break;
                }
                // Glyph of Rake
                case 54821:
                {
                    triggered_spell_id = 54820;
                    break;
                }
                // Glyph of Starfire
                case 54845:
                {
                    triggered_spell_id = 54846;
                    break;
                }
                // Item - Druid T10 Restoration 4P Bonus (Rejuvenation)
                case 70664:
                {
                    if (!procSpell || GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    float radius;
                    SpellEffectEntry const* spellEffect = procSpell->GetSpellEffect(EFFECT_INDEX_0);
                    if (spellEffect && spellEffect->GetRadiusIndex())
                        radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(spellEffect->GetRadiusIndex()));
                    else
                        radius = GetSpellMaxRange(sSpellRangeStore.LookupEntry(procSpell->GetRangeIndex()));

                    ((Player*)this)->ApplySpellMod(procSpell->Id, SPELLMOD_RADIUS, radius);

                    Unit *second = pVictim->SelectRandomFriendlyTarget(pVictim, radius);

                    if (!second)
                        return SPELL_AURA_PROC_FAILED;

                    pVictim->CastSpell(second, procSpell, true, NULL, triggeredByAura, GetObjectGuid());
                    return SPELL_AURA_PROC_OK;
                }
                // Item - Druid T10 Balance 4P Bonus
                case 70723:
                {
                    triggered_spell_id = 71023;             // Languish
                    basepoints[0] = int32(triggerAmount * damage / 100) / GetSpellAuraMaxTicks(triggered_spell_id);
                    break;
                }
                // Vengeance
                case 84840:
                    return HandleVengeanceProc(pVictim, damage, triggerAmount);
            }
            // King of the Jungle
            if (dummySpell->GetSpellIconID() == 2850)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                // Enrage (bear) - single rank - the aura for the bear form from the 2 existing kotj auras has a miscValue == 126
                if (procSpell->Id == 5229 && triggeredByAura->GetMiscValue() == 126)
                {
                    // note : the remove part is done in spellAuras/HandlePeriodicEnergize as RemoveAurasDueToSpell
                    basepoints[0] = triggerAmount;
                    triggered_spell_id = 51185;
                    target = this;
                    break;
                }
                // Tiger Fury (cat) - all ranks - the aura for the cat form from the 2 existing kotj auras has a miscValue != 126
                if (procSpell->GetSpellFamilyFlags().test<CF_DRUID_TIGERS_FURY>()  && triggeredByAura->GetMiscValue() != 126)
                {
                    basepoints[0] = triggerAmount;
                    triggered_spell_id = 51178;
                    target = this;
                    break;
                }
                return SPELL_AURA_PROC_FAILED;
            }
            // King of the Jungle
            if (dummySpell->GetSpellIconID() == 2850)
            {
                switch (effIndex)
                {
                    case EFFECT_INDEX_0:    // Enrage (bear)
                    {
                        // note : aura removal is done in SpellAuraHolder::HandleSpellSpecificBoosts
                        basepoints[0] = triggerAmount;
                        triggered_spell_id = 51185;
                        break;
                    }
                    case EFFECT_INDEX_1:    // Tiger's Fury (cat)
                    {
                        basepoints[0] = triggerAmount;
                        triggered_spell_id = 51178;
                        break;
                    }
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
            }
            // Eclipse
            else if (dummySpell->GetSpellIconID() == 2856)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                // Wrath crit
                if (procSpell->GetSpellFamilyFlags().test<CF_DRUID_WRATH>())
                {
                    if (HasAura(48517))
                        return SPELL_AURA_PROC_FAILED;

                    if (!roll_chance_i(60))
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 48518;
                    target = this;
                    break;
                }
                // Starfire crit
                if (procSpell->GetSpellFamilyFlags().test<CF_DRUID_STARFIRE>())
                {
                    if (HasAura(48518))
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 48517;
                    target = this;
                    break;
                }
                return SPELL_AURA_PROC_FAILED;
            }
            // Living Seed
            else if (dummySpell->GetSpellIconID() == 2860)
            {
                triggered_spell_id = 48504;
                basepoints[0] = triggerAmount * damage / 100;
                break;
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            switch(dummySpell->Id)
            {
                // Clean Escape
                case 23582:
                    // triggered spell have same masks and etc with main Vanish spell
                    if (!procSpell || procSpell->GetSpellEffectIdByIndex(EFFECT_INDEX_0) == SPELL_EFFECT_NONE)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 23583;
                    break;
                // Deadly Throw Interrupt
                case 32748:
                {
                    // Prevent cast Deadly Throw Interrupt on self from last effect (apply dummy) of Deadly Throw
                    if (this == pVictim)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 32747;
                    break;
                }
                // Tricks of the trade
                case 57934:
                {
                    triggered_spell_id = 57933;             // Tricks of the Trade, increased damage buff
                    target = getHostileRefManager().GetThreatRedirectionTarget();

                    if (!target)
                        return SPELL_AURA_PROC_FAILED;

                    CastSpell(this, 59628, true);           // Tricks of the Trade (caster timer)
                    break;
                }
                case 76806:                                 // Main Gauche
                {
                    if (effIndex != EFFECT_INDEX_0 || !roll_chance_i(triggerAmount))
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 86392;
                    break;
                }
                case 84617:                                 // Revealing Strike
                {
                    // Check procSpell for Eviscerate, Envenom, Expose Armor, Kidney Shot and Rupture
                    if (!procSpell || !NeedsComboPoints(procSpell))
                        return SPELL_AURA_PROC_FAILED;
                    break;
                }
            }
            // Murderous Intent
            if (dummySpell->GetSpellIconID() == 134)
            {
                if (triggeredByAura->GetEffIndex() != EFFECT_INDEX_0)
                    return SPELL_AURA_PROC_FAILED;

                if (!pVictim || pVictim->GetHealthPercent() > dummySpell->CalculateSimpleValue(EFFECT_INDEX_1))
                    return SPELL_AURA_PROC_FAILED;

                triggered_spell_id = 79132;
                basepoints[0] = triggerAmount;
                break;
            }
            // Improved Expose Armor
            else if (dummySpell->GetSpellIconID() == 563)
                return SPELL_AURA_PROC_FAILED;              // done in other way
            // Honor Among Thieves
            else if (dummySpell->GetSpellIconID() == 2903)
            {
                if (Unit* caster = triggeredByAura->GetCaster())
                {
                    caster->CastSpell(caster, 51699, true);
                    return SPELL_AURA_PROC_OK;
                }
                return SPELL_AURA_PROC_FAILED;
            }
            // Cut to the Chase
            else if (dummySpell->GetSpellIconID() == 2909)
            {
                // "refresh your Slice and Dice duration to its 5 combo point maximum"
                // lookup Slice and Dice
                if (SpellAuraHolderPtr holder = GetSpellAuraHolder(5171))
                {
                    int32 duration = GetSpellMaxDuration(holder->GetSpellProto());
                    if (GetTypeId() == TYPEID_PLAYER)
                        ((Player*)this)->ApplySpellMod(holder->GetId(), SPELLMOD_DURATION, duration);

                    holder->SetAuraMaxDuration(duration);
                    holder->RefreshHolder();
                    return SPELL_AURA_PROC_OK;
                }

                return SPELL_AURA_PROC_FAILED;
            }
            // Deadly Brew
            else if (dummySpell->GetSpellIconID() == 2963)
            {
                triggered_spell_id = 44289;
                break;
            }
            // Bandit's Guile
            else if (dummySpell->GetSpellIconID() == 2983)
            {
                basepoints[0] = 0;
                triggered_spell_id = 84748;

                // Insight buffs
                uint32 buffs[4] = { 84745, 84746, 84747, 0 };

                if (pVictim->HasAura(triggered_spell_id))
                {
                    int i = 0;
                    bool found = false;
                    for (; i < 3; ++i)
                    {
                        if (SpellAuraHolderPtr holder = GetSpellAuraHolder(buffs[i]))
                        {
                            found = true;
                            RemoveSpellAuraHolder(holder);
                            break;
                        }
                    }

                    if (found)
                        ++i;
                    else
                        i = 0;

                    if (SpellEntry const * spell = sSpellStore.LookupEntry(buffs[i]))
                    {
                        CastSpell(this, spell, true);
                        basepoints[0] = spell->CalculateSimpleValue(EFFECT_INDEX_0);
                    }
                }
                else
                {
                    for (int i = 0; i < 3; ++i)
                        RemoveAurasDueToSpell(buffs[i]);
                }
                break;
            }
            // Quick Recovery
            else if (dummySpell->GetSpellIconID() == 2116)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                // energy cost save
                basepoints[0] = procSpell->GetManaCost() * triggerAmount/100;
                if (basepoints[0] <= 0)
                    return SPELL_AURA_PROC_FAILED;

                target = this;
                triggered_spell_id = 31663;
                break;
            }
            // Venomous Wounds
            else if (dummySpell->GetSpellIconID() == 4888)
            {
                if (!pVictim || triggeredByAura->GetEffIndex() != EFFECT_INDEX_1)
                    return SPELL_AURA_PROC_FAILED;

                // search poison
                bool found = false;
                if (pVictim->HasAuraState(AURA_STATE_DEADLY_POISON))
                    found = true;
                else
                {
                    Unit::SpellAuraHolderMap const& auras = pVictim->GetSpellAuraHolderMap();
                    for (Unit::SpellAuraHolderMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
                    {
                        if (itr->second->GetSpellProto()->GetSpellFamilyName() == SPELLFAMILY_ROGUE &&
                            itr->second->GetSpellProto()->GetDispel() == DISPEL_POISON &&
                            itr->second->GetCasterGuid() == GetObjectGuid())
                        {
                            found = true;
                            break;
                        }
                    }
                }
                if (!found)
                    return SPELL_AURA_PROC_FAILED;

                // Venomous Wound
                CastSpell(pVictim, 79136, true);
                // Venomous Vim
                triggered_spell_id = 51637;
                basepoints[0] = triggerAmount;
                break;
            }
            // Restless Blades
            else if (dummySpell->GetSpellIconID() == 4897)
            {
                if (!procSpell || GetTypeId() != TYPEID_PLAYER || !NeedsComboPoints(procSpell))
                    return SPELL_AURA_PROC_FAILED;

                int32 amt = GetComboPoints() * triggerAmount;
                ((Player*)this)->SendModifyCooldown(2983, -amt);    // Sprint
                ((Player*)this)->SendModifyCooldown(13750, -amt);   // Adrenaline Rush
                ((Player*)this)->SendModifyCooldown(51690, -amt);   // Shadowstep
                ((Player*)this)->SendModifyCooldown(73981, -amt);   // Redirect

                return SPELL_AURA_PROC_OK;
            }
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Thrill of the Hunt
            if (dummySpell->GetSpellIconID() == 2236)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                // mana cost save
                int32 mana = procSpell->GetManaCost() + procSpell->GetManaCostPercentage() * GetCreateMana() / 100;
                basepoints[0] = mana * 40/100;
                if (basepoints[0] <= 0)
                    return SPELL_AURA_PROC_FAILED;

                target = this;
                triggered_spell_id = 34720;
                break;
            }
            // Hunting Party
            if (dummySpell->GetSpellIconID() == 3406)
            {
                triggered_spell_id = 57669;
                target = this;
                break;
            }
            // Lock and Load
            if (dummySpell->GetSpellIconID() == 3579)
            {
                // Proc only from periodic (from trap activation proc another aura of this spell)
                if (!(procFlag & PROC_FLAG_ON_DO_PERIODIC) || !roll_chance_i(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;

                triggered_spell_id = 56453;
                target = this;
                break;
            }
            // Rapid Recuperation
            if (dummySpell->GetSpellIconID() == 3560)
            {
                // This effect only from Rapid Killing (mana regen)
                if (!procSpell->GetSpellFamilyFlags().test<CF_HUNTER_RAPID_KILLING>())
                    return SPELL_AURA_PROC_FAILED;

                target = this;

                switch(dummySpell->Id)
                {
                    case 53228:                             // Rank 1
                        triggered_spell_id = 56654;
                        break;
                    case 53232:                             // Rank 2
                        triggered_spell_id = 58882;
                        break;
                }
                break;
            }
            // Glyph of Mend Pet
            if ( dummySpell->Id == 57870)
            {
                pVictim->CastSpell(pVictim, 57894, true, NULL, NULL, GetObjectGuid());
                return SPELL_AURA_PROC_OK;
            }
            // Misdirection
            else if (dummySpell->Id == 34477)
            {
                triggered_spell_id = 35079;                 // 4 sec buff on self
                target = this;
                break;
            }
            else if (dummySpell->Id == 37483)               // Improved Kill Command - Item set bonus
            {
                triggered_spell_id = 37482;                 // Exploited Weakness
                break;
            }
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Seal of Righteousness - melee proc dummy (addition $MWS*(0.011*$AP+0.022*$SPH) damage)
            if (dummySpell->Id == 20154 && effIndex == EFFECT_INDEX_0)
            {
                triggered_spell_id = 25742;
                float ap = GetTotalAttackPowerValue(BASE_ATTACK);
                int32 holy = SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY);
                if (holy < 0)
                    holy = 0;

                basepoints[0] = int32(GetAttackTime(BASE_ATTACK) * (ap * 0.011f + 0.022f * holy) / 1000);
                break;
            }
            // Seal of Justice - melee proc dummy (addition $MWS*(0.005*$AP+0.01*$SPH) damage)
            else if (dummySpell->Id == 20164)
            {
                triggered_spell_id = 20170;
                float ap = GetTotalAttackPowerValue(BASE_ATTACK);
                int32 holy = SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY);
                if (holy < 0)
                    holy = 0;

                basepoints[1] = int32(GetAttackTime(BASE_ATTACK) * (ap * 0.005f + 0.01f * holy) / 1000);
                break;
            }
            // Vengeance (paladin)
            else if (dummySpell->Id == 84839)
                return HandleVengeanceProc(pVictim, damage, triggerAmount);

            // Divine Purpose
            if (dummySpell->SpellIconID == 2170)
            {
                if (!roll_chance_i(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;

                triggered_spell_id = 90174;
                break;
            }
            // Tower of Radiance, Rank 3
            else if (dummySpell->GetSpellIconID() == 3402)
            {
                // Must be target of Beacon of Light
                if (!pVictim || !pVictim->GetSpellAuraHolder(53563, GetObjectGuid()))
                    return SPELL_AURA_PROC_FAILED;
                break;
            }
            // Selfless Healer
            else if (dummySpell->SpellIconID == 3924)
            {
                target = this;
                basepoints[0] = triggerAmount;
                if (Spell* spell = GetCurrentSpell(CURRENT_GENERIC_SPELL))
                    // Word of Glory
                    if (spell->m_spellInfo->Id == 85673)
                        basepoints[0] *= spell->GetUsedHolyPower();
                break;
            }

            switch (dummySpell->Id)
            {
                // Judgement of Light
                case 20185:
                {
                    if (pVictim == this)
                       return SPELL_AURA_PROC_FAILED;

                    // only at real damage
                    if (!damage)
                        return SPELL_AURA_PROC_FAILED;

                    basepoints[0] = int32( pVictim->GetMaxHealth() * triggeredByAura->GetModifier()->m_amount / 100 );
                    pVictim->CastCustomSpell(pVictim, 20267, &basepoints[0], NULL, NULL, true, NULL, triggeredByAura);
                    return SPELL_AURA_PROC_OK;
                }
                // Judgement of Wisdom
                case 20186:
                {
                    // only at real damage
                    if (!damage)
                        return SPELL_AURA_PROC_FAILED;

                    if (pVictim->getPowerType() == POWER_MANA)
                    {
                        // 2% of maximum base mana
                        basepoints[0] = int32(pVictim->GetCreateMana() * 2 / 100);
                        pVictim->CastCustomSpell(pVictim, 20268, &basepoints[0], NULL, NULL, true, NULL, triggeredByAura);
                    }
                    return SPELL_AURA_PROC_OK;
                }
                // Heart of the Crusader (Rank 1)
                case 20335:
                    triggered_spell_id = 21183;
                    break;
                // Heart of the Crusader (Rank 2)
                case 20336:
                    triggered_spell_id = 54498;
                    break;
                // Heart of the Crusader (Rank 3)
                case 20337:
                    triggered_spell_id = 54499;
                    break;
                case 20911:                                 // Blessing of Sanctuary
                case 25899:                                 // Greater Blessing of Sanctuary
                {
                    target = this;
                    switch (target->getPowerType())
                    {
                        case POWER_MANA:
                            triggered_spell_id = 57319;
                            break;
                        default:
                            return SPELL_AURA_PROC_FAILED;
                    }
                    break;
                }
                // Holy Power (Redemption Armor set)
                case 28789:
                {
                    if (!pVictim)
                        return SPELL_AURA_PROC_FAILED;

                    // Set class defined buff
                    switch (pVictim->getClass())
                    {
                        case CLASS_PALADIN:
                        case CLASS_PRIEST:
                        case CLASS_SHAMAN:
                        case CLASS_DRUID:
                            triggered_spell_id = 28795;     // Increases the friendly target's mana regeneration by $s1 per 5 sec. for $d.
                            break;
                        case CLASS_MAGE:
                        case CLASS_WARLOCK:
                            triggered_spell_id = 28793;     // Increases the friendly target's spell damage and healing by up to $s1 for $d.
                            break;
                        case CLASS_HUNTER:
                        case CLASS_ROGUE:
                            triggered_spell_id = 28791;     // Increases the friendly target's attack power by $s1 for $d.
                            break;
                        case CLASS_WARRIOR:
                            triggered_spell_id = 28790;     // Increases the friendly target's armor
                            break;
                        default:
                            return SPELL_AURA_PROC_FAILED;
                    }
                    break;
                }
                // Spiritual Attunement
                case 31785:
                case 33776:
                {
                    // if healed by another unit (pVictim)
                    if (this == pVictim)
                        return SPELL_AURA_PROC_FAILED;

                    // dont count overhealing
                    uint32 diff = GetMaxHealth()-GetHealth();

                    if (!diff)
                        return SPELL_AURA_PROC_FAILED;

                    basepoints[0] = triggerAmount * (damage > diff ? diff : damage) / 100;
                    target = this;
                    triggered_spell_id = 31786;
                    break;
                }
                // Seal of Truth (damage calc on apply aura)
                case 31801:
                {
                    if (effIndex != EFFECT_INDEX_0)         // effect 1,2 used by seal unleashing code
                        return SPELL_AURA_PROC_FAILED;

                    // At melee attack or Hammer of the Righteous spell damage considered as melee attack
                    if ((procFlag & PROC_FLAG_SUCCESSFUL_MELEE_HIT) || procSpell && procSpell->Id == 53595)
                        triggered_spell_id = 31803;         // Censure

                    // Add 5-stack effect from Censure
                    if (SpellAuraHolderPtr holder = target->GetSpellAuraHolder(triggered_spell_id, GetObjectGuid()))
                        if (holder->GetStackAmount() >= holder->GetSpellProto()->GetStackAmount())
                            CastSpell(target, 42463, true, NULL, triggeredByAura);
                    break;
                }
                // Judgements of the Wise
                case 31878:
                {
                    // triggered only at casted Judgement spells, not at additional Judgement effects
                    if (!procSpell || procSpell->GetCategory() != 1210)
                        return SPELL_AURA_PROC_FAILED;

                    target = this;
                    triggered_spell_id = 31930;
                    break;
                }
                // Paladin Tier 6 Trinket (Ashtongue Talisman of Zeal)
                case 40470:
                {
                    if (!procSpell)
                        return SPELL_AURA_PROC_FAILED;

                    float  chance;

                    // Flash of light/Holy light
                    if (procSpell->GetSpellFamilyFlags().test<CF_PALADIN_FLASH_OF_LIGHT, CF_PALADIN_HOLY_LIGHT>())
                    {
                        triggered_spell_id = 40471;
                        chance = 15.0f;
                    }
                    // Judgement (any)
                    else if (procSpell->GetSpellFamilyFlags().test<CF_PALADIN_JUDGEMENT_ACTIVATE>())
                    {
                        triggered_spell_id = 40472;
                        chance = 50.0f;
                    }
                    else
                        return SPELL_AURA_PROC_FAILED;

                    if (!roll_chance_f(chance))
                        return SPELL_AURA_PROC_FAILED;
                    break;
                }
                // Light's Beacon (heal target area aura)
                case 53651:
                {
                    // not do bonus heal for explicit beacon focus healing
                    if (GetObjectGuid() == triggeredByAura->GetCasterGuid())
                        return SPELL_AURA_PROC_FAILED;

                    // beacon
                    Unit* beacon = triggeredByAura->GetCaster();
                    if (!beacon || !procSpell || procSpell->Id == 20267)
                        return SPELL_AURA_PROC_FAILED;

                    // find caster main aura at beacon
                    Aura const* dummy = NULL;
                    Unit::AuraList const& baa = beacon->GetAurasByType(SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                    for(Unit::AuraList::const_iterator i = baa.begin(); i != baa.end(); ++i)
                    {
                        if ((*i)->GetId() == 53563 && (*i)->GetCasterGuid() == pVictim->GetObjectGuid())
                        {
                            dummy = (*i)();
                            break;
                        }
                    }

                    // original heal must be form beacon caster
                    if (!dummy)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 53652;             // Beacon of Light
                    uint32 radius = GetSpellMaxRange(sSpellRangeStore.LookupEntry(sSpellStore.LookupEntry(triggered_spell_id)->GetRangeIndex()));

                    if (!beacon->IsWithinDistInMap(this, radius) ||
                        !beacon->IsWithinLOSInMap(this) ||
                        !beacon->IsWithinDistInMap(pVictim, radius) ||
                        !beacon->IsWithinLOSInMap(pVictim))
                        return SPELL_AURA_PROC_FAILED;

                    basepoints[0] = triggeredByAura->GetModifier()->m_amount * damage / 100;
                    // Holy Light heals for 100%
                    if (procSpell && procSpell->Id == 635)
                        basepoints[0] *= 2;

                    // cast with original caster set but beacon to beacon for apply caster mods and avoid LoS check
                    beacon->CastCustomSpell(beacon, triggered_spell_id, &basepoints[0], NULL, NULL, true, castItem, triggeredByAura, pVictim->GetObjectGuid());
                    return SPELL_AURA_PROC_OK;
                }
                // Glyph of Holy Light
                case 54937:
                {
                    triggered_spell_id = 54968;
                    basepoints[0] = triggerAmount * damage / 100;
                    break;
                }
                // Item - Paladin T8 Holy 2P Bonus
                case 64890:
                {
                    triggered_spell_id = 64891;             // Holy Mending
                    basepoints[0] = int32(triggerAmount * damage / 100) / GetSpellAuraMaxTicks(triggered_spell_id);
                    break;
                }
                // Item - Paladin T10 Holy 2P Bonus
                case 70755:
                {
                    triggered_spell_id = 71166;
                    break;
                }
                // Item - Paladin T10 Retribution 2P Bonus
                case 70765:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 70769;
                    break;
                }
                // Anger Capacitor
                case 71406:                                 // normal
                case 71545:                                 // heroic
                {
                    if (!pVictim)
                        return SPELL_AURA_PROC_FAILED;

                    SpellEntry const* mote = sSpellStore.LookupEntry(71432);

                    if (!mote)
                        return SPELL_AURA_PROC_FAILED;
                    uint32 maxStack = mote->GetStackAmount() - (dummySpell->Id == 71545 ? 1 : 0);

                    SpellAuraHolderPtr aurHolder = GetSpellAuraHolder(71432);
                    if (aurHolder && uint32(aurHolder->GetStackAmount() +1) >= maxStack)
                    {
                        RemoveAurasDueToSpell(71432);       // Mote of Anger

                        // Manifest Anger (main hand/off hand)
                        CastSpell(pVictim, !haveOffhandWeapon() || roll_chance_i(50) ? 71433 : 71434, true);
                        return SPELL_AURA_PROC_OK;
                    }
                    else
                        triggered_spell_id = 71432;

                    break;
                }
                // Heartpierce, Item - Icecrown 25 Normal Dagger Proc
                case 71880:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    switch (this->getPowerType())
                    {
                        case POWER_ENERGY:
                            triggered_spell_id = 71882;
                            break;
                        case POWER_RAGE:
                            triggered_spell_id = 71883;
                            break;
                        case POWER_MANA:
                            triggered_spell_id = 71881;
                            break;
                        case POWER_RUNIC_POWER:
                            triggered_spell_id = 71884;
                            break;
                        default:
                            return SPELL_AURA_PROC_FAILED;
                    }
                    break;
                }
                // Heartpierce, Item - Icecrown 25 Heroic Dagger Proc
                case 71892:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    switch (this->getPowerType())
                    {
                        case POWER_ENERGY:
                            triggered_spell_id = 71887;
                            break;
                        case POWER_RAGE:
                            triggered_spell_id = 71886;
                            break;
                        case POWER_MANA:
                            triggered_spell_id = 71888;
                            break;
                        case POWER_RUNIC_POWER:
                            triggered_spell_id = 71885;
                            break;
                        default:
                            return SPELL_AURA_PROC_FAILED;
                    }
                    break;
                }
                // Illuminated Healing
                case 76669:
                {
                    if (effIndex != EFFECT_INDEX_0)
                        return SPELL_AURA_PROC_FAILED;

                    triggered_spell_id = 86273;
                    int32 maxAmt = GetHealth() / 3;
                    if (Aura* oldAura = pVictim->GetAura(triggered_spell_id, EFFECT_INDEX_0))
                        basepoints[0] = oldAura->GetModifier()->m_amount;

                    basepoints[0] += int32(triggerAmount * damage / 100);
                    // Must not exceed 1/3 of paladin's health
                    if (basepoints[0] > maxAmt)
                        basepoints[0] = maxAmt;
                    break;
                }
                // Judgements of the Bold
                case 89901:
                    // triggered only at casted Judgement spells, not at additional Judgement effects
                    if (!procSpell || procSpell->GetCategory() != 1210)
                        return SPELL_AURA_PROC_FAILED;

                    target = this;
                    triggered_spell_id = 89906;
                    break;
            }
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            switch(dummySpell->Id)
            {
                // Totemic Power (The Earthshatterer set)
                case 28823:
                {
                    if (!pVictim )
                        return SPELL_AURA_PROC_FAILED;

                    // Set class defined buff
                    switch (pVictim->getClass())
                    {
                        case CLASS_PALADIN:
                        case CLASS_PRIEST:
                        case CLASS_SHAMAN:
                        case CLASS_DRUID:
                            triggered_spell_id = 28824;     // Increases the friendly target's mana regeneration by $s1 per 5 sec. for $d.
                            break;
                        case CLASS_MAGE:
                        case CLASS_WARLOCK:
                            triggered_spell_id = 28825;     // Increases the friendly target's spell damage and healing by up to $s1 for $d.
                            break;
                        case CLASS_HUNTER:
                        case CLASS_ROGUE:
                            triggered_spell_id = 28826;     // Increases the friendly target's attack power by $s1 for $d.
                            break;
                        case CLASS_WARRIOR:
                            triggered_spell_id = 28827;     // Increases the friendly target's armor
                            break;
                        default:
                            return SPELL_AURA_PROC_FAILED;
                    }
                    break;
                }
                // Lesser Healing Wave (Totem of Flowing Water Relic)
                case 28849:
                {
                    target = this;
                    triggered_spell_id = 28850;
                    break;
                }
                // Windfury Weapon (Passive) 1-5 Ranks
                case 33757:
                {
                    if (GetTypeId()!=TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    if (!castItem || !castItem->IsEquipped())
                        return SPELL_AURA_PROC_FAILED;

                    // custom cooldown processing case
                    if (cooldown && HasSpellCooldown(dummySpell->Id))
                        return SPELL_AURA_PROC_FAILED;

                    // Now amount of extra power stored in 1 effect of Enchant spell
                    // Get it by item enchant id
                    uint32 spellId;
                    switch (castItem->GetEnchantmentId(EnchantmentSlot(TEMP_ENCHANTMENT_SLOT)))
                    {
                        case 283: spellId =  8232; break;   // 1 Rank
                        case 284: spellId =  8235; break;   // 2 Rank
                        case 525: spellId = 10486; break;   // 3 Rank
                        case 1669:spellId = 16362; break;   // 4 Rank
                        case 2636:spellId = 25505; break;   // 5 Rank
                        case 3785:spellId = 58801; break;   // 6 Rank
                        case 3786:spellId = 58803; break;   // 7 Rank
                        case 3787:spellId = 58804; break;   // 8 Rank
                        default:
                        {
                            sLog.outError("Unit::HandleDummyAuraProc: non handled item enchantment (rank?) %u for spell id: %u (Windfury)",
                                castItem->GetEnchantmentId(EnchantmentSlot(TEMP_ENCHANTMENT_SLOT)),dummySpell->Id);
                            return SPELL_AURA_PROC_FAILED;
                        }
                    }

                    SpellEntry const* windfurySpellEntry = sSpellStore.LookupEntry(spellId);
                    if(!windfurySpellEntry)
                    {
                        sLog.outError("Unit::HandleDummyAuraProc: nonexistent spell id: %u (Windfury)",spellId);
                        return SPELL_AURA_PROC_FAILED;
                    }

                    int32 extra_attack_power = CalculateSpellDamage(pVictim, windfurySpellEntry, EFFECT_INDEX_1);

                    // Totem of Splintering
                    if (Aura* aura = GetAura(60764, EFFECT_INDEX_0))
                        extra_attack_power += aura->GetModifier()->m_amount;

                    // Off-Hand case
                    if (castItem->GetSlot() == EQUIPMENT_SLOT_OFFHAND)
                    {
                        // Value gained from additional AP
                        basepoints[0] = int32(extra_attack_power/14.0f * GetAttackTime(OFF_ATTACK)/1000/2);
                        triggered_spell_id = 33750;
                    }
                    // Main-Hand case
                    else
                    {
                        // Value gained from additional AP
                        basepoints[0] = int32(extra_attack_power/14.0f * GetAttackTime(BASE_ATTACK)/1000);
                        triggered_spell_id = 25504;
                    }

                    // apply cooldown before cast to prevent processing itself
                    if (cooldown)
                        AddSpellCooldown(dummySpell->Id,0,time(NULL) + cooldown);

                    // Attack Twice
                    for ( uint32 i = 0; i<2; ++i )
                        CastCustomSpell(pVictim,triggered_spell_id,&basepoints[0],NULL,NULL,true,castItem,triggeredByAura);

                    return SPELL_AURA_PROC_OK;
                }
                // Shaman Tier 6 Trinket
                case 40463:
                {
                    if ( !procSpell )
                        return SPELL_AURA_PROC_FAILED;

                    float chance;
                    if (procSpell->GetSpellFamilyFlags().test<CF_SHAMAN_LIGHTNING_BOLT>())
                    {
                        triggered_spell_id = 40465;         // Lightning Bolt
                        chance = 15.0f;
                    }
                    else if (procSpell->GetSpellFamilyFlags().test<CF_SHAMAN_LESSER_HEALING_WAVE>())
                    {
                        triggered_spell_id = 40465;         // Lesser Healing Wave
                        chance = 10.0f;
                    }
                    else if (procSpell->GetSpellFamilyFlags().test<CF_SHAMAN_STORMSTRIKE1>())
                    {
                        triggered_spell_id = 40466;         // Stormstrike
                        chance = 50.0f;
                    }
                    else
                        return SPELL_AURA_PROC_FAILED;

                    if (!roll_chance_f(chance))
                        return SPELL_AURA_PROC_FAILED;

                    target = this;
                    break;
                }
                // Earthen Power
                case 51523:
                case 51524:
                {
                    triggered_spell_id = 63532;
                    break;
                }
                // Glyph of Healing Wave
                case 55440:
                {
                    // Not proc from self heals
                    if (this==pVictim)
                        return SPELL_AURA_PROC_FAILED;
                    basepoints[0] = triggerAmount * damage / 100;
                    target = this;
                    triggered_spell_id = 55533;
                    break;
                }
                // Spirit Hunt
                case 58877:
                {
                    // Cast on owner
                    target = GetOwner();
                    if (!target)
                        return SPELL_AURA_PROC_FAILED;
                    basepoints[0] = triggerAmount * damage / 100;
                    triggered_spell_id = 58879;
                    break;
                }
                // Glyph of Totem of Wrath
                case 63280:
                {
                    Totem* totem = GetTotem(TOTEM_SLOT_FIRE);
                    if (!totem)
                        return SPELL_AURA_PROC_FAILED;

                    // find totem aura bonus
                    AuraList const& spellPower = totem->GetAurasByType(SPELL_AURA_NONE);
                    for(AuraList::const_iterator i = spellPower.begin();i != spellPower.end(); ++i)
                    {
                        // select proper aura for format aura type in spell proto
                        if ((*i)->GetTarget() == totem && (*i)->GetSpellEffect()->EffectApplyAuraName == SPELL_AURA_MOD_HEALING_DONE &&
                            (*i)->GetSpellProto()->GetSpellFamilyName() == SPELLFAMILY_SHAMAN && (*i)->GetSpellProto()->GetSpellFamilyFlags().test<CF_SHAMAN_MISC_TOTEM_EFFECTS>())
                        {
                            basepoints[0] = triggerAmount * (*i)->GetModifier()->m_amount / 100;
                            break;
                        }
                    }

                    if (!basepoints[0])
                        return SPELL_AURA_PROC_FAILED;

                    basepoints[1] = basepoints[0];
                    triggered_spell_id = 63283;             // Totem of Wrath, caster bonus
                    target = this;
                    break;
                }
                // Item - Shaman T8 Elemental 4P Bonus
                case 64928:
                {
                    triggered_spell_id = 64930;            // Electrified
                    basepoints[0] = int32(triggerAmount * damage / 100) / GetSpellAuraMaxTicks(triggered_spell_id);
                    break;
                }
                // Item - Shaman T9 Elemental 4P Bonus (Lava Burst)
                case 67228:
                {
                    triggered_spell_id = 71824;             // Lava Burst
                    basepoints[0] = int32(triggerAmount * damage / 100) / GetSpellAuraMaxTicks(triggered_spell_id);
                    break;
                }
                // Item - Shaman T10 Restoration 4P Bonus
                case 70808:
                {
                    triggered_spell_id = 70809;             // Chained Heal
                    basepoints[0] = int32(triggerAmount * damage / 100) / GetSpellAuraMaxTicks(triggered_spell_id);
                    break;
                }
                // Item - Shaman T10 Elemental 2P Bonus
                case 70811:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return SPELL_AURA_PROC_FAILED;

                    if (procSpell->GetSpellFamilyFlags().test<CF_SHAMAN_LIGHTNING_BOLT, CF_SHAMAN_CHAIN_LIGHTNING>())
                    {
                        ((Player*)this)->SendModifyCooldown(16166,-triggerAmount);
                        return SPELL_AURA_PROC_OK;
                    }
                    return SPELL_AURA_PROC_FAILED;
                }
                // Item - Shaman T10 Elemental 4P Bonus
                case 70817:
                {
                    Aura* aura = pVictim->GetAura<SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_SHAMAN, CF_SHAMAN_FLAME_SHOCK>(GetObjectGuid());
                    if (aura)
                    {
                        SpellAuraHolderPtr holder = aura->GetHolder();
                        if (holder)
                        {
                            int32 duration = aura->GetAuraDuration() + triggerAmount * IN_MILLISECONDS;
                            holder->SetAuraMaxDuration(duration);
                            holder->RefreshHolder();
                            return SPELL_AURA_PROC_OK;
                        }
                    }
                    return SPELL_AURA_PROC_FAILED;
                }
                // Cleansing Waters (Rank 1)
                case 86959:
                {
                    triggered_spell_id = 86961;
                    break;
                }
                // Cleansing Waters (Rank 2)
                case 86962:
                {
                    triggered_spell_id = 86958;
                    break;
                }
                // Fulmination marker
                case 95774:
                {
                    // Earth Shock
                    if (!procSpell || procSpell->Id != 8042)
                        return SPELL_AURA_PROC_FAILED;

                    // Fulmination dmg spell
                    SpellEntry const * triggeredInfo = sSpellStore.LookupEntry(88767);
                    if (!triggeredInfo)
                        return SPELL_AURA_PROC_OK;

                    int32 minCharges = triggeredInfo->CalculateSimpleValue(EFFECT_INDEX_0);

                    // Lightning Shield
                    SpellAuraHolderPtr ls = GetSpellAuraHolder(324);
                    int32 charges = ls ? int32(ls->GetAuraCharges()) : 0;
                    if (!ls || charges <= minCharges)
                        return SPELL_AURA_PROC_OK;

                    SpellEffectEntry const * shieldDmgEff = ls->GetSpellProto()->GetSpellEffect(EFFECT_INDEX_0);
                    if (!shieldDmgEff)
                        return SPELL_AURA_PROC_OK;

                    SpellEntry const * shieldDmgEntry = sSpellStore.LookupEntry(shieldDmgEff->EffectTriggerSpell);
                    if (!shieldDmgEntry)
                        return SPELL_AURA_PROC_OK;

                    int32 bp = CalculateSpellDamage(pVictim, shieldDmgEntry, EFFECT_INDEX_0) * (charges - minCharges);
                    CastCustomSpell(pVictim, triggeredInfo, &bp, NULL, NULL, true);
                    ls->SetAuraCharges(minCharges);
                    return SPELL_AURA_PROC_OK;
                }
            }
            // Earth's Grasp
            if (dummySpell->SpellIconID == 20)
            {
                // Earthbind Totem summon only
                if (!procSpell || procSpell->Id != 2484)
                    return SPELL_AURA_PROC_FAILED;

                if (!roll_chance_i(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;

                triggered_spell_id = 64695;
                break;
            }
            // Focused Insight
            if (dummySpell->SpellIconID == 4674)
            {
                if (effIndex != EFFECT_INDEX_0)
                    return SPELL_AURA_PROC_FAILED;

                int32 powerCost = 0;
                if (Spell* spell = GetCurrentSpell(CURRENT_GENERIC_SPELL))
                    powerCost = spell->GetPowerCost();

                basepoints[0] = -int32(dummySpell->CalculateSimpleValue(EFFECT_INDEX_0) * powerCost / 100.0f);
                basepoints[1] = dummySpell->CalculateSimpleValue(EFFECT_INDEX_1);
                CastCustomSpell(this, 77800, &basepoints[0], &basepoints[1], &basepoints[1], true);
                CastCustomSpell(this, 96300, &basepoints[1], NULL, NULL, true);
                return SPELL_AURA_PROC_OK;
            }
            // Ancestral Awakening
            if (dummySpell->GetSpellIconID() == 3065)
            {
                triggered_spell_id = 52759;
                basepoints[0] = triggerAmount * damage / 100;
                target = this;
                break;
            }
            // Telluric Currents
            if (dummySpell->SpellIconID == 320)
            {
                triggered_spell_id = 82987;
                basepoints[0] = int32(damage * triggerAmount / 100);
                target = this;
                break;
            }
            // Tidal Waves
            if (dummySpell->SpellIconID == 3057)
            {
                triggered_spell_id = 53390;
                basepoints[0] = triggerAmount;
                basepoints[1] = triggerAmount;
                target = this;
                break;
            }
            // Resurgence
            if (dummySpell->SpellIconID == 2287)
            {
                if (!procSpell || !HasAura(52127))  // do not proc if no Water Shield aura present
                    return SPELL_AURA_PROC_FAILED;

                float mod = 1.0f;
                switch (procSpell->Id)
                {
                    case 331:   // Healing Wave
                    case 77472: // Greater Healing Wave
                        mod = 1.0f;
                        break;
                    case 8004:  // Healing Surge
                    case 61295: // Riptide
                    case 73685: // Unleash Life
                        mod = 0.6f;
                        break;
                    case 1064:  // Chain Heal
                        mod = 0.333f;
                        break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }

                triggered_spell_id = 101033;
                SpellEntry const * triggeredInfo = sSpellStore.LookupEntry(triggered_spell_id);
                if (!triggeredInfo)
                    return SPELL_AURA_PROC_FAILED;

                target = this;
                basepoints[0] = int32(CalculateSpellDamage(target, triggeredInfo, EFFECT_INDEX_0) * mod * triggerAmount / 100.0f);
                break;
            }
            // Flametongue Weapon (Passive), Ranks
            if (dummySpell->GetSpellFamilyFlags().test<CF_SHAMAN_FLAMETONGUE_WEAPON>())
            {
                if (GetTypeId()!=TYPEID_PLAYER || !castItem)
                    return SPELL_AURA_PROC_FAILED;

                // Only proc for enchanted weapon
                Item *usedWeapon = ((Player *)this)->GetWeaponForAttack(procFlag & PROC_FLAG_SUCCESSFUL_OFFHAND_HIT ? OFF_ATTACK : BASE_ATTACK, true, true);
                if (usedWeapon != castItem)
                    return SPELL_AURA_PROC_FAILED;

                switch (dummySpell->Id)
                {
                    case 10400: triggered_spell_id =  8026; break; // Rank 1
                    case 15567: triggered_spell_id =  8028; break; // Rank 2
                    case 15568: triggered_spell_id =  8029; break; // Rank 3
                    case 15569: triggered_spell_id = 10445; break; // Rank 4
                    case 16311: triggered_spell_id = 16343; break; // Rank 5
                    case 16312: triggered_spell_id = 16344; break; // Rank 6
                    case 16313: triggered_spell_id = 25488; break; // Rank 7
                    case 58784: triggered_spell_id = 58786; break; // Rank 8
                    case 58791: triggered_spell_id = 58787; break; // Rank 9
                    case 58792: triggered_spell_id = 58788; break; // Rank 10
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
                break;
            }
            // Earth Shield
            if (dummySpell->GetSpellFamilyFlags().test<CF_SHAMAN_EARTH_SHIELD>())
            {
                originalCaster = triggeredByAura->GetCasterGuid();
                target = this;
                basepoints[0] = triggerAmount;

                // Glyph of Earth Shield
                if(Unit* caster = triggeredByAura->GetCaster())
                {
                    if (Aura const* aur = caster->GetDummyAura(63279))
                    {
                        int32 aur_mod = aur->GetModifier()->m_amount;
                        basepoints[0] = int32(basepoints[0] * (aur_mod + 100.0f) / 100.0f);
                    }
                }

                triggered_spell_id = 379;
                break;
            }
            // Improved Water Shield
            if (dummySpell->GetSpellIconID() == 2287)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                // Lesser Healing Wave need aditional 60% roll
                if (procSpell->GetSpellFamilyFlags().test<CF_SHAMAN_LESSER_HEALING_WAVE>() && !roll_chance_i(60))
                    return SPELL_AURA_PROC_FAILED;
                // Chain Heal needs additional 30% roll
                if (procSpell->GetSpellFamilyFlags().test<CF_SHAMAN_CHAIN_HEAL>() && !roll_chance_i(30))
                    return SPELL_AURA_PROC_FAILED;
                // lookup water shield
                AuraList const& vs = GetAurasByType(SPELL_AURA_PROC_TRIGGER_SPELL);
                for(AuraList::const_iterator itr = vs.begin(); itr != vs.end(); ++itr)
                {
                    if ((*itr)->GetSpellProto()->GetSpellFamilyName() == SPELLFAMILY_SHAMAN &&
                        (*itr)->GetSpellProto()->GetSpellFamilyFlags().test<CF_SHAMAN_WATER_SHIELD>())
                    {
                        SpellEffectEntry const* itrSpellEffect = (*itr)->GetSpellProto()->GetSpellEffect((*itr)->GetEffIndex());
                        uint32 spell = itrSpellEffect ? itrSpellEffect->EffectTriggerSpell : 0;
                        CastSpell(this, spell, true, castItem, triggeredByAura);
                        return SPELL_AURA_PROC_OK;
                    }
                }
                return SPELL_AURA_PROC_FAILED;
            }
            // Flametongue Weapon (Passive)
            if (dummySpell->IsFitToFamilyMask(UI64LIT(0x200000)))
            {
                if(GetTypeId()!=TYPEID_PLAYER)
                    return SPELL_AURA_PROC_FAILED;

                if(!castItem || !castItem->IsEquipped())
                    return SPELL_AURA_PROC_FAILED;

                //  firehit =  dummySpell->EffectBasePoints[0] / ((4*19.25) * 1.3);
                SpellEffectEntry const * effect = dummySpell->GetSpellEffect(EFFECT_INDEX_0);
                float fire_onhit = effect ? effect->EffectBasePoints / 100.0 : 0.0f;

                float add_spellpower = SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE)
                                     + pVictim->SpellBaseDamageBonusTaken(SPELL_SCHOOL_MASK_FIRE);

                // 1.3speed = 5%, 2.6speed = 10%, 4.0 speed = 15%, so, 1.0speed = 3.84%
                add_spellpower= add_spellpower / 100.0 * 3.84;

                // Enchant on Off-Hand and ready?
                if ( castItem->GetSlot() == EQUIPMENT_SLOT_OFFHAND && isAttackReady(OFF_ATTACK))
                {
                    float BaseWeaponSpeed = GetAttackTime(OFF_ATTACK)/1000.0;

                    // Value1: add the tooltip damage by swingspeed + Value2: add spelldmg by swingspeed
                    basepoints[EFFECT_INDEX_0] = int32( (fire_onhit * BaseWeaponSpeed) + (add_spellpower * BaseWeaponSpeed) );
                    triggered_spell_id = 10444;
                }

                // Enchant on Main-Hand and ready?
                else if ( castItem->GetSlot() == EQUIPMENT_SLOT_MAINHAND && isAttackReady(BASE_ATTACK))
                {
                    float BaseWeaponSpeed = GetAttackTime(BASE_ATTACK)/1000.0;

                    // Value1: add the tooltip damage by swingspeed +  Value2: add spelldmg by swingspeed
                    basepoints[EFFECT_INDEX_0] = int32( (fire_onhit * BaseWeaponSpeed) + (add_spellpower * BaseWeaponSpeed) );
                    triggered_spell_id = 10444;
                }

                // If not ready, we should  return, shouldn't we?!
                else
                    return SPELL_AURA_PROC_FAILED;

                CastCustomSpell(pVictim,triggered_spell_id,&basepoints[EFFECT_INDEX_0],NULL,NULL,true,castItem,triggeredByAura);
                return SPELL_AURA_PROC_OK;
            }
            // Lightning Overload
            if (dummySpell->GetSpellIconID() == 2018)            // only this spell have SpellFamily Shaman SpellIconID == 2018 and dummy aura
            {
                if(!procSpell || GetTypeId() != TYPEID_PLAYER || !pVictim || !roll_chance_i(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;

                uint32 spellId = 0;
                // Every Lightning Bolt and Chain Lightning spell have duplicate vs half damage and zero cost
                switch (procSpell->Id)
                {
                    // Lightning Bolt
                    case   403: spellId = 45284; break;
                    // Chain Lightning
                    case   421: spellId = 45297; break;
                    default:
                        sLog.outError("Unit::HandleDummyAuraProc: non handled spell id: %u (LO)", procSpell->Id);
                        return SPELL_AURA_PROC_FAILED;
                }

                // Remove cooldown (Chain Lightning - have Category Recovery time)
                if (procSpell->GetSpellFamilyFlags().test<CF_SHAMAN_CHAIN_LIGHTNING>())
                    RemoveSpellCooldown(spellId);

                CastSpell(pVictim, spellId, true, castItem, triggeredByAura);

                if (cooldown)
                    AddSpellCooldown(dummySpell->Id, 0, time(NULL) + cooldown);

                return SPELL_AURA_PROC_OK;
            }
            // Static Shock
            if (dummySpell->GetSpellIconID() == 3059)
            {
                // lookup Lightning Shield
                if (SpellAuraHolderPtr shield = GetSpellAuraHolder(324, GetObjectGuid()))
                {
                    CastSpell(target, 26364, true, castItem, triggeredByAura);
                    return SPELL_AURA_PROC_OK;
                }
                return SPELL_AURA_PROC_FAILED;
            }
            // Frozen Power
            if (dummySpell->GetSpellIconID() == 3780)
            {
                Unit *caster = triggeredByAura->GetCaster();

                if (!procSpell || !caster)
                    return SPELL_AURA_PROC_FAILED;

                float distance = caster->GetDistance(pVictim);
                int32 chance = triggerAmount;

                if (distance < 15.0f || !roll_chance_i(chance))
                    return SPELL_AURA_PROC_FAILED;

                // make triggered cast apply after current damage spell processing for prevent remove by it
                if (Spell* spell = GetCurrentSpell(CURRENT_GENERIC_SPELL))
                    spell->AddTriggeredSpell(63685);
                return SPELL_AURA_PROC_OK;
            }
            // Feedback
            if (dummySpell->SpellIconID == 4628)
            {
                if (GetTypeId() != TYPEID_PLAYER)
                    return SPELL_AURA_PROC_FAILED;

                ((Player*)this)->SendModifyCooldown(16166, triggerAmount);
                return SPELL_AURA_PROC_OK;
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            // Butchery
            if (dummySpell->GetSpellIconID() == 2664)
            {
                basepoints[0] = triggerAmount;
                triggered_spell_id = 50163;
                target = this;
                break;
            }
            // Dancing Rune Weapon
            if (dummySpell->Id == 49028)
            {
                // 1 dummy aura for dismiss rune blade
                if (effIndex != EFFECT_INDEX_1)
                    return SPELL_AURA_PROC_FAILED;

                Pet* runeBlade = FindGuardianWithEntry(27893);

                if (runeBlade && pVictim && damage && procSpell)
                {
                    int32 procDmg = damage * 0.5;
                    runeBlade->CastCustomSpell(pVictim, procSpell->Id, &procDmg, NULL, NULL, true, NULL, NULL, runeBlade->GetObjectGuid());
                    SendSpellNonMeleeDamageLog(pVictim, procSpell->Id, procDmg, SPELL_SCHOOL_MASK_NORMAL, 0, 0, false, 0, false);
                    break;
                }
                else
                    return SPELL_AURA_PROC_FAILED;
            }
            // Mark of Blood
            if (dummySpell->Id == 49005)
            {
                if (!target || target->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_AURA_PROC_FAILED;
                // TODO: need more info (cooldowns/PPM)
                target->CastSpell(target, 61607, true, NULL, triggeredByAura);
                return SPELL_AURA_PROC_OK;
            }
            // Unholy Blight
            if (dummySpell->Id == 49194)
            {
                basepoints[0] = damage * triggerAmount / 100;

                // Glyph of Unholy Blight
                if (Aura const* aura = GetDummyAura(63332))
                    basepoints[0] += basepoints[0] * aura->GetModifier()->m_amount / 100;

                // Split between 10 ticks
                basepoints[0] /= 10;
                triggered_spell_id = 50536;
                break;
            }
            // Vendetta
            if (dummySpell->GetSpellFamilyFlags().test<CF_DEATHKNIGHT_VENDETTA1>())
            {
                basepoints[0] = triggerAmount * GetMaxHealth() / 100;
                triggered_spell_id = 50181;
                target = this;
                break;
            }
            // Necrosis
            if (dummySpell->GetSpellIconID() == 2709)
            {
                // only melee auto attack affected and Rune Strike
                if (procSpell && procSpell->Id != 56815)
                    return SPELL_AURA_PROC_FAILED;

                basepoints[0] = triggerAmount * damage / 100;
                triggered_spell_id = 51460;
                break;
            }
            // Threat of Thassarian
            if (dummySpell->GetSpellIconID() == 2023)
            {
                // Must Dual Wield
                if (!procSpell || !haveOffhandWeapon())
                    return SPELL_AURA_PROC_FAILED;
                // Chance as basepoints for dummy aura
                if (!roll_chance_i(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;

                switch (procSpell->Id)
                {
                    // Obliterate
                    case 49020:                             // Rank 1
                        triggered_spell_id = 66198; break;
                    case 51423:                             // Rank 2
                        triggered_spell_id = 66972; break;
                    case 51424:                             // Rank 3
                        triggered_spell_id = 66973; break;
                    case 51425:                             // Rank 4
                        triggered_spell_id = 66974; break;
                    // Frost Strike
                    case 49143:                             // Rank 1
                        triggered_spell_id = 66196; break;
                    case 51416:                             // Rank 2
                        triggered_spell_id = 66958; break;
                    case 51417:                             // Rank 3
                        triggered_spell_id = 66959; break;
                    case 51418:                             // Rank 4
                        triggered_spell_id = 66960; break;
                    case 51419:                             // Rank 5
                        triggered_spell_id = 66961; break;
                    case 55268:                             // Rank 6
                        triggered_spell_id = 66962; break;
                    // Plague Strike
                    case 45462:                             // Rank 1
                        triggered_spell_id = 66216; break;
                    case 49917:                             // Rank 2
                        triggered_spell_id = 66988; break;
                    case 49918:                             // Rank 3
                        triggered_spell_id = 66989; break;
                    case 49919:                             // Rank 4
                        triggered_spell_id = 66990; break;
                    case 49920:                             // Rank 5
                        triggered_spell_id = 66991; break;
                    case 49921:                             // Rank 6
                        triggered_spell_id = 66992; break;
                    // Death Strike
                    case 49998:                             // Rank 1
                        triggered_spell_id = 66188; break;
                    case 49999:                             // Rank 2
                        triggered_spell_id = 66950; break;
                    case 45463:                             // Rank 3
                        triggered_spell_id = 66951; break;
                    case 49923:                             // Rank 4
                        triggered_spell_id = 66952; break;
                    case 49924:                             // Rank 5
                        triggered_spell_id = 66953; break;
                    // Rune Strike
                    case 56815:
                        triggered_spell_id = 66217; break;
                    // Blood Strike
                    case 45902:                             // Rank 1
                        triggered_spell_id = 66215; break;
                    case 49926:                             // Rank 2
                        triggered_spell_id = 66975; break;
                    case 49927:                             // Rank 3
                        triggered_spell_id = 66976; break;
                    case 49928:                             // Rank 4
                        triggered_spell_id = 66977; break;
                    case 49929:                             // Rank 5
                        triggered_spell_id = 66978; break;
                    case 49930:                             // Rank 6
                        triggered_spell_id = 66979; break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
                break;
            }
            // Runic Power Back on Snare/Root
            if (dummySpell->Id == 61257)
            {
                // only for spells and hit/crit (trigger start always) and not start from self casted spells
                if (procSpell == 0 || !(procEx & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) || this == pVictim)
                    return SPELL_AURA_PROC_FAILED;
                // Need snare or root mechanic
                if (!(GetAllSpellMechanicMask(procSpell) & IMMUNE_TO_ROOT_AND_SNARE_MASK))
                    return SPELL_AURA_PROC_FAILED;
                triggered_spell_id = 61258;
                target = this;
                break;
            }
            // Sudden Doom
            if (dummySpell->GetSpellIconID() == 1939)
            {
                if (!target || !target->isAlive() || this->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_AURA_PROC_FAILED;

                // get highest rank of Death Coil spell
                const PlayerSpellMap& sp_list = ((Player*)this)->GetSpellMap();
                for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                {
                    if(!itr->second.active || itr->second.disabled || itr->second.state == PLAYERSPELL_REMOVED)
                        continue;

                    SpellEntry const *spellInfo = sSpellStore.LookupEntry(itr->first);
                    if (!spellInfo)
                        continue;

                    if (spellInfo->GetSpellFamilyName() == SPELLFAMILY_DEATHKNIGHT && spellInfo->GetSpellFamilyFlags().test<CF_DEATHKNIGHT_DEATH_COIL>())
                    {
                        triggered_spell_id = spellInfo->Id;
                        break;
                    }
                }
                break;
            }
            // Wandering Plague
            if (dummySpell->GetSpellIconID() == 1614)
            {
                if (!roll_chance_f(GetUnitCriticalChance(BASE_ATTACK, pVictim)))
                    return SPELL_AURA_PROC_FAILED;
                basepoints[0] = triggerAmount * damage / 100;
                triggered_spell_id = 50526;
                break;
            }
            // Blood of the North and Reaping
            if (dummySpell->GetSpellIconID() == 3041 || dummySpell->GetSpellIconID() == 22)
            {
                if (GetTypeId()!=TYPEID_PLAYER)
                    return SPELL_AURA_PROC_FAILED;

                Player *player = (Player*)this;
                for (uint32 i = 0; i < MAX_RUNES; ++i)
                {
                    if (player->GetCurrentRune(i) == RUNE_BLOOD)
                    {
                        if(!player->GetRuneCooldown(i))
                            player->ConvertRune(i, RUNE_DEATH, dummySpell->Id);
                        else
                        {
                            // search for another rune that might be available
                            for (uint32 iter = i; iter < MAX_RUNES; ++iter)
                            {
                                if (player->GetCurrentRune(iter) == RUNE_BLOOD && !player->GetRuneCooldown(iter))
                                {
                                    player->ConvertRune(iter, RUNE_DEATH, dummySpell->Id);
                                    const_cast<Aura*>(triggeredByAura)->SetAuraPeriodicTimer(0);
                                    return SPELL_AURA_PROC_OK;
                                }
                            }
                            player->SetNeedConvertRune(i, true, dummySpell->Id);
                        }
                        const_cast<Aura*>(triggeredByAura)->SetAuraPeriodicTimer(0);
                        return SPELL_AURA_PROC_OK;
                    }
                }
                return SPELL_AURA_PROC_FAILED;
            }
            // Death Rune Mastery
            if (dummySpell->GetSpellIconID() == 2622)
            {
                if (GetTypeId()!=TYPEID_PLAYER)
                    return SPELL_AURA_PROC_FAILED;

                Player *player = (Player*)this;
                for (uint32 i = 0; i < MAX_RUNES; ++i)
                {
                    RuneType currRune = player->GetCurrentRune(i);
                    if (currRune == RUNE_UNHOLY || currRune == RUNE_FROST)
                    {
                        uint16 cd = player->GetRuneCooldown(i);
                        if(!cd)
                            player->ConvertRune(i, RUNE_DEATH, dummySpell->Id);
                        else // there is a cd
                            player->SetNeedConvertRune(i, true, dummySpell->Id);
                        // no break because it converts all
                    }
                }
                const_cast<Aura*>(triggeredByAura)->SetAuraPeriodicTimer(0);
                return SPELL_AURA_PROC_OK;
            }
            // Blood-Caked Blade
            if (dummySpell->GetSpellIconID() == 138)
            {
                // only melee auto attack affected and Rune Strike & Offhand Rune Strike
                if ( procSpell && !procSpell->GetSpellFamilyFlags().test<CF_DEATHKNIGHT_RUNE_STRIKE>())
                    return SPELL_AURA_PROC_FAILED;
                if (procFlag & PROC_FLAG_SUCCESSFUL_OFFHAND_HIT)
                    triggered_spell_id=61895; // Offhand Blood-Caked Strike
                break;
            }
            // Hungering Cold - not proc from dummy
            if (dummySpell->GetSpellIconID() == 2797)
            {
                return SPELL_AURA_PROC_CANT_TRIGGER;
            }
            break;
        }
        case SPELLFAMILY_PET:
        {
            switch (dummySpell->GetSpellIconID())
            {
                // Guard Dog
                case 201:
                {
                    triggered_spell_id = 54445;
                    target = this;
                    if (pVictim)
                        pVictim->AddThreat(this,procSpell->CalculateSimpleValue(EFFECT_INDEX_0) * triggerAmount / 100.0f);
                    break;
                }
                // Silverback
                case 1582:
                    triggered_spell_id = dummySpell->Id == 62765 ? 62801 : 62800;
                    target = this;
                    break;
            }
            break;
        }
        default:
            break;
    }

    if (!triggered_spell_id)
    {
        // Linked spells (Proc chain)
        SpellLinkedSet linkedSet = sSpellMgr.GetSpellLinked(dummySpell->Id, SPELL_LINKED_TYPE_PROC);
        if (linkedSet.size() > 0)
        {
            for (SpellLinkedSet::const_iterator itr = linkedSet.begin(); itr != linkedSet.end(); ++itr)
            {
                if (target == NULL)
                    target = !(procFlag & PROC_FLAG_SUCCESSFUL_POSITIVE_SPELL) && IsPositiveSpell(*itr) ? this : pVictim;
                CastSpell(target, *itr, true, castItem, triggeredByAura);
                if (cooldown)
                    AddSpellCooldown(*itr,0,time(NULL) + cooldown);
            }
        }
    }

    // processed charge only counting case
    if (!triggered_spell_id)
        return SPELL_AURA_PROC_OK;

    SpellEntry const* triggerEntry = sSpellStore.LookupEntry(triggered_spell_id);

    if (!triggerEntry)
    {
        sLog.outError("Unit::HandleDummyAuraProc: Spell %u have nonexistent triggered spell %u",dummySpell->Id,triggered_spell_id);
        return SPELL_AURA_PROC_FAILED;
    }

    // default case
    if (!target || (target != this && !target->isAlive()))
        return SPELL_AURA_PROC_FAILED;

    if (cooldown && HasSpellCooldown(triggered_spell_id))
        return SPELL_AURA_PROC_FAILED;

    if (basepoints[EFFECT_INDEX_0] || basepoints[EFFECT_INDEX_1] || basepoints[EFFECT_INDEX_2])
        CastCustomSpell(target, triggerEntry,
            basepoints[EFFECT_INDEX_0] ? &basepoints[EFFECT_INDEX_0] : NULL,
            basepoints[EFFECT_INDEX_1] ? &basepoints[EFFECT_INDEX_1] : NULL,
            basepoints[EFFECT_INDEX_2] ? &basepoints[EFFECT_INDEX_2] : NULL,
            true, castItem, triggeredByAura, originalCaster);
    else
        CastSpell(target, triggerEntry, true, castItem, triggeredByAura);

    if (cooldown)
        AddSpellCooldown(triggered_spell_id,0,time(NULL) + cooldown);

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleProcTriggerSpellAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlags, uint32 procEx, uint32 cooldown)
{
    // Get triggered aura spell info
    SpellEntry const* auraSpellInfo = triggeredByAura->GetSpellProto();
    SpellClassOptionsEntry const* auraClassOptions = auraSpellInfo->GetSpellClassOptions();
    SpellClassOptionsEntry const* procClassOptions = procSpell ? procSpell->GetSpellClassOptions() : NULL;

    // Basepoints of trigger aura
    int32 triggerAmount = triggeredByAura->GetModifier()->m_amount;
    uint32 damage = damageInfo->damage;

    // Set trigger spell id, target, custom basepoints
    SpellEffectEntry const* spellEffect = auraSpellInfo->GetSpellEffect(triggeredByAura->GetEffIndex());
    uint32 trigger_spell_id = spellEffect ? spellEffect->EffectTriggerSpell : 0;
    Unit*  target = NULL;
    int32  basepoints[MAX_EFFECT_INDEX] = {0, 0, 0};

    if (triggeredByAura->GetModifier()->m_auraname == SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE)
        basepoints[0] = triggerAmount;

    Item* castItem = triggeredByAura->GetCastItemGuid() && GetTypeId()==TYPEID_PLAYER
        ? ((Player*)this)->GetItemByGuid(triggeredByAura->GetCastItemGuid()) : NULL;

    // Try handle unknown trigger spells
    // Custom requirements (not listed in procEx) Warning! damage dealing after this
    // Custom triggered spells
    switch (auraSpellInfo->GetSpellFamilyName())
    {
        case SPELLFAMILY_GENERIC:
            switch(auraSpellInfo->Id)
            {
                //case 191:                               // Elemental Response
                //    switch (procSpell->School)
                //    {
                //        case SPELL_SCHOOL_FIRE:  trigger_spell_id = 34192; break;
                //        case SPELL_SCHOOL_FROST: trigger_spell_id = 34193; break;
                //        case SPELL_SCHOOL_ARCANE:trigger_spell_id = 34194; break;
                //        case SPELL_SCHOOL_NATURE:trigger_spell_id = 34195; break;
                //        case SPELL_SCHOOL_SHADOW:trigger_spell_id = 34196; break;
                //        case SPELL_SCHOOL_HOLY:  trigger_spell_id = 34197; break;
                //        case SPELL_SCHOOL_NORMAL:trigger_spell_id = 34198; break;
                //    }
                //    break;
                //case 5301:  break;                        // Defensive State (DND)
                //case 7137:  break:                        // Shadow Charge (Rank 1)
                //case 7377:  break:                        // Take Immune Periodic Damage <Not Working>
                case 12298:                                 // Shield Specialization (Rank 1)
                case 12724:                                 // Shield Specialization (Rank 2)
                case 12725:                                 // Shield Specialization (Rank 3)
                {
                    if (procEx & PROC_EX_REFLECT)
                        if (SpellEntry const * spellInfo = sSpellStore.LookupEntry(trigger_spell_id))
                            basepoints[0] = spellInfo->CalculateSimpleValue(EFFECT_INDEX_0) * 4;
                    break;
                }
                //case 13358: break;                        // Defensive State (DND)
                //case 16092: break;                        // Defensive State (DND)
                //case 18943: break;                        // Double Attack
                //case 19194: break;                        // Double Attack
                //case 19817: break;                        // Double Attack
                //case 19818: break;                        // Double Attack
                //case 22835: break;                        // Drunken Rage
                //    trigger_spell_id = 14822; break;
                case 23780:                                 // Aegis of Preservation (Aegis of Preservation trinket)
                    trigger_spell_id = 23781;
                    break;
                case 25988:                                 // Eye for an Eye, Rank 2
                {
                    // return damage % to attacker but < 50% own total health
                    basepoints[0] = triggerAmount * int32(damage) / 100;
                    if (basepoints[0] > int32(GetMaxHealth() / 2))
                        basepoints[0] = int32(GetMaxHealth() / 2);

                    trigger_spell_id = 25997;
                    break;
                }
                //case 24949: break;                        // Defensive State 2 (DND)
                case 27522:                                 // Mana Drain Trigger
                case 40336:                                 // Mana Drain Trigger
                case 46939:                                 // Black Bow of the Betrayer
                    // On successful melee or ranged attack gain 8 mana and if possible drain 8 mana from the target.
                    if (isAlive())
                        CastSpell(this, 29471, true, castItem, triggeredByAura);
                    if (pVictim && pVictim->isAlive())
                        CastSpell(pVictim, 27526, true, castItem, triggeredByAura);
                    return SPELL_AURA_PROC_OK;
                case 31255:                                 // Deadly Swiftness (Rank 1)
                {
                    // whenever you deal damage to a target who is below 20% health.
                    if (pVictim->GetHealth() > pVictim->GetMaxHealth() / 5)
                        return SPELL_AURA_PROC_FAILED;

                    target = this;
                    trigger_spell_id = 22588;
                    break;
                }
                //case 33207: break;                        // Gossip NPC Periodic - Fidget
                case 33896:                                 // Desperate Defense (Stonescythe Whelp, Stonescythe Alpha, Stonescythe Ambusher)
                    trigger_spell_id = 33898;
                    break;
                //case 34082: break;                        // Advantaged State (DND)
                //case 34783: break:                        // Spell Reflection
                //case 35205: break:                        // Vanish
                //case 35321: break;                        // Gushing Wound
                //case 36096: break:                        // Spell Reflection
                //case 36207: break:                        // Steal Weapon
                //case 36576: break:                        // Shaleskin (Shaleskin Flayer, Shaleskin Ripper) 30023 trigger
                //case 37030: break;                        // Chaotic Temperament
                case 38164:                                 // Unyielding Knights
                    if (pVictim->GetEntry() != 19457)
                        return SPELL_AURA_PROC_FAILED;
                    break;
                //case 38363: break;                        // Gushing Wound
                //case 39215: break;                        // Gushing Wound
                //case 40250: break;                        // Improved Duration
                //case 40329: break;                        // Demo Shout Sensor
                //case 40364: break;                        // Entangling Roots Sensor
                //case 41054: break;                        // Copy Weapon
                //    trigger_spell_id = 41055; break;
                //case 41248: break;                        // Consuming Strikes
                //    trigger_spell_id = 41249; break;
                //case 42730: break:                        // Woe Strike
                //case 43453: break:                        // Rune Ward
                //case 43504: break;                        // Alterac Valley OnKill Proc Aura
                //case 44326: break:                        // Pure Energy Passive
                //case 44526: break;                        // Hate Monster (Spar) (30 sec)
                //case 44527: break;                        // Hate Monster (Spar Buddy) (30 sec)
                //case 44819: break;                        // Hate Monster (Spar Buddy) (>30% Health)
                //case 44820: break;                        // Hate Monster (Spar) (<30%)
                case 45057:                                 // Evasive Maneuvers (Commendation of Kael`thas trinket)
                case 52420:                                 // Deflection        ( Soul Harvester's Charm )
                case 71634:                                 // Item - Icecrown 25 Normal Tank Trinket 1
                case 71640:                                 // Item - Icecrown 25 Heroic Tank Trinket 1
                case 75475:                                 // Item - Chamber of Aspects 25 Tank Trinket
                case 75481:                                 // Item - Chamber of Aspects 25 Heroic Tank Trinket
                    // reduce you below $s1% health (in fact in this specific case can proc from any attack while health in result less $s1%)
                    if (int32(GetHealth()) - int32(damage) >= int32(GetMaxHealth() * triggerAmount / 100))
                        return SPELL_AURA_PROC_FAILED;
                    break;
                //case 45903: break:                        // Offensive State
                //case 46146: break:                        // [PH] Ahune  Spanky Hands
                case 43820:                                 // Charm of the Witch Doctor (Amani Charm of the Witch Doctor trinket)
                    // Pct value stored in dummy
                    basepoints[0] = pVictim->GetCreateHealth() * auraSpellInfo->CalculateSimpleValue(EFFECT_INDEX_1) / 100;
                    target = pVictim;
                    break;
                //case 45205: break;                        // Copy Offhand Weapon
                case 45234:                                 // Focused Will
                case 45243:
                {
                    if (damage * 100 >= pVictim->GetMaxHealth() * 10 ||
                        (procEx & PROC_EX_CRITICAL_HIT) != 0 && (procFlags & PROC_FLAG_ON_TAKE_PERIODIC) == 0)
                        break;

                    return SPELL_AURA_PROC_FAILED;
                }
                //case 45343: break;                        // Dark Flame Aura
                //case 47300: break;                        // Dark Flame Aura
                //case 48876: break;                        // Beast's Mark
                //    trigger_spell_id = 48877; break;
                //case 49059: break;                        // Horde, Hate Monster (Spar Buddy) (>30% Health)
                //case 50051: break;                        // Ethereal Pet Aura
                //case 50689: break;                        // Blood Presence (Rank 1)
                case 50720:                                 // Vigilance
                {
                    // proc Vengeance for caster
                    if (Unit* caster = triggeredByAura->GetCaster())
                        caster->HandleVengeanceProc(pVictim, damage * auraSpellInfo->CalculateSimpleValue(EFFECT_INDEX_1) / 100, 5);
                    break;
                }
                //case 50844: break;                        // Blood Mirror
                //case 52856: break;                        // Charge
                //case 54072: break;                        // Knockback Ball Passive
                //case 54476: break;                        // Blood Presence
                //case 54775: break;                        // Abandon Vehicle on Poly
                case 56702:                                 // Shadow Sickle
                {
                    trigger_spell_id = 56701;
                    break;
                }
                case 57345:                                 // Darkmoon Card: Greatness
                {
                    float stat = 0.0f;
                    // strength
                    if (GetStat(STAT_STRENGTH) > stat) { trigger_spell_id = 60229;stat = GetStat(STAT_STRENGTH); }
                    // agility
                    if (GetStat(STAT_AGILITY)  > stat) { trigger_spell_id = 60233;stat = GetStat(STAT_AGILITY);  }
                    // intellect
                    if (GetStat(STAT_INTELLECT)> stat) { trigger_spell_id = 60234;stat = GetStat(STAT_INTELLECT);}
                    // spirit
                    if (GetStat(STAT_SPIRIT)   > stat) { trigger_spell_id = 60235;                               }
                    break;
                }
                //case 55580: break:                        // Mana Link
                //case 57587: break:                        // Steal Ranged ()
                //case 57594: break;                        // Copy Ranged Weapon
                //case 59237: break;                        // Beast's Mark
                //    trigger_spell_id = 59233; break;
                //case 59288: break;                        // Infra-Green Shield
                //case 59532: break;                        // Abandon Passengers on Poly
                //case 59735: break:                        // Woe Strike
                case 64415:                                 // // Val'anyr Hammer of Ancient Kings - Equip Effect
                {
                    // for DOT procs
                    if (!IsPositiveSpell(procSpell->Id))
                        return SPELL_AURA_PROC_FAILED;
                    break;
                }
                case 64440:                                 // Blade Warding
                {
                    trigger_spell_id = 64442;

                    // need scale damage base at stack size
                    if (SpellEntry const* trigEntry = sSpellStore.LookupEntry(trigger_spell_id))
                        basepoints[EFFECT_INDEX_0] = trigEntry->CalculateSimpleValue(EFFECT_INDEX_0) * triggeredByAura->GetStackAmount();

                    break;
                }
                case 64568:                                 // Blood Reserve
                {
                    // When your health drops below 35% ....
                    int32 health35 = int32(GetMaxHealth() * 35 / 100);
                    if (int32(GetHealth()) - int32(damage) >= health35 || int32(GetHealth()) < health35)
                        return SPELL_AURA_PROC_FAILED;

                    trigger_spell_id = 64569;

                    // need scale damage base at stack size
                    if (SpellEntry const* trigEntry = sSpellStore.LookupEntry(trigger_spell_id))
                        basepoints[EFFECT_INDEX_0] = trigEntry->CalculateSimpleValue(EFFECT_INDEX_0) * triggeredByAura->GetStackAmount();

                    break;
                }
                case 67702:                                 // Death's Choice, Item - Coliseum 25 Normal Melee Trinket
                {
                    float stat = 0.0f;
                    // strength
                    if (GetStat(STAT_STRENGTH) > stat) { trigger_spell_id = 67708;stat = GetStat(STAT_STRENGTH); }
                    // agility
                    if (GetStat(STAT_AGILITY)  > stat) { trigger_spell_id = 67703;                               }
                    break;
                }
                case 67771:                                 // Death's Choice (heroic), Item - Coliseum 25 Heroic Melee Trinket
                {
                    float stat = 0.0f;
                    // strength
                    if (GetStat(STAT_STRENGTH) > stat) { trigger_spell_id = 67773;stat = GetStat(STAT_STRENGTH); }
                    // agility
                    if (GetStat(STAT_AGILITY)  > stat) { trigger_spell_id = 67772;                               }
                    break;
                }
                case 69023:                                 // Mirrored Soul
                {
                    int32 basepoints = (int32) (damage * 0.45f);
                    if (Unit* caster = triggeredByAura->GetCaster())
                        // Actually this spell should be sent with SMSG_SPELL_START
                        CastCustomSpell(caster, 69034, &basepoints, NULL, NULL, true, NULL, triggeredByAura, GetObjectGuid());

                    return SPELL_AURA_PROC_OK;
                }
                case 71494:                                 // Vengeful Blast
                {
                    // despawn Vengeful Shade after proc
                    if  (GetTypeId() == TYPEID_UNIT)
                        ((Creature*)this)->ForcedDespawn(1000);
                    break;
                }
                case 72178:                                 // Blood link Saurfang aura
                {
                    target = this;
                    trigger_spell_id = 72195;
                    break;
                }
                case 72256:                                 // Mark of the Fallen Champion
                {
                    if (SpellEntry const* triggerSpell = sSpellStore.LookupEntry(trigger_spell_id))
                    {
                        if (SpellEntry const* difftrigEntry =  GetSpellEntryByDifficulty(triggerSpell->SpellDifficultyId, GetMap()->GetDifficulty(), GetMap()->IsRaid()))
                            triggerSpell = difftrigEntry;
                        basepoints[EFFECT_INDEX_0] = triggerSpell->CalculateSimpleValue(EFFECT_INDEX_0) + damage;
                    }
                    break;
                }
            }
            break;
        case SPELLFAMILY_MAGE:
            if (auraSpellInfo->GetSpellIconID() == 2127)         // Blazing Speed
            {
                switch (auraSpellInfo->Id)
                {
                    case 31641:  // Rank 1
                    case 31642:  // Rank 2
                        trigger_spell_id = 31643;
                        break;
                    default:
                        sLog.outError("Unit::HandleProcTriggerSpellAuraProc: Spell %u miss possibly Blazing Speed",auraSpellInfo->Id);
                        return SPELL_AURA_PROC_FAILED;
                }
            }
            else if (auraSpellInfo->Id == 26467)             // Persistent Shield (Scarab Brooch trinket)
            {
                // This spell originally trigger 13567 - Dummy Trigger (vs dummy effect)
                basepoints[0] = damage * 15 / 100;
                target = pVictim;
                trigger_spell_id = 26470;
            }
            else if (auraSpellInfo->Id == 71761)             // Deep Freeze Immunity State
            {
                // spell applied only to permanent immunes to stun targets (bosses)
                if (pVictim->GetTypeId() != TYPEID_UNIT ||
                    (((Creature*)pVictim)->GetCreatureInfo()->MechanicImmuneMask & (1 << (MECHANIC_STUN - 1))) == 0)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Missile Barrage
            else if (auraSpellInfo->GetSpellIconID() == 3261)
            {
                // proc chance for spells other than Arcane Blast is always 2 times lower, so we have to roll for 50% now
                if (procSpell->GetSpellIconID() != 2294 && !roll_chance_i(50))
                    return SPELL_AURA_PROC_FAILED;

                if (HasAura(44401) || HasAura(57761))
                    return SPELL_AURA_PROC_FAILED;
            }
            // Fingers of Frost
            else if (auraSpellInfo->GetSpellIconID() == 2947)
            {
                // proc chance for spells in basepoints
                if (!roll_chance_i(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;
            }
            break;
        case SPELLFAMILY_WARRIOR:
            // Deep Wounds (replace triggered spells to directly apply DoT), dot spell have familyflags
            if (auraSpellInfo->GetSpellFamilyName() == SPELLFAMILY_WARRIOR && auraSpellInfo->GetSpellIconID() == 243)
            {
                bool bOffHand = procFlags & PROC_FLAG_SUCCESSFUL_OFFHAND_HIT;
                if (bOffHand && !haveOffhandWeapon())
                {
                    sLog.outError("Unit::HandleProcTriggerSpellAuraProc: offhand %u proc without offhand weapon!",auraSpellInfo->Id);
                    return SPELL_AURA_PROC_FAILED;
                }

                float weaponSpeed = GetAttackTime(bOffHand ? OFF_ATTACK : BASE_ATTACK)/1000.0f;
                float weaponDPS   = ((GetFloatValue(bOffHand ? UNIT_FIELD_MINOFFHANDDAMAGE : UNIT_FIELD_MINDAMAGE) +
                                    GetFloatValue(bOffHand ? UNIT_FIELD_MAXOFFHANDDAMAGE : UNIT_FIELD_MAXDAMAGE))/2.0f) / weaponSpeed;
                float attackPower = GetTotalAttackPowerValue(bOffHand ? OFF_ATTACK : BASE_ATTACK);
                float f_damage    = 0.0f;

                switch (auraSpellInfo->Id)
                {
                    case 12834: f_damage = ((weaponDPS + attackPower / 14.0f) * weaponSpeed)* 0.16f; break;
                    case 12849: f_damage = ((weaponDPS + attackPower / 14.0f) * weaponSpeed)* 0.32f; break;
                    case 12867: f_damage = ((weaponDPS + attackPower / 14.0f) * weaponSpeed)* 0.48f; break;
                    // Impossible case
                    default:
                        sLog.outError("Unit::HandleProcTriggerSpellAuraProc: DW unknown spell rank %u",auraSpellInfo->Id);
                        return SPELL_AURA_PROC_FAILED;
                }

                trigger_spell_id = 12721;

                SpellEntry const* triggerspellInfo = sSpellStore.LookupEntry(trigger_spell_id);

                if (!triggerspellInfo || f_damage < M_NULL_F)
                    return SPELL_AURA_PROC_FAILED;

                SpellEffectEntry const* spellEff = triggerspellInfo->GetSpellEffect(EFFECT_INDEX_0);

                uint32 tickcount = GetSpellDuration(triggerspellInfo) / spellEff->EffectAmplitude;

                basepoints[0] = floor(f_damage / tickcount);

                break;
            }
            else if (auraSpellInfo->GetSpellIconID() == 23)      // Improved Hamstring
            {
                // done in other way
                return SPELL_AURA_PROC_FAILED;
            }
            else if (auraSpellInfo->GetSpellIconID() == 1938)    // Rude Interruption
            {
                // done in other way
                return SPELL_AURA_PROC_FAILED;
            }
            else if (auraSpellInfo->GetSpellIconID() == 2053)    // Impending Victory
            {
                if (!pVictim ||!pVictim->HasAuraState(AURA_STATE_HEALTHLESS_20_PERCENT))
                    return SPELL_AURA_PROC_FAILED;
                break;
            }
            else if (auraSpellInfo->GetSpellIconID() == 2841)    // Incite
            {
                if (HasAura(trigger_spell_id))
                    return SPELL_AURA_PROC_FAILED;
                break;
            }
            else if (auraSpellInfo->GetSpellIconID() == 2961)    // Taste for Blood
            {
                // only at real damage
                if (!damage)
                    return SPELL_AURA_PROC_FAILED;
            }
            else if (auraSpellInfo->GetSpellIconID() == 4660)    // Die by the Sword
            {
                if (!HasAuraState(AURA_STATE_HEALTHLESS_20_PERCENT))
                    return SPELL_AURA_PROC_FAILED;
                break;
            }
            else if (auraSpellInfo->GetSpellIconID() == 4978)    // Lambs to the Slaughter
            {
                if (pVictim)
                {
                    // Refresh Rend on target
                    if (SpellAuraHolderPtr rend = pVictim->GetSpellAuraHolder(94009, GetObjectGuid()))
                        rend->RefreshHolder();
                }
            }
            else if (auraSpellInfo->Id == 50421)            // Scent of Blood
                trigger_spell_id = 50422;
            break;
        case SPELLFAMILY_WARLOCK:
        {
            // Drain Soul
            if (auraSpellInfo->GetSpellFamilyFlags().test<CF_WARLOCK_DRAIN_SOUL>())
            {
                // search for "Improved Drain Soul" dummy aura
                Unit::AuraList const& mDummyAura = GetAurasByType(SPELL_AURA_DUMMY);
                for(Unit::AuraList::const_iterator i = mDummyAura.begin(); i != mDummyAura.end(); ++i)
                {
                    if ((*i)->GetSpellProto()->GetSpellFamilyName() == SPELLFAMILY_WARLOCK && (*i)->GetSpellProto()->GetSpellIconID() == 113)
                    {
                        // basepoints of trigger spell stored in dummyeffect of spellProto
                        int32 basepoints = GetMaxPower(POWER_MANA) * (*i)->GetSpellProto()->CalculateSimpleValue(EFFECT_INDEX_2) / 100;
                        CastCustomSpell(this, 18371, &basepoints, NULL, NULL, true, castItem, triggeredByAura);
                        break;
                    }
                }
                // Not remove charge (aura removed on death in any cases)
                // Need for correct work Drain Soul SPELL_AURA_CHANNEL_DEATH_ITEM aura
                return SPELL_AURA_PROC_FAILED;
            }
            // Consume Shadows
            else if (auraSpellInfo->GetSpellFamilyFlags().test<CF_WARLOCK_VOIDWALKER_SPELLS>())
            {
                Aura* heal = triggeredByAura->GetHolder()->GetAuraByEffectIndex(EFFECT_INDEX_0);
                if (!heal || heal->GetAuraTicks() > 1)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Soul Leech
            else if (auraSpellInfo->SpellIconID == 2027)
            {
                basepoints[0] = triggerAmount;
                basepoints[1] = triggerAmount;

                // Replenishment proc
                CastSpell(this, 57669, true, NULL, triggeredByAura);
                break;
            }
            // Cheat Death
            else if (auraSpellInfo->Id == 28845)
            {
                // When your health drops below 20% ....
                int32 health20 = int32(GetMaxHealth()) / 5;
                if (int32(GetHealth()) - int32(damage) >= health20 || int32(GetHealth()) < health20)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Decimation
            else if (auraSpellInfo->Id == 63156 || auraSpellInfo->Id == 63158)
            {
                // If target's health is not below equal certain value (25%) not proc
                if (!pVictim || pVictim->GetHealthPercent() > auraSpellInfo->CalculateSimpleValue(EFFECT_INDEX_1))
                    return SPELL_AURA_PROC_FAILED;
                break;
            }
            // Aftermath
            else if (auraSpellInfo->SpellIconID == 11)
            {
                if (!procSpell || pVictim == this)
                    return SPELL_AURA_PROC_FAILED;

                int32 chance;
                // Conflagrate - take chance and trigger spell from dbc, Rain of Fire - chance from basepoints
                if (procSpell->Id == 5740)
                {
                    chance = triggerAmount;
                    trigger_spell_id = 85387;
                }
                else
                    chance = auraSpellInfo->GetProcChance();

                if (!roll_chance_i(chance))
                    return SPELL_AURA_PROC_FAILED;
                break;
            }
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            // Greater Heal Refund (Avatar Raiment set)
            if (auraSpellInfo->Id==37594)
            {
                // Not give if target already have full health
                if (pVictim->GetHealth() == pVictim->GetMaxHealth())
                    return SPELL_AURA_PROC_FAILED;
                // If your Greater Heal brings the target to full health, you gain $37595s1 mana.
                if (pVictim->GetHealth() + damage < pVictim->GetMaxHealth())
                    return SPELL_AURA_PROC_FAILED;
                trigger_spell_id = 37595;
            }
            // Blessed Recovery
            else if (auraSpellInfo->GetSpellIconID() == 1875)
            {
                switch (auraSpellInfo->Id)
                {
                    case 27811: trigger_spell_id = 27813; break;
                    case 27815: trigger_spell_id = 27817; break;
                    case 27816: trigger_spell_id = 27818; break;
                    default:
                        sLog.outError("Unit::HandleProcTriggerSpellAuraProc: Spell %u not handled in BR", auraSpellInfo->Id);
                    return SPELL_AURA_PROC_FAILED;
                }
                basepoints[0] = damage * triggerAmount / 100 / 3;
                target = this;
                // increase healing factor with each critical strike. Patch 3.0.2
                if (Aura* old_aura = GetAura(trigger_spell_id, EFFECT_INDEX_0))
                    basepoints[0] += old_aura->GetModifier()->m_amount;
            }
            // Glyph of Shadow Word: Pain
            else if (auraSpellInfo->Id == 55681 )
            {
                basepoints[0] = GetCreateMana() * triggerAmount / 100;
            }
            else if (auraSpellInfo->Id == 55689)
            {
                if(GetShapeshiftForm() != FORM_SHADOW)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Improved Spirit Tap
            else if (auraSpellInfo->Id == 15337 || auraSpellInfo->Id == 15338)
            {
                // proc chance for Mind Flay is 2 times lower, so we have to roll for 50% now
                if (procSpell->GetSpellIconID() == 548 && roll_chance_i(50))
                    return SPELL_AURA_PROC_FAILED;
            }
            // Blessed Resilience
            else if (auraSpellInfo->SpellIconID == 2177)
            {
                if (damage * 100 >= pVictim->GetMaxHealth() * auraSpellInfo->CalculateSimpleValue(EFFECT_INDEX_1) ||
                    (procEx & PROC_EX_CRITICAL_HIT) != 0 && (procFlags & PROC_FLAG_ON_TAKE_PERIODIC) == 0)
                    break;

                return SPELL_AURA_PROC_FAILED;
            }
            // Masochism
            else if (auraSpellInfo->SpellIconID == 2211)
            {
                // If damage inflicted is less that pct health and not from SWD
                if (damage * 100 < pVictim->GetMaxHealth() * auraSpellInfo->CalculateSimpleValue(EFFECT_INDEX_1) &&
                    (!procSpell || procSpell->Id != 32409))
                    return SPELL_AURA_PROC_FAILED;
            }
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            // Druid Forms Trinket
            if (auraSpellInfo->Id==37336)
            {
                switch(GetShapeshiftForm())
                {
                    case FORM_NONE:     trigger_spell_id = 37344;break;
                    case FORM_CAT:      trigger_spell_id = 37341;break;
                    case FORM_BEAR:     trigger_spell_id = 37340; break;
                    case FORM_TREE:     trigger_spell_id = 37342;break;
                    case FORM_MOONKIN:  trigger_spell_id = 37343;break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
            }
            // Druid T9 Feral Relic (Lacerate, Swipe, Mangle, and Shred)
            else if (auraSpellInfo->Id==67353)
            {
                switch(GetShapeshiftForm())
                {
                    case FORM_CAT:      trigger_spell_id = 67355; break;
                    case FORM_BEAR:     trigger_spell_id = 67354; break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
            }
            // Hunter T10 2P Bonus (bad spellfamily)
            else if (auraSpellInfo->Id == 70727)
            {
                if (Pet* pet = GetPet())
                {
                    if (pet->isAlive())
                    {
                        pet->CastSpell(pet,trigger_spell_id,true);
                        return SPELL_AURA_PROC_OK;
                    }
                }
                return SPELL_AURA_PROC_FAILED;
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            if (auraSpellInfo->GetSpellIconID() == 2260)         // Combat Potency
            {
                // proc from offhand hits and Main Gauche
                if (!(procFlags & PROC_FLAG_SUCCESSFUL_OFFHAND_HIT) && (!procSpell || procSpell->Id != 86392))
                    return SPELL_AURA_PROC_FAILED;
            }
            // Item - Rogue T10 2P Bonus
            else if (auraSpellInfo->Id == 70805)
            {
                if (pVictim != this)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Seal Fate
            else if (auraSpellInfo->GetSpellIconID() == 2984)
            {
                if (!procSpell || !IsSpellHaveEffect(procSpell, SPELL_EFFECT_ADD_COMBO_POINTS))
                    return SPELL_AURA_PROC_FAILED;
                break;
            }
            // Master Poisoner
            else if (auraSpellInfo->Id == 58410)
            {
                if (!procSpell || procSpell->GetDispel() != DISPEL_POISON)
                    return SPELL_AURA_PROC_FAILED;
                break;
            }
            // Item - Rogue T10 4P Bonus
            else if (auraSpellInfo->Id == 70803)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;
                // only allow melee finishing move to proc
                if (!procSpell->HasAttribute(SPELL_ATTR_EX_REQ_TARGET_COMBO_POINTS) || procSpell->Id == 26679)
                    return SPELL_AURA_PROC_FAILED;
                trigger_spell_id = 70802;
                target = this;
            }
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Piercing Shots
            if (auraSpellInfo->GetSpellIconID() == 3247 && auraSpellInfo->GetSpellVisual() == 0)
            {
                basepoints[0] = damage * triggerAmount / 100 / 8;
                trigger_spell_id = 63468;
                target = pVictim;
            }
            // Rapid Recuperation
            else if (auraSpellInfo->Id == 53228 || auraSpellInfo->Id == 53232)
            {
                // This effect only from Rapid Fire (ability cast)
                if (!procSpell->GetSpellFamilyFlags().test<CF_HUNTER_RAPID_FIRE>())
                    return SPELL_AURA_PROC_FAILED;
            }
            // Entrapment correction
            else if ((auraSpellInfo->Id == 19184 || auraSpellInfo->Id == 19387 || auraSpellInfo->Id == 19388) &&
                !procSpell->GetSpellFamilyFlags().test<CF_HUNTER_SNAKE_TRAP_EFFECT, CF_HUNTER_FROST_TRAP>())
                    return SPELL_AURA_PROC_FAILED;
            // Lock and Load
            else if (auraSpellInfo->GetSpellIconID() == 3579)
            {
                // Check for Lock and Load Marker
                if (HasAura(67544))
                    return SPELL_AURA_PROC_FAILED;
            }
            // Item - Hunter T9 4P Bonus
            else if (auraSpellInfo->Id == 67151)
            {
                trigger_spell_id = 68130;
                break;
            }
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Tower of Radiance, Ranks 1-2
            if (auraSpellInfo->GetSpellIconID() == 3402)
            {
                // Must be target of Beacon of Light
                if (!pVictim || !pVictim->GetSpellAuraHolder(53563, GetObjectGuid()))
                    return SPELL_AURA_PROC_FAILED;
            }
            // Protector of the Innocent
            if (auraSpellInfo->SpellIconID == 5014)
            {
                // Target must not be caster and not from Holy Radiance periodic heal
                if (pVictim == this || !procSpell || procSpell->Id == 86452)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Healing Discount
            if (auraSpellInfo->Id==37705)
            {
                trigger_spell_id = 37706;
                target = this;
            }
            // Soul Preserver
            if (auraSpellInfo->Id==60510)
            {
                trigger_spell_id = 60515;
                target = this;
            }
            // Lightning Capacitor
            else if (auraSpellInfo->Id==37657)
            {
                if(!pVictim || !pVictim->isAlive())
                    return SPELL_AURA_PROC_FAILED;
                // stacking
                CastSpell(this, 37658, true, NULL, triggeredByAura);

                Aura const* dummy = GetDummyAura(37658);
                // release at 3 aura in stack (cont contain in basepoint of trigger aura)
                if(!dummy || dummy->GetStackAmount() < uint32(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;

                RemoveAurasDueToSpell(37658);
                trigger_spell_id = 37661;
                target = pVictim;
            }
            // Bonus Healing (Crystal Spire of Karabor mace)
            else if (auraSpellInfo->Id == 40971)
            {
                // If your target is below $s1% health
                if (pVictim->GetHealth() > pVictim->GetMaxHealth() * triggerAmount / 100)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Thunder Capacitor
            else if (auraSpellInfo->Id == 54841)
            {
                if(!pVictim || !pVictim->isAlive())
                    return SPELL_AURA_PROC_FAILED;
                // stacking
                CastSpell(this, 54842, true, NULL, triggeredByAura);

                // counting
                Aura const* dummy = GetDummyAura(54842);
                // release at 3 aura in stack (cont contain in basepoint of trigger aura)
                if(!dummy || dummy->GetStackAmount() < uint32(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;

                RemoveAurasDueToSpell(54842);
                trigger_spell_id = 54843;
                target = pVictim;
            }
            // Item - Icecrown 25 Normal/Heroic Healer Weapon Proc
            else if (auraSpellInfo->Id == 71865 || auraSpellInfo->Id == 71868)
            {
                // don't proc on self
                if (procSpell->Id == 71864 || procSpell->Id == 71866)
                    return SPELL_AURA_PROC_FAILED;

                target = pVictim;
            }
            // Item - Coliseum 25 Normal and Heroic Caster Trinket
            else if (auraSpellInfo->Id == 67712 || auraSpellInfo->Id == 67758)
            {
                if (!pVictim || !pVictim->isAlive())
                    return SPELL_AURA_PROC_FAILED;

                uint32 castSpell = auraSpellInfo->Id == 67758 ? 67759 : 67713;

                // stacking
                CastSpell(this, castSpell, true, NULL, triggeredByAura);

                // counting
                Aura const* dummy = GetDummyAura(castSpell);

                // release at 3 aura in stack (count contained in basepoint of trigger aura)
                if (!dummy || dummy->GetStackAmount() < uint32(triggerAmount))
                    return SPELL_AURA_PROC_FAILED;

                RemoveAurasDueToSpell(castSpell);
                trigger_spell_id = castSpell + 1;
                target = pVictim;
                break;
            }
            // Hand of Light
            else if (auraSpellInfo->Id == 76672)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                basepoints[0] = int32(triggerAmount * damage / 100.0f);
                trigger_spell_id = 96172;
                break;
            }
            // Seals of Command
            else if (auraSpellInfo->Id == 85126)
            {
                // Do not proc other spells than seal triggers
                // Seal of Justice, Seal of Righteousness, Censure
                if (!procSpell || procSpell->Id != 20170 && procSpell->Id != 25742 && procSpell->Id != 31803)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Sacred Shield
            else if (auraSpellInfo->Id == 85285)
            {
                if (GetHealthPercent() > 20.0f)
                    return SPELL_AURA_PROC_FAILED;
            }
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            // Lightning Shield (overwrite non existing triggered spell call in spell.dbc
            if (auraSpellInfo->GetSpellFamilyFlags().test<CF_SHAMAN_LIGHTNING_SHIELD>() && auraSpellInfo->GetSpellVisual() == 37)
            {
                switch(auraSpellInfo->Id)
                {
                    case 324:                           // Rank 1
                        trigger_spell_id = 26364; break;
                    case 325:                           // Rank 2
                        trigger_spell_id = 26365; break;
                    case 905:                           // Rank 3
                        trigger_spell_id = 26366; break;
                    case 945:                           // Rank 4
                        trigger_spell_id = 26367; break;
                    case 8134:                          // Rank 5
                        trigger_spell_id = 26369; break;
                    case 10431:                         // Rank 6
                        trigger_spell_id = 26370; break;
                    case 10432:                         // Rank 7
                        trigger_spell_id = 26363; break;
                    case 25469:                         // Rank 8
                        trigger_spell_id = 26371; break;
                    case 25472:                         // Rank 9
                        trigger_spell_id = 26372; break;
                    case 49280:                         // Rank 10
                        trigger_spell_id = 49278; break;
                    case 49281:                         // Rank 11
                        trigger_spell_id = 49279; break;
                    default:
                        sLog.outError("Unit::HandleProcTriggerSpellAuraProc: Spell %u not handled in LShield", auraSpellInfo->Id);
                        return SPELL_AURA_PROC_FAILED;
                }
            }
            // Damage from Lightning Shield (The Ten Storms set)
            else if (auraSpellInfo->Id == 23552)
                trigger_spell_id = 27635;
            // Mana Surge (The Earthfury set)
            else if (auraSpellInfo->Id == 23572)
            {
                if(!procSpell)
                    return SPELL_AURA_PROC_FAILED;
                basepoints[0] = procSpell->GetManaCost() * 35 / 100;
                trigger_spell_id = 23571;
                target = this;
            }
            // Nature's Guardian
            else if (auraSpellInfo->GetSpellIconID() == 2013)
            {
                // Check health condition - should drop to less 30% (trigger at any attack with result health less 30%, independent original health state)
                int32 health30 = int32(GetMaxHealth()) * 3 / 10;
                if (int32(GetHealth()) - int32(damage) >= health30)
                    return SPELL_AURA_PROC_FAILED;

                if (pVictim && pVictim->isAlive())
                    pVictim->getThreatManager().modifyThreatPercent(this,-auraSpellInfo->CalculateSimpleValue(EFFECT_INDEX_1));

                basepoints[0] = triggerAmount * GetMaxHealth() / 100;
                trigger_spell_id = 31616;
                target = this;
            }
            // Item - Shaman T10 Restoration 2P Bonus
            else if (auraSpellInfo->Id == 70807)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;
                // only allow Riptide to proc
                switch(procSpell->Id)
                {
                    case 61295: // Rank 1
                    case 61299: // Rank 2
                    case 61300: // Rank 3
                    case 61301: // Rank 4
                        break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
                trigger_spell_id = 70806;
                target = this;
            }
            // Ancestral Healing
            else if (auraSpellInfo->SpellIconID == 200)
            {
                int32 bp = damage;
                // Cast Ancestral Vigor (max hp part of talent)
                CastCustomSpell(pVictim, 105284, &bp, NULL, NULL, true);
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            // Acclimation
            if (auraSpellInfo->GetSpellIconID() == 1930)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;
                switch(GetFirstSchoolInMask(GetSpellSchoolMask(procSpell)))
                {
                    case SPELL_SCHOOL_NORMAL:
                        return SPELL_AURA_PROC_FAILED;                   // ignore
                    case SPELL_SCHOOL_HOLY:   trigger_spell_id = 50490; break;
                    case SPELL_SCHOOL_FIRE:   trigger_spell_id = 50362; break;
                    case SPELL_SCHOOL_NATURE: trigger_spell_id = 50488; break;
                    case SPELL_SCHOOL_FROST:  trigger_spell_id = 50485; break;
                    case SPELL_SCHOOL_SHADOW: trigger_spell_id = 50489; break;
                    case SPELL_SCHOOL_ARCANE: trigger_spell_id = 50486; break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
            }
            // Glyph of Death's Embrace
            else if (auraSpellInfo->Id == 58677)
            {
                if (procSpell->Id != 47633)
                    return SPELL_AURA_PROC_FAILED;
            }
            // Glyph of Death Grip
            if (auraSpellInfo->Id == 62259)
            {
                // remove cooldown of Death Grip
                RemoveSpellCooldown(49576, true);
                return SPELL_AURA_PROC_OK;
            }
            // Item - Death Knight T10 Melee 4P Bonus
            else if (auraSpellInfo->Id == 70656)
            {
                if (GetTypeId() != TYPEID_PLAYER || getClass() != CLASS_DEATH_KNIGHT)
                    return SPELL_AURA_PROC_FAILED;

                for(uint32 i = 0; i < MAX_RUNES; ++i)
                    if (((Player*)this)->GetRuneCooldown(i) == 0)
                        return SPELL_AURA_PROC_FAILED;
            }
            // Blade Barrier
            else if (auraSpellInfo->GetSpellIconID() == 85)
            {
                if (GetTypeId() != TYPEID_PLAYER || getClass() != CLASS_DEATH_KNIGHT ||
                    !((Player*)this)->IsBaseRuneSlotsOnCooldown(RUNE_BLOOD))
                    return SPELL_AURA_PROC_FAILED;
            }
            // Improved Blood Presence
            else if (auraSpellInfo->Id == 63611)
            {
                if (!damage)
                    return SPELL_AURA_PROC_FAILED;
                basepoints[0] = triggerAmount * damage / 100;
                trigger_spell_id = 50475;
            }
            break;
        }
        default:
            break;
    }

    // All ok. Check current trigger spell
    SpellEntry const* triggerEntry = sSpellStore.LookupEntry(trigger_spell_id);
    if (!triggerEntry)
    {
        // Not cast unknown spell
        // sLog.outError("Unit::HandleProcTriggerSpellAuraProc: Spell %u have 0 in EffectTriggered[%d], not handled custom case?",auraSpellInfo->Id,triggeredByAura->GetEffIndex());
        return SPELL_AURA_PROC_FAILED;
    }

    // not allow proc extra attack spell at extra attack
    if (m_extraAttacks && IsSpellHaveEffect(triggerEntry, SPELL_EFFECT_ADD_EXTRA_ATTACKS))
        return SPELL_AURA_PROC_FAILED;

    // Custom basepoints/target for exist spell
    // dummy basepoints or other customs
    switch(trigger_spell_id)
    {
        // Cast positive spell on enemy target
        case 7099:  // Curse of Mending
        case 39647: // Curse of Mending
        case 29494: // Temptation
        case 20233: // Improved Lay on Hands (cast on target)
        {
            target = pVictim;
            break;
        }
        // Combo points add triggers (need add combopoint only for main target, and after possible combopoints reset)
        case 15250: // Rogue Setup
        {
            if(!pVictim || pVictim != getVictim())   // applied only for main target
                return SPELL_AURA_PROC_FAILED;
            break;                                   // continue normal case
        }
        // Finishing moves that add combo points
        case 14189: // Seal Fate (talent, Netherblade set)
        case 14157: // Ruthlessness
        case 70802: // Mayhem (Shadowblade sets)
        {
            // Need add combopoint AFTER finishing move (or they get dropped in finish phase)
            if (Spell* spell = GetCurrentSpell(CURRENT_GENERIC_SPELL))
            {
                spell->AddTriggeredSpell(trigger_spell_id);
                return SPELL_AURA_PROC_OK;
            }
            return SPELL_AURA_PROC_FAILED;
        }
        // Bloodthirst ${$23881m2/1000}.1%
        case 23880:
        {
            basepoints[0] = int32(GetMaxHealth() * triggerAmount / 100 / 1000);
            break;
        }
        // Shamanistic Rage triggered spell
        case 30824:
        {
            basepoints[0] = int32(GetTotalAttackPowerValue(BASE_ATTACK) * triggerAmount / 100);
            break;
        }
        // Enlightenment (trigger only from mana cost spells)
        case 35095:
        {
            SpellPowerEntry const* spellPower = procSpell->GetSpellPower();
            if (!spellPower || !procSpell || procSpell->GetPowerType() != POWER_MANA || spellPower->manaCost == 0 && spellPower->ManaCostPercentage == 0 && spellPower->manaCostPerlevel == 0)
                return SPELL_AURA_PROC_FAILED;
            break;
        }
        // Demonic Pact
        case 48090:
        {
            // As the spell is proced from pet's attack - find owner
            Unit* owner = GetOwner();
            if (!owner || owner->GetTypeId() != TYPEID_PLAYER)
                return SPELL_AURA_PROC_FAILED;

            // This spell doesn't stack, but refreshes duration. So we receive current bonuses to minus them later.
            int32 curBonus = 0;
            if (Aura* aur = owner->GetAura(48090, EFFECT_INDEX_0))
                curBonus = aur->GetModifier()->m_amount;
            int32 spellDamage  = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_MAGIC) - curBonus;
            if (spellDamage <= 0)
                return SPELL_AURA_PROC_FAILED;

            // percent stored in owner talent dummy
            AuraList const& dummyAuras = owner->GetAurasByType(SPELL_AURA_DUMMY);
            for (AuraList::const_iterator i = dummyAuras.begin(); i != dummyAuras.end(); ++i)
            {
                if ((*i)->GetSpellProto()->GetSpellIconID() == 3220)
                {
                    basepoints[0] = basepoints[1] = int32(spellDamage * (*i)->GetModifier()->m_amount / 100);
                    break;
                }
            }
            break;
        }
        // Sword and Board
        case 50227:
        {
            // Remove cooldown on Shield Slam
            if (GetTypeId() == TYPEID_PLAYER)
                ((Player*)this)->RemoveSpellCategoryCooldown(971, true);
            break;
        }
        // Maelstrom Weapon
        case 53817:
        {
            // Item - Shaman T10 Enhancement 4P Bonus
            // Calculate before roll_chance of ranks
            if (Aura const* dummy = GetDummyAura(70832))
            {
              if (SpellAuraHolderPtr aurHolder = GetSpellAuraHolder(53817))
                if ((aurHolder->GetStackAmount() == aurHolder->GetSpellProto()->GetStackAmount()) && roll_chance_i(dummy->GetBasePoints()))
                    CastSpell(this,70831,true,castItem,triggeredByAura);
            }

            // have rank dependent proc chance, ignore too often cases
            // PPM = 2.5 * (rank of talent),
            uint32 rank = sSpellMgr.GetSpellRank(auraSpellInfo->Id);
            // 5 rank -> 100% 4 rank -> 80% and etc from full rate
            if(!roll_chance_i(20*rank))
                return SPELL_AURA_PROC_FAILED;
            break;
        }
        // Brain Freeze
        case 57761:
        {
            if(!procSpell)
                return SPELL_AURA_PROC_FAILED;
            // For trigger from Blizzard need exist Improved Blizzard
            if (procSpell->GetSpellFamilyName() == SPELLFAMILY_MAGE && procSpell->GetSpellFamilyFlags().test<CF_MAGE_BLIZZARD>())
            {
                bool found = false;
                AuraList const& mOverrideClassScript = GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
                for(AuraList::const_iterator i = mOverrideClassScript.begin(); i != mOverrideClassScript.end(); ++i)
                {
                    int32 script = (*i)->GetModifier()->m_miscvalue;
                    if (script==836 || script==988 || script==989)
                    {
                        found=true;
                        break;
                    }
                }
                if(!found)
                    return SPELL_AURA_PROC_FAILED;
            }
            break;
        }
        // Astral Shift
        case 52179:
        {
            if (procSpell == 0 || !(procEx & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) || this == pVictim)
                return SPELL_AURA_PROC_FAILED;

            // Need stun, fear or silence mechanic
            if (!(GetAllSpellMechanicMask(procSpell) & IMMUNE_TO_SILENCE_AND_STUN_AND_FEAR_MASK))
                return SPELL_AURA_PROC_FAILED;
            break;
        }
        // Burning Determination
        case 54748:
        {
            if(!procSpell)
                return SPELL_AURA_PROC_FAILED;
            // Need Interrupt or Silenced mechanic
            if (!(GetAllSpellMechanicMask(procSpell) & IMMUNE_TO_INTERRUPT_AND_SILENCE_MASK))
                return SPELL_AURA_PROC_FAILED;
            break;
        }
        // Lock and Load
        case 56453:
        {
            // Proc only from trap activation (from periodic proc another aura of this spell)
            // because some spells have both flags (ON_TRAP_ACTIVATION and ON_PERIODIC), but should only proc ON_PERIODIC!!
            if (!(procFlags & PROC_FLAG_ON_TRAP_ACTIVATION) || !procSpell ||
                !(procSpell->GetSchoolMask() & (SPELL_SCHOOL_MASK_FROST | SPELL_SCHOOL_MASK_NATURE)) || !roll_chance_i(triggerAmount))
            {
                return SPELL_AURA_PROC_FAILED;
            }
            // don't proc Explosive Trap on triggering (only on periodic, in other aura proc)
            else if (procSpell->GetSpellFamilyFlags().test<CF_HUNTER_FIRE_TRAP_EFFECTS>())
                return SPELL_AURA_PROC_FAILED;

            break;
        }
        // Freezing Fog (Rime triggered)
        case 59052:
        {
            // Howling Blast cooldown reset
            RemoveSpellCategoryCooldown(1248, true);
            break;
        }
        // Druid - Savage Defense
        case 62606:
        {
            basepoints[0] = int32(GetTotalAttackPowerValue(BASE_ATTACK) * triggerAmount / 100);
            break;
        }
        // Hack for Blood mark (ICC Saurfang)
        case 72255:
        case 72444:
        case 72445:
        case 72446:
        {
            SpellEffectEntry const* spellEff = auraSpellInfo->GetSpellEffect(EFFECT_INDEX_0);

            float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(spellEff->EffectRadiusIndex));
            Map::PlayerList const& pList = GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = pList.begin(); itr != pList.end(); ++itr)
                if (itr->getSource() && itr->getSource()->IsWithinDistInMap(this,radius) && itr->getSource()->HasAura(triggerEntry->GetTargetAuraSpell()))
                {
                    target = itr->getSource();
                    break;
                }
            break;
        }
    }

    if (!trigger_spell_id)
    {
        // Linked spells (Proc chain)
        SpellLinkedSet linkedSet = sSpellMgr.GetSpellLinked(auraSpellInfo->Id, SPELL_LINKED_TYPE_PROC);
        if (linkedSet.size() > 0)
        {
            for (SpellLinkedSet::const_iterator itr = linkedSet.begin(); itr != linkedSet.end(); ++itr)
            {
                if (target == NULL)
                    target = !(procFlags & PROC_FLAG_SUCCESSFUL_POSITIVE_SPELL) && IsPositiveSpell(*itr) ? this : pVictim;
                CastSpell(target, *itr, true, castItem, triggeredByAura);
                if (cooldown)
                    AddSpellCooldown(*itr,0,time(NULL) + cooldown);
            }
        }
    }

    if (cooldown && HasSpellCooldown(trigger_spell_id))
        return SPELL_AURA_PROC_FAILED;

    // try detect target manually if not set
    if (target == NULL)
        target = !(procFlags & PROC_FLAG_SUCCESSFUL_POSITIVE_SPELL) && IsPositiveSpell(trigger_spell_id) ? this : pVictim;

    // default case
    if (!target || (target != this && !target->isAlive()))
        return SPELL_AURA_PROC_FAILED;


    if (SpellEntry const* triggeredSpellInfo = sSpellStore.LookupEntry(trigger_spell_id))
    {
        if (basepoints[EFFECT_INDEX_0] || basepoints[EFFECT_INDEX_1] || basepoints[EFFECT_INDEX_2])
            CastCustomSpell(target,triggeredSpellInfo,
                basepoints[EFFECT_INDEX_0] ? &basepoints[EFFECT_INDEX_0] : NULL,
                basepoints[EFFECT_INDEX_1] ? &basepoints[EFFECT_INDEX_1] : NULL,
                basepoints[EFFECT_INDEX_2] ? &basepoints[EFFECT_INDEX_2] : NULL,
                true, castItem, triggeredByAura);
        else
            CastSpell(target,triggeredSpellInfo,true,castItem,triggeredByAura);
    }
    else
    {
        sLog.outError("HandleProcTriggerSpellAuraProc: unknown spell id %u by caster: %s triggered by aura %u (eff %u)", trigger_spell_id, GetGuidStr().c_str(), triggeredByAura->GetId(), triggeredByAura->GetEffIndex());
        return SPELL_AURA_PROC_FAILED;
    }

    if (cooldown)
        AddSpellCooldown(trigger_spell_id,0,time(NULL) + cooldown);

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleProcTriggerDamageAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlags, uint32 procEx, uint32 cooldown)
{
    SpellEntry const *spellInfo = triggeredByAura->GetSpellProto();
    DEBUG_FILTER_LOG(LOG_FILTER_SPELL_CAST, "ProcDamageAndSpell: doing %u damage from spell id %u (triggered by auratype %u of spell %u)",
        triggeredByAura->GetModifier()->m_amount, spellInfo->Id, triggeredByAura->GetModifier()->m_auraname, triggeredByAura->GetId());

    DamageInfo procDamageInfo(this, pVictim, spellInfo);
    procDamageInfo.damage = triggeredByAura->GetModifier()->m_amount;
    CalculateSpellDamage(&procDamageInfo);
    procDamageInfo.target->CalculateAbsorbResistBlock(this, &procDamageInfo, spellInfo);
    DealDamageMods(&procDamageInfo);
    SendSpellNonMeleeDamageLog(&procDamageInfo);
    DealSpellDamage(&procDamageInfo, true);
    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleOverrideClassScriptAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 /*procFlag*/, uint32 /*procEx*/ ,uint32 cooldown)
{
    int32 scriptId = triggeredByAura->GetModifier()->m_miscvalue;

    if(!pVictim || !pVictim->isAlive())
        return SPELL_AURA_PROC_FAILED;

    Item* castItem = triggeredByAura->GetCastItemGuid() && GetTypeId()==TYPEID_PLAYER
        ? ((Player*)this)->GetItemByGuid(triggeredByAura->GetCastItemGuid()) : NULL;

    // Basepoints of trigger aura
    int32 triggerAmount = triggeredByAura->GetModifier()->m_amount;

    uint32 triggered_spell_id = 0;

    switch(scriptId)
    {
        case 836:                                           // Improved Blizzard (Rank 1)
        {
            if (!procSpell || procSpell->GetSpellVisual() != 9487)
                return SPELL_AURA_PROC_FAILED;
            triggered_spell_id = 12484;
            break;
        }
        case 988:                                           // Improved Blizzard (Rank 2)
        {
            if (!procSpell || procSpell->GetSpellVisual() != 9487)
                return SPELL_AURA_PROC_FAILED;
            triggered_spell_id = 12485;
            break;
        }
        case 989:                                           // Improved Blizzard (Rank 3)
        {
            if (!procSpell || procSpell->GetSpellVisual() != 9487)
                return SPELL_AURA_PROC_FAILED;
            triggered_spell_id = 12486;
            break;
        }
        case 4086:                                          // Improved Mend Pet (Rank 1)
        case 4087:                                          // Improved Mend Pet (Rank 2)
        {
            if(!roll_chance_i(triggerAmount))
                return SPELL_AURA_PROC_FAILED;

            triggered_spell_id = 24406;
            break;
        }
        case 4533:                                          // Dreamwalker Raiment 2 pieces bonus
        {
            // Chance 50%
            if (!roll_chance_i(50))
                return SPELL_AURA_PROC_FAILED;

            switch (pVictim->getPowerType())
            {
                case POWER_MANA:   triggered_spell_id = 28722; break;
                case POWER_RAGE:   triggered_spell_id = 28723; break;
                case POWER_ENERGY: triggered_spell_id = 28724; break;
                default:
                    return SPELL_AURA_PROC_FAILED;
            }
            break;
        }
        case 4537:                                          // Dreamwalker Raiment 6 pieces bonus
            triggered_spell_id = 28750;                     // Blessing of the Claw
            break;
        case 5497:                                          // Improved Mana Gems (Serpent-Coil Braid)
            CastSpell(pVictim, 37445, true);                // Mana Surge (direct because triggeredByAura has no duration)
            return SPELL_AURA_PROC_OK;
        case 7010:                                          // Revitalize (rank 1)
        case 7011:                                          // Revitalize (rank 2)
        case 7012:                                          // Revitalize (rank 3)
        {
            if(!roll_chance_i(triggerAmount))
                return SPELL_AURA_PROC_FAILED;

            switch( pVictim->getPowerType() )
            {
                case POWER_MANA:        triggered_spell_id = 48542; break;
                case POWER_RAGE:        triggered_spell_id = 48541; break;
                case POWER_ENERGY:      triggered_spell_id = 48540; break;
                case POWER_RUNIC_POWER: triggered_spell_id = 48543; break;
                default: return SPELL_AURA_PROC_FAILED;
            }
            break;
        }
        case 7282:                                          // Crypt Fever & Ebon Plaguebringer
        {
            if (!procSpell || pVictim == this)
                return SPELL_AURA_PROC_FAILED;

            bool HasEP = false;
            Unit::AuraList const& scriptAuras = GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
            for(Unit::AuraList::const_iterator i = scriptAuras.begin(); i != scriptAuras.end(); ++i)
            {
                if ((*i)->GetSpellProto()->GetSpellIconID() == 1766)
                {
                    HasEP = true;
                    break;
                }
            }

            if (!HasEP)
                switch(triggeredByAura->GetId())
                {
                    case 49032: triggered_spell_id = 50508; break;
                    case 49631: triggered_spell_id = 50509; break;
                    case 49632: triggered_spell_id = 50510; break;
                    default: return SPELL_AURA_PROC_FAILED;
                }
            else
                switch(triggeredByAura->GetId())
                {
                    case 51099: triggered_spell_id = 51726; break;
                    case 51160: triggered_spell_id = 51734; break;
                    case 51161: triggered_spell_id = 51735; break;
                    default: return SPELL_AURA_PROC_FAILED;
                }
            break;
        }
    }

    // not processed
    if(!triggered_spell_id)
        return SPELL_AURA_PROC_FAILED;

    // standard non-dummy case
    SpellEntry const* triggerEntry = sSpellStore.LookupEntry(triggered_spell_id);

    if(!triggerEntry)
    {
        sLog.outError("Unit::HandleOverrideClassScriptAuraProc: Spell %u triggering for class script id %u",triggered_spell_id,scriptId);
        return SPELL_AURA_PROC_FAILED;
    }

    if ( cooldown && HasSpellCooldown(triggered_spell_id))
        return SPELL_AURA_PROC_FAILED;

    CastSpell(pVictim, triggered_spell_id, true, castItem, triggeredByAura);

    if (cooldown)
        AddSpellCooldown(triggered_spell_id,0,time(NULL) + cooldown);

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleMendingAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const* /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/ )
{
    // aura can be deleted at casts
    SpellEntry const* spellProto = triggeredByAura->GetSpellProto();
    SpellEffectIndex effIdx = triggeredByAura->GetEffIndex();
    int32 heal = triggeredByAura->GetModifier()->m_amount;
    ObjectGuid caster_guid = triggeredByAura->GetCasterGuid();

    // jumps
    int32 jumps = triggeredByAura->GetHolder()->GetAuraCharges() - 1;

    // current aura holder expire
    triggeredByAura->GetHolder()->SetAuraCharges(1, false);

    // next target selection
    if (jumps > 0 && GetTypeId() == TYPEID_PLAYER && caster_guid.IsPlayer())
    {
        SpellEffectEntry const* spellEffect = spellProto->GetSpellEffect(effIdx);
        float radius;
        if (spellEffect && spellEffect->GetRadiusIndex())
            radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(spellEffect->GetRadiusIndex()));
        else
            radius = GetSpellMaxRange(sSpellRangeStore.LookupEntry(spellProto->GetRangeIndex()));

        if (Player* caster = ((Player*)triggeredByAura->GetCaster()))
        {
            caster->ApplySpellMod(spellProto->Id, SPELLMOD_RADIUS, radius);

            SpellAuraHolderPtr holder = GetSpellAuraHolder(spellProto->Id, caster->GetObjectGuid());

            if (Player* target = ((Player*)this)->GetNextRandomRaidMember(radius, true))
            {
                SpellAuraHolderPtr new_holder = CreateSpellAuraHolder(spellProto, target, caster);

                for (int32 i = 0; i < MAX_EFFECT_INDEX; ++i)
                {
                    Aura* aur = holder->GetAuraByEffectIndex(SpellEffectIndex(i));
                    if (!aur)
                        continue;

                    int32 basePoints = aur->GetBasePoints();
                    new_holder->CreateAura(spellProto, aur->GetEffIndex(), &basePoints, new_holder, target, caster, NULL);
                }
                new_holder->SetAuraCharges(jumps, false);

                // lock aura holder (currently SPELL_AURA_PRAYER_OF_MENDING is single target spell, so will attempt removing from old target
                // when applied to new one)
                target->AddSpellAuraHolder(new_holder);
            }

            heal += int32(caster->SpellBaseHealingBonusDone(GetSpellSchoolMask(spellProto)) * 1.59f);

        }
    }

    // heal
    CastCustomSpell(this, 33110, &heal, NULL, NULL, true, NULL, NULL, caster_guid, spellProto);
    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleModCastingSpeedNotStackAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const* procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    SpellEntry const* spellProto = triggeredByAura->GetSpellProto();

    // Speed of Light
    if (spellProto->GetSpellFamilyName() == SPELLFAMILY_PALADIN && spellProto->SpellIconID == 5062)
    {
        int32 bp = triggeredByAura->GetModifier()->m_amount;
        CastCustomSpell(this, 85497, &bp, NULL, NULL, true);
    }

    // Skip melee hits or instant cast spells
    return !(procSpell == NULL || GetSpellCastTime(procSpell) == 0) ? SPELL_AURA_PROC_OK : SPELL_AURA_PROC_FAILED;
}

SpellAuraProcResult Unit::HandleReflectSpellsSchoolAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const* procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    // Skip Melee hits and spells ws wrong school
    return !(procSpell == NULL || (triggeredByAura->GetModifier()->m_miscvalue & procSpell->GetSchoolMask()) == 0) ? SPELL_AURA_PROC_OK : SPELL_AURA_PROC_FAILED;
}

SpellAuraProcResult Unit::HandleModPowerCostSchoolAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const* procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    // Skip melee hits and spells ws wrong school or zero cost
    return !(procSpell == NULL ||
            (procSpell->GetManaCost() == 0 && procSpell->GetManaCostPercentage() == 0) || // Cost check
            (triggeredByAura->GetModifier()->m_miscvalue & procSpell->GetSchoolMask()) == 0) ? SPELL_AURA_PROC_OK : SPELL_AURA_PROC_FAILED;  // School check
}

SpellAuraProcResult Unit::HandleMechanicImmuneResistanceAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const* procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    // Compare mechanic
   return !(procSpell==NULL || int32(procSpell->GetMechanic()) != triggeredByAura->GetModifier()->m_miscvalue)
       ? SPELL_AURA_PROC_OK : SPELL_AURA_PROC_FAILED;
}

SpellAuraProcResult Unit::HandleModDamageFromCasterAuraProc(Unit* pVictim, DamageInfo* /*damageInfo*/, Aura const* triggeredByAura, SpellEntry const* procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    SpellEntry const* spellProto = triggeredByAura->GetSpellProto();

    switch (spellProto->GetSpellFamilyName())
    {
        case SPELLFAMILY_WARRIOR:
        {
            // Juggernaut
            if (spellProto->Id == 64976)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                // add cooldowns
                // procced from Charge
                if (procSpell->Id == 100)
                {
                    CastSpell(this, 96216, false);  // Intercept cooldown
                    // cast proc
                    CastSpell(this, 65156, true);
                }
                else
                    CastSpell(this, 96215, false);  // Charge cooldown
            }
            break;
        }
    }

    // Compare casters
    return triggeredByAura->GetCasterGuid() == pVictim->GetObjectGuid() ? SPELL_AURA_PROC_OK : SPELL_AURA_PROC_FAILED;
}

SpellAuraProcResult Unit::HandleAddFlatModifierAuraProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const * procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    SpellEntry const *spellInfo = triggeredByAura->GetSpellProto();

    switch (spellInfo->GetSpellFamilyName())
    {
        case SPELLFAMILY_HUNTER:
        {
            switch (spellInfo->Id)
            {
                case 53257:                             // Cobra strike
                case 55166:                             // Tidal Force
                {
                    SpellAuraHolderPtr holder = triggeredByAura->GetHolder();
                    if (!holder || holder->IsDeleted())
                        return SPELL_AURA_PROC_FAILED;

                    if (holder->ModStackAmount(-1))
                    {
                        RemoveSpellAuraHolder(holder);
                        return SPELL_AURA_PROC_OK;
                    }
                    else
                        return SPELL_AURA_PROC_CANT_TRIGGER;
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            switch (spellInfo->Id)
            {
                case 80976:     // Blitz Rank 1
                case 80977:     // Blitz Rank 2
                {
                    if (!pVictim || triggeredByAura->GetEffIndex() != EFFECT_INDEX_0)
                        return SPELL_AURA_PROC_FAILED;

                    CastSpell(pVictim, 96273, true);
                    return SPELL_AURA_PROC_OK;
                }
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Pandemic
            if (spellInfo->SpellIconID == 4554)
            {
                if (!roll_chance_i(spellInfo->CalculateSimpleValue(EFFECT_INDEX_0)) ||
                    pVictim->GetHealthPercent() > 25.0f)
                    return SPELL_AURA_PROC_FAILED;

                CastSpell(this, 92931, true);
                return SPELL_AURA_PROC_OK;
            }
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            // Chakra
            if (spellInfo->Id == 14751)
                // Chakra: Serenity
                CastSpell(this, 81208, true);
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            // Deadly Momentum
            if (spellInfo->Id == 84590)
            {
                if (GetTypeId() != TYPEID_PLAYER || ((Player*)this)->HasSpellCooldown(spellInfo->Id))
                    return SPELL_AURA_PROC_FAILED;
                return SPELL_AURA_PROC_OK;
            }
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            if (!procSpell)
                return SPELL_AURA_PROC_FAILED;

            if (!procSpell->GetSpellFamilyFlags().test<CF_PALADIN_JUDGEMENT_ACTIVATE>())
                return SPELL_AURA_PROC_FAILED;

            int bp = triggeredByAura->GetModifier()->m_amount;
            CastCustomSpell(pVictim, 68055, &bp, NULL, NULL, true, NULL, triggeredByAura);
            break;
        }
        default:
            break;
    }

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleAddPctModifierAuraProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 /*procFlag*/, uint32 procEx, uint32 /*cooldown*/)
{
    SpellEntry const *spellInfo = triggeredByAura->GetSpellProto();
    Item* castItem = triggeredByAura->GetCastItemGuid() && GetTypeId()==TYPEID_PLAYER
        ? ((Player*)this)->GetItemByGuid(triggeredByAura->GetCastItemGuid()) : NULL;

    switch(spellInfo->GetSpellFamilyName())
    {
        case SPELLFAMILY_MAGE:
        {
            // Combustion
            if (spellInfo->Id == 11129)
            {
                //last charge and crit
                if (triggeredByAura->GetHolder()->GetAuraCharges() <= 1 && (procEx & PROC_EX_CRITICAL_HIT) )
                    return SPELL_AURA_PROC_OK;                        // charge counting (will removed)

                CastSpell(this, 28682, true, castItem, triggeredByAura);
                return (procEx & PROC_EX_CRITICAL_HIT) ? SPELL_AURA_PROC_OK : SPELL_AURA_PROC_FAILED; // charge update only at crit hits, no hidden cooldowns
            }
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Soulburn
            if (spellInfo->Id == 74434)
            {
                if (!procSpell)
                    return SPELL_AURA_PROC_FAILED;

                switch (procSpell->Id)
                {
                    case 5676:      // Searing Pain
                        CastSpell(this, 79440, true);
                        break;
                    case 6262:      // Healthstone
                        CastSpell(this, 79437, true);
                        break;
                    case 27243:     // Seed of Corruption
                    {
                        // Soulburn: Seed of Corruption, rank 1
                        if (!HasSpell(86664))
                            return SPELL_AURA_PROC_FAILED;

                        if (pVictim)
                        {
                            if (SpellAuraHolderPtr holder = pVictim->GetSpellAuraHolder(procSpell->Id, GetObjectGuid()))
                            {
                                if (Aura* aura = holder->GetAuraByEffectIndex(EFFECT_INDEX_1))
                                    aura->GetModifier()->m_miscvalue = 1;   // mark as affected by Soulburn
                            }
                        }
                        break;
                    }
                    case 48020:     // Demonic Circle: Teleport
                        CastSpell(this, 79438, true);
                        break;
                    case 688:       // Summon Imp
                    case 691:       // Summon Felhunter
                    case 697:       // Summon Voidwalker
                    case 6353:      // Soul Fire
                    case 11519:     // Summon Succubus
                    case 30146:     // Summon Felguard
                    case 89420:     // Drain Life
                        break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }
            }
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            // Chakra
            if (spellInfo->Id == 14751)
                if (triggeredByAura->GetEffIndex() == EFFECT_INDEX_1)
                    // Chakra: Sanctuary
                    CastSpell(this, 81206, true);
                else
                    // Chakra: Chastice
                    CastSpell(this, 81209, true);
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Glyph of Divinity
            if (spellInfo->Id == 54939)
            {
                // Lookup base amount mana restore
                for (int i = 0; i < MAX_EFFECT_INDEX; ++i)
                {
                    SpellEffectEntry const* spellEffect = procSpell->GetSpellEffect(SpellEffectIndex(i));
                    if(!spellEffect)
                        continue;
                    if (spellEffect->Effect == SPELL_EFFECT_ENERGIZE)
                    {
                        int32 mana = procSpell->CalculateSimpleValue(SpellEffectIndex(i));
                        CastCustomSpell(this, 54986, NULL, &mana, NULL, true, castItem, triggeredByAura);
                        break;
                    }
                }
                return SPELL_AURA_PROC_OK;
            }
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            if (spellInfo->Id == 46916)           // Slam!
            {
                if (!(procSpell && procSpell->Id==50782))
                    return SPELL_AURA_PROC_CANT_TRIGGER;
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            if (spellInfo->Id == 70805)           // Item - Rogue T10 2P Bonus (Rank 3)
                return SPELL_AURA_PROC_CANT_TRIGGER;
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Lock and load triggered
            if (spellInfo->Id == 56453)
            {
                // Proc only on first effect
                if (triggeredByAura->GetEffIndex() != EFFECT_INDEX_0)
                    return SPELL_AURA_PROC_CANT_TRIGGER;

                // Remove only single aura from stack
                if (triggeredByAura->GetStackAmount() > 1 && !triggeredByAura->GetHolder()->ModStackAmount(-1))
                    return SPELL_AURA_PROC_CANT_TRIGGER;
            }
            break;
        }
        default:
            break;
    }
    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleModDamagePercentDoneAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 /*procFlag*/, uint32 procEx, uint32 cooldown)
{
    SpellEntry const *spellInfo = triggeredByAura->GetSpellProto();
    Item* castItem = triggeredByAura->GetCastItemGuid() && GetTypeId()==TYPEID_PLAYER
        ? ((Player*)this)->GetItemByGuid(triggeredByAura->GetCastItemGuid()) : NULL;

    // Aspect of the Viper
    if (spellInfo->GetSpellFamilyName() == SPELLFAMILY_HUNTER && spellInfo->GetSpellFamilyFlags().test<CF_HUNTER_ASPECT_OF_THE_VIPER>())
    {
        uint32 maxmana = GetMaxPower(POWER_MANA);
        int32 bp = int32(maxmana* GetAttackTime(RANGED_ATTACK)/1000.0f/100.0f);

        if (cooldown && HasSpellCooldown(34075))
            return SPELL_AURA_PROC_FAILED;

        CastCustomSpell(this, 34075, &bp, NULL, NULL, true, castItem, triggeredByAura);
    }
    // Arcane Blast
    else if (spellInfo->Id == 36032 && procSpell->GetSpellFamilyName() == SPELLFAMILY_MAGE && procSpell->GetSpellIconID() == 2294)
        // prevent proc from self(spell that triggered this aura)
        return SPELL_AURA_PROC_FAILED;
    else if (spellInfo->Id == 16246)
        // Shaman clearcasting - must proc only first effect
        return SPELL_AURA_PROC_CANT_TRIGGER;

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandlePeriodicDummyAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    if (!triggeredByAura)
        return SPELL_AURA_PROC_FAILED;

    SpellEntry const *spellProto = triggeredByAura->GetSpellProto();
    if (!spellProto)
        return SPELL_AURA_PROC_FAILED;

    switch (spellProto->GetSpellFamilyName())
    {
        case SPELLFAMILY_GENERIC:
        {
            switch (spellProto->Id)
            {
                case 66334:                                 // Mistress' Kiss (Trial of the Crusader, ->
                case 67905:                                 // -> Lord Jaraxxus encounter, all difficulties)
                case 67906:                                 // ----- // -----
                case 67907:                                 // ----- // -----
                {
                    CastSpell(this, 66359, true, NULL, triggeredByAura);
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            switch (spellProto->GetSpellIconID())
            {
                // Reaping
                // Death Rune Mastery
                // Blood of the North
                case 22:
                case 2622:
                case 3041:
                {
                    if(!procSpell)
                        return SPELL_AURA_PROC_FAILED;

                    if (getClass() != CLASS_DEATH_KNIGHT)
                        return SPELL_AURA_PROC_FAILED;

                    Player * plr = GetTypeId() == TYPEID_PLAYER? ((Player*)this) : NULL;
                    if (!plr)
                        return SPELL_AURA_PROC_FAILED;

                    //get spell rune cost
                    SpellRuneCostEntry const *runeCost = sSpellRuneCostStore.LookupEntry(procSpell->runeCostID);
                    if (!runeCost)
                        return SPELL_AURA_PROC_FAILED;

                    //convert runes to death
                    for (uint32 i = 0; i < NUM_RUNE_TYPES -1/*don't count death rune*/; ++i)
                    {
                        uint32 remainingCost = runeCost->RuneCost[i];

                        while(remainingCost)
                        {
                            int32  convertedRuneCooldown = -1;
                            uint32 convertedRune = i;
                            for(uint32 j = 0; j < MAX_RUNES; ++j)
                            {
                                // convert only valid runes
                                if (RuneType(i) != plr->GetCurrentRune(j) &&
                                    RuneType(i) != plr->GetBaseRune(j))
                                    continue;

                                // select rune with longest cooldown
                                if (convertedRuneCooldown < plr->GetRuneCooldown(j))
                                {
                                    convertedRuneCooldown = int32(plr->GetRuneCooldown(j));
                                    convertedRune = j;
                                }
                            }
                            if (convertedRuneCooldown >= 0)
                                plr->ConvertRune(convertedRune, RUNE_DEATH);
                            --remainingCost;
                        }
                    }
                    return SPELL_AURA_PROC_OK;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleModRating(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const * /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    SpellEntry const *spellInfo = triggeredByAura->GetSpellProto();

    if (spellInfo->Id == 71564)                             // Deadly Precision
    {
        // Remove only single aura from stack
        if (triggeredByAura->GetStackAmount() > 1 && !triggeredByAura->GetHolder()->ModStackAmount(-1))
            return SPELL_AURA_PROC_CANT_TRIGGER;
    }

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleRemoveByDamageProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown)
{
    SpellAuraHolderPtr holder    = triggeredByAura->GetHolder();
    SpellEntry const*  spellInfo = triggeredByAura->GetSpellProto();

    // Hungering Cold - not break from diseases
    if (spellInfo->GetSpellIconID() == 2797)
    {
        if (procSpell && procSpell->GetDispel() == DISPEL_DISEASE)
            return SPELL_AURA_PROC_FAILED;
    }
    // Gouge
    else if (triggeredByAura->GetId() == 1776)
    {
        if (procSpell && procSpell->Id == triggeredByAura->GetId())
            return SPELL_AURA_PROC_FAILED;

        else if (pVictim && pVictim->GetTypeId() == TYPEID_PLAYER && procSpell && (GetAllSpellMechanicMask(procSpell) & (1 << (MECHANIC_BLEED - 1))))
        {
            // search Sanguinary Vein
            if (SpellEntry const * spellInfo = ((Player*)pVictim)->GetKnownTalentRankById(6520))
                if (roll_chance_i(spellInfo->CalculateSimpleValue(EFFECT_INDEX_1)))
                    return SPELL_AURA_PROC_FAILED;
        }
    }
    // Repentance vs. Censure
    else if (triggeredByAura->GetId() == 20066 && procSpell && procSpell->Id == 31803)
        return SPELL_AURA_PROC_FAILED;

    if (procSpell && triggeredByAura->GetModifier()->m_auraname == SPELL_AURA_MOD_STEALTH)
    {
            if (procSpell->HasAttribute(SPELL_ATTR_EX_NOT_BREAK_STEALTH) || procSpell->HasAttribute(SPELL_ATTR_EX_NO_THREAT) ||
                procSpell->HasAttribute(SPELL_ATTR_EX2_UNK28))
            return SPELL_AURA_PROC_FAILED;
    }

    RemoveAurasByCasterSpell(triggeredByAura->GetSpellProto()->Id, triggeredByAura->GetCasterGuid());

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleSpellMagnetAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown)
{
    if (triggeredByAura->GetId() == 8178)                   // Grounding Totem Effect
    {
        // Frozen Power checks
        if (Unit* owner = GetOwner())
        {
            if (procSpell && owner->GetTypeId() == TYPEID_PLAYER && owner->getClass() == CLASS_SHAMAN)
            {
                if (uint32 spellSchoolMask = GetSpellSchoolMask(procSpell))
                {
                    Player* plrOwner = (Player*)owner;
                    if (SpellEntry const * spellProto = plrOwner->GetKnownTalentRankById(11220))
                    {
                        uint32 triggeredSpell = 0;
                        if (spellSchoolMask & SPELL_SCHOOL_MASK_FIRE)
                            triggeredSpell = 97618;
                        else if (spellSchoolMask & SPELL_SCHOOL_MASK_FROST)
                            triggeredSpell = 97619;
                        else if (spellSchoolMask & SPELL_SCHOOL_MASK_NATURE)
                            triggeredSpell = 97620;
                        else if (spellSchoolMask & SPELL_SCHOOL_MASK_ARCANE)
                            triggeredSpell = 97621;
                        else if (spellSchoolMask & SPELL_SCHOOL_MASK_SHADOW)
                            triggeredSpell = 97622;
                        if (triggeredSpell)
                        {
                            int32 bp = GetResistancesAtLevel(plrOwner->getLevel()) * (spellProto->Id == 16086 ? 0.5f : 1.0f);
                            plrOwner->CastCustomSpell(plrOwner, triggeredSpell, &bp, NULL, NULL, true);
                        }
                    }
                }
            }
        }

        // for spells that doesn't do damage but need to destroy totem anyway
        if ((!damageInfo->damage || damageInfo->damage < GetHealth()) && GetTypeId() == TYPEID_UNIT && ((Creature*)this)->IsTotem())
        {
            KillSelf();
            return SPELL_AURA_PROC_OK;
        }
    }
    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleManaShieldAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const * procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown)
{
    SpellEntry const *dummySpell = triggeredByAura->GetSpellProto ();
    SpellClassOptionsEntry const* dummyClassOptions = dummySpell->GetSpellClassOptions();

    Item* castItem = triggeredByAura->GetCastItemGuid() && GetTypeId()==TYPEID_PLAYER
        ? ((Player*)this)->GetItemByGuid(triggeredByAura->GetCastItemGuid()) : NULL;

    uint32 triggered_spell_id = 0;
    Unit* target = pVictim;

    switch(dummyClassOptions->SpellFamilyName)
    {
        case SPELLFAMILY_MAGE:
        {
            // Incanter's Regalia set (add trigger chance to Mana Shield)
            if (dummySpell->GetSpellFamilyFlags().test<CF_MAGE_MANA_SHIELD>())
            {
                if (GetTypeId() != TYPEID_PLAYER)
                    return SPELL_AURA_PROC_FAILED;

                target = this;
                triggered_spell_id = 37436;
                break;
            }
            break;
        }
        default:
            break;
    }

    // processed charge only counting case
    if (!triggered_spell_id)
        return SPELL_AURA_PROC_OK;

    SpellEntry const* triggerEntry = sSpellStore.LookupEntry(triggered_spell_id);

    if (!triggerEntry)
    {
        sLog.outError("Unit::HandleManaShieldAuraProc: Spell %u have nonexistent triggered spell %u",dummySpell->Id,triggered_spell_id);
        return SPELL_AURA_PROC_FAILED;
    }

    // default case
    if (!target || (target != this && !target->isAlive()))
        return SPELL_AURA_PROC_FAILED;

    if (cooldown && HasSpellCooldown(triggered_spell_id))
        return SPELL_AURA_PROC_FAILED;

    CastSpell(target, triggered_spell_id, true, castItem, triggeredByAura);

    if (cooldown)
        AddSpellCooldown(triggered_spell_id,0,time(NULL) + cooldown);

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleModResistanceAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const* /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    SpellEntry const *spellInfo = triggeredByAura->GetSpellProto();

    // Inner Fire
    if (spellInfo->IsFitToFamily<SPELLFAMILY_PRIEST, CF_PRIEST_INNER_FIRE>())
    {
        // only at real damage
        if (!damageInfo->damage)
            return SPELL_AURA_PROC_FAILED;
    }

    return SPELL_AURA_PROC_OK;
}

/**
 * Function to operations with custom hardcoded proc-like effects, maked over proc system
 *
 * @param - as
 * @retcode - enum SpellAuraProcResult
        SPELL_AURA_PROC_OK           - aura must proc anyway
        SPELL_AURA_PROC_CANT_TRIGGER - aura not may proc anyway
        SPELL_AURA_PROC_FAILED       - aura may proc, if this defined by Unit::IsTriggeredAtSpellProcEvent
 */

SpellAuraProcResult Unit::IsTriggeredAtCustomProcEvent(Unit *pVictim, SpellAuraHolderPtr holder, SpellEntry const* procSpell, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, bool isVictim, SpellProcEventEntry const*& spellProcEvent )
{
    if (!holder || holder->IsDeleted())
        return SPELL_AURA_PROC_CANT_TRIGGER;

    SpellEntry const* spellProto = holder->GetSpellProto();

    if (procSpell == spellProto)
        return SPELL_AURA_PROC_FAILED;

    uint32 EventProcFlag = GetProcFlag(spellProto);

    for (int32 i = 0; i < MAX_EFFECT_INDEX; ++i)
    {
        if (Aura* aura = holder->GetAuraByEffectIndex(SpellEffectIndex(i)))
        {
            switch (aura->GetModifier()->m_auraname)
            {
                // Removable by damage auras (same rules as for CC auras)
                case SPELL_AURA_WATER_WALK:
                // Crowd Control auras
                case SPELL_AURA_MOD_CONFUSE:
                case SPELL_AURA_MOD_FEAR:
                case SPELL_AURA_MOD_STUN:
                case SPELL_AURA_MOD_ROOT:
                case SPELL_AURA_TRANSFORM:
                {
                    if ((procFlag & PROC_FLAG_TAKEN_ANY_DAMAGE) &&
                        ((spellProto->GetAuraInterruptFlags() & AURA_INTERRUPT_FLAG_DAMAGE) ||
                        (spellProto->HasAttribute(SPELL_ATTR_BREAKABLE_BY_DAMAGE))))
                        return SPELL_AURA_PROC_OK;
                    if (((procFlag & PROC_FLAG_TAKEN_ANY_DAMAGE) ||
                        (procExtra & PROC_EX_ABSORB)) &&
                        (spellProto->HasAttribute(SPELL_ATTR_EX_BREAKABLE_BY_ANY_DAMAGE) &&
                        ((spellProto->GetAuraInterruptFlags() & AURA_INTERRUPT_FLAG_DIRECT_DAMAGE) ||
                        (spellProto->GetAuraInterruptFlags() & AURA_INTERRUPT_FLAG_DAMAGE) ||
                        spellProto->HasAttribute(SPELL_ATTR_BREAKABLE_BY_DAMAGE))))
                        return SPELL_AURA_PROC_OK;
                    else if (EventProcFlag || spellProcEvent)
                        return SPELL_AURA_PROC_FAILED;
                    else
                        return SPELL_AURA_PROC_CANT_TRIGGER;
                }
                case SPELL_AURA_DAMAGE_SHIELD:
                    if (procFlag & PROC_FLAG_TAKEN_MELEE_HIT)
                        return SPELL_AURA_PROC_OK;
                    break;
                // Fake death auras
                case SPELL_AURA_FEIGN_DEATH:
                // Invisibility auras
                case SPELL_AURA_MOD_STEALTH:
                case SPELL_AURA_MOD_INVISIBILITY:
                {
                    if ((!EventProcFlag && !spellProcEvent) &&
                        ((procFlag & PROC_FLAG_TAKEN_ANY_DAMAGE) ||
                        (procExtra & PROC_EX_DIRECT_DAMAGE)) &&
                        !((procFlag &  PROC_FLAG_TAKEN_AOE_SPELL_HIT) ||
                        (procFlag &  PROC_FLAG_ON_TAKE_PERIODIC)) &&
                        !((procFlag & PROC_FLAG_SUCCESSFUL_AOE_SPELL_HIT) ||
                        (procFlag & PROC_FLAG_ON_DO_PERIODIC))
                        )
                        return SPELL_AURA_PROC_OK;
                    else if (EventProcFlag || spellProcEvent)
                        return SPELL_AURA_PROC_FAILED;
                    else
                        return SPELL_AURA_PROC_CANT_TRIGGER;
                }
                default:
                {
                // Default rules for all other auras.
                    if (EventProcFlag || spellProcEvent)
                        return SPELL_AURA_PROC_FAILED;
                    else if ((procFlag & PROC_FLAG_TAKEN_ANY_DAMAGE) &&
                        ((spellProto->GetAuraInterruptFlags() & AURA_INTERRUPT_FLAG_DAMAGE) ||
                        spellProto->HasAttribute(SPELL_ATTR_BREAKABLE_BY_DAMAGE)))
                        return SPELL_AURA_PROC_OK;
                    break;
                }
            }

        }
    }
    return SPELL_AURA_PROC_FAILED;
}

SpellAuraProcResult Unit::HandleDamageShieldAuraProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown)
{
    if (!triggeredByAura)
        return SPELL_AURA_PROC_FAILED;

    SpellEntry const *spellProto = triggeredByAura->GetSpellProto();

    if (!spellProto)
        return SPELL_AURA_PROC_FAILED;

    DamageInfo procDamageInfo =  DamageInfo(this, pVictim, spellProto, triggeredByAura->GetModifier()->m_amount);
    procDamageInfo.CleanDamage(0, 0, BASE_ATTACK, MELEE_HIT_NORMAL);
    procDamageInfo.damageType = SPELL_DIRECT_DAMAGE;

    SpellDamageBonusDone(&procDamageInfo);
    pVictim->SpellDamageBonusTaken(&procDamageInfo);
    pVictim->CalculateDamageAbsorbAndResist(this, &procDamageInfo, !spellProto->HasAttribute(SPELL_ATTR_EX_CANT_REFLECTED));
    DealDamageMods(&procDamageInfo);

    uint32 targetHealth = pVictim->GetHealth();
    uint32 overkill     = procDamageInfo.damage > targetHealth ? procDamageInfo.damage - targetHealth : 0;

    WorldPacket data(SMSG_SPELLDAMAGESHIELD,(8+8+4+4+4+4));
    data << GetObjectGuid();
    data << pVictim->GetObjectGuid();
    data << uint32(procDamageInfo.GetSpellId());
    data << uint32(procDamageInfo.damage);                  // Damage
    data << uint32(overkill);                   // Overkill
    data << uint32(procDamageInfo.GetSchoolMask());
    SendMessageToSet(&data, true);

    DealDamage(pVictim, &procDamageInfo, true);

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleDropChargeByDamageProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown)
{
    if (!triggeredByAura)
        return SPELL_AURA_PROC_FAILED;

    if (SpellAuraHolderPtr holder = triggeredByAura->GetHolder())
    {
        if (holder->DropAuraCharge())
            RemoveSpellAuraHolder(holder);
    }
    else
        return SPELL_AURA_PROC_FAILED;

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleRemoveByDamageChanceProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown)
{
    if (!triggeredByAura)
        return SPELL_AURA_PROC_FAILED;

    SpellEntry const *spellProto = triggeredByAura->GetSpellProto();

    if (!spellProto)
        return SPELL_AURA_PROC_FAILED;

    if ((spellProto->GetAuraInterruptFlags() & AURA_INTERRUPT_FLAG_DAMAGE) || (procEx & PROC_EX_IGNORE_CC))
        return HandleRemoveByDamageProc(pVictim, damageInfo, triggeredByAura, procSpell, procFlag, procEx, cooldown);

    if (const_cast<Aura*>(triggeredByAura)->IsAffectedByCrowdControlEffect(damageInfo->damage + damageInfo->absorb))
        return SPELL_AURA_PROC_FAILED;

    // The chance to dispel an aura depends on the damage taken with respect to the casters level.
    uint32 max_dmg = getLevel() > 8 ? 25 * getLevel() - 150 : 50;
    float chance = float(damageInfo->damage) / max_dmg * 100.0f;

    if (roll_chance_f(chance))
        return HandleRemoveByDamageProc(pVictim, damageInfo, triggeredByAura, procSpell, procFlag, procEx, cooldown);

    return SPELL_AURA_PROC_FAILED;
}

SpellAuraProcResult Unit::HandleIgnoreUnitStateAuraProc(Unit* /*pVictim*/, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    SpellEntry const* spellInfo = triggeredByAura->GetSpellProto();
    if (spellInfo->Id == 52437)
    {
        //Remove only single aura from stack
        return SPELL_AURA_PROC_CANT_TRIGGER;
    }

    return SPELL_AURA_PROC_FAILED;
}

SpellAuraProcResult Unit::HandleAuraProcOnPowerAmount(Unit* /*pVictim*/, DamageInfo* /*damageInfo*/, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    SpellEntry const* spellProto = triggeredByAura->GetSpellProto();
    int32 triggerAmount = triggeredByAura->GetModifier()->m_amount;

    Powers powerType = Powers(triggeredByAura->GetModifier()->m_miscvalue);
    if (GetPowerIndex(powerType) == INVALID_POWER_INDEX)
        return SPELL_AURA_PROC_FAILED;

    int32 powerAmount = GetPower(powerType);

    switch (spellProto->Id)
    {
        case 79577:         // Eclipse Mastery Driver Passive
        {
            if (!procSpell)
                return SPELL_AURA_PROC_FAILED;

            // forbid proc when not in balance spec
            if (!HasSpell(78674))
                return SPELL_AURA_PROC_FAILED;

            bool hasMarker = false;
            int32 direction = 1;
            // lunar Eclipse Marker
            if (HasAura(67484))
            {
                hasMarker = true;
                direction = -1;
            }
            // solar Eclipse Marker
            else if (HasAura(67483))
                hasMarker = true;

            int32 powerMod = 0;
            // Starfire
            if (procSpell->Id == 2912)
                powerMod = procSpell->CalculateSimpleValue(EFFECT_INDEX_1);
            // Wrath
            else if (procSpell->Id == 5176)
                powerMod = -procSpell->CalculateSimpleValue(EFFECT_INDEX_1);
            // Starsurge
            else if (procSpell->Id == 78674)
                powerMod = direction * procSpell->CalculateSimpleValue(EFFECT_INDEX_1);

            // proc failed if wrong spell or spell direction does not match marker direction
            if (!powerMod || hasMarker && direction * powerMod < 0)
                return SPELL_AURA_PROC_FAILED;

            if (powerMod > 0 && triggeredByAura->GetEffIndex() != EFFECT_INDEX_0)
                return SPELL_AURA_PROC_FAILED;
            else if (powerMod < 0 && triggeredByAura->GetEffIndex() != EFFECT_INDEX_1)
                return SPELL_AURA_PROC_FAILED;

            ModifyPower(powerType, powerMod);
            int32 newPower = GetPower(powerType);

            if (newPower == powerAmount)
                return SPELL_AURA_PROC_FAILED;

            // Eclipse is cleared when eclipse power reaches 0
            if (powerAmount * newPower <= 0)
            {
                RemoveAurasDueToSpell(48517);
                RemoveAurasDueToSpell(48518);
            }

            uint32 markerSpellAdd, markerSpellRemove;
            if (newPower == triggerAmount)
            {
                // cast Eclipse
                CastSpell(this, triggeredByAura->GetSpellEffect()->EffectTriggerSpell, true);

                // solar marker or lunar marker
                markerSpellAdd = triggeredByAura->GetEffIndex() == EFFECT_INDEX_0 ? 67484 : 67483;
                markerSpellRemove = triggeredByAura->GetEffIndex() == EFFECT_INDEX_0 ? 67483 : 67484;

                RemoveAurasDueToSpell(markerSpellRemove);
                if (!HasAura(markerSpellAdd))
                    CastSpell(this, markerSpellAdd, true);
            }
            // Marker caster only on Starfire and Wrath
            else if (!hasMarker && (procSpell->Id == 2912 || procSpell->Id == 5176))
            {
                uint32 markerSpellAdd, markerSpellRemove;
                // solar marker or lunar marker
                markerSpellAdd = powerMod > 0 ? 67483 : 67484;
                markerSpellRemove = powerMod < 0 ? 67483 : 67484;

                RemoveAurasDueToSpell(markerSpellRemove);
                if (!HasAura(markerSpellAdd))
                    CastSpell(this, markerSpellAdd, true);
            }
            break;
        }
        default:
            break;
    }

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleVengeanceProc(Unit* pVictim, int32 damage, int32 triggerAmount)
{
    // do not proc from damage
    if (!pVictim || pVictim->GetCharmerOrOwnerPlayerOrPlayerItself())
        return SPELL_AURA_PROC_FAILED;

    if (int32 basebp = damage)
    {
        int32 bp = 0;
        uint32 triggered_spell_id = 76691;
        // stack with old buff
        if (SpellAuraHolderPtr oldHolder = GetSpellAuraHolder(triggered_spell_id, GetObjectGuid()))
        {
            basebp = int32(basebp * triggerAmount / 100);
            if (Aura* oldAura = oldHolder->GetAuraByEffectIndex(EFFECT_INDEX_0))
                bp += oldAura->GetModifier()->m_amount;
        }
        else
            basebp = int32(basebp * 33 / 100);

        bp += basebp;

        // not more than pct of stamina
        int32 maxVal = int32((GetCreateHealth() + GetTotalStatValue(STAT_STAMINA)) / 10);
        if (bp > maxVal)
            bp = maxVal;

        CastCustomSpell(this, triggered_spell_id, &bp, &bp, &basebp, true);
    }

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleIncreaseSpeedAuraProc(Unit* /*pVictim*/, DamageInfo* /*damageInfo*/, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 cooldown)
{
    SpellEntry const* spellProto = triggeredByAura->GetSpellProto();

    switch (spellProto->Id)
    {
        case 26022:                     // Pursuit of Justice
        case 26023:
        {
            if (!procSpell || GetTypeId() != TYPEID_PLAYER)
                return SPELL_AURA_PROC_FAILED;

            if (!IsSpellAppliesAura(procSpell))
                return SPELL_AURA_PROC_FAILED;

            uint32 triggered_spell_id = 89024;
            uint32 cooldown_spell_id = 89023;

            // Pursuit of Justice shares cooldown with Blessed Life proc
            if (cooldown && ((Player*)this)->HasSpellCooldown(cooldown_spell_id))
                return SPELL_AURA_PROC_FAILED;

            if (GetAllSpellMechanicMask(procSpell) & MECHANIC_IMMOBILIZE)
            {
                // energize
                CastSpell(this, triggered_spell_id, true);
                if (cooldown)
                    ((Player*)this)->AddSpellCooldown(cooldown_spell_id, 0, time(NULL) + cooldown);
            }

            return SPELL_AURA_PROC_OK;
        }
    }

    return SPELL_AURA_PROC_OK;
}

SpellAuraProcResult Unit::HandleHasteAllProc(Unit* /*pVictim*/, DamageInfo* /*damageInfo*/, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
{
    SpellEntry const* spellProto = triggeredByAura->GetSpellProto();

    // Dark Intent (target)
    if (spellProto->Id == 85767)
    {
        if (Unit* caster = triggeredByAura->GetCaster())
            caster->CastSpell(caster, 94310, true);
    }
    // Dark Intent (caster)
    else if (spellProto->Id == 85768)
    {
        if (Unit* caster = triggeredByAura->GetTarget())
        {
            if (Unit* singleTarget = caster->GetSingleCastSpellTarget(85767))
            {
                uint32 triggered_spell = 0;

                switch (singleTarget->getClass())
                {
                    case CLASS_WARRIOR: triggered_spell = 94313; break;
                    case CLASS_PALADIN: triggered_spell = 94323; break;
                    case CLASS_HUNTER: triggered_spell = 94320; break;
                    case CLASS_ROGUE: triggered_spell = 94324; break;
                    case CLASS_PRIEST: triggered_spell = 94311; break;
                    case CLASS_DEATH_KNIGHT: triggered_spell = 94312; break;
                    case CLASS_SHAMAN: triggered_spell = 94319; break;
                    case CLASS_MAGE: triggered_spell = 85759; break;
                    case CLASS_WARLOCK: triggered_spell = 94310; break;
                    case CLASS_DRUID: triggered_spell = 94318; break;
                    default:
                        return SPELL_AURA_PROC_FAILED;
                }

                int32 bp = 1;
                singleTarget->CastCustomSpell(singleTarget, triggered_spell, &bp, &bp, NULL, true);
            }
        }
    }

    return SPELL_AURA_PROC_OK;
}
