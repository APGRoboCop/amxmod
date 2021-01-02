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
#include "CMenu.h"

// *****************************************************
// class MenuMngr
// *****************************************************
MenuMngr::MenuCommand::MenuCommand(CPluginMngr::CPlugin *a, int mi, int k, int f) {
	plugin = a;
	keys = k;
	menuid = mi;
	function = f;
	next = 0;
}

MenuMngr::~MenuMngr() {
	clear();
}

int MenuMngr::findMenuId(const char* name, AMX* amx) {
  for(MenuIdEle* b = headid; b; b = b->next) {
    if((!b->amx || amx == b->amx) && strstr(name, b->name.str())) {
      return b->id;
    }
  }
  return 0;
}

int MenuMngr::registerMenuId(const char* n, AMX* a) {
  int id = findMenuId(n, a);
  if(id) {
    return id;
  }
  headid = new MenuIdEle(n, a, headid);
  if(!headid) {
		return 0;	
  }
  return headid->id;
}

void MenuMngr::registerMenuCmd(CPluginMngr::CPlugin *a, int mi, int k, int f) {
	MenuCommand** temp = &headcmd;
	while(*temp) temp = &(*temp)->next;
	*temp = new MenuCommand(a, mi, k, f);
}

void MenuMngr::clear() {
  while(headid) {
    MenuIdEle* a = headid->next;
    delete headid;
    headid = a;
  }

  while(headcmd) {
    MenuCommand* a = headcmd->next;
    delete headcmd;
    headcmd = a;
  }
}

int MenuMngr::MenuIdEle::uniqueid = 0;
