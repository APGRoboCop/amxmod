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


void Client_ResetHUD_End(void* mValue){
  mPlayer->clearStats = gpGlobals->time + 0.25f;
}

void Client_RoundState(void* mValue){
  if ( mPlayer ) return;
  int result = *(int*)mValue;
  if ( result == 1 ){
    for (int i=1;i<=gpGlobals->maxClients;i++){
      CPlayer *pPlayer = GET_PLAYER_POINTER_I(i);
      if (pPlayer->ingame) {
        memset(&pPlayer->round,0,sizeof(pPlayer->round));
        memset(pPlayer->weaponsRnd,0,sizeof(pPlayer->weaponsRnd));
      }
    }
  }
}

void Client_TeamScore(void* mValue){
  static int index;
  switch(mState++){
  case 0:
    index = *(int*)mValue;
    break;
  case 1:
    switch (index){
    case 1:
      AlliesScore = *(int*)mValue;
      break;
    case 2:
      AxisScore = *(int*)mValue;
      break;
    }
    break;
  }
}

void Client_ObjScore(void* mValue){
  static CPlayer *pPlayer;
  static int TMScore; //total map score :-)
  switch(mState++){
  case 0:
    pPlayer = GET_PLAYER_POINTER_I(*(int*)mValue);
    break;
  case 1:
    TMScore = *(int*)mValue;
    pPlayer->lastScore = TMScore - pPlayer->savedScore;
    if ( pPlayer->lastScore && isModuleActive() ){
      pPlayer->updateScore(pPlayer->current,pPlayer->lastScore);
      pPlayer->sendScore = gpGlobals->time + 0.25f;
    }
    pPlayer->savedScore = TMScore;
    break;
  }
}

void Client_CurWeapon(void* mValue){
  static int iState;
  static int iId;
  switch (mState++){
  case 0:
    iState = *(int*)mValue;
    break;
  case 1:
    if (!iState) break;
    iId = *(int*)mValue;
    break;
  case 2:
    if ( !iState )
      break;
    int iClip = *(int*)mValue;
    mPlayer->current = iId;

    if ( weaponData[iId].needcheck ){
      iId = get_weaponid(mPlayer);
      mPlayer->current = iId;
    }
    if ( iClip > -1 && isModuleActive() ) {
      if ( mPlayer->current == 17 ){
        if ( iClip+2 == mPlayer->weapons[iId].clip)
        mPlayer->saveShot(iId);
      }
      else {
        if ( iClip+1 == mPlayer->weapons[iId].clip)
        mPlayer->saveShot(iId);
      }
    }
    mPlayer->weapons[iId].clip = iClip;
  }
}

void Client_Health_End(void* mValue){
  edict_t *enemy = mPlayer->pEdict->v.dmg_inflictor;
  int damage = (int)mPlayer->pEdict->v.dmg_take;

  if ( !mPlayer || !damage || !enemy )
    return;

  int weapon = 0;
  int aim = 0;

  mPlayer->pEdict->v.dmg_take = 0.0;

  CPlayer* pAttacker = NULL;
  if ( enemy->v.flags & (FL_CLIENT | FL_FAKECLIENT) ){
    pAttacker = GET_PLAYER_POINTER(enemy);
    weapon = pAttacker->current;
    if ( weaponData[weapon].needcheck )
      weapon = get_weaponid(pAttacker);
    aim = pAttacker->aiming;
    if ( weaponData[weapon].melee && isModuleActive() )
      pAttacker->saveShot(weapon);
  }
  else g_grenades.find( enemy , &pAttacker , weapon );

  int TA = 0;
  if ( !pAttacker ){
    pAttacker = mPlayer;
  }

  if ( pAttacker->index != mPlayer->index ){
    if( isModuleActive() )
      pAttacker->saveHit( mPlayer , weapon , damage, aim );
    if ( mPlayer->pEdict->v.team == pAttacker->pEdict->v.team )
      TA = 1;
  }

  if(g_forwards.forwardsExist(FF_DamageInfo) == true) {
    g_forwards.executeForward(FF_DamageInfo, pAttacker->index, mPlayer->index, damage, weapon, aim, TA);
  }

  if ( !mPlayer->IsAlive() ) {
    if( isModuleActive() ) {
      pAttacker->saveKill(mPlayer,weapon,( aim == 1 ) ? 1:0 ,TA);
    }
    if(g_forwards.forwardsExist(FF_DeathInfo) == true) {
      g_forwards.executeForward(FF_DeathInfo, pAttacker->index, mPlayer->index, weapon, aim, TA);
    }
  }
}

void Client_AmmoX(void* mValue){
  static int iAmmo;
  switch (mState++){
  case 0:
    iAmmo = *(int*)mValue;
    break;
  case 1:
    if (!mPlayer ) break;
    for(int i = 1; i < MAX_WEAPONS ; ++i)
      if (iAmmo == weaponData[i].ammoSlot)
        mPlayer->weapons[i].ammo = *(int*)mValue;
  }
}

void Client_AmmoShort(void* mValue){
  static int iAmmo;
  switch (mState++){
  case 0:
    iAmmo = *(int*)mValue;
    break;
  case 1:
  if (!mPlayer ) break;
    for(int i = 1; i < MAX_WEAPONS ; ++i)
      if (iAmmo == weaponData[i].ammoSlot)
        mPlayer->weapons[i].ammo = *(int*)mValue;
  }
}
