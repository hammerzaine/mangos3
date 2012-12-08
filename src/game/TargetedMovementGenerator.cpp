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

#include "ByteBuffer.h"
#include "TargetedMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Player.h"
#include "World.h"
#include "movement/MoveSplineInit.h"
#include "movement/MoveSpline.h"

//-----------------------------------------------//
template<class T, typename D>
void TargetedMovementGeneratorMedium<T, D>::_setTargetLocation(T& owner, bool updateDestination)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return;

    if (owner.hasUnitState(UNIT_STAT_NOT_MOVE))
        return;

    if (owner.IsNonMeleeSpellCasted(false))
    {
        // some spells should be able to be cast while moving
        // maybe some attribute? here, check the entry of creatures useing these spells
        switch(owner.GetEntry())
        {
            case 36633: // Ice Sphere (Lich King)
            case 37562: // Volatile Ooze and Gas Cloud (Putricide)
            case 37697:
                break;
            default:
                return;
        }
    }

    if (!i_target->isInAccessablePlaceFor(&owner))
        return;

    float x, y, z;
    bool targetIsVictim = owner.getVictim() && owner.getVictim()->GetObjectGuid() == i_target->GetObjectGuid();

    // i_path can be NULL in case this is the first call for this MMGen (via Update)
    // Can happen for example if no path was created on MMGen-Initialize because of the owner being stunned
    if (updateDestination || !i_path)
    {
        // prevent redundant micro-movement for pets, other followers.
        if ((fabs(i_offset) > M_NULL_F) && i_target->IsWithinDistInMap(&owner, i_offset + PET_FOLLOW_DIST))
        {
            if (!owner.movespline->Finalized())
                return;

            owner.GetPosition(x, y, z);
        }
        else if (fabs(i_offset) < M_NULL_F)
        {
            // to nearest contact position
            float dist = 0.0f;
            if (targetIsVictim)
                dist = owner.GetFloatValue(UNIT_FIELD_COMBATREACH) + i_target->GetFloatValue(UNIT_FIELD_COMBATREACH) - i_target->GetObjectBoundingRadius() - owner.GetObjectBoundingRadius() - 1.0f;

            if (dist < 0.5f)
                dist = 0.5f;

            i_target->GetContactPoint(&owner, x, y, z, dist);
        }
        else
        {
            // to at i_offset distance from target and i_angle from target facing
            i_target->GetClosePoint(x, y, z, owner.GetObjectBoundingRadius(), i_offset, i_angle, &owner);
        }
    }
    else
    {
        MANGOS_ASSERT(i_path);

        // the destination has not changed, we just need to refresh the path (usually speed change)
        G3D::Vector3 end = i_path->getEndPosition();
        x = end.x;
        y = end.y;
        z = end.z;
    }

    if (!i_path)
        i_path = new PathFinder(&owner);

    // allow pets following their master to cheat while generating paths
    bool forceDest = (owner.GetTypeId() == TYPEID_UNIT && ((Creature*)&owner)->IsPet()
                      && owner.hasUnitState(UNIT_STAT_FOLLOW));
    i_path->calculate(x, y, z, forceDest);
    if (i_path->getPathType() & PATHFIND_NOPATH)
    {
        DEBUG_FILTER_LOG(LOG_FILTER_AI_AND_MOVEGENSS,"TargetedMovementGeneratorMedium::  unit %s cannot find path to %s (%f, %f, %f),  gained PATHFIND_NOPATH! Owerride used.",
            owner.GetObjectGuid().GetString().c_str(),
            i_target.isValid() ? i_target->GetObjectGuid().GetString().c_str() : "<none>",
            x,y,z);
        //return;
    }

    D::_addUnitStateMove(owner);
    i_targetReached = false;
    m_speedChanged = false;

    Movement::MoveSplineInit init(owner);
    init.MovebyPath(i_path->getPath());
    init.SetWalk(((D*)this)->EnableWalking());
    init.Launch();
}

#define RECHECK_DISTANCE_TIMER 50
#define TARGET_NOT_ACCESSIBLE_MAX_TIMER 5000

