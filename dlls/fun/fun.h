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

#ifndef FUNMOD_H
#define FUNMOD_H

#ifdef LINUX
  #include <sclinux.h>
#endif
#include "amx_module.h"

#define SETCLIENTMAXSPEED         (*g_engfuncs.pfnSetClientMaxspeed)
#define SET_CLIENT_KEYVALUE       (*g_engfuncs.pfnSetClientKeyValue)
#define GET_INFO_KEYBUFFER        (*g_engfuncs.pfnGetInfoKeyBuffer)
#define GETCLIENTLISTENING		    (*g_engfuncs.pfnVoice_GetClientListening)
#define SETCLIENTLISTENING		    (*g_engfuncs.pfnVoice_SetClientListening)

#define GET_PLAYER_POINTER(e)   (&players[ENTINDEX(e)])
#define GET_PLAYER_POINTER_I(i) (&players[i])

extern DLL_FUNCTIONS *g_pFunctionTable;
extern DLL_FUNCTIONS *g_pFunctionTable_Post;
extern enginefuncs_t *g_pengfuncsTable;
extern enginefuncs_t *g_pengfuncsTable_Post;

extern AMX_NATIVE_INFO fun_natives[];

class String
{
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

class Slist
{
protected:
  struct Obj{
    void* data;
    Obj* next;
  } *head;

  Obj* create( void* n ){
    Obj* d = (Obj*)malloc( sizeof(Obj) );
    d->data = (char*)malloc(strlen( (char*)n ) + 1);
    strcpy( (char*)d->data, (char*)n );
    d->next = head;
    return  head = d;
  }

  Obj** find_ptr( void* n ){
    Obj** d = &head;
    while( *d ) {
      if ( strcmp( (char*)n , (char*)(*d)->data  ) == 0 )
        return d;
      d = &(*d)->next;
    }
    return d;
  }
public:

  Slist( ){
    head = NULL;
  }

  ~Slist( ){
    clear();
  }
  void* set( void* n ){
    Obj** b = find_ptr( n );
    if ( *b ) return (*b)->data;
    return create( n )->data;
  }
  void clear(){
    while( head ){
      Obj* d = head;
      head = head->next;
      free( d->data );
      free( d);
    }
  }
  void erase( void* n ){
    Obj** b = find_ptr( n );
    if ( *b == 0 ) return;
    Obj* a = *b;
    *b = (*b)->next;
    free( a->data);
    free( a);
  }
};


struct player_t {
  edict_t* pEdict;
  int index;
  float defgravity;
  bool ingame;
  unsigned char bodyhits[33];
  int money;
  int deaths;
  int team;
  bool modeled;
  char modelname[32];
  bool nofootsteps;

  inline bool IsAlive() {
    return (pEdict->v.deadflag==DEAD_NO)&&(pEdict->v.health>0);
  }

  //void Connect() { bot = IsBot();  }
  void PutInServer() {
    ingame = true;
  }
  void Disconnect() { ingame = false; }

  void ResetSettings() {
    g_pFunctionTable->pfnPlayerPostThink = NULL;
    g_pFunctionTable->pfnClientUserInfoChanged = NULL;

    pEdict->v.renderfx = kRenderFxNone;
    pEdict->v.rendercolor.x = 255;
    pEdict->v.rendercolor.y = 255;
    pEdict->v.rendercolor.z = 255;
    pEdict->v.rendermode = kRenderNormal;
    pEdict->v.renderamt = 16;
    defgravity = pEdict->v.gravity;
    memset(bodyhits,0xFF,sizeof(unsigned char)*33);
    modeled = false;
    *modelname = 0;
    nofootsteps = false;
  }
};

struct fakecmd_t {
  char args[256];
  const char *argv[3];
  int argc;
  bool fake;
};

typedef void (*function_t)(void*);

extern player_t players[33];
typedef void (*funEventCall)(void*);
extern funEventCall modMsgsEnd[MAX_REG_MSGS];
extern funEventCall modMsgs[MAX_REG_MSGS];
extern void (*function)(void*);
extern void (*endfunction)(void*);
extern player_t* mPlayer;
extern int mPlayerId;
extern int state;
extern edict_t* mEdict;
extern fakecmd_t g_fakecmd;
extern bool g_bmod_cstrike;

extern int gmsgTextMsg;
extern int gmsgResetHUD;
extern int gmsgMoney;
extern int gmsgScoreInfo;
extern int gmsgTeamInfo;
extern int gmsgStatusIcon;
extern int gmsgNVGToggle;

void Client_ResetHUD(void*);
void Client_ScoreInfo(void*);
void UTIL_FakeClientCommand(edict_t *pEdict, const char *cmd, const char *arg1 = NULL, const char *arg2 = NULL);
int countFootSteps();
int countModelled();
bool disableTraceLine();

void ClientUserInfoChanged(edict_t *pEntity, char *infobuffer);
void PlayerPostThink(edict_t *pEntity);
void TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *e, TraceResult *ptr);
const char *Cmd_Args(void);
const char *Cmd_Argv(int argc);
int Cmd_Argc(void);

void WriteByte(int iValue);
void WriteChar(int iValue);
void WriteShort(int iValue);
void WriteLong(int iValue);
void WriteAngle(float flValue);
void WriteCoord(float flValue);
void WriteString(const char *sz);
void WriteEntity(int iValue);
void MessageEnd(void);

#endif // FUNMOD_H
