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

/******************************* AMX API **************************************/

void AMXAttach() {
  MF_AddNatives(csstats_natives);
}

void AMXPluginsLoaded() {
  g_Planter = 0;
  g_Defuser = 0;
  g_DefuserHasDefuseKit = 0;
  g_Vip = 0;
  g_bombForwards = 0;

  g_pFunctionTable_Post->pfnStartFrame = NULL;

  g_pengfuncsTable_Post->pfnWriteByte = NULL;
  g_pengfuncsTable_Post->pfnWriteChar = NULL;
  g_pengfuncsTable_Post->pfnWriteShort = NULL;
  g_pengfuncsTable_Post->pfnWriteLong = NULL;
  g_pengfuncsTable_Post->pfnWriteAngle = NULL;
  g_pengfuncsTable_Post->pfnWriteCoord = NULL;
  g_pengfuncsTable_Post->pfnWriteString = NULL;
  g_pengfuncsTable_Post->pfnWriteEntity = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;
  
  g_forwards.findForwards();
}

void AMXDetach() {
  if(scr_plug.program) {
    MF_UnloadAmxScript(&scr_plug.amx,&scr_plug.program);
  }
  delete_rank();
  delete_grenades();
}

/******************************* MODULE API ***********************************/

cvar_t  init_csstats_maxsize = {"csstats_maxsize", "3500", 0 , 3500.0 };
cvar_t  init_csstats_reset = {"csstats_reset", "0"};
cvar_t  init_csstats_rank = {"csstats_rank", "0"};
cvar_t  init_csstats_rankbots = {"csstats_rankbots", "1"};
cvar_t  init_csstats_pause = {"csstats_pause", "0"};
cvar_t *csstats_maxsize;
cvar_t *csstats_reset;
cvar_t *csstats_rank;
cvar_t *csstats_rankbots;
cvar_t *csstats_pause;

void MetaAttach() {
  scr_plug.program = NULL;

  CVAR_REGISTER(&init_csstats_maxsize);
  CVAR_REGISTER(&init_csstats_reset);
  CVAR_REGISTER(&init_csstats_rank);
  CVAR_REGISTER(&init_csstats_rankbots);
  CVAR_REGISTER(&init_csstats_pause);
  csstats_maxsize=CVAR_GET_POINTER(init_csstats_maxsize.name);
  csstats_reset=CVAR_GET_POINTER(init_csstats_reset.name);
  csstats_rank=CVAR_GET_POINTER(init_csstats_rank.name);
  csstats_rankbots=CVAR_GET_POINTER(init_csstats_rankbots.name);
  csstats_pause=CVAR_GET_POINTER(init_csstats_pause.name);

  REG_SVR_COMMAND("csstats_savetext", csstats_savetext_command);
  REG_SVR_COMMAND("csstats_loadtext", csstats_loadtext_command);
  REG_SVR_COMMAND("csstats_savesv", csstats_savesv_command);
}


Forwards g_forwards;

function_event_call modMsgsEnd[MAX_REG_MSGS];
function_event_call modMsgs[MAX_REG_MSGS];
void (*function)(void*);
void (*endfunction)(void*);
player_t* mPlayer;
int mPlayerIndex;
int mState;
//bool rankBots;
player_t players[33];

int gmsgCurWeapon;
int gmsgDeathMsg;
int gmsgDamage;
int gmsgDamageEnd;
int gmsgWeaponList;
int gmsgResetHUD;
int gmsgAmmoX;
int gmsgScoreInfo;
int gmsgAmmoPickup;
int gmsgWeapPickup;
int gmsgTextMsg;
int gmsgSendAudio;
int gmsgStatusIcon;
int gmsgBarTime;
int gmsgMoney;
int gmsgScoreAttrib;

int g_bombForwards;
int g_Planter;
int g_Defuser;
int g_DefuserHasDefuseKit;
int g_Vip;

bool g_LoadText;
char g_LoadTextFile[256];

/************************* Begin Hooked Functions *****************************/