template<class T, typename D>
bool TargetedMovementGeneratorMedium<T, D>::Update(T& owner, const uint32& time_diff)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return false;

    if (owner.hasUnitState(UNIT_STAT_NOT_MOVE))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    // prevent movement while casting spells with cast time or channel time
    if (owner.IsNonMeleeSpellCasted(false, false,  true))
    {
        if (!owner.IsStopped())
        {
            // some spells should be able to be cast while moving
            // maybe some attribute? here, check the entry of creatures useing these spells
            switch(owner.GetEntry())
            {
                case 36633: // Ice Sphere (Lich King)
                case 37562: // Volatile Ooze and Gas Cloud (Putricide)
                case 37697:
                    break;
                default:
                    owner.StopMoving();
                    break;
            }
        }
        if (owner.IsStopped())
            return true;
    }

    // prevent crash after creature killed pet
    if (static_cast<D*>(this)->_lostTarget(owner))
    {
        D::_clearUnitStateMove(owner);
        if (i_targetSearchingTimer >= TARGET_NOT_ACCESSIBLE_MAX_TIMER)
            return false;
        else
        {
            i_targetSearchingTimer += 5 * time_diff;
            return true;
        }
    }

    if (!i_target->isInAccessablePlaceFor(&owner))
        return true;

    bool targetMoved = false;

    i_recheckDistance.Update(time_diff);
    if (i_recheckDistance.Passed())
    {
        i_recheckDistance.Reset(RECHECK_DISTANCE_TIMER);

        //More distance let have better performance, less distance let have more sensitive reaction at target move.
        //float allowed_dist = owner.GetObjectBoundingRadius() + sWorld.getConfig(CONFIG_FLOAT_RATE_TARGET_POS_RECALCULATION_RANGE);

        float allowed_dist = 0.0f;
        bool targetIsVictim = owner.getVictim() && owner.getVictim()->GetObjectGuid() == i_target->GetObjectGuid();
        if (targetIsVictim)
            allowed_dist = owner.GetMeleeAttackDistance(owner.getVictim()) + owner.GetObjectBoundingRadius();
        else
            allowed_dist = i_target->GetObjectBoundingRadius() + owner.GetObjectBoundingRadius() + sWorld.getConfig(CONFIG_FLOAT_RATE_TARGET_POS_RECALCULATION_RANGE);

        if (allowed_dist < owner.GetObjectBoundingRadius())
            allowed_dist = owner.GetObjectBoundingRadius();

        G3D::Vector3 dest = owner.movespline->FinalDestination();

        if (owner.GetTypeId() == TYPEID_UNIT && (((Creature*)&owner)->CanFly() || ((Creature*)&owner)->IsLevitating()))
            targetMoved = !i_target->IsWithinDist3d(dest.x, dest.y, dest.z, allowed_dist);
        else
            targetMoved = !i_target->IsWithinDist2d(dest.x, dest.y, allowed_dist);

        if (targetIsVictim && owner.GetTypeId() == TYPEID_UNIT && !((Creature*)&owner)->IsPet())
        {
            if ((!owner.getVictim() || !owner.getVictim()->isAlive()) && owner.movespline->Finalized())
                return false;

            if (!i_offset && owner.movespline->Finalized() && !owner.CanReachWithMeleeAttack(owner.getVictim())
                && !i_target->m_movementInfo.HasMovementFlag(MOVEFLAG_PENDINGSTOP))
            {
                if (i_targetSearchingTimer >= TARGET_NOT_ACCESSIBLE_MAX_TIMER)
                {
                    return false;
                }
                else
                {
                    i_targetSearchingTimer += RECHECK_DISTANCE_TIMER;
                    targetMoved = true;
                }
            }
            else
                i_targetSearchingTimer = 0;
        }
        else
            i_targetSearchingTimer = 0;
    }

    if (m_speedChanged || targetMoved)
        _setTargetLocation(owner, true);

    if (m_speedChanged || targetMoved)
        _setTargetLocation(owner, targetMoved);

    if (owner.movespline->Finalized())
    {
        if (fabs(i_angle) < M_NULL_F && !owner.HasInArc(0.01f, i_target.getTarget()))
            owner.SetInFront(i_target.getTarget());

        if (!i_targetReached)
        {
            i_targetReached = true;
            static_cast<D*>(this)->_reachTarget(owner);
        }
    }
    return true;
};

//-----------------------------------------------//
template<class T>
void ChaseMovementGenerator<T>::_reachTarget(T& owner)
{
    if (owner.CanReachWithMeleeAttack(this->i_target.getTarget()))
        owner.Attack(this->i_target.getTarget(), true);
}

template<>
void ChaseMovementGenerator<Player>::Initialize(Player& owner)
{
    owner.addUnitState(UNIT_STAT_CHASE | UNIT_STAT_CHASE_MOVE);
    _setTargetLocation(owner, true);
}

template<>
void ChaseMovementGenerator<Creature>::Initialize(Creature& owner)
{
    owner.SetWalk(false, false);                            // Chase movement is running
    owner.addUnitState(UNIT_STAT_CHASE | UNIT_STAT_CHASE_MOVE);
    _setTargetLocation(owner, true);
/*
    if (owner.getVictim() && !owner.hasUnitState(UNIT_STAT_MELEE_ATTACKING))
        owner.Attack(owner.getVictim(), !owner.IsNonMeleeSpellCasted(true));
*/
}

