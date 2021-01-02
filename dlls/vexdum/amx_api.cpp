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

#include "vexdum.h"

/******************************* AMX API **************************************/

void AMXAttach() {
  MF_AddNatives(VexdUM_Gen);
  MF_AddNatives(VexdUM_Ent);
  MF_AddNatives(VexdUM_Dmg);
  MF_AddNatives(VexdUM_Bot);
  MF_AddNatives(VexdUM_Misc);
  MF_AddNatives(VexdUM_Globals);
  MF_AddNatives(VexdUM_Tr);
	MF_AddNatives(VexdUM_Meta);
}

void AMXPluginsLoaded() {
  char modName[32];
	MF_GetModName(modName);
	if(!stricmp(modName, "cstrike") || !stricmp(modName, "czero")) {
    g_bmod_cstrike = true;
  }
  else {
    g_bmod_cstrike = false;
  }
  
  // Optimization idea from AMXMODX
  g_pFunctionTable->pfnClientKill = NULL;
	g_pFunctionTable->pfnPlayerPostThink = NULL;
	g_pFunctionTable->pfnPlayerPreThink = NULL;
	g_pengfuncsTable->pfnEmitSound = NULL;
	g_pengfuncsTable->pfnEmitAmbientSound = NULL;
	g_pFunctionTable->pfnThink = NULL;
	g_pFunctionTable->pfnTouch = NULL;
	g_pFunctionTable->pfnSpawn = NULL;
	g_pFunctionTable_Post->pfnStartFrame = NULL;
	g_pengfuncsTable->pfnSetModel = NULL;
	g_pengfuncsTable->pfnTraceLine = NULL;
	g_pengfuncsTable->pfnSetClientKeyValue = NULL;
	g_pFunctionTable->pfnKeyValue = NULL;
	g_pFunctionTable->pfnGetGameDescription = NULL;
	g_pFunctionTable->pfnClientUserInfoChanged = NULL;
  g_pengfuncsTable->pfnVoice_SetClientListening = NULL;
  g_pengfuncsTable->pfnAlertMessage = NULL;
  g_pengfuncsTable->pfnClientPrintf = NULL;
	g_pFunctionTable->pfnAddToFullPack = NULL;

  g_pengfuncsTable->pfnWriteByte = NULL;
  g_pengfuncsTable->pfnWriteChar = NULL;
  g_pengfuncsTable->pfnWriteShort = NULL;
  g_pengfuncsTable->pfnWriteLong = NULL;
  g_pengfuncsTable->pfnWriteAngle = NULL;
  g_pengfuncsTable->pfnWriteCoord = NULL;
  g_pengfuncsTable->pfnWriteString = NULL;
  g_pengfuncsTable->pfnWriteEntity = NULL;
  g_pengfuncsTable->pfnMessageEnd = NULL;
  
  g_forwards.findForwards();
  
	if(g_forwards.forwardsExist(FF_ClientKill) == true)
		g_pFunctionTable->pfnClientKill=ClientKill;
		
	if(g_forwards.forwardsExist(FF_ClientPostThink) == true)
		g_pFunctionTable->pfnPlayerPostThink=PlayerPostThink;
		
	if(g_forwards.forwardsExist(FF_ClientPreThink) == true)
		g_pFunctionTable->pfnPlayerPreThink=PlayerPreThink;
		
	if(g_forwards.forwardsExist(FF_EmitSound) == true)
		g_pengfuncsTable->pfnEmitSound=EmitSound;
		
	if(g_forwards.forwardsExist(FF_EmitAmbientSound) == true)
		g_pengfuncsTable->pfnEmitAmbientSound=EmitAmbientSound;
		
	if(g_forwards.forwardsExist(FF_EntityThink) == true)
		g_pFunctionTable->pfnThink=EntityThink;
		
	if(g_forwards.forwardsExist(FF_EntityTouch) == true)
		g_pFunctionTable->pfnTouch=EntityTouch;
		
	if(g_forwards.forwardsExist(FF_EntitySpawn) == true)
		g_pFunctionTable->pfnSpawn=EntitySpawn;
		
	if(g_forwards.forwardsExist(FF_ServerFrame) == true)
		g_pFunctionTable_Post->pfnStartFrame=StartFrame_Post;
		
	if(g_forwards.forwardsExist(FF_SetModel) == true)
		g_pengfuncsTable->pfnSetModel=SetModel;
		
	if(g_forwards.forwardsExist(FF_TraceLine) == true)
		g_pengfuncsTable->pfnTraceLine=TraceLine;
		
	if(g_forwards.forwardsExist(FF_SetClientKeyValue) == true)
		g_pengfuncsTable->pfnSetClientKeyValue=SetClientKeyValue;
		
	if(g_forwards.forwardsExist(FF_KeyValue) == true)
		g_pFunctionTable->pfnKeyValue=KeyValue;
		
	if(g_forwards.forwardsExist(FF_GetGameDescription) == true)
		g_pFunctionTable->pfnGetGameDescription=GetGameDescription;

	if(g_forwards.forwardsExist(FF_AddToFullPack) == true)
		g_pFunctionTable->pfnAddToFullPack=AddToFullPack;
}

