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

#include "rank.h"

void Forwards::findForwards() {
	static char *forwardName[] = {
		"damage_info",
		"death_info",
		"grenade_throw",
		"bomb_planting",
		"bomb_planted",
		"bomb_exploded",
		"bomb_defusing",
		"bomb_defused"
	};
	AMX* amx;
	void* code;
	const char* filename;
	int i = 0;
	register int j;
	int iFunc;

	// Search for plugins that have my forward functions.
	while((amx = MF_GetAmxScript( i++, &code, &filename )) != 0) {  // Iterate Plugin List
		for(j = 0; j < FORWARD_NUM; ++j) {
			if(AMX_FINDPUBLIC(amx, forwardName[j], &iFunc) == AMX_ERR_NONE) {
				g_forwards.registerForward(amx, iFunc, j);
			}
		}
	}
}

void Forwards::registerForward( AMX* p, int func, int type ) {
	AmxCall** a = &head[ type ];
	while(*a) a = &(*a)->next;
	*a = new AmxCall( p , func );
}

void Forwards::clearForwards( AmxCall** a ) {
	while( *a )	{
		AmxCall* b = (*a)->next;
		delete *a;
		*a = b;
	}
}

void Forwards::clear() {
	register int a;
	for( a = 0; a < FORWARD_NUM; ++a )
		clearForwards( &head[ a ] );
}

void Forwards::executeForward(int type, int d1) {
	cell ret = 0;
	AmxCall* a = head[ type ];
	while(a) {
		if(MF_IsPluginRunning(a->amx)) {
			AMX_EXEC(a->amx, &ret, a->getFunction(), 1, d1);
			if(ret) break;
		}
		a = a->next;
	}
}

void Forwards::executeForward(int type, int d1, int d2) {
	cell ret = 0;
	AmxCall* a = head[ type ];
	while(a) {
		if(MF_IsPluginRunning(a->amx)) {
			AMX_EXEC(a->amx, &ret, a->getFunction(), 2, d1, d2);
			if(ret) break;
		}
		a = a->next;
	}
}

void Forwards::executeForward(int type, int d1, int d2, int d3) {
	cell ret = 0;
	AmxCall* a = head[ type ];
	while(a) {
		if(MF_IsPluginRunning(a->amx)) {
			AMX_EXEC(a->amx, &ret, a->getFunction(), 3, d1, d2, d3);
			if(ret) break;
		}
		a = a->next;
	}
}

void Forwards::executeForward(int type, int d1, int d2, int d3, int d4, int d5) {
	cell ret = 0;
	AmxCall* a = head[ type ];
	while(a) {
		if(MF_IsPluginRunning(a->amx)) {
			AMX_EXEC(a->amx, &ret, a->getFunction(), 5, d1, d2, d3, d4, d5);
			if(ret) break;
		}
		a = a->next;
	}
}

void Forwards::executeForward(int type, int d1, int d2, int d3, int d4, int d5, int d6) {
	cell ret = 0;
	AmxCall* a = head[ type ];
	while(a) {
		if(MF_IsPluginRunning(a->amx)) {
			AMX_EXEC(a->amx, &ret, a->getFunction(), 6, d1, d2, d3, d4, d5, d6);
			if(ret) break;
		}
		a = a->next;
	}
}
