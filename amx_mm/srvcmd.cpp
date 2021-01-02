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
#include "amxmod.h"

void amx_command() {
  const char* cmd = CMD_ARGV(1);
	g_translator.setDest(0);
	
	if(!strcmp(cmd, "plugins") || !strcmp(cmd, "list")) {
    print_srvconsole(TR("Currently loaded plugins:\n"));
#if !defined JITORNOT
		print_srvconsole("       %-32.31s %-8.7s %-17.16s %-16.15s %-9.8s\n",
			TR("name"),
			TR("version"),
			TR("author"),
			TR("file"),
			TR("status"));
#else
		print_srvconsole("       %-32.31s %-8.7s %-17.16s %-16.15s %-9.8s %-6.5s\n",
			TR("name"),
			TR("version"),
			TR("author"),
			TR("file"),
			TR("status"),
			TR("jit"));
#endif

		int plugins = 0;
		int	running = 0;
		CPluginMngr::iterator a = g_plugins.begin();

		while(a) {
			++plugins;

			if((*a).isValid() && !(*a).isPaused()) {
				++running;
			}

#if !defined JITORNOT
			print_srvconsole(" [%3d] %-32.31s %-8.7s %-17.16s %-16.15s %-9.8s\n",
				plugins, (*a).getTitle(), (*a).getVersion(), 
				(*a).getAuthor(), (*a).getName(), (*a).getStatus());
#else
			print_srvconsole(" [%3d] %-32.31s %-8.7s %-17.16s %-16.15s %-9.8s %-6.5s\n",
				plugins, (*a).getTitle(), (*a).getVersion(), (*a).getAuthor(), (*a).getName(), 
        (*a).getStatus(), (*a).isValid() ? ((*a).getJIT() ? TR("yes") : TR("no")) : "-" );
#endif
			++a;
    }

    a = g_plugins.begin();
		while(a) {
			if(!stricmp((*a).getStatus(), "bad load")) {
				print_srvconsole(TR("Failed to load \"%s\" : %s\n"), (*a).getName(), (*a).getErrorMessage());
			}
			++a;
		}
		print_srvconsole(TR("%d plugins, %d running\n"), plugins, running);
  }
	else if(!strcmp(cmd, "pause") && CMD_ARGC() > 2) {
		const char* sPlugin = CMD_ARGV(2);
		CPluginMngr::CPlugin *plugin = g_plugins.findPlugin(sPlugin);

		if(plugin && plugin->isValid()) {
			plugin->pausePlugin();
			print_srvconsole(TR("Paused plugin \"%s\"\n"), plugin->getName());
		}
		else {
      print_srvconsole(TR("Couldn't find plugin matching \"%s\"\n"), sPlugin);
    }
	}
	else if(!strcmp(cmd, "unpause") && CMD_ARGC() > 2) {
		const char* sPlugin = CMD_ARGV(2);
		CPluginMngr::CPlugin *plugin = g_plugins.findPlugin(sPlugin);

		if(plugin && plugin->isValid()) {
			plugin->unpausePlugin();
			print_srvconsole(TR("Unpaused plugin \"%s\"\n"), plugin->getName());
		}
		else {
      print_srvconsole(TR("Couldn't find plugin matching \"%s\"\n"), sPlugin);
    }
	}
	else if(!strcmp(cmd, "cvars")) {
		print_srvconsole(TR("Registered cvars:\n"));
		print_srvconsole("       %-24.23s %-24.23s %-16.15s\n",
			TR("name"),
			TR("value"),
			TR("plugin"));

		int amount = 0;

		CList<CCVar>::iterator a = g_cvars.begin();
		
		if(CMD_ARGC() > 2) {
			const char* sPlugin = CMD_ARGV(2);
      const char* sPluginName;
			while(a) {
		    sPluginName = (*a).getPluginName();
		    if(stristr(sPluginName, sPlugin) != NULL) {
          print_srvconsole(" [%3d] %-24.23s %-24.23s %-16.15s\n", ++amount,
            (*a).getName(), CVAR_GET_STRING((*a).getName()), sPluginName);
        }
        ++a;
		  }
		}
    else {
			while(a) {
        print_srvconsole(" [%3d] %-24.23s %-24.23s %-16.15s\n", ++amount,
          (*a).getName(), CVAR_GET_STRING((*a).getName()), (*a).getPluginName());
        ++a;
		  }
    }
		print_srvconsole(TR("%d cvars\n"), amount);
	}
	else if(!strcmp(cmd,"cmds")) {
		g_translator.setDest(0);
		print_srvconsole(TR("Registered commands:\n"));
		print_srvconsole("       %-24.23s %-16.15s %-8.7s %-16.15s\n", 
			TR("name"),
			TR("access"),
			TR("type"),
			TR("plugin"));
		
		int amount = 0;
		char access[32];

		CmdMngr::iterator a = g_commands.begin(CMD_ConsoleCommand);

    if(CMD_ARGC() > 2) {
      const char* sPlugin = CMD_ARGV(2);
      const char* sPluginName;
      while(a) {
        sPluginName = (*a).getPlugin()->getName();
        if(stristr(sPluginName, sPlugin) != NULL) {
    			UTIL_GetFlags(access, (*a).getFlags());
    			print_srvconsole(" [%3d] %-24.23s %-16.15s %-8.7s %-16.15s %s\n",
    				++amount, (*a).getCmdLine(), access, (*a).getCmdType(), sPluginName, (*a).getCmdInfo());
    		}
  			++a;
  		}
    }
    else {
  		while(a) {
  			UTIL_GetFlags(access, (*a).getFlags());
  			print_srvconsole(" [%3d] %-24.23s %-16.15s %-8.7s %-16.15s %s\n",
  				++amount, (*a).getCmdLine(), access, (*a).getCmdType(), (*a).getPlugin()->getName(), (*a).getCmdInfo());
  			++a;
  		}
    }
		print_srvconsole(TR("%d commands\n"), amount);
	}
	else if(!strcmp(cmd, "version")) {
		print_srvconsole("%s %s (%s)\n", Plugin_info.name, Plugin_info.version, Plugin_info.url);
		print_srvconsole("%s:\n  AMX Mod Team, whose members are:\n", TR("authors"));
		print_srvconsole("    Aleksander \"OLO\" Naszko, Julien \"dJeyL\" Laurent,\n");
		print_srvconsole("    KRoT@L, T(+)rget, KleeneX, m0gely, DoubleDoom, Steven \"HugeEgo\" Kal\n");
    print_srvconsole("  and since some code was taken from AMX Mod X,\n");
    print_srvconsole("  the AMX Mod X Dev Team, whose members are:\n");
    print_srvconsole("    David \"BAILOPAN\" Anderson, Pavol \"PM OnoTo\" Marko\n");
		print_srvconsole("    Felix \"SniperBeamer\" Geyer, Jonny \"Got His Gun\" Bergstrom\n");
		print_srvconsole("    Lukasz \"SidLuke\" Wlasinski, Christian \"Basic-Master\" Hammacher\n");
		print_srvconsole("    Borja \"faluco\" Ferrer, Scott \"Damaged Soul\" Ehlert\n");
		print_srvconsole("%s: %s\n", TR("compiled"), __DATE__ ", " __TIME__);
	
	}
	else if(!strcmp(cmd, "modules")) {
		print_srvconsole(TR("Currently loaded modules:\n"));
		print_srvconsole("      %-23.22s %-8.7s %-20.19s %-11.10s\n",
			TR("name"),
			TR("version"),
			TR("author"),
			TR("status"));

		int running = 0;
		int modules = 0;
		CList<CModule>::iterator a = g_modules.begin();

		while(a) {
			if((*a).getStatusValue() == MODULE_LOADED) {
				++running;
			}
			++modules;

			print_srvconsole(" [%2d] %-23.22s %-8.7s %-20.19s %-11.10s\n", modules,
				(*a).getName(), (*a).getVersion(), (*a).getAuthor(), TR((*a).getStatus()));
		
			++a;
		}
		print_srvconsole(TR("%d modules, %d correct\n"), modules, running);
	}
	else {
		print_srvconsole(TR("Usage: amx < command > [ argument ]\n"));
		print_srvconsole(TR("Commands:\n"));
		print_srvconsole(TR("   version                - display amx mod version info\n"));
		print_srvconsole(TR("   plugins                - list plugins currently loaded\n"));
		print_srvconsole(TR("   modules                - list modules currently loaded\n"));
		print_srvconsole(TR("   cvars [ plugin ]       - list cvars registered by plugins or by the specified plugin (partial search)\n"));
		print_srvconsole(TR("   cmds [ plugin ]        - list commands registered by plugins or by the specified plugin (partial search)\n"));
		print_srvconsole(TR("   pause < plugin >       - pause a running plugin\n"));
		print_srvconsole(TR("   unpause < plugin >     - unpause a previously paused plugin\n"));
	}
}

void plugin_srvcmd() {
	cell ret = 0;
	int err;
	const char* cmd = CMD_ARGV(0);

	CmdMngr::iterator a = g_commands.srvcmdbegin();

	while(a) {
		if((*a).matchCommand(cmd) && (*a).getPlugin()->isExecutable((*a).getFunction())) {
			if((err = amx_Exec((*a).getPlugin()->getAMX(), &ret, (*a).getFunction(), 3,  
				g_srvindex, (*a).getFlags(), (*a).getId())) != AMX_ERR_NONE) {
				amx_logerror((*a).getPlugin()->getAMX(), err, "");
			}
			if(ret) {
        break;
      }
		}
		++a;
	}
}
