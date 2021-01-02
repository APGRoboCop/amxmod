/* <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <>
 *  Project:
 *      Vexd Utility Module
 *
 *  Original Author:
 *      Vexd
 *
 *  Additional Author:
 *      T(+)rget
 *
 *  Credits:
 *    Pierre-Marie Baty for dot_points, dot_product, can_see, in_view_cone natives
 *    AMXX Team for new naming scheme for mutual interests, some natives, some forwards
 *    tcquest78 for get/set_offset_int natives
 *
 *    Some code was taken from AMX Mod X, credits for this code go to them.
 *    The authors of AMX Mod X are:
 *    David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
 *	  Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
 *		Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
 *	  Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.
 *
 *  Purpose:
 *      This module provides more options for working with entities and other
 *      things that are half-life related using amxmod scripts.
 *
 *  Licence:
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Full licence can be found at:
 *      http://www.gnu.org/licenses/gpl.txt
 * <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> */

#ifndef vexdum_h
#define vexdum_h

#include <extdll.h>
#include <meta_api.h>
#include <dllapi.h>
#include <sdk_util.h>
#include <h_export.h>
#include "amx_module.h"
#include <modules.h>
#include <cbase.h>
#include <usercmd.h>
#include <entity_state.h>
#include "vexdum_defs.h"
#include "vexdum_enum.h"
#include "vexdum_fwds.h"

extern DLL_FUNCTIONS *g_pFunctionTable;
extern DLL_FUNCTIONS *g_pFunctionTable_Post;
extern enginefuncs_t *g_pengfuncsTable;
extern enginefuncs_t *g_pengfuncsTable_Post;

extern AMX_NATIVE_INFO VexdUM_Gen[];
extern AMX_NATIVE_INFO VexdUM_Ent[];
extern AMX_NATIVE_INFO VexdUM_Dmg[];
extern AMX_NATIVE_INFO VexdUM_Bot[];
extern AMX_NATIVE_INFO VexdUM_Misc[];
extern AMX_NATIVE_INFO VexdUM_Globals[];
extern AMX_NATIVE_INFO VexdUM_Tr[];
extern AMX_NATIVE_INFO VexdUM_Meta[];

extern char* UTIL_VarArgs(char *fmt, ...);

extern int mPlayerId;
extern int mState;

typedef void (*funEventCall)(void*);
extern funEventCall modMsgsEnd[MAX_REG_MSGS];
extern funEventCall modMsgs[MAX_REG_MSGS];
extern void (*function)(void*);
extern void (*endfunction)(void*);

extern int gmsgResetHUD;
extern int gmsgScoreInfo;
extern int gmsgTeamInfo;
extern int gmsgDeathMsg;
extern int gmsgDamage;
extern int gmsgTextMsg;
extern int gmsgSendAudio;

// Some registered functions (vexdum_base.cpp)
//void Client_ResetHUD(void*);
void Client_ScoreInfo(void*);
void Client_TeamInfo(void*);
void Client_TextMsg(void*);
void Client_SendAudio(void*);

BOOL is_breakable( edict_t* pBreak );
BOOL is_monster( edict_t* pMonster );
void hurt_monster( edict_t* pMonster, float dmg, int bit, const float *origin );
float ArmorDamage( edict_t* pVictim, float dmg, int bit );
void RadiusDamage( edict_t* pAttacker, float dmg, Vector origin, float rad, int bit, const char* weapon, int hs );
void Damage( edict_t* pVictim, edict_t* pAttacker, const float *origin, float dmg, int bit, const char* weapon, int hs );
void Death( edict_t* pVictim, edict_t* pKiller, const char* weapon, int hs );
void UTIL_LogPrintf( char *fmt, ... );
char* UTIL_VarArgs( char *fmt, ... );
edict_t *UTIL_FindEntityInSphere( edict_t *pStart, const Vector &vecCenter, float flRadius );
int countModelled(bool checkUnsetModel);
bool disableVoiceClientListening();

// Stuctures
class String {
  char* napis;
  short int len;

public:
  String();
  String( const char* n );
  ~String();
  void set( const char* n );
  inline bool empty() const {  return (len == 0); }
  inline const char* str() const {  return napis ? napis : "(null)"; }
  inline short int size() const {  return len; }
  void clear();
};

extern bool g_bmod_cstrike;

struct GlobalInfo {
  bool  bLights;
  float fNextLights;
  char  szLastLights[128];
  char  szRealLights[128];
  int   iMessageBlock[MAX_REG_MSGS];
  bool  msgBlock;
  bool  logBlock;
};

extern GlobalInfo gInfo;
extern TraceResult *vx_tr;