/******************************* MODULE API ***********************************/

cvar_t init_friendlyfire = {"sv_friendlyfire", "0", FCVAR_SERVER | FCVAR_SPONLY};
cvar_t init_knockback1 = {"sv_knockback1", "12", FCVAR_SPONLY};
cvar_t init_knockback2 = {"sv_knockback2", "4", FCVAR_SPONLY};
cvar_t init_vxdebug = {"vx_debug", "0", FCVAR_SPONLY};
cvar_t *friendlyfire;
cvar_t *knockback1;
cvar_t *knockback2;
cvar_t *vxdebug;

void MetaAttach() {
  CVAR_REGISTER(&init_friendlyfire);
	CVAR_REGISTER(&init_knockback1);
	CVAR_REGISTER(&init_knockback2);
	CVAR_REGISTER(&init_vxdebug);
  friendlyfire=CVAR_GET_POINTER(init_friendlyfire.name);
  knockback1=CVAR_GET_POINTER(init_knockback1.name);
  knockback2=CVAR_GET_POINTER(init_knockback2.name);
  vxdebug=CVAR_GET_POINTER(init_vxdebug.name);
}


StringStorage stringVault;
Forwards g_forwards;

funEventCall modMsgsEnd[MAX_REG_MSGS];
funEventCall modMsgs[MAX_REG_MSGS];
void (*function)(void*);
void (*endfunction)(void*);
CPlayer* mPlayer;
int mPlayerId;
int mState;
CPlayer players[33];
GlobalInfo gInfo;
bool g_bmod_cstrike;
KeyValueData *g_pkvd;
bool g_inKeyValue = false;
bool g_inGameDescription = false;
char g_GameDescription[128];
ForwardReturn g_FwdReturnVal;

int gmsgResetHUD;
int gmsgScoreInfo;
int gmsgTeamInfo;
int gmsgDeathMsg;
int gmsgDamage;
int gmsgTextMsg;
int gmsgSendAudio;

/************************* Begin Hooked Functions *****************************/

BOOL ClientConnect_Post(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  //pPlayer->Connect();
  pPlayer->ResetData();
  RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

void ClientPutInServer_Post(edict_t *pEntity) {
  GET_PLAYER_POINTER(pEntity)->PutInServer();
  RETURN_META(MRES_IGNORED);
}

// Check if they are using a model, if so, don't let CS know.
// HACKHACK: this might mess up some stuff with other infobuffers,
// ie you may not see when a modeled player changes their name, etc.
void ClientUserInfoChanged(edict_t *pEntity, char *infobuffer) {
  META_RES result = MRES_IGNORED;
  if(g_bmod_cstrike) {
    if(GET_PLAYER_POINTER(pEntity)->modeled && GET_PLAYER_POINTER(pEntity)->SetModel > 0 && pEntity->v.deadflag == DEAD_NO)
      result = MRES_SUPERCEDE;
  }
  else {
    if(GET_PLAYER_POINTER(pEntity)->modeled && pEntity->v.deadflag == DEAD_NO)
      result = MRES_SUPERCEDE;
  }

  RETURN_META(result);
}

// Don't ask, bots need this for reinitialising :/
void ClientUserInfoChanged_Post(edict_t *pEntity, char *infobuffer) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  if(MF_IsPlayerBot(pPlayer->id)) {
    //pPlayer->bot = true;
    pPlayer->PutInServer();
  }

  RETURN_META(MRES_IGNORED);
}

void ClientDisconnect(edict_t *pEntity) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  if(pPlayer->ingame) {
    pPlayer->Disconnect();
    pPlayer->ResetData();
  }

  RETURN_META(MRES_IGNORED);
}

