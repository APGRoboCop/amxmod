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

#ifndef __MODULES_H__
#define __MODULES_H__

#include "amx.h"

#ifdef USE_METAMOD
#include <extdll.h>
#endif


#define AMX_INTERFACE_VERSION 9

#define RELOAD_MODULE 0
#define STATIC_MODULE 1


struct module_info_s {
  const char* name;
  const char* author;
  const char* version;
  int ivers;
  int type;
};

// Small scripting language
struct pfnamx_engine_g {
  uint16_t*   (*pfnamx_Align16)(uint16_t *);                     // value
  uint32_t*   (*pfnamx_Align32)(uint32_t *);                     // value
#if defined _I64_MAX || defined HAVE_I64
  uint64_t*   (*pfnamx_Align64)(uint64_t *);
#endif
  int         (*pfnamx_Allot)(AMX*, int, cell*, cell**);           // amx, length, amx_addr, phys_addr
  int         (*pfnamx_Callback)(AMX*, cell, cell*, cell*);        // amx, index,result,params
  int         (*pfnamx_Clone)(AMX*, AMX*, void*);                  // amxClone, amxSrc, data
  int         (*pfnamx_Debug)(AMX*);                               // default debug procedure, does nothing   // amx
  int         (*pfnamx_Exec)(AMX*, cell*, int, int, ...);          // amx, return val, index, num_params, ...
  int         (*pfnamx_Execv)(AMX*, cell*, int, int, cell[]);      // amx, return val, index, num_params, param[]
  int         (*pfnamx_FindPublic)(AMX*, const char*, int*);       // amx, func name, index
  int         (*pfnamx_FindPubVar)(AMX*, const char*, cell*);      // anx, var name, amx_addr
  int         (*pfnamx_FindTagId)(AMX*, cell, char*);              // amx. tag_id, tagname
  int         (*pfnamx_Flags)(AMX*, uint16_t *);                   // amx, flags
  int         (*pfnamx_GetAddr)(AMX*, cell, cell**);               // amx, amx_addr, phys_addr
  int         (*pfnamx_GetPublic)(AMX*, int, char*);               // amx, index, funcname
  int         (*pfnamx_GetPubVar)(AMX*, int, char*, cell*);        // amx, index, varname, amx_addr
  int         (*pfnamx_GetString)(char*dest, const cell*, int);    // dest, source
  int         (*pfnamx_GetTag)(AMX*, int, char*, cell*);           // amx, index, tagname, tag_id
  int         (*pfnamx_GetUserData)(AMX*, long, void **);          // amx, tag, ptr
  int         (*pfnamx_Init)(AMX*, void *);                        // amx, program
  int         (*pfnamx_InitJIT)(AMX*, void *, void *);             // amx, reloc_table, native_code
  int         (*pfnamx_MemInfo)(AMX*, long*, long*, long*);        // amx, codesize, datasize, stackheap
  int         (*pfnamx_NameLength)(AMX*, int*);                    // amx, length
  AMX_NATIVE_INFO*  (*pfnamx_NativeInfo)(const char*, AMX_NATIVE); // name, func
  int         (*pfnamx_NumPublics)(AMX*, int*);                    // amx, number
  int         (*pfnamx_NumPubVars)(AMX*, int*);                    // amx, number
  int         (*pfnamx_NumTags)(AMX*, int*);                       // amx, number
  int         (*pfnamx_RaiseError)(AMX*, int);                     // amx, error
  int         (*pfnamx_Register)(AMX*, AMX_NATIVE_INFO*, int);     // amx, nativelist, number
  int         (*pfnamx_Release)(AMX*, cell);                       // amx, amx_addr
  int         (*pfnamx_SetCallback)(AMX*, AMX_CALLBACK);           // amx, callback
  int         (*pfnamx_SetDebugHook)(AMX*, AMX_DEBUG);             // amx, debug
  int         (*pfnamx_SetString)(cell*, const char*, int, int);   // dest, source, pack
  int         (*pfnamx_SetUserData)(AMX*, long, void*);            // amx, tag, prt
  int         (*pfnamx_StrLen)(cell*, int*);                       // amx, cstring, length
};
extern pfnamx_engine_g* g_engAmxFunc;

