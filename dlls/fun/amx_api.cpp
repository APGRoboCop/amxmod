/*
 * Copyright (c) 2002-2011 - Aleksander Naszko, Julien Laurent
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

#include "fun.h"

/******************************* AMX API **************************************/

void AMXAttach() {
  MF_AddNatives(fun_natives);
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
  
  g_pFunctionTable->pfnClientUserInfoChanged = NULL;
	g_pengfuncsTable->pfnTraceLine = NULL;
	g_pFunctionTable->pfnPlayerPostThink = NULL;
	g_pengfuncsTable->pfnCmd_Args = NULL;
	g_pengfuncsTable->pfnCmd_Argv = NULL;
	g_pengfuncsTable->pfnCmd_Argc = NULL;

  g_pengfuncsTable->pfnWriteByte = NULL;
  g_pengfuncsTable->pfnWriteChar = NULL;
  g_pengfuncsTable->pfnWriteShort = NULL;
  g_pengfuncsTable->pfnWriteLong = NULL;
  g_pengfuncsTable->pfnWriteAngle = NULL;
  g_pengfuncsTable->pfnWriteCoord = NULL;
  g_pengfuncsTable->pfnWriteString = NULL;
  g_pengfuncsTable->pfnWriteEntity = NULL;
  g_pengfuncsTable->pfnMessageEnd = NULL;
}

/******************************* MODULE API ***********************************/

funEventCall modMsgsEnd[MAX_REG_MSGS];
funEventCall modMsgs[MAX_REG_MSGS];

void (*function)(void*);
void (*endfunction)(void*);

player_t* mPlayer;
int mPlayerId;
int state;
player_t players[33];
fakecmd_t g_fakecmd;
bool g_bmod_cstrike;
int gmsgTextMsg;
int gmsgResetHUD;
int gmsgMoney;
int gmsgScoreInfo;
int gmsgTeamInfo;
int gmsgStatusIcon;
int gmsgNVGToggle;

/************************* Begin Hooked Functions *****************************/

/*BOOL ClientConnect_Post(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]){
	GET_PLAYER_POINTER(pEntity)->Connect();
	RETURN_META_VALUE(MRES_IGNORED, TRUE);
}*/

void ClientPutInServer_Post(edict_t *pEntity) {
	GET_PLAYER_POINTER(pEntity)->PutInServer();
	RETURN_META(MRES_IGNORED);
}

