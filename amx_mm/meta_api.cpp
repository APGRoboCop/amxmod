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

#include <extdll.h>
#include <meta_api.h>
#include <time.h>
#include <dirent.h>
#include "amxmod.h"
#include "optimizer.h"


plugin_info_t Plugin_info = {
  META_INTERFACE_VERSION, // ifvers
  "AMX Mod",  // name
  AMX_VERSION,  // version
  DATE, // date
  AUTHORS, // author
  URL,  // url
  "AMX Mod",  // logtag
  PT_ANYTIME, // (when) loadable
  PT_ANYTIME, // (when) unloadable
};

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;
enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;

DLL_FUNCTIONS gFunctionTable;
DLL_FUNCTIONS gFunctionTable_Post;
enginefuncs_t meta_engfuncs;
enginefuncs_t meta_engfuncs_post;
NEW_DLL_FUNCTIONS gNewDLLFunctionTable;

DLL_FUNCTIONS *g_pFunctionTable;
DLL_FUNCTIONS *g_pFunctionTable_Post;
enginefuncs_t *g_pengfuncsTable;
enginefuncs_t *g_pengfuncsTable_Post;

funEventCall modMsgsEnd[MAX_REG_MSGS];
funEventCall modMsgs[MAX_REG_MSGS];
void (*function)(void*);
void (*endfunction)(void*);

CForwardMngr g_forwards;
CList<CPlayer*> g_auth;
CList<CCVar> g_cvars;
CList<ForceObject> g_forcemodels;
CList<ForceObject> g_forcesounds;
CList<ForceObject> g_forcegeneric;
CPlayer g_players[33];
CPlayer* mPlayer;
CPluginMngr g_plugins;
CTaskMngr g_tasksMngr;
CmdMngr g_commands;
EventsMngr g_events;
Grenades g_grenades;
LogEventsMngr g_logevents;
MenuMngr g_menucmds;
String g_log_dir;
String g_mod_name;
char g_plugins_file[256];
XVars g_xvars;
bool g_bmod_cstrike;
bool g_bmod_dod;
bool g_bmod_tfc;
bool g_dontprecache;
bool g_pluginsloaded;
bool g_forcedmodels;
bool g_forcedsounds;
bool g_initialized;
bool g_activated = false;
fakecmd_t g_fakecmd;
float g_game_restarting;
float g_game_timeleft;
float g_task_time;
float g_auth_time;
hudtextparms_t g_hudset;
int g_players_num;
int mPlayerIndex;
int mState;
int g_eventFound;
bool g_msgBlock;
int g_srvindex;
int g_autoload_modules = 0;
int g_autoloading_modules = 0;

cvar_t init_amx_version={"amx_version", "", FCVAR_SERVER | FCVAR_SPONLY};
cvar_t init_amx_modules={"amx_modules", "", FCVAR_SERVER | FCVAR_SPONLY};
cvar_t init_amx_debug={"amx_debug", "0", FCVAR_SPONLY};
cvar_t init_amx_langdebug={"amx_langdebug", "", FCVAR_SPONLY};
cvar_t init_amx_log_to_hl_logs={"amx_log_to_hl_logs", "0", FCVAR_SPONLY};
//cvar_t* amx_version = NULL;
//cvar_t* amx_modules = NULL;
cvar_t* cvar_amx_debug = NULL;
cvar_t* cvar_amx_langdebug = NULL;
cvar_t* cvar_amx_log_to_hl_logs = NULL;
cvar_t* hostname = NULL;
cvar_t* mp_timelimit = NULL;


// Precache stuff from force consistency calls
// or check for pointed files won't be done
int PrecacheModel(char *s) {
  if(!g_forcedmodels) {
    g_forcedmodels = true;
    g_pengfuncsTable->pfnPrecacheModel = NULL;
    for(CList<ForceObject>::iterator a = g_forcemodels.begin(); a; ++a) {
      PRECACHE_MODEL((char*)(*a).getFilename());
      ENGINE_FORCE_UNMODIFIED((*a).getForceType(), (*a).getMin(), (*a).getMax(), (*a).getFilename());
    }
  }
  RETURN_META_VALUE(MRES_IGNORED, 0);
}

int PrecacheSound(char *s) {
  if(!g_forcedsounds) {
    g_forcedsounds = true;
    g_pengfuncsTable->pfnPrecacheSound = NULL;
    for(CList<ForceObject>::iterator a = g_forcesounds.begin(); a; ++a) {
      PRECACHE_SOUND((char*)(*a).getFilename());
      ENGINE_FORCE_UNMODIFIED((*a).getForceType(), (*a).getMin(), (*a).getMax(), (*a).getFilename());
    }
    if(!g_bmod_cstrike){
      PRECACHE_SOUND("weapons/cbar_hitbod1.wav");
      PRECACHE_SOUND("weapons/cbar_hitbod2.wav");
      PRECACHE_SOUND("weapons/cbar_hitbod3.wav");
    }
  }
  RETURN_META_VALUE(MRES_IGNORED, 0);
}

// On InconsistentFile call forward function from plugins
int InconsistentFile(const edict_t *player, const char *filename, char *disconnect_message) {
  /*if(!g_forwards.forwardsExist(FF_InconsistentFile)) {
    RETURN_META_VALUE(MRES_IGNORED, FALSE);
  }*/
  if(MDLL_InconsistentFile(player, filename, disconnect_message)) {
    cell ret = 0;
    CPlayer *pPlayer = GET_PLAYER_POINTER((edict_t *)player);
    CForwardMngr::iterator a = g_forwards.begin(FF_InconsistentFile);

    while(a) {
      if((*a).getPlugin()->isExecutable((*a).getFunction())) {
        AMX* c = (*a).getPlugin()->getAMX();
        cell amx_addr1, *phys_addr1;
        cell amx_addr2, *phys_addr2;
        if((amx_Allot(c, 64, &amx_addr1, &phys_addr1) != AMX_ERR_NONE) ||
          (amx_Allot(c, 64, &amx_addr2, &phys_addr2) != AMX_ERR_NONE)) {
          logerror("[AMX] Failed to allocate AMX memory (plugin \"%s\")\n", (*a).getPlugin()->getName());
        }
        else {
          int err;
          set_amxstring(c, amx_addr1, filename, 63);
          set_amxstring(c, amx_addr2, disconnect_message, 63);
          if((err = amx_Exec(c, &ret, (*a).getFunction(), 3, pPlayer->index, amx_addr1, amx_addr2)) != AMX_ERR_NONE) {
            amx_logerror((*a).getPlugin()->getAMX(), err, "");
          }
          int len;
          strcpy(disconnect_message, get_amxstring(c, amx_addr2, 0, len));
          amx_Release(c, amx_addr2);
          amx_Release(c, amx_addr1);
        }
        if(ret & 1) {
          RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
        }
      }
      ++a;
    }
    RETURN_META_VALUE(MRES_SUPERCEDE, TRUE );
  }
  RETURN_META_VALUE(MRES_IGNORED, FALSE);
}

