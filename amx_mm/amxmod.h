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
 
#ifndef AMXMOD_H
#define AMXMOD_H

#include "modules.h"
#include "CString.h"
#include "CList.h"
#include "CTranslator.h"
#include "CPlugin.h"
#include "CMisc.h"
#include "CVault.h"
#include "CModule.h"
#include "CTask.h"
#include "CLogEvent.h"
#include "CForward.h"
#include "CCmd.h"
#include "CMenu.h"
#include "CEvent.h"

#include <meta_api.h>

#define AUTHORS "AMX Mod Team and [APG]"
#define AMX_VERSION "2021.0-APG"
#define URL "www.amxmod.net"
#define DATE __DATE__

#ifdef __cplusplus
extern "C" {
#endif
extern AMX_NATIVE_INFO core_Natives[];
extern AMX_NATIVE_INFO time_Natives[];
extern AMX_NATIVE_INFO power_Natives[];
#ifdef __cplusplus
}
#endif
extern AMX_NATIVE_INFO amxmod_Natives[];
extern AMX_NATIVE_INFO file_Natives[];
extern AMX_NATIVE_INFO float_Natives[];
extern AMX_NATIVE_INFO string_Natives[];
extern AMX_NATIVE_INFO vault_Natives[];
extern AMX_NATIVE_INFO math_Natives[];
extern AMX_NATIVE_INFO vector_Natives[];
extern AMX_NATIVE_INFO message_Natives[];

#ifndef LINUX
#define DLLOAD(path) (DLHANDLE)LoadLibrary(path);
#define DLPROC(m,func) GetProcAddress(m, func);
#define DLFREE(m) FreeLibrary(m);
#else
#define DLLOAD(path) (DLHANDLE)dlopen(path, RTLD_NOW);
#define DLPROC(m,func) dlsym(m,func);
#define DLFREE(m) dlclose(m);
#endif

#ifndef LINUX
typedef HINSTANCE DLHANDLE;
#else
typedef void* DLHANDLE;
#endif

#ifndef GETPLAYERAUTHID
#define GETPLAYERAUTHID   (*g_engfuncs.pfnGetPlayerAuthId)
#endif

#define ANGLEVECTORS        (*g_engfuncs.pfnAngleVectors)
#define CLIENT_PRINT        (*g_engfuncs.pfnClientPrintf)
#define CVAR_DIRECTSET      (*g_engfuncs.pfnCvar_DirectSet)
#define GETCLIENTLISTENING  (*g_engfuncs.pfnVoice_GetClientListening)
#define RUNPLAYERMOVE       (*g_engfuncs.pfnRunPlayerMove)
#define SETCLIENTLISTENING  (*g_engfuncs.pfnVoice_SetClientListening)
#define SETCLIENTMAXSPEED   (*g_engfuncs.pfnSetClientMaxspeed)

char* UTIL_SplitHudMessage(const char *src);
int UTIL_ReadFlags(const char* c);
void UTIL_ClientPrint(edict_t *pEntity, int msg_dest, char *msg);
void UTIL_FakeClientCommand(edict_t *pEdict, const char *cmd, const char *arg1 = nullptr, const char *arg2 = nullptr);
void UTIL_GetFlags(char* flags, int flag);
void UTIL_HudMessage(edict_t *pEntity, const hudtextparms_t &textparms, char *pMessage);
void UTIL_IntToString(int value, char *output);
void UTIL_ShowMOTD( edict_t *client, char *motd, int mlen, const char *name);
void UTIL_ShowMenu( edict_t* pEntity, int slots, int time, char *menu, int mlen);

#define GET_PLAYER_POINTER(e)   (&g_players[ENTINDEX(e)])
#define GET_PLAYER_POINTER_I(i) (&g_players[i])

struct WeaponsVault {
	String fullName;
	short int iId;
	short int ammoSlot;
};

struct fakecmd_t {
	char args[256];
	const char *argv[3];
	int argc;
	bool fake;
};

