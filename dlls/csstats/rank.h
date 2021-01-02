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

#ifndef RANK_H
#define RANK_H

#include "amx_module.h"
#include "csstats_fwds.h"

#define GET_PLAYER_POINTER(e) (&players[ENTINDEX(e)])
#define GET_PLAYER_POINTER_I(i) (&players[i])

#ifndef GETPLAYERAUTHID
#define GETPLAYERAUTHID   (*g_engfuncs.pfnGetPlayerAuthId)
#endif

#define CSW_P228          1
#define CSW_SCOUT         3
#define CSW_HEGRENADE     4
#define CSW_XM1014        5
#define CSW_C4            6
#define CSW_MAC10         7
#define CSW_AUG           8
#define CSW_SMOKEGRENADE  9
#define CSW_ELITE         10
#define CSW_FIVESEVEN     11
#define CSW_UMP45         12
#define CSW_SG550         13
#define CSW_GALI          14
#define CSW_GALIL         14
#define CSW_FAMAS         15
#define CSW_USP           16
#define CSW_GLOCK18       17
#define CSW_AWP           18
#define CSW_MP5NAVY       19
#define CSW_M249          20
#define CSW_M3            21
#define CSW_M4A1          22
#define CSW_TMP           23
#define CSW_G3SG1         24
#define CSW_FLASHBANG     25
#define CSW_DEAGLE        26
#define CSW_SG552         27
#define CSW_AK47          28
#define CSW_KNIFE         29
#define CSW_P90           30

#define BOMB_PLANTING     1
#define BOMB_PLANTED      2
#define BOMB_EXPLODED     3
#define BOMB_DEFUSING     4
#define BOMB_DEFUSED      5

extern DLL_FUNCTIONS *g_pFunctionTable;
extern DLL_FUNCTIONS *g_pFunctionTable_Post;
extern enginefuncs_t *g_pengfuncsTable;
extern enginefuncs_t *g_pengfuncsTable_Post;

struct stats_t {
  char* name;
  char* unique;
  short int namelen;
  short int uniquelen;
  int flags;
  int score;
  int id;
  int hits;
  int shots;
  int damage;
  int hs;
  int tks;
  int kills;
  int deaths;
  int ammo;
  int clip;
  int body_hits[8];
  int had_bomb;
  int bomb_planted;
  int bomb_exploded;
  int bomb_defusions;
  int bomb_defused;
  int hostages_touched;
  int hostages_rescued;
  int was_vip;
  int escaped_as_vip;
  int killed_vip;
  int suicides;
  int playtime;
  stats_t* next;
  stats_t* prev;
};

stats_t* find_name_in_rank(const char* unique, const char* name);
void score_stats_in_rank(const char* name,stats_t* p, stats_t* a);

struct player_t {
  edict_t* pEdict;
  char name[32];
  char ip[32];
  int index;
  int team_id;
  int aiming;
  int current;
  bool ingame;
  //bool bot;
  float clear_stats;
  int money;
  float playtime;

  bool reset_hud;

  stats_t weapons_rnd[MAX_WEAPONS]; // DEC-Weapon (Round) stats

  stats_t weapons[MAX_WEAPONS];
  stats_t attackers[33];
  stats_t victims[33];
  stats_t* weapon;
  stats_t* rank;
  stats_t life;
};

bool PlayerIsAlive(edict_t *pEdict);
void PlayerDisconnect(player_t* pPlayer);
void PlayerPutInServer(player_t* pPlayer);
void PlayerConnect(player_t* pPlayer,const char* name,const char* ip );
//bool PlayerIsBot(edict_t *pEdict);

struct grenades_t {
  player_t* pPlayer;
  edict_t* pGrenade;
  float time;
  int index;
  grenades_t* next;
  grenades_t* prev;
};

struct weapons_t {
  char full_name[32];
  const char* name;
  short int iId;
  short int ammo_slot;
};

typedef  void (*function_event_call)(void*);

extern AMX_NATIVE_INFO csstats_natives[];

struct score_plug_t {
  AMX amx;
  void *program;
  int func;
  cell amx_addr1;
  cell *phys_addr1;
  cell amx_addr2;
  cell *phys_addr2;
  cell amx_addr3;
  cell *phys_addr3;
};