const char* get_localinfo(const char* name, const char* def) {
  const char* b = LOCALINFO((char*)name);
  if(b == 0 || *b == 0) {
    char val[256];
    if(!strncmp(def, "$basedir", 8)) {
      snprintf(val, 255, "%s%s", get_localinfo("amx_basedir", "addons/amx"), def + 8);
      b = val;
    }
    else if(!strncmp(def, "$configdir", 10)) {
      snprintf(val, 255, "%s%s", get_localinfo("amx_configdir", "$basedir/config"), def + 10);
      b = val;
    }
    else if(!strncmp(def, "$langdir", 8)) {
      snprintf(val, 255, "%s%s", get_localinfo("amx_langdir", "$basedir/lang"), def + 8);
      b = val;
    }
    else if(!strncmp(def,"$modulesdir",11))
    {
      snprintf(val,255,"%s%s",get_localinfo("amx_modulesdir","$basedir/modules"),def+11);
      b = val;
    }
    else if(!strncmp(def,"$pluginsdir",11))
    {
      snprintf(val,255,"%s%s",get_localinfo("amx_pluginsdir","$basedir/plugins"),def+11);
      b = val;
    }
    else if(!strncmp(def,"$logdir",7))
    {
      snprintf(val,255,"%s%s",get_localinfo("amx_logdir","$basedir/logs"),def+7);
      b = val;
    }
    else {
      b = def;
    }
    SET_LOCALINFO((char*)name, (char *)b);
    b = LOCALINFO((char*)name);
  }
  return b;
}

bool isValidMapPluginsFile(const char* name, const char* mapName) {
  size_t len;
  len = strlen(name);
  if(len <= 11 || (len > 11 && (strncmp(name, "prefix_", 7) || strncmp(&name[len-4], ".ini", 4))))
    return false;
  len = strlen(&name[7]) - 4;
  if(strncmp(mapName, &name[7], len) == 0)
    return true;
  return false;
}

bool isValidPluginsFile(const char* name) {
  size_t len;
  len = strlen(name);
  if(len > 12 && strncmp(name, "plugins-", 8) == 0 && strncmp(&name[len-4], ".ini", 4) == 0)
    return true;
  return false;
}

void loadMapPluginsIni() {
  char path[192];
  snprintf(path, sizeof(path)-1, "%s", build_pathname("%s/", get_localinfo("amx_mapspluginsdir", "$configdir/maps_plugins")));
  bool doNotExecPrefix = false;
  
  char currentMap[64];
  snprintf(currentMap, sizeof(currentMap)-1, "%s", STRING(gpGlobals->mapname));
  
  char file[256];
  snprintf(file, sizeof(file)-1, "%smapgroups.ini", path);
  
  FILE* fp;
  
  fp = fopen(file, "rt");
  if(fp != NULL) {
    char line[128], iniFile[64], mapName[64];
    while(!feof(fp)) {
      line[0] = '\0';
      fgets(line, 127, fp);
      if(line[0] == ';' || line[0] == '/' || line[0] == '\n') continue;
  	  iniFile[0] = '\0';
  		mapName[0] = '\0';
  		if(sscanf(line, "%s %s", iniFile, mapName) == EOF) {
        continue;
      }
      if(strcmp(mapName, currentMap) == 0) {
        g_plugins_file[0] = '\0';
        snprintf(g_plugins_file, sizeof(g_plugins_file)-1, "%s%s", path, iniFile);
        g_plugins.loadPluginsFromFile(g_plugins_file);
        doNotExecPrefix = true;
        break;
      }
    }
    fclose(fp);
  }
  
  if(doNotExecPrefix == false) {
    struct dirent *ep;
    DIR *dp;
    if((dp = opendir(path)) != NULL) {
      ep=readdir(dp); // .
      ep=readdir(dp); // ..
      while((ep=readdir(dp)) != NULL) {
        if(isValidMapPluginsFile(ep->d_name, currentMap)) {
          g_plugins_file[0] = '\0';
          snprintf(g_plugins_file, sizeof(g_plugins_file)-1, "%s%s", path, ep->d_name);
          g_plugins.loadPluginsFromFile(g_plugins_file);
          break;
        }
      }
      closedir(dp);
    }
  }
  
  g_plugins_file[0] = '\0';
  snprintf(g_plugins_file, sizeof(g_plugins_file)-1, "%s%s.ini", path, STRING(gpGlobals->mapname));
  fp = fopen(g_plugins_file, "rt");
  if(fp != NULL) {
    fclose(fp);
    g_plugins.loadPluginsFromFile(g_plugins_file);
  }
}

void loadPluginsIni() {
  g_plugins.loadPluginsFromFile(build_pathname("%s", get_localinfo("amx_plugins", "$configdir/plugins.ini")));

  char path[192];
  snprintf(path, sizeof(path)-1, "%s", build_pathname("%s/", get_localinfo("amx_configdir", "$basedir/config")));
  struct dirent *ep;
  DIR *dp;

  if((dp = opendir(path)) != NULL) {
    ep=readdir(dp); // .
    ep=readdir(dp); // ..
    while((ep=readdir(dp)) != NULL) {
      if(isValidPluginsFile(ep->d_name)) {
        g_plugins_file[0] = '\0';
        snprintf(g_plugins_file, sizeof(g_plugins_file)-1, "%s%s", path, ep->d_name);
        g_plugins.loadPluginsFromFile(g_plugins_file);
      }
    }
    closedir(dp);
  }
  
  loadMapPluginsIni();
  
  return;
}