enum {
  TR_AllSolid,      // (int) if true, plane is not valid
  TR_StartSolid,    // (int) if true, the initial point was in a solid area
  TR_InOpen,        // (int)
  TR_InWater,       // (int)
  TR_Fraction,      // (float) time completed, 1.0 = didn't hit anything
  TR_EndPos,        // (vector) final position
  TR_PlaneDist,     // (float)
  TR_PlaneNormal,   // (vector) surface normal at impact
  TR_Hit,           // (entity) entity the surface is on
  TR_Hitgroup       // (int) 0 == generic, non zero is specific body part
};

struct CPlayer {
  edict_t* pEdict;
  int      id;
  bool     ingame;
  //bool     bot;
  int      deaths;
  bool     modeled;
  float    fModelSet;
  bool     UnSetModel;
  int      SetModel;
  char     modelname[32];
  char     speakflags[33];
  int      team;
  char*    teamname;

  //void Connect() { bot = IsBot(); }
  void PutInServer() { ingame = true; }
  void Disconnect() { ingame = false; }
  void ResetData() {
    modeled = false;
    UnSetModel = false;
    SetModel = 0;
    fModelSet = 0.0f;
    memset(modelname, 0x00, sizeof(char)*32);
    memset(speakflags, 0x00, sizeof(char)*33);
  }
  //inline bool IsBot() { return (pEdict->v.flags & FL_FAKECLIENT) ? true : false; }
};

extern CPlayer players[33];
extern CPlayer* mPlayer;

// Classes
class StringStorage {
public:

  struct String {
    char* name;

    String* next;
    String(char* n) {
      name = new char[strlen(n) + 1];
      strcpy(name, n);
      next = 0;
    }
    ~String() { delete[] name; }
  } *head;

  StringStorage() { head = 0; }
  ~StringStorage() { clear(); }
  char* put(char* n) {
    String** a = &head;
    while(  *a  ) {
      if(strcmp( (*a)->name, n) == 0) {
        return (*a)->name;
      }
      a = &(*a)->next;
    }

    *a = new String( n );
    return (*a)->name;
  }
  void clear() {
    while ( head ) {
      String* a = head->next;
      delete head;
      head = a;
    }
  }
};

extern StringStorage stringVault;

extern KeyValueData *g_pkvd;
extern bool g_inKeyValue;
extern bool g_inGameDescription;
extern char g_GameDescription[128];

// By AMXMODX Dev Team
struct KVD_Wrapper
{
	KeyValueData *kvd;
	String cls;
	String key;
	String val;
};
extern KVD_Wrapper g_kvd;
extern entity_state_t g_es;
extern clientdata_t g_cd;
extern usercmd_t g_uc;
// End of By AMXMODX Dev Team

struct ForwardReturn {
	int iVal;
	float fVal;
	char* sVal;
	void Clear() {
		iVal = 0;
		fVal = 0.0f;
		sVal = 0;
	};
};
extern ForwardReturn g_FwdReturnVal;

extern cvar_t *friendlyfire;
extern cvar_t *knockback1;
extern cvar_t *knockback2;
extern cvar_t *vxdebug;


void ClientUserInfoChanged(edict_t *pEntity, char *infobuffer);
void ClientKill(edict_t *pEntity);
void PlayerPreThink(edict_t *pEntity);
void PlayerPostThink(edict_t *pEntity);
int EntitySpawn(edict_t *pEntity);
void EntityTouch(edict_t *pToucher, edict_t *pTouched);
void EntityThink(edict_t *pent);
void StartFrame_Post();
qboolean Voice_SetClientListening(int iReceiver, int iSender, qboolean bListen);
void EmitSound(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch);
void EmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);
void SetModel(edict_t *pEdict, const char *model);
void TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *e, TraceResult *ptr);
void SetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value);
void KeyValue(edict_t *entity, KeyValueData *pkvd);
const char* GetGameDescription(void);
void AlertMessage(ALERT_TYPE atype, char *szFmt, ...);
void ClientPrintf(edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg);
int AddToFullPack(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet);

void WriteByte(int iValue);
void WriteChar(int iValue);
void WriteShort(int iValue);
void WriteLong(int iValue);
void WriteAngle(float flValue);
void WriteCoord(float flValue);
void WriteString(const char *sz);
void WriteEntity(int iValue);
void MessageEnd(void);

// By AMXMODX Dev Team
inline edict_t* INDEXENT2(int iEdictNum)
{
	if(iEdictNum >= 1 && iEdictNum <= gpGlobals->maxClients)
		return MF_GetPlayerEdict(iEdictNum);
	else
		return (*g_engfuncs.pfnPEntityOfEntIndex)(iEdictNum);
}

#endif // vexdum_h
