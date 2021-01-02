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

#include "fun.h"


// Game Messages
void Client_ResetHUD(void* value) {
	if( mPlayer && !mPlayer->ingame ) {
		//mPlayer->Connect();
		mPlayer->PutInServer();
	}

	mPlayer->ResetSettings();
}

void Client_ScoreInfo(void* value) {
	static int id;
	static int deaths;
	switch(state++) {
		case 0:
			id = *(int*)value;
			break;
		case 2:
			deaths = *(int*)value;
			break;
		case 4:
			if(id < 1 || id > gpGlobals->maxClients) {
				break;
			}
			player_t* pPlayer = GET_PLAYER_POINTER_I(id);
			pPlayer->deaths = deaths;
			pPlayer->team = *(int*)value;
	}
}

void UTIL_FakeClientCommand(edict_t *pEdict, const char *cmd, const char *arg1, const char *arg2) {
	if (!cmd) return;
	g_fakecmd.argv[0] = cmd;
	if (arg2){
		g_fakecmd.argc = 3;
		g_fakecmd.argv[1] = arg1;
		g_fakecmd.argv[2] = arg2;
		snprintf( g_fakecmd.args, 255, "%s %s", arg1, arg2 );
		g_fakecmd.args[255] = 0;
	}
	else if (arg1){
		g_fakecmd.argc = 2;
		g_fakecmd.argv[1] = arg1;
		snprintf( g_fakecmd.args, 255, "%s" , arg1 );
		g_fakecmd.args[255] = 0;
	}
	else
		g_fakecmd.argc = 1;
	g_fakecmd.fake = true;
	g_pengfuncsTable->pfnCmd_Args = Cmd_Args;
	g_pengfuncsTable->pfnCmd_Argv = Cmd_Argv;
	g_pengfuncsTable->pfnCmd_Argc = Cmd_Argc;
	MDLL_ClientCommand(pEdict);
	g_fakecmd.fake = false;
}

int countFootSteps() {
  int num = 0;
  int i;
  for(i = 1; i <= gpGlobals->maxClients; ++i) {
    if(GET_PLAYER_POINTER_I(i)->nofootsteps == true) {
      ++num;
    }
  }
  return num;
}

int countModelled() {
  int num = 0;
  int i;
  for(i = 1; i <= gpGlobals->maxClients; ++i) {
    if(GET_PLAYER_POINTER_I(i)->modeled == true) {
      ++num;
    }
  }
  return num;
}

bool disableTraceLine() {
  bool same = true;
  int hitzones = -1;
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    if(hitzones == -1) {
      hitzones = (int)GET_PLAYER_POINTER_I(i)->bodyhits[0];
    }
    else {
      if((int)GET_PLAYER_POINTER_I(i)->bodyhits[0] != hitzones) {
        same = false;
      }
    }
  }
  if(same) {
    return (hitzones == 1) ? -1 : (hitzones == 255) ? true : false;
  }
  return false;
}