// Very first point at map load
// Load AMX modules for new native functions
// Initialize AMX stuff and load its plugins from plugins.ini list
// Call precache forward function from plugins
int Spawn(edict_t *pent) {
  if(g_initialized) {
    RETURN_META_VALUE(MRES_IGNORED, 0);
  }

  g_pengfuncsTable->pfnPrecacheModel = NULL;
  g_pengfuncsTable->pfnPrecacheSound = NULL;
  //g_pengfuncsTable->pfnCmd_Args = NULL;
  //g_pengfuncsTable->pfnCmd_Argv = NULL;
  //g_pengfuncsTable->pfnCmd_Argc = NULL;

  g_pengfuncsTable_Post->pfnAlertMessage = NULL;
  g_pFunctionTable->pfnInconsistentFile = NULL;
  g_pengfuncsTable->pfnChangeLevel = NULL;

  g_pengfuncsTable->pfnWriteByte = NULL;
  g_pengfuncsTable->pfnWriteChar = NULL;
  g_pengfuncsTable->pfnWriteShort = NULL;
  g_pengfuncsTable->pfnWriteLong = NULL;
  g_pengfuncsTable->pfnWriteAngle = NULL;
  g_pengfuncsTable->pfnWriteCoord = NULL;
  g_pengfuncsTable->pfnWriteString = NULL;
  g_pengfuncsTable->pfnWriteEntity = NULL;
  g_pengfuncsTable->pfnMessageEnd = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;

  g_activated = false;
  g_initialized = true;
  g_forcedmodels = false;
  g_forcedsounds = false;
  g_pengfuncsTable->pfnPrecacheModel = PrecacheModel;
  g_pengfuncsTable->pfnPrecacheSound = PrecacheSound;

  g_srvindex = IS_DEDICATED_SERVER() ? 0 : 1;

  hostname = CVAR_GET_POINTER("hostname");
  mp_timelimit = CVAR_GET_POINTER("mp_timelimit");

  // ###### Initialize task manager
  g_tasksMngr.registerTimers(&gpGlobals->time, &mp_timelimit->value, &g_game_timeleft);

  // ###### Initialize commands prefixes
  g_commands.registerPrefix("amx");
  g_commands.registerPrefix("say");
  g_commands.registerPrefix("admin_");
  g_commands.registerPrefix("sm_");
  g_commands.registerPrefix("cm_");

  //  ###### Make sure local infos are set
  get_localinfo("amx_basedir", "addons/amx");
  get_localinfo("amx_configdir", "$basedir/config");
  get_localinfo("amx_langdir", "$basedir/lang");
  get_localinfo("amx_modulesdir", "$basedir/modules");
  get_localinfo("amx_pluginsdir", "$basedir/plugins");
  get_localinfo("amx_mapspluginsdir", "$configdir/maps_plugins");
  get_localinfo("amx_logdir", "$basedir/logs");
  get_localinfo("amx_modules", "$configdir/modules.ini");
  get_localinfo("amx_plugins", "$configdir/plugins.ini");
  get_localinfo("amx_vault", "$configdir/vault.ini");
  get_localinfo("amx_usejit", "config");
  get_localinfo("autoload_modules", "1");
  get_localinfo("optimizer", "7");

  Vault amx_config;
  // ###### Load custom paths configuration
  amx_config.setSource(build_pathname("%s", get_localinfo("amx_cfg", "$configdir/paths.ini")));

  if(amx_config.loadVault()) {
    Vault::iterator a = amx_config.begin();
    while(a != amx_config.end()) {
      SET_LOCALINFO((char*)a.key().str(), (char*)a.value().str());
      ++a;
    }
    amx_config.clear();
  }

  // ###### Save log dir
  g_log_dir.set(get_localinfo("amx_logdir", "$basedir/logs"));

  // ###### Load modules
  loadModules(get_localinfo("amx_modules", "$configdir/modules.ini"), PT_ANYTIME);

  // ###### Load Vault
  g_vault.setSource(build_pathname("%s", get_localinfo("amx_vault", "$configdir/vault.ini")));
  g_vault.loadVault();

  // ###### Init time and freeze tasks
  g_game_timeleft = g_bmod_dod ? 1.0f : 0.0f;
  g_task_time = gpGlobals->time + 99999.0f;
  g_auth_time = gpGlobals->time + 99999.0f;
  g_players_num = 0;

  // ###### Set server flags
  memset(g_players[0].flags, -1, sizeof(g_players[0].flags));

  // ######  Get AutoLoadModules Setting
  g_autoload_modules = atoi(get_localinfo("autoload_modules", "1"));

  // ###### Load AMX scripts
  g_pluginsloaded = false;
  if(g_autoload_modules) {
    g_opt_level = 65536;
    g_autoloading_modules = 1;
    loadPluginsIni();
    g_autoloading_modules = 0;
    g_plugins.clear();
  }
  
  g_opt_level = atoi(get_localinfo("optimizer", "7"));
  if(!g_opt_level) {
    g_opt_level = 7;
  }

  g_autoloading_modules = 0;
  loadPluginsIni();
  g_autoloading_modules = -1;
  g_pluginsloaded = true;
  
  char string[256];
  char date[32];
  time_t td; time(&td);
  strftime(date, 31, "error%Y%m%d.log", localtime(&td));
  CPluginMngr::iterator a = g_plugins.begin();
	while(a) {
		if(!stricmp((*a).getStatus(), "bad load")) {
			string[0] = '\0';
      snprintf(string, 255, "[AMX Error] Plugin: %s - Error: %s\n", (*a).getName(), (*a).getErrorMessage());
      amx_log(date, string);
		}
		++a;
	}
    
  g_forwards.findForwards();

  if(g_forwards.forwardsExist(FF_PluginLog)) {
    g_pengfuncsTable_Post->pfnAlertMessage = AlertMessage_Post;
  }
  if(g_forwards.forwardsExist(FF_InconsistentFile)) {
    g_pFunctionTable->pfnInconsistentFile = InconsistentFile;
  }
  if(g_forwards.forwardsExist(FF_ChangeLevel)) {
    g_pengfuncsTable->pfnChangeLevel = ChangeLevel;
  }

  modulesPluginsLoaded();

  // ###### Set some info about amx version and modules
  CVAR_SET_STRING(init_amx_version.name, AMX_VERSION);
  int loaded = countLoadedModules();
  char buffer[16];
  snprintf(buffer, 15, "%d", loaded);
  CVAR_SET_STRING(init_amx_modules.name, buffer);

  // ###### Call precache forward function
  g_dontprecache = false;
  g_forwards.executeForwards(FF_PluginPrecache);
  g_dontprecache = true;

  for(CList<ForceObject>::iterator a = g_forcegeneric.begin(); a; ++a) {
    PRECACHE_GENERIC((char*)(*a).getFilename());
    ENGINE_FORCE_UNMODIFIED((*a).getForceType(), (*a).getMin(), (*a).getMax(), (*a).getFilename());
  }
  RETURN_META_VALUE(MRES_IGNORED, 0);
}

struct sUserMsg {
  const char* name;
  int* id;
  funEventCall func;
  bool endmsg;
  bool cstrike;
} g_user_msg[] = {
  { "CurWeapon", &gmsgCurWeapon, Client_CurWeapon, false, false },
  { "Damage", &gmsgDamage, Client_DamageEnd, true, true },
  { "DeathMsg", &gmsgDeathMsg, Client_DeathMsg, false, true },
  { "TextMsg", &gmsgTextMsg, Client_TextMsg, false, false },
  { "TeamInfo", &gmsgTeamInfo, Client_TeamInfo, false, false },
  { "WeaponList", &gmsgWeaponList, Client_WeaponList, false, false },
  { "MOTD", &gmsgMOTD, 0, false, false },
  { "ServerName", &gmsgServerName, 0, false, false },
  { "Health", &gmsgHealth, 0, false, false },
  { "Battery", &gmsgBattery, 0, false, false },
  { "ShowMenu", &gmsgShowMenu, Client_ShowMenu, false, false },
  { "SendAudio", &gmsgSendAudio, 0, false, false },
  { "AmmoX", &gmsgAmmoX, Client_AmmoX, false, false },
  { "ScoreInfo", &gmsgScoreInfo, Client_ScoreInfo, false, false },
  { "VGUIMenu", &gmsgVGUIMenu, Client_VGUIMenu, false, false },
  { "AmmoPickup", &gmsgAmmoPickup, Client_AmmoPickup, false, false },
  { "WeapPickup", &gmsgWeapPickup, 0, false, false },
  { "ResetHUD", &gmsgResetHUD, 0, false, false },
  { "RoundTime", &gmsgRoundTime, 0, false, false },
  { 0, 0, 0, false, false }
};