// pfnClientKill, called when a player types kill in console
void ClientKill(edict_t *pEntity) {
  META_RES result = g_forwards.executeForward(FF_ClientKill, ENTINDEX(pEntity));
  RETURN_META(result);
}

// pfnTouch, this is a forward that is called whenever 2 entities collide.
void EntityTouch(edict_t *pToucher, edict_t *pTouched) {
  META_RES result = g_forwards.executeForward(FF_EntityTouch, ENTINDEX(pToucher), ENTINDEX(pTouched));
  RETURN_META(result);
}

// pfnThink, this is a forward that is called for npc entities for thinking.
void EntityThink(edict_t *pEntity) {
  META_RES result = g_forwards.executeForward(FF_EntityThink, ENTINDEX(pEntity));
  RETURN_META(result);
}

// pfnSpawn, this is a forward that is called when an entity is spawned.
int EntitySpawn(edict_t *pEntity) {
  META_RES result = g_forwards.executeForward(FF_EntitySpawn, ENTINDEX(pEntity));
  RETURN_META_VALUE(result, 0);
}

// pfnPlayerPreThink
void PlayerPreThink(edict_t *pEntity) {
  META_RES result = g_forwards.executeForward(FF_ClientPreThink, ENTINDEX(pEntity));
  RETURN_META(result);
}

// pfnPlayerPostThink, this code is to set the model at a specified time.
// The second part of the code activates the plugin forward functionality.
void PlayerPostThink(edict_t *pEntity) {
  if(g_bmod_cstrike) {
    CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
    if(pPlayer->fModelSet > 0.0f && pPlayer->fModelSet < gpGlobals->time) {
      if(pPlayer->UnSetModel == true) {
        MDLL_ClientUserInfoChanged(pPlayer->pEdict, GET_INFOKEYBUFFER(pPlayer->pEdict));
        pPlayer->UnSetModel = false;
        pPlayer->SetModel = 0;
        pPlayer->fModelSet = 0.0f;
        if(countModelled(true) == 0 && g_forwards.forwardsExist(FF_ClientPostThink) == false) {
          g_pFunctionTable->pfnPlayerPostThink = NULL;
        }
      }
      else if(pPlayer->modeled && (pPlayer->SetModel == 2 || pPlayer->SetModel == -1)) {
        if(strcmp(pPlayer->modelname, GETCLIENTKEYVALUE(GET_INFOKEYBUFFER(pPlayer->pEdict), "model")) != 0) {
          SET_CLIENT_KEYVALUE(pPlayer->id, GET_INFOKEYBUFFER(pPlayer->pEdict), "model", pPlayer->modelname);
          pPlayer->SetModel = 1;
        }
      }
    }
  }
  else {
    CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
    if(pPlayer->modeled && strcmp(pPlayer->modelname, GETCLIENTKEYVALUE(GET_INFOKEYBUFFER(pPlayer->pEdict), "model")) != 0) {
      SET_CLIENT_KEYVALUE(pPlayer->id, GET_INFOKEYBUFFER(pPlayer->pEdict), "model", pPlayer->modelname);
    }
  }

  META_RES result = MRES_IGNORED;
  if(g_forwards.forwardsExist(FF_ClientPostThink) == true)
    result = g_forwards.executeForward(FF_ClientPostThink, ENTINDEX(pEntity));
  RETURN_META(result);
}

// This is called once every server frame. This code resets the lights once every second.
// this is so joining players will see ther correct lighting.
void StartFrame_Post(void) {
  if(*gInfo.szLastLights) {
    if(gInfo.fNextLights < gpGlobals->time) {
      LIGHT_STYLE(0, gInfo.szLastLights);
      gInfo.fNextLights = gpGlobals->time + 1;
    }
  }
  if(g_forwards.forwardsExist(FF_ServerFrame) == true)
    g_forwards.executeForward(FF_ServerFrame);

  RETURN_META(MRES_IGNORED);
}

void AlertMessage(ALERT_TYPE atype, char *szFmt, ...) {
  META_RES result = MRES_IGNORED;
  
  if(g_forwards.forwardsExist(FF_AlertMessage) == true) {
    va_list argptr;
    static char string[1024];
    va_start(argptr, szFmt);
    vsprintf(string, szFmt, argptr);
    va_end(argptr);
    result = g_forwards.executeForward(FF_AlertMessage, (int)atype, string);
  }

  if(gInfo.logBlock) {
    gInfo.logBlock = false;
    result = MRES_SUPERCEDE;
  }

  RETURN_META(result);
}

