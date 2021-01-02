/*
 * Copyright (c) 2002-2011 - Aleksander Naszko, Julien Laurent
 *
 *
 *    By AMX Mod Team.
 *
 *    Some code was taken from AMX Mod X, credits for this code go to them.
 *    The authors of AMX Mod X are:
 *    David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
 *	  Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
 *		Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
 *	  Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.
 *
 *
 *    This file is part of AMX Mod.
 *
 *
 *    AMX Mod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    AMX Mod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with AMX Mod; if not, write to the Free Software Foundation,
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
 *
 */

#include "rank.h"


weapons_t weapon_data[MAX_WEAPONS];
grenades_t* grenades;
int g_iWeapon;
int g_iDamage;
int g_iAim;
int g_iTA;
bool g_bNoDamageEnd;
player_t* pAttacker;


void Client_ResetHUD_End(void* mValue){
  mPlayer->reset_hud = true;
}

void Client_WeaponList(void* mValue){
  static int wpnList;
  static int iId;
  static int iSlot;
  static const char* wpn_name;
  switch (mState++) {
  case 0:
    wpn_name = (char*)mValue;
    break;
  case 1:
    iSlot = *(int*)mValue;
    break;
  case 7:
    iId = *(int*)mValue;
    if((iId < 0 || iId >= MAX_WEAPONS) || (wpnList & (1<<iId))) {
      break;
    }

    wpnList |= (1<<iId);
    weapon_data[iId].iId = iId;
    weapon_data[iId].ammo_slot = iSlot;

    strncpy(weapon_data[iId].full_name, wpn_name , 31 );

    weapon_data[iId].full_name[31] = 0;

    const char* wpn_prefix = strstr( weapon_data[iId].full_name ,"weapon_");

    if(wpn_prefix)
      weapon_data[iId].name = wpn_prefix + 7;

    if(strcmp(weapon_data[iId].name, "hegrenade") == 0)
      weapon_data[iId].name += 2;
  }
}

// By AMXMODX Dev Team
void Client_Damage(void* mValue){
  static int bits;
  switch(mState++) {
  case 1:
    g_iDamage = *(int*)mValue;
    g_bNoDamageEnd = false;
    break;
  case 2:
    bits = *(int*)mValue;
    break;
  case 3:
    if(!mPlayer || !g_iDamage || bits) {
      g_bNoDamageEnd = true;
      break;
    }
    edict_t *enemy = NULL;
    enemy = mPlayer->pEdict->v.dmg_inflictor;
    if(FNullEnt(enemy)) {
      g_bNoDamageEnd = true;
      break;
    }
    pAttacker = NULL;
    g_iWeapon = 0;
    g_iAim = 0;
    if(enemy->v.flags & (FL_CLIENT | FL_FAKECLIENT)) {
      pAttacker = GET_PLAYER_POINTER(enemy);
      g_iWeapon = pAttacker->current;
      g_iAim = pAttacker->aiming;
      if(pAttacker != mPlayer && isModuleActive()) {
        fire_hit(mPlayer, pAttacker, g_iWeapon, g_iDamage, g_iAim);
      }
      break;
    }
    grenades_t* grenade = NULL;
    if((grenade = find_grenade(enemy)) != NULL) {
      pAttacker = grenade->pPlayer;
      g_iWeapon = grenade->index;
      if(pAttacker != mPlayer && isModuleActive()) {
        fire_hit(mPlayer, pAttacker, g_iWeapon, g_iDamage, g_iAim);
      }
    }
    else if(strcmp(STRING(enemy->v.classname), "grenade") == 0) {
      g_iWeapon = CSW_C4;
    }
  }
}

// By AMXMODX Dev Team
void Client_Damage_End(void* mValue){
  if(g_bNoDamageEnd) {
    return;
  }
  if(!pAttacker) {
    pAttacker = mPlayer;
  }
  if((mPlayer->team_id == pAttacker->team_id) && (mPlayer != pAttacker)) {
    g_iTA = 1;
  }
  else {
    g_iTA = 0;
  }
  if(g_forwards.forwardsExist(FF_DamageInfo) == true) {
    g_forwards.executeForward(FF_DamageInfo, pAttacker->index, mPlayer->index, g_iDamage, g_iWeapon, g_iAim, g_iTA);
  }
  if(!PlayerIsAlive(mPlayer->pEdict)) {
    if(isModuleActive()) {
      if(g_iWeapon != CSW_C4) {
        fire_kill(mPlayer, pAttacker, g_iWeapon, (g_iAim == 1) ? 1 : 0, g_iTA);
      }
      if(pAttacker == mPlayer) {
        fire_suicide(mPlayer);
      }
      else if(g_Vip && mPlayer->index == g_Vip) {
        fire_killed_vip(pAttacker);
      }
    }
    if(g_forwards.forwardsExist(FF_DeathInfo) == true) {
      g_forwards.executeForward(FF_DeathInfo, pAttacker->index, mPlayer->index, g_iWeapon, g_iAim, g_iTA);
    }
  }
}