int RegUserMsg_Post(const char *pszName, int iSize) {
  for(int i = 0; g_user_msg[ i ].name; ++i) {
    if(strcmp(g_user_msg[i].name, pszName) == 0) {
      int id = META_RESULT_ORIG_RET(int);
      *g_user_msg[i].id = id;

      if(!g_user_msg[i].cstrike || g_bmod_cstrike) {
        if(g_user_msg[i].endmsg) {
          modMsgsEnd[id] = g_user_msg[i].func;
        }
        else {
          modMsgs[id] = g_user_msg[i].func;
        }
      }
      break;
    }
  }
  RETURN_META_VALUE(MRES_IGNORED, 0);
}

/*
Much more later after precache. All is precached, server
will be flaged as ready to use so call
plugin_init forward function from plugins
*/
void ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
  int id;
  for(int i = 0; g_user_msg[i].name; ++i) {
    if((*g_user_msg[i].id == 0) && (id = GET_USER_MSG_ID(PLID, g_user_msg[ i ].name, NULL)) != 0) {
      *g_user_msg[i].id = id;

      if(!g_user_msg[i].cstrike || g_bmod_cstrike) {
        if(g_user_msg[i].endmsg) {
          modMsgsEnd[id] = g_user_msg[i].func;
        }
        else {
          modMsgs[id] = g_user_msg[i].func;
        }
      }
    }
  }
  RETURN_META(MRES_IGNORED);
}

void ServerActivate_Post(edict_t *pEdictList, int edictCount, int clientMax) {
  if(g_activated) {
    RETURN_META(MRES_IGNORED);
  }
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(i);
    pPlayer->Init(pEdictList + i, i);
  }

  g_dictionary = g_translator.loadDictionary("amxcore");
  g_forwards.executeForwards(FF_PluginInit);
  g_forwards.executeForwards(FF_PluginCfg);

  // Correct time in Counter-Strike and other mods (except DOD)
  //if ( !g_bmod_dod )  g_game_timeleft = 0;
  g_game_timeleft = 0;
  g_task_time = gpGlobals->time;
  g_auth_time = gpGlobals->time;
  g_activated = true;

  RETURN_META(MRES_IGNORED);
}

// Call plugin_end forward function from plugins.
void ServerDeactivate() {
  if(!g_activated) {
    RETURN_META(MRES_IGNORED);
  }
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(i);
    if(pPlayer->initialized) {
      g_forwards.executeForwards(FF_ClientDisconnect, 1, pPlayer->index);
    }
    if(pPlayer->ingame) {
      pPlayer->Disconnect();
      --g_players_num;
    }
  }
  g_players_num = 0;
  g_forwards.executeForwards(FF_PluginEnd);

  g_pengfuncsTable->pfnPrecacheModel = NULL;
  g_pengfuncsTable->pfnPrecacheSound = NULL;
  //g_pengfuncsTable->pfnCmd_Args = NULL;
  //g_pengfuncsTable->pfnCmd_Argv = NULL;
  //g_pengfuncsTable->pfnCmd_Argc = NULL;

  g_pengfuncsTable_Post->pfnAlertMessage = NULL;
  g_pFunctionTable->pfnInconsistentFile = NULL;
  g_pengfuncsTable->pfnChangeLevel = NULL;

  g_pengfuncsTable->pfnWriteByte = NULL;
  g_pengfuncsTable->pfnWriteChar = NULL;
  g_pengfuncsTable->pfnWriteShort = NULL;
  g_pengfuncsTable->pfnWriteLong = NULL;
  g_pengfuncsTable->pfnWriteAngle = NULL;
  g_pengfuncsTable->pfnWriteCoord = NULL;
  g_pengfuncsTable->pfnWriteString = NULL;
  g_pengfuncsTable->pfnWriteEntity = NULL;
  g_pengfuncsTable->pfnMessageEnd = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;

  RETURN_META(MRES_IGNORED);
}

// After all clear whole AMX configuration
// However leave AMX modules which are loaded only once
void ServerDeactivate_Post() {
  if(!g_initialized) {
    RETURN_META(MRES_IGNORED);
  }

  detachReloadModules();

  g_auth.clear();
  g_forwards.clear();
  g_commands.clear();
  g_forcemodels.clear();
  g_forcesounds.clear();
  g_forcegeneric.clear();
  g_grenades.clear();
  g_tasksMngr.clear();
  g_logevents.clearLogEvents();
  g_events.clearEvents();
  g_events.clearMessageHooks();
  g_menucmds.clear();
  g_vault.clear();
  g_xvars.clear();
  g_plugins.clear();
  g_translator.clear();
  g_dictionary = NULL;

  g_initialized = false;

  RETURN_META(MRES_IGNORED);
}

BOOL ClientConnect_Post(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]) {
  CPlayer* pPlayer = GET_PLAYER_POINTER(pEntity);
  if(!pPlayer->IsBot()) {
    bool a = pPlayer->Connect(pszName, pszAddress);
    g_forwards.executeForwards(FF_ClientConnect, 1, pPlayer->index);

    if(a) {
      CPlayer** aa = new CPlayer*(pPlayer);
      if(aa) {
        g_auth.put(aa);
      }
    }
    else {
      pPlayer->Authorize();
      g_forwards.executeForwards(FF_ClientAuthorized, 1, pPlayer->index);
    }
  }
  RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

void ClientDisconnect(edict_t *pEntity) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  if(pPlayer->initialized) {
    g_forwards.executeForwards(FF_ClientDisconnect, 1, pPlayer->index);
  }
  if(pPlayer->ingame) {
    --g_players_num;
  }
  pPlayer->Disconnect();

  RETURN_META(MRES_IGNORED);
}

void ClientPutInServer_Post(edict_t *pEntity) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  if(!pPlayer->IsBot()) {
    pPlayer->PutInServer();
    ++g_players_num;

    g_forwards.executeForwards(FF_ClientPutInServer, 1, pPlayer->index);
  }
  RETURN_META(MRES_IGNORED);
}

void ClientUserInfoChanged_Post(edict_t *pEntity, char *infobuffer) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  g_forwards.executeForwards(FF_ClientInfoChanged, 1, pPlayer->index);
  const char* name = INFOKEY_VALUE(infobuffer, "name");

  // Emulate bot connection and putinserver
  if(pPlayer->ingame) {
    pPlayer->name.set(name); // Make sure players have name up to date
  }
  else if(pPlayer->IsBot()) {
    pPlayer->Connect(name, "127.0.0.1"/*CVAR_GET_STRING("net_address")*/);

    g_forwards.executeForwards(FF_ClientConnect, 1, pPlayer->index);

    pPlayer->Authorize();
    g_forwards.executeForwards(FF_ClientAuthorized, 1, pPlayer->index);

    pPlayer->PutInServer();
    ++g_players_num;

    g_forwards.executeForwards(FF_ClientPutInServer, 1, pPlayer->index);
  }
  RETURN_META(MRES_IGNORED);
}