void ClientPrintf(edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg) {
  META_RES result = MRES_IGNORED;
  if(gInfo.logBlock) {
    result = MRES_SUPERCEDE;
  }

  RETURN_META(result);
}

// make sure that if we currently have an edited light value, to use it.
void LightStyle(int style, char *val) {
  if(style == 0) strcpy(gInfo.szRealLights, val);

  RETURN_META(MRES_IGNORED);
}

// This checks who can hear who through voice comm. this reads flags set,
// and lets us choose who hears who based on the previously set flags.
qboolean Voice_SetClientListening(int iReceiver, int iSender, qboolean bListen) {
  CPlayer *pPlayerR = GET_PLAYER_POINTER_I(iReceiver);
  CPlayer *pPlayerS = GET_PLAYER_POINTER_I(iSender);

  if( ((int)pPlayerS->speakflags[0] & SPEAK_MUTED) != 0 || ((int)pPlayerS->speakflags[iReceiver] & SPEAK_MUTED) != 0 ) {
    SET_CLIENT_LISTEN(iReceiver, iSender, false);
    RETURN_META_VALUE(MRES_SUPERCEDE, false);
  }
  if( ((int)pPlayerS->speakflags[0] & SPEAK_ALL) != 0 || ((int)pPlayerS->speakflags[iReceiver] & SPEAK_ALL) != 0 ) {
    SET_CLIENT_LISTEN(iReceiver, iSender, true);
    RETURN_META_VALUE(MRES_SUPERCEDE, true);
  }
  if( ((int)pPlayerR->speakflags[0] & SPEAK_LISTENALL) != 0 || ((int)pPlayerR->speakflags[iSender] & SPEAK_LISTENALL) != 0 ) {
    SET_CLIENT_LISTEN(iReceiver, iSender, true);
    RETURN_META_VALUE(MRES_SUPERCEDE, true);
  }

  RETURN_META_VALUE(MRES_IGNORED, bListen);
}

void EmitSound(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch) {
  META_RES result = g_forwards.executeForward(FF_EmitSound, ENTINDEX(entity), sample);
  RETURN_META(result);
}

void EmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch) {
  META_RES result = g_forwards.executeForward(FF_EmitAmbientSound, ENTINDEX(entity), samp);
  RETURN_META(result);
}

void SetModel(edict_t *pEdict, const char *model) {
  META_RES result = g_forwards.executeForward(FF_SetModel, ENTINDEX(pEdict), model);
  RETURN_META(result);
}

void TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *e, TraceResult *ptr) {
  TRACE_LINE(v1, v2, fNoMonsters, e, ptr); 
  vx_tr = ptr;

  META_RES result = g_forwards.executeForward(FF_TraceLine, ENTINDEX(e));
  RETURN_META(result);
}

void SetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value) {
  META_RES result = g_forwards.executeForward(FF_SetClientKeyValue, clientIndex, key, value);
  RETURN_META(result);
}

// By AMXMODX Dev Team
void KeyValue(edict_t *entity, KeyValueData *pkvd) {
	g_inKeyValue = true;
	g_pkvd = pkvd;
	META_RES result = g_forwards.executeForward(FF_KeyValue, ENTINDEX(entity));
	g_inKeyValue = false;
  RETURN_META(result);
}

const char* GetGameDescription(void) {
  g_inGameDescription = true;
  g_GameDescription[0] = '\0';
	META_RES result = g_forwards.executeForward(FF_GetGameDescription, 0);
	g_inGameDescription = false;
  RETURN_META_VALUE(result, g_GameDescription);
}

int AddToFullPack(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet) {
	META_RES result = g_forwards.executeForward(FF_AddToFullPack, (cell)state, e, ENTINDEX(ent), ENTINDEX(host), hostflags, player, (cell)pSet);
	RETURN_META_VALUE(result, g_FwdReturnVal.iVal);
}