template<class T>
void ChaseMovementGenerator<T>::Finalize(T& owner)
{
    owner.clearUnitState(UNIT_STAT_CHASE | UNIT_STAT_CHASE_MOVE);
    if (owner.GetTypeId() == TYPEID_UNIT && owner.isAlive() && !owner.IsInEvadeMode())
        owner.AddEvent(new EvadeDelayEvent(owner), EVADE_TIME_DELAY);
}

template<class T>
void ChaseMovementGenerator<T>::Interrupt(T& owner)
{
    owner.clearUnitState(UNIT_STAT_CHASE | UNIT_STAT_CHASE_MOVE);
}

template<class T>
void ChaseMovementGenerator<T>::Reset(T& owner)
{
    Initialize(owner);
}

//-----------------------------------------------//
template<>
bool FollowMovementGenerator<Creature>::EnableWalking() const
{
    return i_target.isValid() && i_target->IsWalking();
}

template<>
bool FollowMovementGenerator<Player>::EnableWalking() const
{
    return false;
}

template<>
void FollowMovementGenerator<Player>::_updateSpeed(Player& /*u*/)
{
    // nothing to do for Player
}

template<>
void FollowMovementGenerator<Creature>::_updateSpeed(Creature& u)
{
    u.UpdateSpeed(MOVE_RUN, true);
    u.UpdateSpeed(MOVE_WALK, true);
    u.UpdateSpeed(MOVE_SWIM, true);
}

template<>
void FollowMovementGenerator<Player>::Initialize(Player& owner)
{
    owner.addUnitState(UNIT_STAT_FOLLOW | UNIT_STAT_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<>
void FollowMovementGenerator<Creature>::Initialize(Creature& owner)
{
    owner.addUnitState(UNIT_STAT_FOLLOW | UNIT_STAT_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<class T>
void FollowMovementGenerator<T>::Finalize(T& owner)
{
    owner.clearUnitState(UNIT_STAT_FOLLOW | UNIT_STAT_FOLLOW_MOVE);
    _updateSpeed(owner);
}

template<class T>
void FollowMovementGenerator<T>::Interrupt(T& owner)
{
    owner.clearUnitState(UNIT_STAT_FOLLOW | UNIT_STAT_FOLLOW_MOVE);
    _updateSpeed(owner);
}

template<class T>
void FollowMovementGenerator<T>::Reset(T& owner)
{
    Initialize(owner);
}

//-----------------------------------------------//
template void TargetedMovementGeneratorMedium<Player, ChaseMovementGenerator<Player> >::_setTargetLocation(Player&, bool);
template void TargetedMovementGeneratorMedium<Player, FollowMovementGenerator<Player> >::_setTargetLocation(Player&, bool);
template void TargetedMovementGeneratorMedium<Creature, ChaseMovementGenerator<Creature> >::_setTargetLocation(Creature&, bool);
template void TargetedMovementGeneratorMedium<Creature, FollowMovementGenerator<Creature> >::_setTargetLocation(Creature&, bool);
template bool TargetedMovementGeneratorMedium<Player, ChaseMovementGenerator<Player> >::Update(Player&, const uint32&);
template bool TargetedMovementGeneratorMedium<Player, FollowMovementGenerator<Player> >::Update(Player&, const uint32&);
template bool TargetedMovementGeneratorMedium<Creature, ChaseMovementGenerator<Creature> >::Update(Creature&, const uint32&);
template bool TargetedMovementGeneratorMedium<Creature, FollowMovementGenerator<Creature> >::Update(Creature&, const uint32&);

template void ChaseMovementGenerator<Player>::_reachTarget(Player&);
template void ChaseMovementGenerator<Creature>::_reachTarget(Creature&);
template void ChaseMovementGenerator<Player>::Finalize(Player&);
template void ChaseMovementGenerator<Creature>::Finalize(Creature&);
template void ChaseMovementGenerator<Player>::Interrupt(Player&);
template void ChaseMovementGenerator<Creature>::Interrupt(Creature&);
template void ChaseMovementGenerator<Player>::Reset(Player&);
template void ChaseMovementGenerator<Creature>::Reset(Creature&);

template void FollowMovementGenerator<Player>::Finalize(Player&);
template void FollowMovementGenerator<Creature>::Finalize(Creature&);
template void FollowMovementGenerator<Player>::Interrupt(Player&);
template void FollowMovementGenerator<Creature>::Interrupt(Creature&);
template void FollowMovementGenerator<Player>::Reset(Player&);
template void FollowMovementGenerator<Creature>::Reset(Creature&);