void ClientCommand(edict_t *pEntity) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  META_RES result = MRES_IGNORED;
  cell ret = 0;
  int err;

  const char* cmd = CMD_ARGV(0);
  const char* arg = CMD_ARGV(1);

  /* check amxmodules command */
  /*if(!strcmp(cmd, "amxmodules")) {
    UTIL_ClientPrint(pEntity, HUD_PRINTCONSOLE, TR("Currently loaded modules:\n"));
    UTIL_ClientPrint(pEntity, HUD_PRINTCONSOLE, UTIL_VarArgs("      %-23.22s %-8.7s %-20.19s %-11.10s\n",
      TR("name"),
      TR("version"),
      TR("author"),
      TR("status")));

    int modules = 0;

    CList<CModule>::iterator a = g_modules.begin();

    while(a) {
      if((*a).getStatusValue() == MODULE_LOADED) {
        ++modules;
        UTIL_ClientPrint(pEntity, HUD_PRINTCONSOLE, UTIL_VarArgs(" [%2d] %-23.22s %-8.7s %-20.19s %-11.10s\n", modules,
          (*a).getName(), (*a).getVersion(), (*a).getAuthor(), TR((*a).getStatus())));
      }
      ++a;
    }
    RETURN_META(MRES_SUPERCEDE);
  }*/

  /* because of PLUGIN_HANDLED_MAIN we must call function (client_command) manually */
  CForwardMngr::iterator a = g_forwards.begin(FF_ClientCommand);

  while(a) {
    if((*a).getPlugin()->isExecutable((*a).getFunction())) {
      if((err = amx_Exec((*a).getPlugin()->getAMX(), &ret, (*a).getFunction(), 1, pPlayer->index)) != AMX_ERR_NONE) {
        amx_logerror((*a).getPlugin()->getAMX(), err, "");
      }

      if(ret & 2) {
        result = MRES_SUPERCEDE;
      }
      else if(ret) {
        RETURN_META(MRES_SUPERCEDE);
      }
    }
    ++a;
  }

  /* check for command and if needed also for first argument and call proper function */
  CmdMngr::iterator aa = g_commands.clcmdprefixbegin(cmd);
  if(!aa) {
    aa = g_commands.clcmdbegin();
  }

  while(aa) {
    if((*aa).matchCommandLine(cmd, arg) && (*aa).getPlugin()->isExecutable((*aa).getFunction())) {
      if((err = amx_Exec((*aa).getPlugin()->getAMX(), &ret, (*aa).getFunction(), 3, pPlayer->index, (*aa).getFlags(), (*aa).getId())) != AMX_ERR_NONE) {
        amx_logerror((*aa).getPlugin()->getAMX(), err, "");
      }
      if(ret & 2) {
        result = MRES_SUPERCEDE;
      }
      if(ret & 1) {
        RETURN_META(MRES_SUPERCEDE);
      }
    }
    ++aa;
  }

  if((g_bmod_cstrike || g_bmod_dod) && !strcmp(cmd, "fullupdate")) {
    RETURN_META(MRES_SUPERCEDE);
  }

  /* check menu commands */
  if(!strcmp(cmd, "menuselect")) {
    int pressed_key = atoi(arg) - 1;
    int bit_key = (1<<pressed_key);

    if(pPlayer->keys & bit_key) {
      int menuid = pPlayer->menu;
      pPlayer->menu = 0;
      pPlayer->keys = 0;
      pPlayer->clearmenu = 0;
      pPlayer->clearmenutime = 0.0f;

      //print_srvconsole("[AMX] %s used menu #%d, item #%d\n", pPlayer->name.str(), menuid, atoi(arg)%10);
      MenuMngr::iterator a = g_menucmds.begin();

      while(a) {
        if((*a).matchCommand(menuid, bit_key) && (*a).getPlugin()->isExecutable((*a).getFunction())) {
          if((err = amx_Exec((*a).getPlugin()->getAMX(), &ret, (*a).getFunction(), 2, pPlayer->index,pressed_key)) != AMX_ERR_NONE) {
            amx_logerror((*a).getPlugin()->getAMX(), err, "");
          }

          if(ret & 2) {
            result = MRES_SUPERCEDE;
          }
          if(ret & 1) {
            RETURN_META(MRES_SUPERCEDE);
          }
        }
        ++a;
      }
    }
  }
  /* check for PLUGIN_HANDLED_MAIN and block HL call if needed */
  RETURN_META(result);
}

void StartFrame_Post(void) {
  if(g_auth_time < gpGlobals->time) {
    g_auth_time = gpGlobals->time + 0.7f;
    CList<CPlayer*>::iterator a = g_auth.begin();

    while(a) {
      const char* auth = GETPLAYERAUTHID((*a)->pEdict);

      if((auth == 0) || (*auth == 0)) {
        a.remove();
        continue;
      }

      if(strcmp(auth, "STEAM_ID_PENDING")) {
        (*a)->Authorize();
        g_forwards.executeForwards(FF_ClientAuthorized, 1, (*a)->index);
        a.remove();
        continue;
      }
      ++a;
    }
  }

  if(g_task_time > gpGlobals->time) {
    RETURN_META(MRES_IGNORED);
  }

  g_task_time = gpGlobals->time + 0.1f;

  CPlayer* pPlayer;
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    pPlayer = GET_PLAYER_POINTER_I(i);
    if(pPlayer->initialized && pPlayer->clearmenutime > 0.0f && pPlayer->clearmenutime <= gpGlobals->time) {
      if(pPlayer->clearmenu == pPlayer->menu) {
        pPlayer->menu = 0;
        pPlayer->keys = 0;
        pPlayer->clearmenu = 0;
        pPlayer->clearmenutime = 0.0f;
      }
    }
  }

  for(CTaskMngr::iterator a = g_tasksMngr.begin(); a; ++a) {
    CTaskMngr::CTask& task = *a;
    CPluginMngr::CPlugin* plugin = task.getPlugin();
    int err;

    if(plugin->isExecutable(task.getFunction())) {
      if(task.getParamLen()) { // call with arguments
        cell amx_addr, *phys_addr;

        if(amx_Allot(plugin->getAMX(), task.getParamLen(), &amx_addr, &phys_addr) != AMX_ERR_NONE) {
          logerror("[AMX] Failed to allocate AMX memory (task \"%d\") (plugin \"%s\")\n", task.getTaskId(), plugin->getName());
        }
        else {
          copy_amxmemory(phys_addr, task.getParam(), task.getParamLen());

          if((err = amx_Exec(plugin->getAMX(), NULL, task.getFunction(), 2, amx_addr, task.getTaskId())) != AMX_ERR_NONE) {
            amx_logerror(plugin->getAMX(), err, "");
          }
          amx_Release(plugin->getAMX(), amx_addr);
        }
      }
      else { // call without arguments
        if((err = amx_Exec(plugin->getAMX(), NULL, task.getFunction(), 1, task.getTaskId())) != AMX_ERR_NONE) {
          amx_logerror(plugin->getAMX(), err, "");
        }
      }
    }
  }
  RETURN_META(MRES_IGNORED);
}

void MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) {
  META_RES result = MRES_IGNORED;
  
  if(ed) {
    if(g_bmod_cstrike && msg_type == gmsgBattery) {
      void* ptr = GET_PRIVATE(ed);
#ifdef LINUX
      int *z = (int*)ptr + 0x171;
#else
      int *z = (int*)ptr + 0x16C;
#endif
      int stop = (int)ed->v.armorvalue;
      *z = stop;
      ed->v.armorvalue = (float)stop;
    }
    mPlayerIndex = ENTINDEX(ed);
    mPlayer = GET_PLAYER_POINTER_I(mPlayerIndex);
  }
  else {
    mPlayerIndex = 0;
    mPlayer = 0;
  }
  if(msg_type < 0 || msg_type >= MAX_REG_MSGS) {
    msg_type = 0;
  }
  mState = 0;
  function = modMsgs[msg_type];
  endfunction = modMsgsEnd[msg_type];
  
  if(g_events.iMessageBlock[msg_type]) {
    g_msgBlock = true;
    if(g_events.iMessageBlock[msg_type] == BLOCK_ONCE) {
      g_events.iMessageBlock[msg_type] = BLOCK_NOT;
    }
  }

  g_eventFound = g_events.parserInit(msg_type, &gpGlobals->time, mPlayer, mPlayerIndex, msg_dest, pOrigin);
  if(function || g_eventFound == 1) {
    g_pengfuncsTable->pfnWriteByte = WriteByte;
    g_pengfuncsTable->pfnWriteChar = WriteChar;
    g_pengfuncsTable->pfnWriteShort = WriteShort;
    g_pengfuncsTable->pfnWriteLong = WriteLong;
    g_pengfuncsTable->pfnWriteEntity = WriteEntity;
    g_pengfuncsTable->pfnWriteAngle = WriteAngle;
    g_pengfuncsTable->pfnWriteCoord = WriteCoord;
    g_pengfuncsTable->pfnWriteString = WriteString;
    g_pengfuncsTable_Post->pfnMessageEnd = MessageEnd_Post;
  }
  if(endfunction || g_eventFound == 2) {
    g_pengfuncsTable_Post->pfnMessageEnd = MessageEnd_Post;
  }
  if(g_msgBlock || g_events.isInMessageHook()) {
    result = MRES_SUPERCEDE;
    g_pengfuncsTable->pfnWriteByte = WriteByte;
    g_pengfuncsTable->pfnWriteChar = WriteChar;
    g_pengfuncsTable->pfnWriteShort = WriteShort;
    g_pengfuncsTable->pfnWriteLong = WriteLong;
    g_pengfuncsTable->pfnWriteEntity = WriteEntity;
    g_pengfuncsTable->pfnWriteAngle = WriteAngle;
    g_pengfuncsTable->pfnWriteCoord = WriteCoord;
    g_pengfuncsTable->pfnWriteString = WriteString;
    g_pengfuncsTable->pfnMessageEnd = MessageEnd;
  }
  RETURN_META(result);
}