void Client_CurWeapon(void* mValue){
  static int iState;
  static int iId;
  switch(mState++){
  case 0:
    iState = *(int*)mValue;
    break;
  case 1:
    if(!iState) break;
    iId = *(int*)mValue;
    break;
  case 2:
    if(!iState) break;
    int iClip = *(int*)mValue;
    if(isModuleActive() && iClip > -1 && iClip < mPlayer->weapons[iId].clip) {
      fire_shot(mPlayer, iId);
    }
    mPlayer->weapons[iId].clip = iClip;
    mPlayer->weapon = &mPlayer->weapons[iId];
    mPlayer->current = iId;
  }
}

void Client_AmmoX(void* mValue){
  static int iAmmo;
  switch (mState++){
  case 0:
    iAmmo = *(int*)mValue;
    break;
  case 1:
    for(int i=0; i < MAX_WEAPONS ; ++i)
      if (iAmmo == weapon_data[i].ammo_slot)
        mPlayer->weapons[i].ammo = *(int*)mValue;
  }
}

void Client_AmmoPickup(void* mValue){
  static int iSlot;
  switch (mState++){
  case 0:
    iSlot = *(int*)mValue;
    break;
  case 1:
    for(int i = 0; i < MAX_WEAPONS ; ++i)
      if (weapon_data[i].ammo_slot == iSlot)
        mPlayer->weapons[i].ammo += *(int*)mValue;
  }
}

void Client_WeapPickup(void* mValue){
  int iId = *(int*)mValue;
  mPlayer->weapons[iId].name = (char*)weapon_data[iId].name;
  mPlayer->weapons[iId].id = iId;
}

void Client_ScoreInfo(void* mValue){
  static int index;
  switch (mState++){
  case 0:
    index = *(int*)mValue;
    break;
  case 4:
    GET_PLAYER_POINTER_I( index )->team_id = *(int*)mValue;
  }
}

void Client_TextMsg(void* mValue){
  if(!mPlayer && mState == 1) {
    static const char* text;
    text = (const char*)mValue;
    if(text[1] == 'T' && text[8] == 'B' && g_Planter) {
      if(isModuleActive()) {
        player_t *pPlanter = GET_PLAYER_POINTER_I(g_Planter);
        fire_bomb_exploded(pPlanter);
      }
      if(g_forwards.forwardsExist(FF_BombExploded) == true) {
        g_pFunctionTable_Post->pfnStartFrame = StartFrame_Post;
        g_bombForwards = BOMB_EXPLODED;
      }
    }
    else if(text[1] == 'G' && (text[6] == 'C' || text[6] == 'w')) {
      g_Vip = 0;
    }
  }
  ++mState;
}

void Client_SendAudio(void* mValue){
  if(mState == 1) {
    static const char* text;
    text = (const char*)mValue;
    if(text[7] == 'B' && !mPlayer) {
      if(text[11] == 'P' && g_Planter) {
        if(isModuleActive()) {
          player_t *pPlanter = GET_PLAYER_POINTER_I(g_Planter);
          fire_bomb_planted(pPlanter);
        }
        if(g_forwards.forwardsExist(FF_BombPlanted) == true) {
          g_pFunctionTable_Post->pfnStartFrame = StartFrame_Post;
          g_bombForwards = BOMB_PLANTED;
        }
      }
      else if(text[11] == 'D' && g_Defuser) {
        if(isModuleActive()) {
          player_t *pDefuser = GET_PLAYER_POINTER_I(g_Defuser);
          fire_bomb_defused(pDefuser);
        }
        if(g_forwards.forwardsExist(FF_BombDefused) == true) {
          g_pFunctionTable_Post->pfnStartFrame = StartFrame_Post;
          g_bombForwards = BOMB_DEFUSED;
        }
      }
    }
    else if(g_Vip && !mPlayer) {
      if(text[7] == 'c' && text[9] == 'w') {
        if(isModuleActive()) {
          player_t *pVip = GET_PLAYER_POINTER_I(g_Vip);
          fire_escaped_as_vip(pVip);
        }
        g_Vip = 0;
      }
      else if((text[7] == 't' && text[10] == 'w') || (text[7] == 'r' && text[12] == 'd')) {
        g_Vip = 0;
      }
    }
  }
  ++mState;
}