// Engine message functions.
void MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) {
  META_RES result = MRES_IGNORED;
  if(ed) {
    mPlayerId = ENTINDEX(ed);
    mPlayer = GET_PLAYER_POINTER_I(mPlayerId);
  }
  else {
    mPlayerId = 0;
    mPlayer = 0;
  }

  if(g_bmod_cstrike) {
    if(msg_type == gmsgResetHUD && ed) {
      if(mPlayer->modeled) {
        if(countModelled(false) >= MAX_MODELLED) {
          if(mPlayer->SetModel != -2) {
            mPlayer->fModelSet = gpGlobals->time + RANDOM_FLOAT(0.18f, 0.28f);
            mPlayer->SetModel = -1;
          }
        }
        else {
          mPlayer->fModelSet = gpGlobals->time + 0.05f;
          mPlayer->SetModel = -1;
        }
      }
    }
  }

  mState = 0;
  if( msg_type < 0 || msg_type >= MAX_REG_MSGS ) {
    msg_type = 0;
  }
  function=modMsgs[ msg_type ];
  endfunction=modMsgsEnd[ msg_type ];

  if(gInfo.iMessageBlock[ msg_type ]) {
    gInfo.msgBlock = true;
    result = MRES_SUPERCEDE;

    if(gInfo.iMessageBlock[ msg_type ] == BLOCK_ONCE) {
      gInfo.iMessageBlock[ msg_type ] = BLOCK_NOT;
    }
  }
  
  if(function || gInfo.msgBlock) {
    g_pengfuncsTable->pfnWriteByte = WriteByte;
    g_pengfuncsTable->pfnWriteChar = WriteChar;
    g_pengfuncsTable->pfnWriteShort = WriteShort;
    g_pengfuncsTable->pfnWriteLong = WriteLong;
    g_pengfuncsTable->pfnWriteAngle = WriteAngle;
    g_pengfuncsTable->pfnWriteCoord = WriteCoord;
    g_pengfuncsTable->pfnWriteString = WriteString;
    g_pengfuncsTable->pfnWriteEntity = WriteEntity;
    g_pengfuncsTable->pfnMessageEnd = MessageEnd;
  }
  if(endfunction) {
    g_pengfuncsTable->pfnMessageEnd = MessageEnd;
  }
  RETURN_META(result);
}