void WriteByte(int iValue) {
  META_RES result = MRES_IGNORED;
  if(g_eventFound || g_events.isInMessageHook()) g_events.parseValue(iValue, MSG_ARG_BYTE);
  if(function) {
    (*function)((void *)&iValue);
  }
  if(g_msgBlock || g_events.isInMessageHook()) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteChar(int iValue) {
  META_RES result = MRES_IGNORED;
  if(g_eventFound || g_events.isInMessageHook()) g_events.parseValue(iValue, MSG_ARG_CHAR);
  if(function) {
    (*function)((void *)&iValue);
  }
  if(g_msgBlock || g_events.isInMessageHook()) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteShort(int iValue) {
  META_RES result = MRES_IGNORED;
  if(g_eventFound || g_events.isInMessageHook()) g_events.parseValue(iValue, MSG_ARG_SHORT);
  if(function) {
    (*function)((void *)&iValue);
  }
  if(g_msgBlock || g_events.isInMessageHook()) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteLong(int iValue) {
  META_RES result = MRES_IGNORED;
  if(g_eventFound || g_events.isInMessageHook()) g_events.parseValue(iValue, MSG_ARG_LONG);
  if(function) {
    (*function)((void *)&iValue);
  }
  if(g_msgBlock || g_events.isInMessageHook()) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteEntity(int iValue) {
  META_RES result = MRES_IGNORED;
  if(g_eventFound || g_events.isInMessageHook()) g_events.parseValue(iValue, MSG_ARG_ENTITY);
  if(function) {
    (*function)((void *)&iValue);
  }
  if(g_msgBlock || g_events.isInMessageHook()) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteAngle(float flValue) {
  META_RES result = MRES_IGNORED;
  if(g_eventFound || g_events.isInMessageHook()) g_events.parseValue(flValue, MSG_ARG_ANGLE);
  if (function) {
    (*function)((void *)&flValue);
  }
  if(g_msgBlock || g_events.isInMessageHook()) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteCoord(float flValue) {
  META_RES result = MRES_IGNORED;
  if(g_eventFound || g_events.isInMessageHook()) g_events.parseValue(flValue, MSG_ARG_COORD);
  if(function) {
    (*function)((void *)&flValue);
  }
  if(g_msgBlock || g_events.isInMessageHook()) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void WriteString(const char *sz) {
  META_RES result = MRES_IGNORED;
  if(g_eventFound || g_events.isInMessageHook()) g_events.parseValue(sz);
  if(function) {
    (*function)((void *)sz);
  }
  if(g_msgBlock || g_events.isInMessageHook()) result = MRES_SUPERCEDE;
  RETURN_META(result);
}

void MessageEnd(void) {
  if(g_msgBlock) {
    g_msgBlock = false;
  }
  else if(g_events.isInMessageHook()) {
    g_events.executeMessageHooks();
  }
  g_pengfuncsTable->pfnWriteByte = NULL;
  g_pengfuncsTable->pfnWriteChar = NULL;
  g_pengfuncsTable->pfnWriteShort = NULL;
  g_pengfuncsTable->pfnWriteLong = NULL;
  g_pengfuncsTable->pfnWriteEntity = NULL;
  g_pengfuncsTable->pfnWriteAngle = NULL;
  g_pengfuncsTable->pfnWriteCoord = NULL;
  g_pengfuncsTable->pfnWriteString = NULL;
  g_pengfuncsTable->pfnMessageEnd = NULL;
  RETURN_META(MRES_SUPERCEDE);
}

void MessageEnd_Post(void) {
  if(g_eventFound) g_events.executeEvents();
  if(endfunction) {
    (*endfunction)(NULL);
  }
  g_pengfuncsTable->pfnWriteByte = NULL;
  g_pengfuncsTable->pfnWriteChar = NULL;
  g_pengfuncsTable->pfnWriteShort = NULL;
  g_pengfuncsTable->pfnWriteLong = NULL;
  g_pengfuncsTable->pfnWriteEntity = NULL;
  g_pengfuncsTable->pfnWriteAngle = NULL;
  g_pengfuncsTable->pfnWriteCoord = NULL;
  g_pengfuncsTable->pfnWriteString = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;
  RETURN_META(MRES_IGNORED);
}

const char *Cmd_Args(void) {
  if(g_fakecmd.fake) {
    RETURN_META_VALUE(MRES_SUPERCEDE, (g_fakecmd.argc > 1) ? g_fakecmd.args : NULL);
  }
  RETURN_META_VALUE(MRES_IGNORED, NULL);
}

const char *Cmd_Argv(int argc) {
  if(g_fakecmd.fake) {
    RETURN_META_VALUE(MRES_SUPERCEDE, (argc < 3) ? g_fakecmd.argv[argc] : "");
  }
  RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int Cmd_Argc(void) {
  if(g_fakecmd.fake) {
    RETURN_META_VALUE(MRES_SUPERCEDE, g_fakecmd.argc);
  }
  RETURN_META_VALUE(MRES_IGNORED, 0);
}

void SetModel(edict_t *e, const char *m) {
  if(e->v.owner && m[7] == 'w' && m[8] == '_' && m[9] == 'h') {
    g_grenades.put(e, 1.75, 4, GET_PLAYER_POINTER(e->v.owner));
  }
  RETURN_META(MRES_IGNORED);
}

void TraceLine_Post(const float *v1, const float *v2, int fNoMonsters, edict_t *e, TraceResult *ptr) {
  if(e && (e->v.flags & (FL_CLIENT | FL_FAKECLIENT))) {
    CPlayer* pPlayer = GET_PLAYER_POINTER(e);
    if(ptr->pHit && (ptr->pHit->v.flags & (FL_CLIENT | FL_FAKECLIENT))) {
      pPlayer->aiming = ptr->iHitgroup;
    }
    pPlayer->lastTrace = pPlayer->thisTrace;
    pPlayer->thisTrace = ptr->vecEndPos;
  }
  RETURN_META(MRES_IGNORED);
}

void AlertMessage_Post(ALERT_TYPE atype, char *szFmt, ...) {
  if(atype != at_logged) {
    RETURN_META(MRES_IGNORED);
  }
  /*  There are also more messages but we want only logs
  at_notice,
  at_console,   // same as at_notice, but forces a ConPrintf, not a message box
  at_aiconsole, // same as at_console, but only shown if developer level is 2!
  at_warning,
  at_error,
  at_logged   // Server print to console ( only in multiplayer games ).
  */
  if(g_logevents.logEventsExist() || g_forwards.forwardsExist(FF_PluginLog)) {
    va_list logArgPtr;
    va_start(logArgPtr, szFmt);
    g_logevents.setLogString(szFmt, logArgPtr);
    va_end(logArgPtr);
    g_logevents.parseLogString();
    if(g_logevents.logEventsExist()) {
      g_logevents.executeLogEvents();
    }
    g_forwards.executeForwards(FF_PluginLog);
  }
  RETURN_META(MRES_IGNORED);
}

// By AMXMODX Dev Team
void ChangeLevel(char *map, char *dummy) {
  g_forwards.executeForwards(FF_ChangeLevel, map);
  RETURN_META(MRES_IGNORED);
}

void CvarValue2(const edict_t *pEdict, int requestId, const char *cvar, const char *value) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEdict);
  CList<cvarQuery>::iterator a = pPlayer->cvar_queries.begin();
  while(a) {
    if((*a).requestId == requestId) {
      cell ret = 0;
      cell amxAddr_cvar, *physAddr_cvar;
      cell amxAddr_value, *physAddr_value;
      AMX* amx = (*a).amx;
      amx_Allot(amx, strlen(cvar)+1, &amxAddr_cvar, &physAddr_cvar);
      amx_Allot(amx, strlen(value)+1, &amxAddr_value, &physAddr_value);
      set_amxstring(amx, amxAddr_cvar, cvar, 63);
      set_amxstring(amx, amxAddr_value, value, 63);
      int functionToExec = (*a).functionToExec;
      a.remove();
      amx_Exec(amx, &ret, functionToExec, 3, pPlayer->index, amxAddr_cvar, amxAddr_value);
      amx_Release(amx, amxAddr_cvar);
      amx_Release(amx, amxAddr_value);
      break;
    }
    ++a;
  }

  RETURN_META(MRES_HANDLED);
}

C_DLLEXPORT int Meta_Query(char *ifvers, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs) {
  gpMetaUtilFuncs = pMetaUtilFuncs;
  *pPlugInfo = &Plugin_info;
  if(strcmp(ifvers, Plugin_info.ifvers)) {
    int mmajor=0, mminor=0, pmajor=0, pminor=0;
    LOG_MESSAGE(PLID, "WARNING: meta-interface version mismatch; requested=%s ours=%s", Plugin_info.logtag, ifvers);
    sscanf(ifvers, "%d:%d", &mmajor, &mminor);
    sscanf(META_INTERFACE_VERSION, "%d:%d", &pmajor, &pminor);
    if(pmajor > mmajor) {
      LOG_ERROR(PLID, "metamod version is too old for this plugin; update metamod");
      return (FALSE);
    }
    else if(pmajor < mmajor) {
      LOG_ERROR(PLID, "metamod version is incompatible with this plugin; please find a newer version of this plugin");
      return (FALSE);
    }
    else if(pmajor == mmajor) {
      if(pminor > mminor) {
        LOG_ERROR(PLID, "metamod version is incompatible with this plugin; please find a newer version of this plugin");
        return FALSE;
      }
      else if(pminor < mminor) {
        LOG_MESSAGE(PLID, "WARNING: metamod version is newer than expected; consider finding a newer version of this plugin");
      }
    }
    else {
      LOG_ERROR(PLID, "unexpected version comparison; metavers=%s, mmajor=%d, mminor=%d; plugvers=%s, pmajor=%d, pminor=%d", ifvers, mmajor, mminor, META_INTERFACE_VERSION, pmajor, pminor);
    }
  }
  return(TRUE);
}

static META_FUNCTIONS gMetaFunctionTable;
C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs) {
  if(now > Plugin_info.loadable) {
    LOG_ERROR(PLID, "Can't load plugin right now");
    return(FALSE);
  }
  gpMetaGlobals = pMGlobals;
  gMetaFunctionTable.pfnGetEntityAPI2 = GetEntityAPI2;
  gMetaFunctionTable.pfnGetEntityAPI2_Post = GetEntityAPI2_Post;
  gMetaFunctionTable.pfnGetEngineFunctions = GetEngineFunctions;
  gMetaFunctionTable.pfnGetEngineFunctions_Post = GetEngineFunctions_Post;
  gMetaFunctionTable.pfnGetNewDLLFunctions = GetNewDLLFunctions;
  memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
  gpGamedllFuncs = pGamedllFuncs;
  CVAR_REGISTER(&init_amx_version);
  CVAR_REGISTER(&init_amx_modules);
  CVAR_REGISTER(&init_amx_debug);
  CVAR_REGISTER(&init_amx_langdebug);
  CVAR_REGISTER(&init_amx_log_to_hl_logs);
  cvar_amx_debug = CVAR_GET_POINTER(init_amx_debug.name);
  cvar_amx_langdebug = CVAR_GET_POINTER(init_amx_langdebug.name);
  cvar_amx_log_to_hl_logs = CVAR_GET_POINTER(init_amx_log_to_hl_logs.name);
  //amx_version = CVAR_GET_POINTER(init_amx_version.name);
  //amx_modules = CVAR_GET_POINTER(init_amx_modules.name);
  REG_SVR_COMMAND("amx", amx_command);

  char gameDir[512];
  GET_GAME_DIR(gameDir);
  char *a = gameDir;
  int i = 0;
  while(gameDir[i]) {
    if(gameDir[i++] == '/') {
      a = &gameDir[i];
    }
  }
  g_mod_name.set(a);

  get_localinfo("amx_basedir", "addons/amx");
  get_localinfo("amx_configdir", "$basedir/config");
  get_localinfo("amx_langdir", "$basedir/lang");
  get_localinfo("amx_modulesdir", "$basedir/modules");
  get_localinfo("amx_pluginsdir", "$basedir/plugins");
  get_localinfo("amx_mapspluginsdir", "$configdir/maps_plugins");
  get_localinfo("amx_logdir", "$basedir/logs");
  get_localinfo("amx_modules", "$configdir/modules.ini");
  get_localinfo("amx_plugins", "$configdir/plugins.ini");
  get_localinfo("amx_vault", "$configdir/vault.ini");
  get_localinfo("amx_usejit", "config");
  get_localinfo("autoload_modules", "1");
  get_localinfo("optimizer", "7");

  Vault amx_config;
  // ###### Load custom paths configuration
  amx_config.setSource(build_pathname("%s", get_localinfo("amx_cfg", "$configdir/paths.ini")));

  if(amx_config.loadVault()) {
    Vault::iterator a = amx_config.begin();
    while(a != amx_config.end()) {
      SET_LOCALINFO((char*)a.key().str(), (char*)a.value().str());
      ++a;
    }
    amx_config.clear();
  }

  loadModules(get_localinfo("amx_modules", "$configdir/modules.ini"), now);

  // ###### Get AutoLoadModules Setting
  g_autoload_modules = atoi(get_localinfo("autoload_modules", "1"));

  // ###### Autoload modules if needed
  if(g_autoload_modules) {
    g_pluginsloaded = true;
    g_opt_level = 65536;
    g_autoloading_modules = 1;
    loadPluginsIni();
    g_autoloading_modules = 0;
    g_plugins.clear();
    g_pluginsloaded = false;
  }

  GET_HOOK_TABLES(PLID, &g_pEngTable, NULL, NULL); // By AMXMODX Dev Team

  return(TRUE);
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason) {
  if(now > Plugin_info.unloadable && reason != PNL_CMD_FORCED) {
    LOG_ERROR(PLID, "Can't unload plugin right now");
    return(FALSE);
  }
  g_auth.clear();
  g_forwards.clear();
  g_commands.clear();
  g_forcemodels.clear();
  g_forcesounds.clear();
  g_forcegeneric.clear();
  g_grenades.clear();
  g_tasksMngr.clear();
  g_logevents.clearLogEvents();
  g_events.clearEvents();
  g_events.clearMessageHooks();
  g_menucmds.clear();
  g_vault.clear();
  g_xvars.clear();
  g_plugins.clear();
  g_cvars.clear();
  g_translator.clear();

  detachModules();
  return(TRUE);
}

C_DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals) {
  memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
  gpGlobals = pGlobals;
  g_dictionary = NULL;
}

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion) {
  memset(&gFunctionTable, 0, sizeof(DLL_FUNCTIONS));
  gFunctionTable.pfnSpawn = Spawn;
  gFunctionTable.pfnClientCommand = ClientCommand;
  gFunctionTable.pfnServerDeactivate = ServerDeactivate;
  gFunctionTable.pfnClientDisconnect = ClientDisconnect;
  gFunctionTable.pfnServerActivate = ServerActivate;

  if(*interfaceVersion != INTERFACE_VERSION) {
    LOG_ERROR(PLID, "GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
    *interfaceVersion = INTERFACE_VERSION;
    return(FALSE);
  }
  memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
  g_pFunctionTable = pFunctionTable;
  return(TRUE);
}

C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion) {
  memset(&gFunctionTable_Post, 0, sizeof(DLL_FUNCTIONS));
  gFunctionTable_Post.pfnClientPutInServer = ClientPutInServer_Post;
  gFunctionTable_Post.pfnClientUserInfoChanged = ClientUserInfoChanged_Post;
  gFunctionTable_Post.pfnServerActivate = ServerActivate_Post;
  gFunctionTable_Post.pfnClientConnect = ClientConnect_Post;
  gFunctionTable_Post.pfnStartFrame = StartFrame_Post;
  gFunctionTable_Post.pfnServerDeactivate = ServerDeactivate_Post;

  if(*interfaceVersion != INTERFACE_VERSION) {
    LOG_ERROR(PLID, "GetEntityAPI2_Post version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
    *interfaceVersion = INTERFACE_VERSION;
    return(FALSE);
  }
  memcpy(pFunctionTable, &gFunctionTable_Post, sizeof(DLL_FUNCTIONS));
  g_pFunctionTable_Post = pFunctionTable;
  return(TRUE);
}

C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion) {
  memset(&meta_engfuncs, 0, sizeof(enginefuncs_t));
  meta_engfuncs.pfnCmd_Args = Cmd_Args;
	meta_engfuncs.pfnCmd_Argv = Cmd_Argv;
	meta_engfuncs.pfnCmd_Argc = Cmd_Argc;
	meta_engfuncs.pfnMessageBegin = MessageBegin;

  if(!stricmp(g_mod_name.str(), "cstrike") || !stricmp(g_mod_name.str(), "czero")) {
    meta_engfuncs.pfnSetModel = SetModel;
    g_bmod_cstrike = true;
  }
  else {
    g_bmod_cstrike = false;
    g_bmod_dod = !stricmp(g_mod_name.str(), "dod");
    g_bmod_tfc = !stricmp(g_mod_name.str(), "tfc");
  }

  if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
    LOG_ERROR(PLID, "GetEngineFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
    *interfaceVersion = ENGINE_INTERFACE_VERSION;
    return(FALSE);
  }
  memcpy(pengfuncsFromEngine, &meta_engfuncs, sizeof(enginefuncs_t));
  g_pengfuncsTable = pengfuncsFromEngine;
  return(TRUE);
}

C_DLLEXPORT int GetEngineFunctions_Post(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion) {
  memset(&meta_engfuncs_post, 0, sizeof(enginefuncs_t));
  meta_engfuncs_post.pfnTraceLine = TraceLine_Post;
  meta_engfuncs_post.pfnRegUserMsg = RegUserMsg_Post;

  if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
    LOG_ERROR(PLID, "GetEngineFunctions_Post version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
    *interfaceVersion = ENGINE_INTERFACE_VERSION;
    return(FALSE);
  }
  memcpy(pengfuncsFromEngine, &meta_engfuncs_post, sizeof(enginefuncs_t));
  g_pengfuncsTable_Post = pengfuncsFromEngine;
  return(TRUE);
}

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion) {
  memset(&gNewDLLFunctionTable, 0, sizeof(NEW_DLL_FUNCTIONS));
  if(g_engfuncs.pfnQueryClientCvarValue2) {
    gNewDLLFunctionTable.pfnCvarValue2 = CvarValue2;
  }
  memcpy(pNewFunctionTable, &gNewDLLFunctionTable, sizeof(NEW_DLL_FUNCTIONS));
  return(TRUE);
}