extern DLL_FUNCTIONS *g_pFunctionTable;
extern DLL_FUNCTIONS *g_pFunctionTable_Post;
extern enginefuncs_t *g_pengfuncsTable;
extern enginefuncs_t *g_pengfuncsTable_Post;
extern enginefuncs_t *g_pEngTable;

extern CPluginMngr g_plugins;
extern CTaskMngr g_tasksMngr;
extern CPlayer g_players[33];
extern CPlayer* mPlayer;
extern CmdMngr g_commands;
extern CTranslator g_translator;
extern CTranslator::CDictionary *g_dictionary;
extern CList<CCVar> g_cvars;
extern CList<ForceObject> g_forcemodels;
extern CList<ForceObject> g_forcesounds;
extern CList<ForceObject> g_forcegeneric;
extern CList<CModule> g_modules;
extern CList<CPlayer*> g_auth;
extern EventsMngr g_events;
extern Grenades g_grenades;
extern LogEventsMngr g_logevents;
extern MenuMngr g_menucmds;
extern String g_log_dir;
extern String g_mod_name;
extern TeamIds g_teamsIds;
extern Vault g_vault;
extern CForwardMngr g_forwards;
extern WeaponsVault g_weaponsData[MAX_WEAPONS];
extern XVars g_xvars;
extern bool g_bmod_cstrike;
extern bool g_bmod_dod;
extern bool g_dontprecache;
extern bool g_initialized;
extern bool g_pluginsloaded;
extern int g_srvindex;
//extern cvar_t* amx_version;
//extern cvar_t* amx_modules;
extern cvar_t* cvar_amx_debug;
extern cvar_t* cvar_amx_langdebug;
extern cvar_t* cvar_amx_log_to_hl_logs;
extern cvar_t* hostname;
extern cvar_t* mp_timelimit;
extern fakecmd_t g_fakecmd;
extern float g_game_restarting;
extern float g_game_timeleft;
extern float g_task_time;
extern float g_auth_time;
extern hudtextparms_t g_hudset;
extern int g_players_num;
extern int mPlayerIndex;
extern int mState;
extern int g_autoload_modules;
extern int g_autoloading_modules;
extern void (*endfunction)(void*);
extern void (*function)(void*);
extern char* UTIL_VarArgs(char *fmt, ...);

typedef void (*funEventCall)(void*);
extern funEventCall modMsgsEnd[MAX_REG_MSGS];
extern funEventCall modMsgs[MAX_REG_MSGS];

extern int gmsgAmmoPickup;
extern int gmsgAmmoX;
extern int gmsgBattery;
extern int gmsgCurWeapon;
extern int gmsgDamage;
extern int gmsgDeathMsg;
extern int gmsgHealth;
extern int gmsgMOTD;
extern int gmsgScoreInfo;
extern int gmsgSendAudio;
extern int gmsgServerName;
extern int gmsgShowMenu;
extern int gmsgTeamInfo;
extern int gmsgTextMsg;
extern int gmsgVGUIMenu;
extern int gmsgWeapPickup;
extern int gmsgWeaponList;
extern int gmsgintermission;
extern int gmsgResetHUD;
extern int gmsgRoundTime;

void Client_AmmoPickup(void*);
void Client_AmmoX(void*);
void Client_CurWeapon(void*);
void Client_ScoreInfo(void*);
void Client_ShowMenu(void*);
void Client_TeamInfo(void*);
void Client_TextMsg(void*);
void Client_VGUIMenu(void*);
void Client_WeaponList(void*);
void Client_DamageEnd(void*);
void Client_DeathMsg(void*);

void amx_command();
void plugin_srvcmd();

const char* stristr(const char* a, const char* b);
char *strptime(const char *buf, const char *fmt, struct tm *tm, short addthem);

int loadModule(const char *modulename, PLUG_LOADTIME now, bool addExt);
void loadModules(const char* filename, PLUG_LOADTIME now);
void detachModules();
void detachReloadModules();
void attachModules();
void modulesPluginsLoaded();
int countLoadedModules();
extern CModule *g_calledModule;