void WriteByte(int iValue) {
  META_RES result = MRES_IGNORED;
  if(function) (*function)((void *)&iValue);
  if(gInfo.msgBlock) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteChar(int iValue) {
  META_RES result = MRES_IGNORED;
  if(function) (*function)((void *)&iValue);
  if(gInfo.msgBlock) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteShort(int iValue) {
  META_RES result = MRES_IGNORED;
  if(function) (*function)((void *)&iValue);
  if(gInfo.msgBlock) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteLong(int iValue) {
  META_RES result = MRES_IGNORED;
  if(function) (*function)((void *)&iValue);
  if(gInfo.msgBlock) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteAngle(float flValue) {
  META_RES result = MRES_IGNORED;
  if(function) (*function)((void *)&flValue);
  if(gInfo.msgBlock) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteCoord(float flValue) {
  META_RES result = MRES_IGNORED;
  if(function) (*function)((void *)&flValue);
  if(gInfo.msgBlock) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteString(const char *sz) {
  META_RES result = MRES_IGNORED;
  if(function) (*function)((void *)sz);
  if(gInfo.msgBlock) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteEntity(int iValue) {
  META_RES result = MRES_IGNORED;
  if(function) (*function)((void *)&iValue);
  if(gInfo.msgBlock) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void MessageEnd(void) {
  META_RES result = MRES_IGNORED;
  if(endfunction) (*endfunction)(NULL);
  if(gInfo.msgBlock) {
    gInfo.msgBlock = false;
    result = MRES_SUPERCEDE;
  }
  g_pengfuncsTable->pfnWriteByte = NULL;
  g_pengfuncsTable->pfnWriteChar = NULL;
  g_pengfuncsTable->pfnWriteShort = NULL;
  g_pengfuncsTable->pfnWriteLong = NULL;
  g_pengfuncsTable->pfnWriteAngle = NULL;
  g_pengfuncsTable->pfnWriteCoord = NULL;
  g_pengfuncsTable->pfnWriteString = NULL;
  g_pengfuncsTable->pfnWriteEntity = NULL;
  g_pengfuncsTable->pfnMessageEnd = NULL;
  RETURN_META(result);
}

struct sUserMsg {
  const char* name;
  int* id;
  funEventCall func;
  bool endmsg;
} g_user_msg[] = {
  //{ "ResetHUD", &gmsgResetHUD, Client_ResetHUD, true },
  { "ScoreInfo", &gmsgScoreInfo, Client_ScoreInfo, false },
  { "TeamInfo", &gmsgTeamInfo, Client_TeamInfo, false },
  { "TextMsg", &gmsgTextMsg, Client_TextMsg, false },
  { "SendAudio", &gmsgSendAudio, Client_SendAudio, false },
  { 0, 0, 0, false }
};


int RegUserMsg_Post(const char *pszName, int iSize) {
  for(int i = 0; g_user_msg[ i ].name; ++i) {
    if( strcmp( g_user_msg[ i ].name , pszName ) == 0 ) {
      int id = META_RESULT_ORIG_RET( int );

      *g_user_msg[ i ].id = id;

      if( g_user_msg[ i ].endmsg ) {
        modMsgsEnd[ id ] = g_user_msg[ i ].func;
      }
      else {
        modMsgs[ id ] = g_user_msg[ i ].func;
      }
      break;
    }
  }

  RETURN_META_VALUE(MRES_IGNORED, 0);
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

  RETURN_META(MRES_IGNORED);
}

// ServerActivate. This is called when the server starts a new map.
void ServerActivate_Post(edict_t *pEdictList, int edictCount, int clientMax) {
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(i);
    pPlayer->pEdict = &pEdictList[i];
    pPlayer->id = i;
  }

  gmsgResetHUD = GET_USER_MSG_ID(PLID, "ResetHUD", NULL );
  gmsgDamage = GET_USER_MSG_ID(PLID, "Damage", NULL );
  gmsgDeathMsg = GET_USER_MSG_ID(PLID, "DeathMsg", NULL );

  RETURN_META(MRES_IGNORED);
}

// Every call to ServerActivate is matched by ServerDeactivate. This is called when the server
// Is unloading the old map.
void ServerDeactivate() {
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(i);
    if(pPlayer->ingame) {
      pPlayer->Disconnect();
      pPlayer->ResetData();
    }
  }
  // Clear the call list for forwards. this is important as the address of plugins can change
  // from map-to-map.
  g_forwards.clear();
  stringVault.clear();

  // Reset Lights.
  *gInfo.szLastLights = 0;
  *gInfo.szRealLights = 0;
  gInfo.bLights = false;
  gInfo.fNextLights = 0;

  // Reset message blocks.
  for(int j = 0; j < MAX_REG_MSGS; ++j) {
    gInfo.iMessageBlock[j] = BLOCK_NOT;
  }
  
  g_pFunctionTable->pfnClientKill = NULL;
	g_pFunctionTable->pfnPlayerPostThink = NULL;
	g_pFunctionTable->pfnPlayerPreThink = NULL;
	g_pengfuncsTable->pfnEmitSound = NULL;
	g_pengfuncsTable->pfnEmitAmbientSound = NULL;
	g_pFunctionTable->pfnThink = NULL;
	g_pFunctionTable->pfnTouch = NULL;
	g_pFunctionTable->pfnSpawn = NULL;
	g_pFunctionTable_Post->pfnStartFrame = NULL;
	g_pengfuncsTable->pfnSetModel = NULL;
	g_pengfuncsTable->pfnTraceLine = NULL;
	g_pengfuncsTable->pfnSetClientKeyValue = NULL;
	g_pFunctionTable->pfnKeyValue = NULL;
	g_pFunctionTable->pfnGetGameDescription = NULL;
	g_pFunctionTable->pfnClientUserInfoChanged = NULL;
  g_pengfuncsTable->pfnVoice_SetClientListening = NULL;
  g_pengfuncsTable->pfnAlertMessage = NULL;
  g_pengfuncsTable->pfnClientPrintf = NULL;
	g_pFunctionTable->pfnAddToFullPack = NULL;
  
  g_pengfuncsTable->pfnWriteByte = NULL;
  g_pengfuncsTable->pfnWriteChar = NULL;
  g_pengfuncsTable->pfnWriteShort = NULL;
  g_pengfuncsTable->pfnWriteLong = NULL;
  g_pengfuncsTable->pfnWriteAngle = NULL;
  g_pengfuncsTable->pfnWriteCoord = NULL;
  g_pengfuncsTable->pfnWriteString = NULL;
  g_pengfuncsTable->pfnWriteEntity = NULL;
  g_pengfuncsTable->pfnMessageEnd = NULL;

  RETURN_META(MRES_IGNORED);
}

/************************* End Hooked Functions *******************************/

String::String() {
  len = 0;
  napis = 0;
}

String::String( const char* n ) {
  napis = 0;
  set(n);
}

String::~String() {
  clear();
}

void String::set( const char* n ) {
  clear();
  if ( n != 0 ){
    len = strlen( n );
    napis = new char[ len + 1 ];
    if ( napis ) strcpy( napis , n );
    else len = 0;
  }
}

void String::clear() {
  delete[] napis;
  napis = 0;
  len = 0;
}