struct sUserMsg {
  const char* name;
  int* id;
  funEventCall func;
  bool endmsg;
} g_user_msg[] = {
  { "TextMsg",&gmsgTextMsg,Client_TextMsg,false },
  { "CurWeapon",&gmsgCurWeapon,Client_CurWeapon,false },
  { "Damage",&gmsgDamage,Client_Damage,false },
  { "Damage",&gmsgDamageEnd,Client_Damage_End,true },
  { "WeaponList",&gmsgWeaponList,Client_WeaponList,false },
  { "ResetHUD",&gmsgResetHUD,Client_ResetHUD_End,true },
  { "AmmoX",&gmsgAmmoX,Client_AmmoX,false },
  { "ScoreInfo",&gmsgScoreInfo,Client_ScoreInfo,false},
  { "AmmoPickup",&gmsgAmmoPickup,Client_AmmoPickup,false},
  { "WeapPickup",&gmsgWeapPickup,Client_WeapPickup,false },
  { "SendAudio",&gmsgSendAudio,Client_SendAudio,false },
  { "StatusIcon",&gmsgStatusIcon,Client_StatusIcon,false },
  { "BarTime",&gmsgBarTime,Client_BarTime,false },
  { "Money",&gmsgMoney,Client_Money,false },
  { "ScoreAttrib",&gmsgScoreAttrib,Client_ScoreAttrib,false },
  { 0,0,0,false }
};

bool PlayerIsAlive(edict_t *pEdict){
  return (((pEdict->v.deadflag==DEAD_NO)&&(pEdict->v.health>0)) ? true : false);
}

/*bool PlayerIsBot(edict_t *pEdict){
  return (pEdict->v.flags & FL_FAKECLIENT) ? true : false;
}*/

void PlayerDisconnect(player_t* pPlayer) {
  if ( !ignoreBots(pPlayer->index/*pPlayer->pEdict*/) && isModuleActive() ){
    pPlayer->life.playtime += (int)((gpGlobals->time - pPlayer->playtime) / 60.0f);
    score_stats_in_rank(pPlayer->name, pPlayer->rank, &pPlayer->life);
  }
  pPlayer->ingame = false;
  pPlayer->reset_hud = false;
}

void PlayerPutInServer(player_t* pPlayer) {
  pPlayer->ingame = true;
  pPlayer->playtime = gpGlobals->time;
  if ( ignoreBots(pPlayer->index/*pPlayer->pEdict*/) )
    return;
  const char* unique = NULL;
  switch((int)csstats_rank->value) {
    case 1:
      unique = GETPLAYERAUTHID(pPlayer->pEdict);
      break;
    case 2:
      unique = pPlayer->ip;
      break;
    default:
      unique = pPlayer->name;
  }
  if ( !unique )  unique = "";
  pPlayer->rank = find_name_in_rank( unique , pPlayer->name );
}

void PlayerConnect(player_t* pPlayer,const char* name, const char* ip) {
  //pPlayer->bot = PlayerIsBot(pPlayer->pEdict);
  strcpy(pPlayer->name,name);
  strcpy(pPlayer->ip,ip);
  memset(pPlayer->weapons,0,sizeof(stats_t)*MAX_WEAPONS);
  memset(pPlayer->weapons_rnd,0,sizeof(stats_t)*MAX_WEAPONS);   //DEC-Weapon (Round) stats
  memset(pPlayer->attackers,0,sizeof(stats_t)*33);
  memset(pPlayer->victims,0,sizeof(stats_t)*33);
  memset(&pPlayer->life,0,sizeof(stats_t));
}

int RegUserMsg_Post(const char *pszName, int iSize){
  for (int i = 0; g_user_msg[ i ].name; ++i ){
    if ( !*g_user_msg[i].id && strcmp( g_user_msg[ i ].name , pszName  ) == 0 ){
      int id = META_RESULT_ORIG_RET( int );

      *g_user_msg[ i ].id = id;

      if ( g_user_msg[ i ].endmsg )
        modMsgsEnd[ id ] = g_user_msg[ i ].func;
      else
        modMsgs[ id ] = g_user_msg[ i ].func;

      //break;
    }
  }

  RETURN_META_VALUE(MRES_IGNORED, 0);
}