#define AMX_ALIGN16       (*g_engAmxFunc->pfnamx_Align16)
#define AMX_ALIGN32       (*g_engAmxFunc->pfnamx_Align32)
#if defined _I64_MAX || defined HAVE_I64
#define AMX_ALIGN64       (*g_engAmxFunc->pfnamx_Align64)
#endif
#define AMX_ALLOT         (*g_engAmxFunc->pfnamx_Allot)
#define AMX_CALLBACK      (*g_engAmxFunc->pfnamx_Callback)
#define AMX_CLONE         (*g_engAmxFunc->pfnamx_Clone)
#define AMX_DEBUG         (*g_engAmxFunc->pfnamx_Debug)
#define AMX_EXEC          (*g_engAmxFunc->pfnamx_Exec)
#define AMX_EXECV         (*g_engAmxFunc->pfnamx_Execv)
#define AMX_FINDPUBLIC    (*g_engAmxFunc->pfnamx_FindPublic)
#define AMX_FINDPUBVAR    (*g_engAmxFunc->pfnamx_FindPubVar)
#define AMX_FINDTAGID     (*g_engAmxFunc->pfnamx_FindTagId)
#define AMX_FLAGS         (*g_engAmxFunc->pfnamx_Flags)
#define AMX_GETADDR       (*g_engAmxFunc->pfnamx_GetAddr)
#define AMX_GETPUBLIC     (*g_engAmxFunc->pfnamx_GetPublic)
#define AMX_GETPUBVAR     (*g_engAmxFunc->pfnamx_GetPubVar)
#define AMX_GETSTRING     (*g_engAmxFunc->pfnamx_GetString)
#define AMX_GETTAG        (*g_engAmxFunc->pfnamx_GetTag)
#define AMX_GETUSERDATA   (*g_engAmxFunc->pfnamx_GetUserData)
#define AMX_INIT          (*g_engAmxFunc->pfnamx_Init)
#define AMX_INITJIT       (*g_engAmxFunc->pfnamx_InitJIT)
#define AMX_MEMINFO       (*g_engAmxFunc->pfnamx_MemInfo)
#define AMX_NAMELENGTH    (*g_engAmxFunc->pfnamx_NameLength)
#define AMX_NATIVEINFO    (*g_engAmxFunc->pfnamx_NativeInfo)
#define AMX_NUMPUBLICS    (*g_engAmxFunc->pfnamx_NumPublics)
#define AMX_NUMPUBVARS    (*g_engAmxFunc->pfnamx_NumPubVars)
#define AMX_NUMTAGS       (*g_engAmxFunc->pfnamx_NumTags)
#define AMX_RAISEERROR    (*g_engAmxFunc->pfnamx_RaiseError)
#define AMX_REGISTER      (*g_engAmxFunc->pfnamx_Register)
#define AMX_RELEASE       (*g_engAmxFunc->pfnamx_Release)
#define AMX_SETCALLBACK   (*g_engAmxFunc->pfnamx_SetCallback)
#define AMX_SETDEBUGHOOK  (*g_engAmxFunc->pfnamx_SetDebugHook)
#define AMX_SETSTRING     (*g_engAmxFunc->pfnamx_SetString)
#define AMX_SETUSERDATA   (*g_engAmxFunc->pfnamx_SetUserData)
#define AMX_STRLEN        (*g_engAmxFunc->pfnamx_StrLen)