extern cvar_t* csstats_maxsize;
extern cvar_t* csstats_reset;
extern cvar_t* csstats_rank;
extern cvar_t* csstats_rankbots;
extern cvar_t* csstats_pause;
extern bool g_LoadText;
extern char g_LoadTextFile[256];
extern score_plug_t scr_plug;

extern int gmsgTextMsg;
extern int gmsgCurWeapon;
extern int gmsgDamage;
extern int gmsgDamageEnd;
extern int gmsgWeaponList;
extern int gmsgResetHUD;
extern int gmsgAmmoX;
extern int gmsgScoreInfo;
extern int gmsgAmmoPickup;
extern int gmsgWeapPickup;
extern int gmsgSendAudio;
extern int gmsgStatusIcon;
extern int gmsgBarTime;
extern int gmsgMoney;
extern int gmsgScoreAttrib;

extern player_t players[33];
extern function_event_call modMsgsEnd[MAX_REG_MSGS];
extern function_event_call modMsgs[MAX_REG_MSGS];
extern void (*function)(void*);
extern void (*endfunction)(void*);
extern player_t* mPlayer;
extern int mPlayerIndex;
extern int mState;
//extern bool rankBots;
extern weapons_t weapon_data[MAX_WEAPONS];
extern grenades_t* grenades;
extern int g_Planter;
extern int g_Defuser;
extern int g_DefuserHasDefuseKit;
extern int g_Vip;
extern int g_bombForwards;

void Client_TextMsg(void*);
void Client_AmmoX(void*);
void Client_CurWeapon(void*);
void Client_Damage(void*);
void Client_Damage_End(void*);
void Client_WeaponList(void*);
void Client_AmmoPickup(void*);
void Client_WeapPickup(void*);
void Client_ScoreInfo(void*);
void Client_SendAudio(void*);
void Client_StatusIcon(void*);
void Client_BarTime(void*);
void Client_Money(void*);
void Client_ScoreAttrib(void*);
void Client_ResetHUD_End(void*);

typedef void (*funEventCall)(void*);

grenades_t* find_grenade(edict_t* enemy);
void load_get_score_plugin(const char* filename, char* error);
void load_rank(const char* filename);
void save_rank(const char* filename);
bool save_rank_text(const char* filename);
bool save_rank_sv(const char* filename, int parserType);
bool rank_text_to_binary(char filename[]);
bool is_valid_text_file(const char* filename);
void delete_rank();
void delete_grenades();
void fire_kill(player_t* pVictim, player_t* pKiller, int weapon, int hs, int tk);
void fire_hit(player_t* pVictim,player_t* pAttacker, int weapon, int damage, int aiming);
void fire_shot(player_t* pPlayer, int weapon);
void fire_has_bomb(player_t* pPlayer);
void fire_bomb_planted(player_t* pPlayer);
void fire_bomb_exploded(player_t* pPlayer);
void fire_is_defusing(player_t* pPlayer);
void fire_bomb_defused(player_t* pPlayer);
void fire_hostages_touched(player_t* pPlayer);
void fire_hostages_rescued(player_t* pPlayer);
void fire_is_vip(player_t* pPlayer);
void fire_escaped_as_vip(player_t* pPlayer);
void fire_killed_vip(player_t* pPlayer);
void fire_suicide(player_t* pPlayer);
void put_grenade(edict_t* pGre, float time, int type, player_t* pPlayer);
//bool ignoreBots(edict_t *pEnt, edict_t *pOther = NULL);
bool ignoreBots(int index, edict_t *pOther = NULL);
bool isModuleActive();
int get_stats_num();

void StartFrame_Post();
void WriteByte_Post(int iValue);
void WriteChar_Post(int iValue);
void WriteShort_Post(int iValue);
void WriteLong_Post(int iValue);
void WriteAngle_Post(float flValue);
void WriteCoord_Post(float flValue);
void WriteString_Post(const char *sz);
void WriteEntity_Post(int iValue);
void MessageEnd_Post(void);
void csstats_savetext_command();
void csstats_loadtext_command();
void csstats_savesv_command();

#endif // RANK_H