BOOL ClientConnect_Post( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  ){
  PlayerConnect(GET_PLAYER_POINTER(pEntity),pszName,pszAddress);
  RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

void ClientDisconnect( edict_t *pEntity ) {
  player_t *pPlayer = GET_PLAYER_POINTER(pEntity);
  if (pPlayer->ingame) PlayerDisconnect(pPlayer);
  RETURN_META(MRES_IGNORED);
}

void ClientPutInServer_Post( edict_t *pEntity ) {
  PlayerPutInServer(GET_PLAYER_POINTER(pEntity));
  RETURN_META(MRES_IGNORED);
}

void ClientUserInfoChanged_Post( edict_t *pEntity, char *infobuffer ) {
  player_t *pPlayer = GET_PLAYER_POINTER(pEntity);
  const char* name = INFOKEY_VALUE(infobuffer,"name");
  if ( strcmp(pPlayer->name,name) != 0 ) {
    strcpy(pPlayer->name,name);
    if (csstats_rank->value == 0 && pPlayer->ingame)
      pPlayer->rank = find_name_in_rank(name, name);
  }
  RETURN_META(MRES_IGNORED);
}

void ClientKill(edict_t *pEntity){
  player_t* pPlayer = GET_PLAYER_POINTER(pEntity);
  if(!PlayerIsAlive(pEntity))
    RETURN_META(MRES_IGNORED);

  if(g_forwards.forwardsExist(FF_DamageInfo) == true) {
    g_forwards.executeForward(FF_DamageInfo, pPlayer->index,pPlayer->index, 0, 0, 0, 0);
  }
  if(isModuleActive()) {
    fire_kill(pPlayer, pPlayer, 0, 0, 0);
  }
  if(g_forwards.forwardsExist(FF_DeathInfo) == true) {
    g_forwards.executeForward(FF_DeathInfo, pPlayer->index, pPlayer->index, 0, 0, 0);
  }

  RETURN_META(MRES_IGNORED);
}

void MessageBegin_Post(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) {
  if (ed){
    mPlayerIndex = ENTINDEX(ed);
    mPlayer = GET_PLAYER_POINTER_I(mPlayerIndex);
  } else {
    mPlayerIndex = 0;
    mPlayer = NULL;
  }
  mState = 0;
  if ( msg_type < 0 || msg_type >= MAX_REG_MSGS )
    msg_type = 0;
  function=modMsgs[msg_type];
  endfunction=modMsgsEnd[msg_type];

  if(function) {
    g_pengfuncsTable_Post->pfnWriteByte = WriteByte_Post;
    g_pengfuncsTable_Post->pfnWriteChar = WriteChar_Post;
    g_pengfuncsTable_Post->pfnWriteShort = WriteShort_Post;
    g_pengfuncsTable_Post->pfnWriteLong = WriteLong_Post;
    g_pengfuncsTable_Post->pfnWriteAngle = WriteAngle_Post;
    g_pengfuncsTable_Post->pfnWriteCoord = WriteCoord_Post;
    g_pengfuncsTable_Post->pfnWriteString = WriteString_Post;
    g_pengfuncsTable_Post->pfnWriteEntity = WriteEntity_Post;
    g_pengfuncsTable_Post->pfnMessageEnd = MessageEnd_Post;
  }
  if(endfunction) {
    g_pengfuncsTable_Post->pfnMessageEnd = MessageEnd_Post;
  }

  RETURN_META(MRES_IGNORED);
}

void WriteByte_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}
void WriteChar_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}
void WriteShort_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}
void WriteLong_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}
void WriteAngle_Post(float flValue) {
  if (function) (*function)((void *)&flValue);
  RETURN_META(MRES_IGNORED);
}
void WriteCoord_Post(float flValue) {
  if (function) (*function)((void *)&flValue);
  RETURN_META(MRES_IGNORED);
}
void WriteString_Post(const char *sz) {
  if (function) (*function)((void *)sz);
  RETURN_META(MRES_IGNORED);
}
void WriteEntity_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}

void MessageEnd_Post(void) {
  if (endfunction) (*endfunction)(NULL);
  g_pengfuncsTable_Post->pfnWriteByte = NULL;
  g_pengfuncsTable_Post->pfnWriteChar = NULL;
  g_pengfuncsTable_Post->pfnWriteShort = NULL;
  g_pengfuncsTable_Post->pfnWriteLong = NULL;
  g_pengfuncsTable_Post->pfnWriteAngle = NULL;
  g_pengfuncsTable_Post->pfnWriteCoord = NULL;
  g_pengfuncsTable_Post->pfnWriteString = NULL;
  g_pengfuncsTable_Post->pfnWriteEntity = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;
  RETURN_META(MRES_IGNORED);
}

void SetModel_Post(edict_t *e, const char *m){
  if(e->v.owner && m[7] == 'w' && m[8] == '_') {
    int wpnId = 0;
    player_t* pPlayer = GET_PLAYER_POINTER(e->v.owner);
    switch(m[9]) {
      case 'h':
        wpnId = CSW_HEGRENADE;
        put_grenade(e, 1.75, CSW_HEGRENADE, pPlayer);
        if(isModuleActive()) {
          fire_shot(pPlayer, CSW_HEGRENADE);
        }
        break;
      case 'f':
        if(m[10]=='l') {
          wpnId = CSW_FLASHBANG;
        }
        break;
      case 's':
        if(m[10]=='m') {
          wpnId = CSW_SMOKEGRENADE;
        }
        break;
    }
    if(wpnId && g_forwards.forwardsExist(FF_GrenadeThrow) == true) {
      g_forwards.executeForward(FF_GrenadeThrow, pPlayer->index, ENTINDEX(e), wpnId);
    }
  }

  RETURN_META(MRES_IGNORED);
}