// Modules API
struct pfnmodule_engine_g {
  int         (*pfnadd_amxnatives)(AMX_NATIVE_INFO*);                         // list
  int         (*pfnset_amxnatives)(AMX*, char[64]);                           // amx, error info
  void*       (*pfnalloc_amxmemory)(void**, int size);
  void        (*pfncopy_amxmemory)(cell*, cell*, int);                        // dest, src, len
  void        (*pfnfree_amxmemory)(void**);
  cell*       (*pfnget_amxaddr)(AMX*, cell);                                  // amx, cell
  int         (*pfnset_amxstring)(AMX*, cell, const char*, int);              // amx, dest, string, maxlen
  char*       (*pfnget_amxstring)(AMX*, cell, int, int&);                     // amx, src, buffer (0-3), len
  int         (*pfnamxstring_length)(cell*);                                  // src
  char*       (*pfnformat_amxstring)(AMX*, cell*, int , int&);                // amx, format, start pos, len
  AMX*        (*pfnget_amxscript)(int, void**, const char**);                 // id, code, name
  const char* (*pfnget_amxscriptname)(AMX* amx);                              // amx
  int         (*pfnFindAmxScriptByAmx)(const AMX *amx);
  int         (*pfnFindAmxScriptByName)(const char *name);
  int         (*pfnload_amxscript)(AMX*, void**, const char*, char[64], int); // amx, code, path, error info, want jit
  int         (*pfnunload_amxscript)(AMX* amx, void**);                       // amx, code
  bool        (*pfnis_plugin_running)(AMX*);
  void        (*pfnprint_srvconsole)(char*, ...);                             // format, ...
  void        (*pfnreport_error)(int code, char*, ...);
  void        (*pfnamx_logerror)(AMX*, int, const char*, ...);
  void        (*pfnlogerror)(const char*, ...);
  const char* (*pfnget_localinfo)(const char* name, const char* defaultvalue);
  void        (*pfnget_modname)(char*);                                       // modname
  char*       (*pfnbuild_pathname)(char*, ...);                               // format, ...
  bool        (*pfnIsPlayer)(int, int, AMX*);
  bool        (*pfnIsEntity)(int, AMX*);
  bool        (*pfnIsBot)(int, AMX*);
// By AMXMODX Dev Team
  int         (*pfnIsPlayerValid)(int id);
  int         (*pfnIsPlayerHLTV)(int id);
  int         (*pfnIsPlayerBot)(int id);
  int         (*pfnIsPlayerConnecting)(int id);
  int         (*pfnIsPlayerAuthorized)(int id);
  int         (*pfnIsPlayerInGame)(int id);
  int         (*pfnIsPlayerAlive)(int id);
  const char* (*pfnGetPlayerName)(int id);
  const char* (*pfnGetPlayerIP)(int id);
  float       (*pfnGetPlayerTime)(int id);
  float       (*pfnGetPlayerPlayTime)(int id);
  int         (*pfnGetPlayerCurweapon)(int id);
  const char* (*pfnGetPlayerTeam)(int id);
  int         (*pfnGetPlayerTeamID)(int id);
  int         (*pfnGetPlayerFrags)(int id);
  int         (*pfnGetPlayerDeaths)(int id);
  int         (*pfnGetPlayerHealth)(int id);
  int         (*pfnGetPlayerArmor)(int id);
  int         (*pfnGetPlayerMenu)(int id);
  int         (*pfnGetPlayerKeys)(int id);
  int         (*pfnGetPlayerFlags)(int id);
#ifdef USE_METAMOD
  edict_t*    (*pfnGetPlayerEdict)(int id);
#else
  void*       (*pfnGetPlayerEdict)(int id);
#endif
  int         (*pfnSetPlayerTeamInfo)(int id, int teamid, const char *teamname);
  void*       (*pfnPlayerPropAddr)(int id, int prop);
// End of By AMXMODX Dev Team
};
extern pfnmodule_engine_g* g_engModuleFunc;

