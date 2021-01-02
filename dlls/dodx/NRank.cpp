/*
 * Copyright (c) 2003-2011 Lukasz Wlasinski
 *
 *    This file is part of DoD XMod.
 *
 *    DoD XMod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    DoD XMod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with DoD XMod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */


#include <extdll.h>
#include <meta_api.h>
#include "dodxmod.h"


NATIVE(get_user_astats) /* 6 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int attacker = params[2];
    if (attacker<0||attacker>gpGlobals->maxClients){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid player id %d", attacker);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->attackers[attacker].hits){
      int totalShots = 0;
      if(attacker == 0) {
        for(int i = 1; i <= gpGlobals->maxClients; ++i) {
          CPlayer* pAttacker = GET_PLAYER_POINTER_I(i);
          if(pAttacker->ingame && i != index) {
            totalShots += pAttacker->victims[0].shots;
          }
        }
      }
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      CPlayer::PlayerWeapon* stats = &pPlayer->attackers[attacker];
      cpStats[0] = stats->kills;
      cpStats[1] = stats->deaths;
      cpStats[2] = stats->hs;
      cpStats[3] = stats->tks;
      cpStats[4] = (attacker > 0) ? GET_PLAYER_POINTER_I(attacker)->victims[0].shots : totalShots;
      cpStats[5] = stats->hits;
      cpStats[6] = stats->damage;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats->bodyHits[i];
      if (params[6] && attacker && stats->name)
        MF_SetAmxString(amx,params[5],stats->name,params[6]);
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_vstats) /* 6 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int victim = params[2];
    if (victim<0||victim>gpGlobals->maxClients){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid player id %d", victim);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->victims[victim].hits){
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      CPlayer::PlayerWeapon* stats = &pPlayer->victims[victim];
      cpStats[0] = stats->kills;
      cpStats[1] = stats->deaths;
      cpStats[2] = stats->hs;
      cpStats[3] = stats->tks;
      cpStats[4] = pPlayer->victims[0].shots;
      cpStats[5] = stats->hits;
      cpStats[6] = stats->damage;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats->bodyHits[i];
      if (params[6] && victim && stats->name)
        MF_SetAmxString(amx,params[5],stats->name,params[6]);
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_wlstats) /* 4 param */ // DEC-Weapon (round) stats (end)
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int weapon = params[2];
    if (weapon<0||weapon>=DODMAX_WEAPONS){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", weapon);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->weaponsLife[weapon].shots){
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      Stats* stats = &pPlayer->weaponsLife[weapon];
      cpStats[0] = stats->kills;
      cpStats[1] = stats->deaths;
      cpStats[2] = stats->hs;
      cpStats[3] = stats->tks;
      cpStats[4] = stats->shots;
      cpStats[5] = stats->hits;
      cpStats[6] = stats->damage;
      cpStats[7] = stats->points;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats->bodyHits[i];
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_wrstats) /* 4 param */ // DEC-Weapon (round) stats (end)
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int weapon = params[2];
    if (weapon<0||weapon>=DODMAX_WEAPONS){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", weapon);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->weaponsLife[weapon].shots){
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      Stats* stats = &pPlayer->weaponsRnd[weapon];
      cpStats[0] = stats->kills;
      cpStats[1] = stats->deaths;
      cpStats[2] = stats->hs;
      cpStats[3] = stats->tks;
      cpStats[4] = stats->shots;
      cpStats[5] = stats->hits;
      cpStats[6] = stats->damage;
      cpStats[7] = stats->points;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats->bodyHits[i];
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_wstats) /* 4 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int weapon = params[2];
    if (weapon<0||weapon>=DODMAX_WEAPONS){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", weapon);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->weapons[weapon].shots){
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      CPlayer::PlayerWeapon* stats = &pPlayer->weapons[weapon];
      cpStats[0] = stats->kills;
      cpStats[1] = stats->deaths;
      cpStats[2] = stats->hs;
      cpStats[3] = stats->tks;
      cpStats[4] = stats->shots;
      cpStats[5] = stats->hits;
      cpStats[6] = stats->damage;
      cpStats[7] = stats->points;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats->bodyHits[i];
      return 1;
    }
  }
  return 0;
}

NATIVE(reset_user_wstats) /* 6 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    pPlayer->restartStats();
    return 1;
  }
  return 0;
}

NATIVE(get_user_stats) /* 3 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!pPlayer->rank) {
      return 0;
    }
    cell *cpStats = MF_GetAmxAddr(amx,params[2]);
    cell *cpBodyHits = MF_GetAmxAddr(amx,params[3]);
    cpStats[0] = pPlayer->rank->kills;
    cpStats[1] = pPlayer->rank->deaths;
    cpStats[2] = pPlayer->rank->hs;
    cpStats[3] = pPlayer->rank->tks;
    cpStats[4] = pPlayer->rank->shots;
    cpStats[5] = pPlayer->rank->hits;
    cpStats[6] = pPlayer->rank->damage;
    cpStats[7] = pPlayer->rank->points;
    cpStats[8] = pPlayer->rank->getPosition();
    for(int i = 1; i < 8; ++i)
      cpBodyHits[i] = pPlayer->rank->bodyHits[i];
    return pPlayer->rank->getPosition();
  }
  return 0;
}

NATIVE(get_user_lstats) /* 3 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!pPlayer->rank) {
      return 0;
    }
    cell *cpStats = MF_GetAmxAddr(amx,params[2]);
    cell *cpBodyHits = MF_GetAmxAddr(amx,params[3]);
    cpStats[0] = pPlayer->life.kills;
    cpStats[1] = pPlayer->life.deaths;
    cpStats[2] = pPlayer->life.hs;
    cpStats[3] = pPlayer->life.tks;
    cpStats[4] = pPlayer->life.shots;
    cpStats[5] = pPlayer->life.hits;
    cpStats[6] = pPlayer->life.damage;
    cpStats[7] = pPlayer->life.points;
    for(int i = 1; i < 8; ++i)
      cpBodyHits[i] = pPlayer->life.bodyHits[i];
    return 1;
  }
  return 0;
}

NATIVE(get_user_rstats) /* 3 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    cell *cpStats = MF_GetAmxAddr(amx,params[2]);
    cell *cpBodyHits = MF_GetAmxAddr(amx,params[3]);
    cpStats[0] = pPlayer->round.kills;
    cpStats[1] = pPlayer->round.deaths;
    cpStats[2] = pPlayer->round.hs;
    cpStats[3] = pPlayer->round.tks;
    cpStats[4] = pPlayer->round.shots;
    cpStats[5] = pPlayer->round.hits;
    cpStats[6] = pPlayer->round.damage;
    cpStats[7] = pPlayer->round.points;
    for(int i = 1; i < 8; ++i)
      cpBodyHits[i] = pPlayer->round.bodyHits[i];
    return 1;
  }
  return 0;
}

NATIVE(get_stats) /* 3 param */
{
  int index = params[1] + 1;
  for(RankSystem::iterator a = g_rank.front(); a ; --a){
    if ((*a).getPosition() == index)  {
      cell *cpStats = MF_GetAmxAddr(amx,params[2]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[3]);
      cpStats[0] = (*a).kills;
      cpStats[1] = (*a).deaths;
      cpStats[2] = (*a).hs;
      cpStats[3] = (*a).tks;
      cpStats[4] = (*a).shots;
      cpStats[5] = (*a).hits;
      cpStats[6] = (*a).damage;
      cpStats[7] = (*a).points;
      cpStats[8] = (*a).getPosition();
      MF_SetAmxString(amx,params[4],(*a).getName(),params[5]);
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = (*a).bodyHits[i];
      return --a ? (*a).getPosition() : 0;
    }
  }

  return 0;
}

NATIVE(get_statsnum)
{
  return g_rank.getRankNum();
}

AMX_NATIVE_INFO stats_natives[] = {
  { "get_stats",        get_stats},
  { "get_statsnum",     get_statsnum},
  { "get_user_astats",  get_user_astats },
  { "get_user_rstats",  get_user_rstats }, // round stats , cleared after RoundState = 1
  { "get_user_lstats",  get_user_lstats }, // from spawn to spawn stats
  { "get_user_stats",   get_user_stats },
  { "get_user_vstats",  get_user_vstats },
  { "get_user_wlstats", get_user_wlstats}, // DEC-Weapon(Life) Stats
  { "get_user_wrstats", get_user_wrstats}, // DEC-Weapon(Round) Stats
  { "get_user_wstats",  get_user_wstats},
  { "reset_user_wstats",reset_user_wstats },
  { NULL,               NULL }
};
