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
#include "amxmod.h"

typedef int (_FAR *QUERYMOD)(module_info_s**);
typedef int (_FAR *ATTACHMOD)(pfnamx_engine_g*, pfnmodule_engine_g*);
typedef int (_FAR *DETACHMOD)(void);
typedef void (_FAR *PLUGINSLOAD)(void);

QUERYMOD QueryModule;
ATTACHMOD AttachModule;
DETACHMOD DetachModule;
PLUGINSLOAD PluginsLoad;


pfnamx_engine_g engAmxFunc = {
	amx_Align16,
	amx_Align32,
#if defined _I64_MAX || defined HAVE_I64
	amx_Align64,
#endif
	amx_Allot,
	amx_Callback,
	amx_Clone,
	amx_Debug,
	amx_Exec,
	amx_Execv,
	amx_FindPublic,
	amx_FindPubVar,
	amx_FindTagId,
	amx_Flags,
	amx_GetAddr,
	amx_GetPublic,
	amx_GetPubVar,
	amx_GetString,
	amx_GetTag,
	amx_GetUserData,
	amx_Init,
	amx_InitJIT,
	amx_MemInfo,
	amx_NameLength,
	amx_NativeInfo,
	amx_NumPublics,
	amx_NumPubVars,
	amx_NumTags,
	amx_RaiseError,
	amx_Register,
	amx_Release,
	amx_SetCallback,
	amx_SetDebugHook,
	amx_SetString,
	amx_SetUserData,
	amx_StrLen,
};

pfnmodule_engine_g engModuleFunc = {
	add_amxnatives,
	set_amxnatives,
	alloc_amxmemory,
	copy_amxmemory,
	free_amxmemory,
	get_amxaddr,
	set_amxstring,
	get_amxstring,
	amxstring_len,
	format_amxstring,
	get_amxscript,
	get_amxscriptname,
	MODFUNC_FindAmxScriptByAmx,
	MODFUNC_FindAmxScriptByName,
	load_amxscript,
	unload_amxscript,
	is_plugin_running,
	print_srvconsole,
	report_error,
	amx_logerror,
	logerror,
	get_localinfo,
	get_modname,
	build_pathname,
	is_player,
	is_entity,
	is_bot,
	// By AMXMODX Dev Team
	MODFUNC_IsPlayerValid,
	MODFUNC_IsPlayerHLTV,
	MODFUNC_IsPlayerBot,
	MODFUNC_IsPlayerConnecting,
	MODFUNC_IsPlayerAuthorized,
	MODFUNC_IsPlayerInGame,
	MODFUNC_IsPlayerAlive,
	MODFUNC_GetPlayerName,
	MODFUNC_GetPlayerIP,
	MODFUNC_GetPlayerTime,
	MODFUNC_GetPlayerPlayTime,
	MODFUNC_GetPlayerCurweapon,
	MODFUNC_GetPlayerTeam,
	MODFUNC_GetPlayerTeamID,
	MODFUNC_GetPlayerFrags,
  MODFUNC_GetPlayerDeaths,
  MODFUNC_GetPlayerHealth,
  MODFUNC_GetPlayerArmor,
  MODFUNC_GetPlayerMenu,
  MODFUNC_GetPlayerKeys,
  MODFUNC_GetPlayerFlags,
  MODFUNC_GetPlayerEdict,
  MODFUNC_SetPlayerTeamInfo,
  MODFUNC_PlayerPropAddr,
  // End of By AMXMODX Dev Team
};

// *****************************************************
// class CModule
// *****************************************************

CModule::CModule(const char* fname) : filename(fname) {
	metamod = false;
	info = 0;
	module = 0;
	status = MODULE_NONE;
}

CModule::~CModule() {
	if(module) DLFREE(module);

	metamod = false;
	info = 0;
	module = 0;
	status = MODULE_NONE;
	natives.clear();
}

bool CModule::attachModuleToMetamod(const char *moduleFile, PLUG_LOADTIME now) {
	void **handle;
	void *null_handle = NULL;
	if(module) handle = (void **)&module;
	else handle = &null_handle;

	int result = loadModuleByMetamod(moduleFile, handle, now);
	if(!result) metamod = false;

	return true;
}

bool CModule::attachModule() {
	if(status != MODULE_QUERY || !module) return false;

	AttachModule = (ATTACHMOD)DLPROC(module, "AMX_Attach");
	if(!AttachModule) return false;

  g_calledModule = this; // By AMXMODX Dev Team
	(*AttachModule)(&engAmxFunc, &engModuleFunc);
	g_calledModule = NULL; // By AMXMODX Dev Team
	status = MODULE_LOADED;
	return true;
}

bool CModule::queryModule() {
	if(status != MODULE_NONE) return false;

	module = DLLOAD(filename.str());
	if(!module) {
		status = MODULE_BADLOAD;
		return false;
	}
	int meta = (int)DLPROC(module, "Meta_Attach")
	if(meta) metamod = true;

	QueryModule = (QUERYMOD)DLPROC(module, "AMX_Query");
	if(QueryModule == 0) {
		status = MODULE_NOQUERY;
		return false;
	}
	(*QueryModule)(&info);
	if(info == 0) {
		status = MODULE_NOINFO;
		return false;
	}
	if(info->ivers != AMX_INTERFACE_VERSION)	{
		status = MODULE_OLD;
		return false;
	}
	AttachModule = (ATTACHMOD)DLPROC(module, "AMX_Attach");
	if(AttachModule == 0) {
		status = MODULE_NOATTACH;
		return false;
	}
	status = MODULE_QUERY;
	return true;
}

bool CModule::detachModule() {
	if(status != MODULE_LOADED) return false;

	DetachModule = (DETACHMOD)DLPROC(module, "AMX_Detach");
	if(DetachModule) (*DetachModule)();
	if(isMetamod()) unloadModuleByMetamod(module);

	DLFREE(module);
	metamod = false;
	info = 0;
	module = 0;
	natives.clear();
	status = MODULE_NONE;
	return true;
}

void CModule::pluginsLoaded() {
	if(status != MODULE_LOADED) return;
	if(!module) return;

	PluginsLoad = (PLUGINSLOAD)DLPROC(module, "AMX_PluginsLoaded");
	if(!PluginsLoad) return;

	PluginsLoad();
}

const char* CModule::getStatus() const {
	switch(status) {
		case MODULE_NONE: return "error";
		case MODULE_QUERY: return "pending";
		case MODULE_BADLOAD: return "bad load";
		case MODULE_LOADED: return "running";
		case MODULE_NOINFO: return "no info";
		case MODULE_NOQUERY: return "no query";
		case MODULE_NOATTACH: return "no attach";
		case MODULE_OLD: return "old";
	}
	return "unknown";
}