void Client_StatusIcon(void* mValue){
  static int status;
  switch (mState++){
  case 0:
    status = *(int*)mValue;
    break;
  case 1:
    static const char* text;
    text = (const char*)mValue;
    if(text[0] == 'c' && text[1] == '4') {
      if(status == 1) {
          g_Planter = mPlayerIndex;
          if(isModuleActive()) {
            fire_has_bomb(mPlayer);
          }
      }
    }
  }
}

void Client_BarTime(void* mValue){
  int time = *(int*)mValue;
  if(PlayerIsAlive(mPlayer->pEdict) && time) {
    if(time == 3) {
      g_Planter = mPlayerIndex;
      g_Defuser = 0;
      if(g_forwards.forwardsExist(FF_BombPlanting) == true) {
        g_pFunctionTable_Post->pfnStartFrame = StartFrame_Post;
        g_bombForwards = BOMB_PLANTING;
      }
    }
    else if(time == 5 || time == 10) {
      g_Defuser = mPlayerIndex;
      g_DefuserHasDefuseKit = (time == 5) ? 1 : 0;
      if(isModuleActive()) {
        fire_is_defusing(mPlayer);
      }
      if(g_forwards.forwardsExist(FF_BombDefusing) == true) {
        g_pFunctionTable_Post->pfnStartFrame = StartFrame_Post;
        g_bombForwards = BOMB_DEFUSING;
      }
    }
  }
}

void Client_Money(void* mValue){
  static int money;
  static int flash;
  switch (mState++){
  case 0:
    money = *(int*)mValue;
    break;
  case 1:
    flash = *(int*)mValue;
    if(PlayerIsAlive(mPlayer->pEdict) && flash == 1 && isModuleActive()) {
      if((money - mPlayer->money) == 150) {
        fire_hostages_touched(mPlayer);
      }
      else if((money - mPlayer->money) == 1000) {
        fire_hostages_rescued(mPlayer);
      }
    }
    mPlayer->money = money;
  }
}

void Client_ScoreAttrib(void* mValue){
  static int index;
  static int flag;
  switch (mState++){
  case 0:
    index = *(int*)mValue;
    break;
  case 1:
    flag = *(int*)mValue;
    if(flag == 4 && mPlayerIndex == index) {
      if(g_Vip != index) {
        g_Vip = index;
        if(isModuleActive()) {
          player_t *pVip = GET_PLAYER_POINTER_I(index);
          fire_is_vip(pVip);
        }
      }
    }
  }
}

void put_grenade(edict_t* pGre, float time,int type, player_t* pPlayer){
  grenades_t* newone = (grenades_t*)malloc(sizeof(grenades_t));
  if (!newone) return;
  newone->time = gpGlobals->time + time;
  newone->pPlayer = pPlayer;
  newone->pGrenade = pGre;
  newone->index = type;
  newone->prev = NULL;
  newone->next = grenades;
  if (grenades) grenades->prev = newone;
  grenades = newone;
}

grenades_t* find_grenade(edict_t* enemy){
  grenades_t* result = NULL;
  for(grenades_t* p = grenades; p ; ) {
    if ( p->time > gpGlobals->time ) {
      if (p->pGrenade == enemy)
        result = p;
    }
    else{
      grenades_t* next = p->next;
      if (p->prev)  p->prev->next = next;
      else  grenades = next;
      if (next) next->prev = p->prev;
      free(p);
      p = next;
      continue;
    }
    p = p->next;
  }
  return result;
}

void delete_grenades(){
  while(grenades){
    grenades_t* a = grenades;
    grenades = grenades->next;
    free(a);
  }
}

//bool ignoreBots (edict_t *pEnt, edict_t *pOther){
  //if((int)csstats_rankbots->value == 0 && (pEnt->v.flags & FL_FAKECLIENT || (pOther && pOther->v.flags & FL_FAKECLIENT)))
bool ignoreBots (int index, edict_t *pOther){
  if((int)csstats_rankbots->value == 0 && (MF_IsPlayerBot(index) || (pOther && MF_IsPlayerBot(GET_PLAYER_POINTER(pOther)->index))))
    return true;
  return false;
}

bool isModuleActive(){
  if((int)csstats_pause->value == 0)
    return true;
  return false;
}
