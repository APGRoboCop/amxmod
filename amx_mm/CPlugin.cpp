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
 
#include <extdll.h>
#include <meta_api.h>
#include <ctime>
#include "amxmod.h"
#include "CPlugin.h"
#include "CForward.h"
#include "CFile.h"

#if !defined JITORNOT
CPluginMngr::CPlugin* CPluginMngr::loadPlugin(const char* path, const char* name, char* error) {
#else
CPluginMngr::CPlugin* CPluginMngr::loadPlugin(const char* path, const char* name, char* error, int tryjit) {
#endif
	CPlugin** a = &head;
	while(*a) {
    a = &(*a)->next;
  }
	#if !defined JITORNOT
	*a = new CPlugin(pCounter++, path, name, error);
	#else
	*a = new CPlugin(pCounter++, path, name, error, tryjit);
	#endif
	char string[256];
  snprintf(string, 255, "%s", error);
	(*a)->setErrorMessage(string);
	return *error ? nullptr : *a;
}

void CPluginMngr::unloadPlugin(CPlugin** a) {
	CPlugin* next = (*a)->next;
	delete *a;
	*a = next;
	--pCounter;
}

extern const char* get_localinfo(const char* name, const char* def);

int CPluginMngr::loadPluginsFromFile(const char* filename) {
	FILE* fp = fopen(filename, "rt");

	if(!fp) {
    if(g_autoloading_modules != 1) {
		  logerror("[AMX] Plugins list not found (file \"%s\")\n", filename);
    }
		return 1;
	}

	char pluginName[256], line[256], error[256];
	char keyWord[256];
  int len;
	
	while(!feof(fp)) {
		line[0] = '\0';
		fgets(line, 255, fp);
		
		if(line[0] == ';' || line[0] == '/' || line[0] == '\n') continue;
		
	  pluginName[0] = '\0';
		keyWord[0] = '\0';
		if(sscanf(line, "%s %s", pluginName, keyWord) == EOF) {
      continue;
    }
    if(pluginName[0] == '#') {
      if(strcmp(&pluginName[1], "disable_all") == 0) {
        clear();
        continue;
      }
      else if(strcmp(&pluginName[1], "include_file") == 0 && isalnum(*keyWord)) {
        loadPluginsFromFile(build_pathname("%s/%s", get_localinfo("amx_configdir", "$basedir/config"), keyWord));
        continue;
      }
    }
    if(!isalnum(*pluginName)) {
      continue;
    }
    if(strcmp(keyWord, "disabled") == 0) {
      len = strlen(pluginName);
      CPlugin**a = &head;
	    while(*a) {
        if(strncmp((*a)->name.str(), pluginName, len) == 0) {
          unloadPlugin(a);
          break;
        }
    		a = &(*a)->next;
    	}
      continue;
    }

		#if !defined JITORNOT
		loadPlugin(get_localinfo("amx_pluginsdir", "$basedir/plugins"), pluginName, error);
		#else
		loadPlugin(get_localinfo("amx_pluginsdir", "$basedir/plugins"), pluginName, error, strcmp(keyWord, "nojit") ? 1 : 0);
		#endif
	}
	fclose(fp);
	return pCounter;
}

void CPluginMngr::clear() {
	CPlugin**a = &head;	
	while(*a) {
		unloadPlugin(a);
	}
}

CPluginMngr::CPlugin* CPluginMngr::findPluginFast(AMX *amx) { 
	return (CPlugin*)(amx->userdata[3]); 
}

CPluginMngr::CPlugin* CPluginMngr::findPlugin(AMX *amx) {
	CPlugin*a = head;
	while(a && &a->amx != amx) {
		a = a->next;
	}
	return a;
}
	
CPluginMngr::CPlugin* CPluginMngr::findPlugin(int index) {
	CPlugin*a = head;
	while(a && index--) {
		a = a->next;
	}
	return a;
}
	
CPluginMngr::CPlugin* CPluginMngr::findPlugin(const char* name) {
	if(!name) {
    return nullptr;
  }
	int len = strlen(name);
	if(!len) {
    return nullptr;
  }
	CPlugin*a = head;
	while(a && strncmp(a->name.str(), name, len)) {
		a = a->next;
	}
	return a;
}

const char* CPluginMngr::CPlugin::getStatus() const {
  switch(status) {
  case ps_running:
    return "running";
  case ps_paused:
    return "paused";
  case ps_bad_load:
    return "bad load";
  case ps_stopped:
    return "stopped";
  case ps_locked:
    return "locked";
  }
  return "error";
}

#if !defined JITORNOT
CPluginMngr::CPlugin::CPlugin(int i, const char* p, const char* n, char* e) : name(n), title(n), dictionary(nullptr) {
#else
CPluginMngr::CPlugin::CPlugin(int i, const char* p, const char* n, char* e, int j) : name(n), title(n), dictionary(0) {
#endif
	const char* unk = "unknown";
	title.set(unk);
	author.set(unk);
	version.set(unk);
	char* path = build_pathname("%s/%s", p, n);
	code = nullptr;
	#if !defined JITORNOT
	int err = load_amxscript(&amx, &code, path, e, 0);
	#else
	int err = load_amxscript(&amx, &code, path, e, j);
	#endif
	if(err == AMX_ERR_NONE) {
    status = ps_running;
  }
	else status = ps_bad_load;
	amx.userdata[3] = this;
	paused_fun = 0;
	next = nullptr;
	useML = false;
	id = i;
}

CPluginMngr::CPlugin::~CPlugin(){
	unload_amxscript(&amx, &code);
}

void CPluginMngr::CPlugin::pauseFunction(int id) { 
	if(isValid()) {
		paused_fun |= (1<<id);
		g_commands.clearBufforedInfo();
	}
}

void CPluginMngr::CPlugin::unpauseFunction(int id) {
	if(isValid()) {
		paused_fun &= ~(1<<id); 
		g_commands.clearBufforedInfo();
	}
}

void CPluginMngr::CPlugin::setStatus(int a) { 
	status = a; 
	g_commands.clearBufforedInfo(); // ugly way
}

void CPluginMngr::CPlugin::setLocTitle(const char *t) {
	int i = dictionary->getStringId(t);
	if(i!=-1) {
		setTitle("<L7D>");
		titleID = i;
	}
	else {
		setTitle(t);
	}
}

const char *CPluginMngr::CPlugin::getTitle() {
	const char *t = title.str();
	int l;
	if(!strcmp(t, "<L7D>")) {
		t = dictionary->getMessage(0xFF, titleID, l);
	}
	return t;		
}