void EmitSound_Post(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch) {
  if(!isModuleActive())
    RETURN_META(MRES_IGNORED);

  if (sample[0]=='w' && sample[1]=='e' && sample[8]=='k' && sample[9]=='n' && sample[14]!='d') {
    player_t*pPlayer = GET_PLAYER_POINTER(entity);
    fire_shot(pPlayer, pPlayer->weapon->id);
  }
  RETURN_META(MRES_IGNORED);
}

void TraceLine_Post(const float *v1, const float *v2, int fNoMonsters, edict_t *e, TraceResult *ptr) {
  if(!isModuleActive())
    RETURN_META(MRES_IGNORED);

  if(ptr->pHit && (ptr->pHit->v.flags & (FL_CLIENT | FL_FAKECLIENT)) && e && (e->v.flags & (FL_CLIENT | FL_FAKECLIENT))) {
    GET_PLAYER_POINTER(e)->aiming = ptr->iHitgroup;
  }
  RETURN_META(MRES_IGNORED);
}

void PlayerPreThink_Post(edict_t *pEntity) {
  if(!isModuleActive())
    RETURN_META(MRES_IGNORED);

  player_t *pPlayer = GET_PLAYER_POINTER(pEntity);
  if (pPlayer->clear_stats && pPlayer->clear_stats < gpGlobals->time){
    pPlayer->clear_stats = 0.0;
    memset(pPlayer->attackers,0,sizeof(stats_t)*33);
    memset(pPlayer->victims ,0,sizeof(stats_t)*33);
    memset(&pPlayer->life,0,sizeof(stats_t));
    memset(pPlayer->weapons_rnd,0,sizeof(stats_t)*MAX_WEAPONS);   //DEC-Weapon (Round) stats
  }
  else if ( pPlayer->reset_hud ) {
    pPlayer->reset_hud = false;
    if ( pPlayer->ingame ) {
      pPlayer->clear_stats = gpGlobals->time + 0.25;
      if ( !ignoreBots(pPlayer->index/*pEntity*/) ){
        score_stats_in_rank(pPlayer->name, pPlayer->rank, &pPlayer->life);
      }
    }
    else if ( MF_IsPlayerBot(pPlayer->index) ) {
      PlayerConnect(pPlayer,ENTITY_KEYVALUE( pEntity ,"name" ), "localhost");
      PlayerPutInServer(pPlayer);
    }
  }
  RETURN_META(MRES_IGNORED);
}

// By AMXMODX Dev Team
void StartFrame_Post() {
	if(g_bombForwards){
		switch(g_bombForwards) {
  		case BOMB_PLANTING:
  			if(g_Planter) g_forwards.executeForward(FF_BombPlanting, g_Planter);
  			break;
  		case BOMB_PLANTED:
  			if(g_Planter) g_forwards.executeForward(FF_BombPlanted, g_Planter);
  			break;
  		case BOMB_EXPLODED:
  			if(g_Planter) g_forwards.executeForward(FF_BombExploded, g_Planter, g_Defuser);
  			break;
  		case BOMB_DEFUSING:
  			if(g_Defuser) g_forwards.executeForward(FF_BombDefusing, g_Defuser, g_DefuserHasDefuseKit);
  			break;
  		case BOMB_DEFUSED:
  			if(g_Defuser) g_forwards.executeForward(FF_BombDefused, g_Defuser, g_DefuserHasDefuseKit);
  			break;
		}
		g_bombForwards = 0;
		g_pFunctionTable_Post->pfnStartFrame = NULL;
	}
	RETURN_META(MRES_IGNORED);
}

void ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
  int id;
  for(int i = 0; g_user_msg[ i ].name; ++i )  {
    if(( *g_user_msg[ i ].id == 0 ) && ( id = GET_USER_MSG_ID(PLID, g_user_msg[ i ].name, NULL) ) != 0) {
      *g_user_msg[ i ].id = id;

      if( g_user_msg[ i ].endmsg ) {
        modMsgsEnd[ id ] = g_user_msg[ i ].func;
      }
      else {
        modMsgs[ id ] = g_user_msg[ i ].func;
      }
    }
  }

  memset(players,0,sizeof(player_t)*33);
  const char* path =  MF_GetLocalInfo("csstats_score","$pluginsdir/csstats.amx");
  if(path && *path) {
    char error[128];
    load_get_score_plugin(MF_BuildPathname("%s",path), error);
  }
  load_rank(MF_BuildPathname("%s", MF_GetLocalInfo("csstats","$modulesdir/csstats.dat")));

  g_Planter = 0;
  g_Defuser = 0;
  g_DefuserHasDefuseKit = 0;
  g_Vip = 0;
  g_bombForwards = 0;

  RETURN_META(MRES_IGNORED);
}

void ServerActivate_Post( edict_t *pEdictList, int edictCount, int clientMax ){
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    player_t *pPlayer = GET_PLAYER_POINTER_I(i);
    pPlayer->pEdict = &pEdictList[i];
    pPlayer->weapon = &pPlayer->weapons[0];
    pPlayer->index = i;
  }

  g_LoadText = false;

  RETURN_META(MRES_IGNORED);
}

void ServerDeactivate() {
  for(int i = 1;i<=gpGlobals->maxClients; ++i){
    player_t *pPlayer = GET_PLAYER_POINTER_I(i);
    if (pPlayer->ingame) PlayerDisconnect(pPlayer);
  }

  if(g_LoadText == true) {
    if(rank_text_to_binary(g_LoadTextFile) == true) {
      MF_PrintSrvConsole("[AMX] Successfully converted \"%s\" to csstats.dat\n", g_LoadTextFile);
    }
    else {
      MF_PrintSrvConsole("[AMX] Couldn't convert \"%s\" to csstats.dat\n", g_LoadTextFile);
      save_rank(MF_BuildPathname("%s", MF_GetLocalInfo("csstats","$modulesdir/csstats.dat")));
    }
  }
  else {
    save_rank(MF_BuildPathname("%s", MF_GetLocalInfo("csstats","$modulesdir/csstats.dat")));
  }
  if(scr_plug.program) {
    MF_UnloadAmxScript(&scr_plug.amx,&scr_plug.program);
  }

  delete_rank();
  delete_grenades();
  g_forwards.clear();

  g_Planter = 0;
  g_Defuser = 0;
  g_DefuserHasDefuseKit = 0;
  g_Vip = 0;
  g_bombForwards = 0;

  g_pFunctionTable_Post->pfnStartFrame = NULL;

  g_pengfuncsTable_Post->pfnWriteByte = NULL;
  g_pengfuncsTable_Post->pfnWriteChar = NULL;
  g_pengfuncsTable_Post->pfnWriteShort = NULL;
  g_pengfuncsTable_Post->pfnWriteLong = NULL;
  g_pengfuncsTable_Post->pfnWriteAngle = NULL;
  g_pengfuncsTable_Post->pfnWriteCoord = NULL;
  g_pengfuncsTable_Post->pfnWriteString = NULL;
  g_pengfuncsTable_Post->pfnWriteEntity = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;

  RETURN_META(MRES_IGNORED);
}

void csstats_savetext_command() {
  const char* filename = CMD_ARGV(1);
  if(save_rank_text(MF_BuildPathname("%s", filename)) == true) {
    MF_PrintSrvConsole("[AMX] csstats.dat has been saved to \"%s\"\n", filename);
  }
}

void csstats_loadtext_command() {
  const char* filename = CMD_ARGV(1);
  if(strcmp(filename, "")) {
    const char* path = MF_BuildPathname("%s", filename);
    if(is_valid_text_file(path) == false) {
      return;
    }
    snprintf(g_LoadTextFile,255,"%s",path);
    g_LoadText = true;
    MF_PrintSrvConsole("[AMX] \"%s\" will be loaded at the end of the current map\n", g_LoadTextFile);
  }
  else if(!strcmp(filename, "") && g_LoadText == true) {
    g_LoadText = false;
    MF_PrintSrvConsole("[AMX] \"%s\" will not be loaded any more at the end of the current map\n", g_LoadTextFile);
  }
}

void csstats_savesv_command() {
  const char* filename = CMD_ARGV(1);
  const char* parsertype = CMD_ARGV(2);
  if(save_rank_sv(MF_BuildPathname("%s", filename), parsertype ? atoi(parsertype) : 0) == true) {
    MF_PrintSrvConsole("[AMX] csstats.dat has been saved to \"%s\"\n", filename);
  }
}

/************************* End Hooked Functions *******************************/