#define MF_AddNatives           (*g_engModuleFunc->pfnadd_amxnatives)
#define MF_SetNatives           (*g_engModuleFunc->pfnset_amxnatives)
#define MF_AllocAmxMemory       (*g_engModuleFunc->pfnalloc_amxmemory)
#define MF_CopyAmxMemory        (*g_engModuleFunc->pfncopy_amxmemory)
#define MF_FreeAmxMemory        (*g_engModuleFunc->pfnfree_amxmemory)
#define MF_GetAmxAddr           (*g_engModuleFunc->pfnget_amxaddr)
#define MF_SetAmxString         (*g_engModuleFunc->pfnset_amxstring)
#define MF_GetAmxString         (*g_engModuleFunc->pfnget_amxstring)
#define MF_GetAmxStringLen      (*g_engModuleFunc->pfnamxstring_length)
#define MF_FormatAmxString      (*g_engModuleFunc->pfnformat_amxstring)
#define MF_GetAmxScript         (*g_engModuleFunc->pfnget_amxscript)
#define MF_GetAmxScriptName     (*g_engModuleFunc->pfnget_amxscriptname)
#define MF_FindScriptByAmx      (*g_engModuleFunc->pfnFindAmxScriptByAmx)
#define MF_FindScriptByName     (*g_engModuleFunc->pfnFindAmxScriptByName)
#define MF_LoadAmxScript        (*g_engModuleFunc->pfnload_amxscript)
#define MF_UnloadAmxScript      (*g_engModuleFunc->pfnunload_amxscript)
#define MF_IsPluginRunning      (*g_engModuleFunc->pfnis_plugin_running)
#define MF_PrintSrvConsole      (*g_engModuleFunc->pfnprint_srvconsole)
#define MF_ReportError          (*g_engModuleFunc->pfnreport_error)
#define MF_AmxLogError          (*g_engModuleFunc->pfnamx_logerror)
#define MF_LogError             (*g_engModuleFunc->pfnlogerror)
#define MF_GetLocalInfo         (*g_engModuleFunc->pfnget_localinfo)
#define MF_GetModName           (*g_engModuleFunc->pfnget_modname)
#define MF_BuildPathname        (*g_engModuleFunc->pfnbuild_pathname)
#define MF_IsPlayer(a, b)       (*g_engModuleFunc->pfnIsPlayer)(a, b, amx)
#define MF_IsEntity(a)          (*g_engModuleFunc->pfnIsEntity)(a, amx)
#define MF_IsBot(a)             (*g_engModuleFunc->pfnIsBot)(a, amx)
// By AMXMODX Dev Team
#define MF_IsPlayerValid        (*g_engModuleFunc->pfnIsPlayerValid)
#define MF_IsPlayerHLTV         (*g_engModuleFunc->pfnIsPlayerHLTV)
#define MF_IsPlayerBot          (*g_engModuleFunc->pfnIsPlayerBot)
#define MF_IsPlayerConnecting   (*g_engModuleFunc->pfnIsPlayerConnecting)
#define MF_IsPlayerAuthorized   (*g_engModuleFunc->pfnIsPlayerAuthorized)
#define MF_IsPlayerIngame       (*g_engModuleFunc->pfnIsPlayerInGame)
#define MF_IsPlayerAlive        (*g_engModuleFunc->pfnIsPlayerAlive)
#define MF_GetPlayerName        (*g_engModuleFunc->pfnGetPlayerName)
#define MF_GetPlayerIP          (*g_engModuleFunc->pfnGetPlayerIP)
#define MF_GetPlayerTime        (*g_engModuleFunc->pfnGetPlayerTime)
#define MF_GetPlayerPlayTime    (*g_engModuleFunc->pfnGetPlayerPlayTime)
#define MF_GetPlayerCurweapon   (*g_engModuleFunc->pfnGetPlayerCurweapon)
#define MF_GetPlayerTeam        (*g_engModuleFunc->pfnGetPlayerTeam)
#define MF_GetPlayerTeamID      (*g_engModuleFunc->pfnGetPlayerTeamID)
#define MF_GetPlayerFrags       (*g_engModuleFunc->pfnGetPlayerFrags)
#define MF_GetPlayerDeaths      (*g_engModuleFunc->pfnGetPlayerDeaths)
#define MF_GetPlayerHealth      (*g_engModuleFunc->pfnGetPlayerHealth)
#define MF_GetPlayerArmor       (*g_engModuleFunc->pfnGetPlayerArmor)
#define MF_GetPlayerMenu        (*g_engModuleFunc->pfnGetPlayerMenu)
#define MF_GetPlayerKeys        (*g_engModuleFunc->pfnGetPlayerKeys)
#define MF_GetPlayerFlags       (*g_engModuleFunc->pfnGetPlayerFlags)
#define MF_GetPlayerEdict       (*g_engModuleFunc->pfnGetPlayerEdict)
#define MF_SetPlayerTeamInfo    (*g_engModuleFunc->pfnSetTeamInfo)
#define MF_PlayerPropAddr       (*g_engModuleFunc->pfnPlayerPropAddr)
// End of By AMXMODX Dev Team

// Misc
#define NATIVE(n) static cell AMX_NATIVE_CALL n(AMX *amx, cell *params)
inline cell  FloatToCell(float fValue)   { return *(cell  *)((void *)&fValue); }
inline float CellToFloat(cell cellValue) { return *(float *)((void *)&cellValue); }

// By AMXMODX Dev Team
typedef enum
{
  Player_Name,          //String
  Player_Ip,            //String
  Player_Team,          //String
  Player_Ingame,        //bool
  Player_Authorized,    //bool
  Player_Time,          //float
  Player_Playtime,      //float
  Player_ClearMenuTime, //float
  Player_Weapons,       //struct{int,int}[32]
  Player_CurrentWeapon, //int
  Player_TeamID,        //int
  Player_Deaths,        //int
  Player_Aiming,        //int
  Player_Menu,          //int
  Player_Keys,          //int
  Player_Flags,         //int[32]
}  PlayerProp;

#endif //__MODULES_H__
