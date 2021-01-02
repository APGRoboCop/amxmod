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

#ifndef DODXMOD_H
#define DODXMOD_H

#include "CMisc.h"
#include "CRank.h"


#include "amx_module.h"
#include "dodstats_fwds.h"


#define GET_PLAYER_POINTER(e)   (&players[ENTINDEX(e)])
#define GET_PLAYER_POINTER_I(i) (&players[i])

#ifndef GETPLAYERAUTHID
#define GETPLAYERAUTHID   (*g_engfuncs.pfnGetPlayerAuthId)
#endif

#define DODMAX_WEAPONS 42

extern DLL_FUNCTIONS *g_pFunctionTable;
extern DLL_FUNCTIONS *g_pFunctionTable_Post;
extern enginefuncs_t *g_pengfuncsTable;
extern enginefuncs_t *g_pengfuncsTable_Post;

typedef  void (*function_event_call)(void*);

extern AMX_NATIVE_INFO stats_natives[];
extern AMX_NATIVE_INFO base_natives[];

struct weapon_t {
	bool needcheck;
	bool melee;
	char logname[16];
	char name[32];
	int ammoSlot;
};

//extern bool rankBots;
extern int mState;
extern int mPlayerIndex;
extern int AlliesScore;
extern int AxisScore;

void Client_CurWeapon(void*);
void Client_Health_End(void*);
void Client_ResetHUD_End(void*);
void Client_ObjScore(void*);
void Client_TeamScore(void*);
void Client_RoundState(void*);
void Client_AmmoX(void*);
void Client_AmmoShort(void*);


typedef void (*funEventCall)(void*);

extern int gmsgScoreShort;
extern int gmsgPTeam;

extern cvar_t* dodstats_maxsize;
extern cvar_t* dodstats_rank;
extern cvar_t* dodstats_reset;
extern cvar_t* dodstats_rankbots;
extern cvar_t* dodstats_pause;

extern bool g_LoadText;
extern char g_LoadTextFile[256];

extern weapon_t weaponData[DODMAX_WEAPONS];

extern Grenades g_grenades;
extern RankSystem g_rank;
extern CPlayer players[33];
extern CPlayer* mPlayer;
extern CMapInfo g_map;


int get_weaponid(CPlayer* player);
edict_t *FindEntityByString(edict_t *pentStart, const char *szKeyword, const char *szValue);
edict_t *FindEntityByClassname(edict_t *pentStart, const char *szName);
//bool ignoreBots(edict_t *pEnt, edict_t *pOther = NULL);
bool ignoreBots(int index, edict_t *pOther = NULL);
bool isModuleActive();


void WriteByte_Post(int iValue);
void WriteChar_Post(int iValue);
void WriteShort_Post(int iValue);
void WriteLong_Post(int iValue);
void WriteAngle_Post(float flValue);
void WriteCoord_Post(float flValue);
void WriteString_Post(const char *sz);
void WriteEntity_Post(int iValue);
void MessageEnd_Post(void);
void dodstats_savetext_command();
void dodstats_loadtext_command();
void dodstats_savesv_command();

#endif // RANK_H