char * TR(const char *string);

int add_amxnatives(AMX_NATIVE_INFO* natives);
cell* get_amxaddr(AMX *amx, cell amx_addr);
char* build_pathname(char *fmt, ...);
void create_path(char *path);
char* format_amxstring(AMX *amx, cell *params, int parm, int& len);
AMX* get_amxscript(int, void**, const char**);
const char* get_amxscriptname(AMX* amx);
char* get_amxstring(AMX *amx, cell amx_addr, int id, int& len);
extern "C" size_t get_amxstring_r(AMX *amx, cell amx_addr, char *destination, int maxlen);
int amxstring_len(cell* cstr);
const char* get_localinfo(const char* name, const char* def);
int load_amxscript(AMX* amx, void** program, const char* path, char error[64], int wantjit);
int set_amxnatives(AMX* amx, char error[64]);
int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);
int unload_amxscript(AMX* amx, void** program);
void copy_amxmemory(cell* dest, cell* src, int len);
void get_modname(char*);
void amx_logerror(AMX *amx, int err, const char *error, ...);
void amx_log(char *szfile, char *message);
void print_srvconsole(char *fmt, ...);
void report_error(int code, char* fmt, ...);
void logerror(const char *error, ...);
void logtranslatorerror(const char *error, ...);
void* alloc_amxmemory(void**, int size);
void free_amxmemory(void **ptr);
bool is_plugin_running(AMX *amx);
int fo_numargs(AMX *amx);
bool is_player(int id, int ingame, AMX *amx);
bool is_entity(int id, AMX *amx);
bool is_bot(int id, AMX *amx);
// By AMXMODX Dev Team
int MODFUNC_FindAmxScriptByAmx(const AMX *amx);
int MODFUNC_FindAmxScriptByName(const char *name);
int MODFUNC_IsPlayerValid(int id);
const char* MODFUNC_GetPlayerName(int id);
const char* MODFUNC_GetPlayerIP(int id);
int MODFUNC_IsPlayerInGame(int id);
int MODFUNC_IsPlayerBot(int id);
int MODFUNC_IsPlayerAuthorized(int id);
float MODFUNC_GetPlayerTime(int id);
float MODFUNC_GetPlayerPlayTime(int id);
int MODFUNC_GetPlayerCurweapon(int id);
const char* MODFUNC_GetPlayerTeam(int id);
int MODFUNC_GetPlayerTeamID(int id);
int MODFUNC_GetPlayerDeaths(int id);
int MODFUNC_GetPlayerMenu(int id);
int MODFUNC_GetPlayerKeys(int id);
int MODFUNC_IsPlayerAlive(int id);
int MODFUNC_GetPlayerFrags(int id);
int MODFUNC_IsPlayerConnecting(int id);
int MODFUNC_IsPlayerHLTV(int id);
int MODFUNC_GetPlayerArmor(int id);
int MODFUNC_GetPlayerHealth(int id);
int MODFUNC_GetPlayerFlags(int id);
edict_t* MODFUNC_GetPlayerEdict(int id);
int MODFUNC_SetPlayerTeamInfo(int id, int teamid, const char *teamname);
void* MODFUNC_PlayerPropAddr(int id, int prop);
// End of By AMXMODX Dev Team

void WriteByte(int iValue);
void WriteChar(int iValue);
void WriteShort(int iValue);
void WriteLong(int iValue);
void WriteEntity(int iValue);
void WriteAngle(float flValue);
void WriteCoord(float flValue);
void WriteString(const char *sz);
void MessageEnd();
void MessageEnd_Post();
int PrecacheModel(char *s);
int PrecacheSound(char *s);
//const char *Cmd_Args(void);
//const char *Cmd_Argv(int argc);
//int Cmd_Argc(void);
void AlertMessage_Post(ALERT_TYPE atype, char *szFmt, ...);
int InconsistentFile(const edict_t *player, const char *filename, char *disconnect_message);
void ChangeLevel(char *map, char *dummy);

#endif // AMXMOD_H