void ClientUserInfoChanged(edict_t *pEntity, char *infobuffer) {
	if(GET_PLAYER_POINTER(pEntity)->modeled && pEntity->v.deadflag == DEAD_NO) {
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void ClientUserInfoChanged_Post(edict_t *pEntity, char *infobuffer) {
	player_t *pPlayer = GET_PLAYER_POINTER(pEntity);
	if(MF_IsPlayerBot(pPlayer->index)) {
		pPlayer->PutInServer();
	}

	RETURN_META(MRES_IGNORED);
}

void ClientDisconnect( edict_t *pEntity ) {
	player_t *pPlayer = GET_PLAYER_POINTER(pEntity);
	if(pPlayer->ingame) pPlayer->Disconnect();
	RETURN_META(MRES_IGNORED);
}

void PlayerPostThink(edict_t *pEntity) {
	player_t *pPlayer = GET_PLAYER_POINTER(pEntity);

	if(pPlayer->nofootsteps) {
		pEntity->v.flTimeStepSound = 999;
	}
	
	if(pPlayer->modeled) {
		SET_CLIENT_KEYVALUE(pPlayer->index, GET_INFO_KEYBUFFER(pPlayer->pEdict), "model", pPlayer->modelname);
	}

	RETURN_META(MRES_IGNORED);
}

void TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *e, TraceResult *ptr) {
	TRACE_LINE(v1, v2, fNoMonsters, e, ptr);
	if(ptr->pHit && (ptr->pHit->v.flags & (FL_CLIENT | FL_FAKECLIENT) ) && e && (e->v.flags & (FL_CLIENT | FL_FAKECLIENT))){
		player_t* pPlayer = GET_PLAYER_POINTER(e);
		if(!((int)pPlayer->bodyhits[ENTINDEX(ptr->pHit)] & (1<<ptr->iHitgroup))) {
			ptr->flFraction = 1.0;
			RETURN_META(MRES_SUPERCEDE);
		}
	}
	RETURN_META(MRES_IGNORED);
}

const char *Cmd_Args(void) {
  if(g_fakecmd.fake) {
    g_pengfuncsTable->pfnCmd_Args = NULL;
    RETURN_META_VALUE(MRES_SUPERCEDE, (g_fakecmd.argc>1)?g_fakecmd.args:NULL);
  }
  RETURN_META_VALUE(MRES_IGNORED, NULL);
}

const char *Cmd_Argv(int argc) {
  if(g_fakecmd.fake) {
    g_pengfuncsTable->pfnCmd_Argv = NULL;
    RETURN_META_VALUE(MRES_SUPERCEDE, (argc<3)?g_fakecmd.argv[argc]:"");
  }
  RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int Cmd_Argc(void) {
  if(g_fakecmd.fake) {
    g_pengfuncsTable->pfnCmd_Argc = NULL;
    RETURN_META_VALUE(MRES_SUPERCEDE, g_fakecmd.argc);
  }
  RETURN_META_VALUE(MRES_IGNORED, 0);
}

void MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) {
  if(ed) {
		mPlayerId = ENTINDEX(ed);
		mPlayer = GET_PLAYER_POINTER_I(mPlayerId);
	}
	else {
		mPlayerId = 0;
		mPlayer = 0;
	}
	state = 0;
	if( msg_type < 0 || msg_type >= MAX_REG_MSGS ) {
		msg_type = 0;
	}
	function=modMsgs[ msg_type ];
	endfunction=modMsgsEnd[ msg_type ];

  if(function) {
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
    g_pengfuncsTable->pfnMessageEnd=MessageEnd;
  }
  RETURN_META(MRES_IGNORED);
}

void WriteByte(int iValue) {
	if(function) (*function)((void *)&iValue);
	RETURN_META(MRES_IGNORED);
}

void WriteChar(int iValue) {
	if(function) (*function)((void *)&iValue);
	RETURN_META(MRES_IGNORED);
}

void WriteShort(int iValue) {
	if(function) (*function)((void *)&iValue);
	RETURN_META(MRES_IGNORED);
}

void WriteLong(int iValue) {
	if(function) (*function)((void *)&iValue);
	RETURN_META(MRES_IGNORED);
}

void WriteAngle(float flValue) {
	if(function) (*function)((void *)&flValue);
	RETURN_META(MRES_IGNORED);
}

void WriteCoord(float flValue) {
	if(function) (*function)((void *)&flValue);
	RETURN_META(MRES_IGNORED);
}

void WriteString(const char *sz) {
	if(function) (*function)((void *)sz);
	RETURN_META(MRES_IGNORED);
}

void WriteEntity(int iValue) {
	if(function) (*function)((void *)&iValue);
	RETURN_META(MRES_IGNORED);
}

void MessageEnd(void) {
	if(endfunction) (*endfunction)(NULL);
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

struct sUserMsg {
	const char* name;
	int* id;
	funEventCall func;
	bool endmsg;
} g_user_msg[] = {
	{ "ResetHUD", &gmsgResetHUD, Client_ResetHUD, true },
	{ "ScoreInfo", &gmsgScoreInfo, Client_ScoreInfo, false },
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
	for(int i = 0; g_user_msg[ i ].name; ++i )	{
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

void ServerActivate_Post( edict_t *pEdictList, int edictCount, int clientMax ) {
	for(int i = 1; i <= gpGlobals->maxClients; ++i) {
		player_t *pPlayer = GET_PLAYER_POINTER_I(i);
		pPlayer->pEdict = &pEdictList[i];
		pPlayer->index = i;
	}

	gmsgTextMsg = GET_USER_MSG_ID(PLID, "TextMsg", NULL );
	gmsgResetHUD = GET_USER_MSG_ID(PLID, "ResetHUD", NULL );
	gmsgMoney = GET_USER_MSG_ID(PLID, "Money", NULL );
	gmsgScoreInfo = GET_USER_MSG_ID(PLID, "ScoreInfo", NULL );
	gmsgTeamInfo = GET_USER_MSG_ID(PLID, "TeamInfo", NULL );
	gmsgStatusIcon = GET_USER_MSG_ID(PLID, "StatusIcon", NULL );
	gmsgNVGToggle = GET_USER_MSG_ID(PLID, "NVGToggle", NULL );

	RETURN_META(MRES_IGNORED);
}

// Every call to ServerActivate is matched by ServerDeactivate. This is called when the server
// Is unloading the old map.
void ServerDeactivate() {
  for(int i = 1; i<=gpGlobals->maxClients; ++i){
		player_t *pPlayer = GET_PLAYER_POINTER_I(i);
		if(pPlayer->ingame) pPlayer->Disconnect();
	}
  
	g_pFunctionTable->pfnClientUserInfoChanged = NULL;
	g_pengfuncsTable->pfnTraceLine = NULL;
	g_pFunctionTable->pfnPlayerPostThink = NULL;
	g_pengfuncsTable->pfnCmd_Args = NULL;
	g_pengfuncsTable->pfnCmd_Argv = NULL;
	g_pengfuncsTable->pfnCmd_Argc = NULL;
	
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

