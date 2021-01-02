/*
 * Copyright (c) 2003-2011 Lukasz Wlasinski
 *
 *    Some code was taken from AMX Mod X, credits for this code go to them.
 *    The authors of AMX Mod X are:
 *    David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
 *	  Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
 *		Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
 *	  Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.
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



NATIVE(get_weapon_name){ // from id to name 3 params id, name, len
  int id = params[1];
  if (id<0 || id>=DODMAX_WEAPONS){
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", id);
    return 0;
  }
  return MF_SetAmxString(amx,params[2],weaponData[id].name,params[3]);
}


NATIVE(wpnlog_to_name){ // from log to name
  int iLen;
  char *log = MF_GetAmxString(amx,params[1],0,iLen);
  int i;
  for ( i=0; i<DODMAX_WEAPONS; ++i ){
    if ( strcmp(log,weaponData[i].logname ) == 0 )
      return MF_SetAmxString(amx,params[2],weaponData[i].name,params[3]);
  }
  return 0;
}


NATIVE(wpnlog_to_id){ // from log to id
  int iLen;
  char *log = MF_GetAmxString(amx,params[1],0,iLen);

  int i;
  for ( i=0; i<DODMAX_WEAPONS; ++i ){
    if ( strcmp(log,weaponData[i].logname) == 0 )
      return i;
  }
  return 0;
}

NATIVE(get_weapon_logname){ // from id to log
  int id = params[1];
  if (id<0 || id>=DODMAX_WEAPONS){
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", id);
    return 0;
  }
  return MF_SetAmxString(amx,params[2],weaponData[id].logname,params[3]);
}

NATIVE(is_melee){
  int id = params[1];
  if (id<0 || id>=DODMAX_WEAPONS){
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", id);
    return 0;
  }
  if ( weaponData[id].melee )
    return 1;
  return 0;
}

NATIVE(get_team_score){
  int index = params[1];
  switch ( index ){
  case 1:
    return AlliesScore;
    break;
  case 2:
    return AxisScore;
    break;
  }
  return 0;
}

NATIVE(get_user_score){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    return pPlayer->savedScore;
  }
  return 0;
}

NATIVE(set_player_stamina){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    int min = params[2];
    if ( min<0 || min>100 ){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid min stamina value %d", min);
      return 0;
    }
    int max = params[3];
    if ( max<min || max>100 ){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid max stamina value %d", max);
      return 0;
    }
    pPlayer->staminaMin = min;
    pPlayer->staminaMax = max;
    pPlayer->staminaSet = true;
    return 1;
  }
  return 0;
}

NATIVE(reset_player_stamina){
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    pPlayer->staminaMin = 0;
    pPlayer->staminaMax = 100;
    pPlayer->staminaSet = false;
    return 1;
  }
  return 0;
}

NATIVE(get_user_class){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    return pPlayer->pEdict->v.playerclass;
  }
  return 0;
}

NATIVE(get_user_pronestate){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->pEdict->v.iuser3){
      return pPlayer->pEdict->v.iuser3;
    }
    else {
      if ( *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_WDEPLOY) == 1 )
        return 3;
    }
  }
  return 0;
}

NATIVE(get_user_weapon){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    int wpn = pPlayer->current;
    int wpnflags = pPlayer->pEdict->v.weapons;
    if (!(wpnflags & (1<<wpn))){
      pPlayer->current = 0;
      wpn = 0;
    }
    cell *cpTemp = MF_GetAmxAddr(amx,params[2]);
    *cpTemp = (wpn == 0) ? 0 : pPlayer->weapons[wpn].clip;
    cpTemp = MF_GetAmxAddr(amx,params[3]);
    *cpTemp = (wpn == 0) ? 0 : pPlayer->weapons[wpn].ammo;
    return wpn;
  }
  return 0;
}

NATIVE(dod_user_kill){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->IsAlive() ){
      pPlayer->killPlayer();
      return 1;
    }
  }
  return 0;
}

NATIVE(detect_allies_country){
  return g_map.detect_allies_country;
}

NATIVE(detect_allies_paras){
  return g_map.detect_allies_paras;
}

NATIVE(detect_axis_paras){
  return g_map.detect_axis_paras;
}

NATIVE(nade_set_fuse){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);

    float fFuse = *(float *)((void *)&params[2]);
    if ( fFuse<0.1 || fFuse>20.0 ){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid fuse value %f", fFuse);
      return 0;
    }

    int iFType = params[3];

    pPlayer->nadeFuse = fFuse;
    pPlayer->fuseSet = true;
    pPlayer->fuseType = iFType;
    return 1;
  }
  return 0;
}

NATIVE(nade_reset_fuse){
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    pPlayer->fuseSet = false;
    pPlayer->nadeFuse = 0;
    return 1;
  }
  return 0;
}

NATIVE(set_user_class){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    int iClass = params[2];

    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);

    if (iClass){
      *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_CLASS) = iClass;
      *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_RCLASS) = 0; // disable random class
    }
    else {
      *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_RCLASS) = 1; // set random class
    }

    return 1;
  }
  return 0;
}

NATIVE(set_user_team){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    int iTeam = params[2];
    if ( iTeam<1 || iTeam>3 ){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid team id %d", iTeam);
      return 0;
    }

    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);

    pPlayer->killPlayer();
    pPlayer->pEdict->v.team = iTeam;

    switch( iTeam ){
    case 1:   pPlayer->setTeamName("Allies");
      break;
    case 2:   pPlayer->setTeamName("Axis");
      break;
    case 3: pPlayer->setTeamName("Spectators");
      break;
    }

    *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_RCLASS) = 1; // set random class

    if (params[3]){
      MESSAGE_BEGIN(MSG_ALL,gmsgPTeam);
      WRITE_BYTE(pPlayer->index);
      WRITE_BYTE( iTeam );
      MESSAGE_END();
    }
    
    return 1;
  }
  return 0;
}

NATIVE(get_user_nextclass){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    return *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_CLASS);
  }
  return 0;
}

NATIVE(is_randomclass){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    return *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_RCLASS);
  }
  return 0;
}

NATIVE(get_user_team){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);

    int iTeam = pPlayer->pEdict->v.team;
    if ( params[3] ){
      char *szTeam = "";
      switch(iTeam){
      case 1:
        szTeam = "Allies";
        break;
      case 2:
        szTeam = "Axis";
         break;
      }
      MF_SetAmxString(amx,params[2],szTeam,params[3]);
    }
    return iTeam;
  }
  return 0;
}

NATIVE(get_user_deaths){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    return *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_DEATHS );
  }
  return 0;
}

NATIVE(set_user_deaths){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);

    *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_DEATHS ) = params[2];
    if ( params[3]){
      //ScoreShort message
      MESSAGE_BEGIN(MSG_ALL,gmsgScoreShort);
      WRITE_BYTE(pPlayer->index);
      WRITE_SHORT( *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_SCORE ) );
      WRITE_SHORT((int)pPlayer->pEdict->v.frags);
      WRITE_SHORT(params[2]);
      WRITE_BYTE(pPlayer->IsAlive() ? 0 : 1);
      MESSAGE_END();
    }
    return 1;
  }
  return 0;
}

NATIVE(set_user_score){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);

    *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_SCORE ) = params[2];
    int score = params[2] - pPlayer->savedScore;
    if ( score && isModuleActive() ){
      pPlayer->updateScore(pPlayer->current,score);
    }
    pPlayer->savedScore = params[2];
    if ( params[3]){
      //ScoreShort message
      MESSAGE_BEGIN(MSG_ALL,gmsgScoreShort);
      WRITE_BYTE(pPlayer->index);
      WRITE_SHORT(params[2]);
      WRITE_SHORT((int)pPlayer->pEdict->v.frags);
      WRITE_SHORT( *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_DEATHS ) );
      WRITE_BYTE(pPlayer->IsAlive() ? 0 : 1);
      MESSAGE_END();
    }
    return 1;
  }
  return 0;
}

NATIVE(set_user_frags){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);

    pPlayer->pEdict->v.frags = (float)params[2];

    if ( params[3]){
      //ScoreShort message
      MESSAGE_BEGIN(MSG_ALL,gmsgScoreShort);
      WRITE_BYTE(pPlayer->index);
      WRITE_SHORT( *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_SCORE ) );
      WRITE_SHORT((int)pPlayer->pEdict->v.frags);
      WRITE_SHORT( *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_DEATHS ) );
      WRITE_BYTE(pPlayer->IsAlive() ? 0 : 1);
      MESSAGE_END();
    }
    return 1;
  }
  return 0;
}

NATIVE(get_user_frags){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    return (int)pPlayer->pEdict->v.frags;
  }
  return 0;
}

// By AMXMODX Dev Team
NATIVE(set_user_ammo) {
	int index = params[1];
	if(MF_IsPlayer(index, 1)) {
  	CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  	switch(params[2]) {
  		//53,284,316
  		case DODW_COLT:
  		case DODW_LUGER:
  		case DODW_WEBLEY:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 53+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 284+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 316+LINUXOFFSET ) = params[3];
  			break;
  		//54,283,315
  		case DODW_GARAND:
  		case DODW_KAR:
  		case DODW_SCOPED_KAR:
  		case DODW_ENFIELD:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 54+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 283+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 315+LINUXOFFSET ) = params[3];
  			break;
  		//57,286,318
  		case DODW_STG44:
  		case DODW_BAR:
  		case DODW_FG42:
  		case DODW_BREN:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 57+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 286+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 318+LINUXOFFSET ) = params[3];
  			break;
  		//56,281,313
  		case DODW_THOMPSON:
  		case DODW_GREASEGUN:
  		case DODW_MP40:
  		case DODW_STEN:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 56+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 281+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 313+LINUXOFFSET ) = params[3];
  			break;
  		//58,282,314
  		case DODW_K43:
  		case DODW_M1_CARBINE:
  		case DODW_MG34:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 58+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 282+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 314+LINUXOFFSET ) = params[3];
  			break;
  		//55,285,317
  		case DODW_SPRINGFIELD:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 55+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 285+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 317+LINUXOFFSET ) = params[3];
  			break;
  		//59,289,321
  		case DODW_HANDGRENADE:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 59+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 289+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 321+LINUXOFFSET ) = params[3];
  			break;
  		//61,291,323
  		case DODW_STICKGRENADE:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 61+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 291+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 323+LINUXOFFSET ) = params[3];
  			break;
  		//287,319
  		case DODW_MG42:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 287+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 319+LINUXOFFSET ) = params[3];
  			break;
  		//288,320
  		case DODW_30_CAL:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 288+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 320+LINUXOFFSET ) = params[3];
  			break;
  		//49,293,325
  		case DODW_BAZOOKA:
  		case DODW_PANZERSCHRECK:
  		case DODW_PIAT:
  			*( (int*)pPlayer->pEdict->pvPrivateData + 49+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 293+LINUXOFFSET ) = params[3];
  			*( (int*)pPlayer->pEdict->pvPrivateData + 325+LINUXOFFSET ) = params[3];
  			break;
  	}
  	return 1;
  }
	return 0;
}

// By AMXMODX Dev Team
NATIVE(get_user_ammo) {
	int index = params[1];
  if(MF_IsPlayer(index, 1)) {
  	CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  	switch(params[2]){
  		//53,284,316
  		case DODW_COLT:
  		case DODW_LUGER:
  		case DODW_WEBLEY:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 53+LINUXOFFSET );
  			break;
  		//49,293,325
  		case DODW_BAZOOKA:
  		case DODW_PANZERSCHRECK:
  		case DODW_PIAT:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 49+LINUXOFFSET );
  			break;
  		//54,283,315
  		case DODW_GARAND:
  		case DODW_KAR:
  		case DODW_SCOPED_KAR:
  		case DODW_ENFIELD:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 54+LINUXOFFSET );
  			break;
  		//55,285,317
  		case DODW_SPRINGFIELD:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 55+LINUXOFFSET );
  			break;
  		//56,281,313
  		case DODW_THOMPSON:
  		case DODW_GREASEGUN:
  		case DODW_MP40:
  		case DODW_STEN:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 56+LINUXOFFSET );
  			break;
  		//57,286,318
  		case DODW_STG44:
  		case DODW_BAR:
  		case DODW_FG42:
  		case DODW_BREN:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 57+LINUXOFFSET );
  			break;
  		//58,282,314
  		case DODW_K43:
  		case DODW_M1_CARBINE:
  		case DODW_MG34:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 58+LINUXOFFSET );
  			break;
  		//59,289,321
  		case DODW_HANDGRENADE:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 59+LINUXOFFSET );
  			break;
  		//61,291,323
  		case DODW_STICKGRENADE:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 61+LINUXOFFSET );
  			break;
  		//287,319
  		case DODW_MG42:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 287+LINUXOFFSET );
  			break;
  		//288,320
  		case DODW_30_CAL:
  			return *( (int*)pPlayer->pEdict->pvPrivateData + 288+LINUXOFFSET );
  			break;
  	}
  	return 1;
  }
  return 0;
}

NATIVE(is_weapon_deployed){
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if ( *( (int*)pPlayer->pEdict->pvPrivateData + PDOFFSET_WDEPLOY) == 1 )
      return 1;
  }
  return 0;
}

AMX_NATIVE_INFO base_natives[] = {
  { "dod_get_wpnname", get_weapon_name },
  { "dod_get_wpnlogname", get_weapon_logname },
  { "dod_wpnlog_to_name", wpnlog_to_name },
  { "dod_wpnlog_to_id", wpnlog_to_id },
  { "dod_is_melee", is_melee },

  { "dod_get_team_score", get_team_score },
  { "dod_get_user_score", get_user_score },
  { "dod_get_user_class", get_user_class },
  { "dod_get_user_team", get_user_team },
  { "dod_get_user_weapon", get_user_weapon },

  { "detect_allies_team", detect_allies_country },
  { "detect_allies_paras", detect_allies_paras },
  { "detect_axis_paras", detect_axis_paras },

  // Fun Natives
  { "set_user_stamina", set_player_stamina },
  { "reset_user_stamina", reset_player_stamina },

  { "dod_get_pronestate", get_user_pronestate },

  { "dod_user_kill", dod_user_kill },
  { "nade_set_fuse", nade_set_fuse },
  { "nade_reset_fuse", nade_reset_fuse },

  { "dod_set_user_class", set_user_class },
  { "dod_set_user_team", set_user_team },
  { "dod_get_next_class", get_user_nextclass },
  { "dod_is_randomclass", is_randomclass },
  { "dod_get_user_deaths", get_user_deaths },
  { "dod_set_user_deaths", set_user_deaths },
  { "dod_set_user_score", set_user_score },
  { "dod_get_user_kills", get_user_frags },
  { "dod_set_user_kills", set_user_frags },
  { "dod_get_user_ammo", get_user_ammo },
  { "dod_set_user_ammo", set_user_ammo },
  { "dod_is_deployed", is_weapon_deployed },

  { NULL, NULL }
};
