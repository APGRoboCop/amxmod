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

#ifndef vexdum_fwds_h
#define vexdum_fwds_h

#define FORWARD_NUM 17

enum {
	FF_ClientKill,
	FF_ClientPostThink,
	FF_ClientPreThink,
	FF_EmitSound,
	FF_EmitAmbientSound,
	FF_EntityThink,
	FF_EntityTouch,
	FF_EntitySpawn,
	FF_ServerFrame,
	FF_SetModel,
	FF_TraceLine,
	FF_MonsterHurt,
	FF_SetClientKeyValue,
	FF_KeyValue,
	FF_AlertMessage,
	FF_GetGameDescription,
	FF_AddToFullPack
};

class Forwards {
public:

	class AmxCall {
		friend class Forwards;
		AMX *amx;
		int function;
		AmxCall* next;
		AmxCall( AMX *p, int func ): amx(p), function(func) {next=0;}
		inline int getFunction() {return function;}
	};

private:
	AmxCall *head[FORWARD_NUM];
	void clearForwards(AmxCall** a);

public:
	Forwards() {memset( head, 0, sizeof(head));}
	~Forwards() { clear(); }
	void clear();
	inline bool forwardsExist(int type) { return head[(int)type] ? true : false; }

	void findForwards();
	void registerForward( AMX *p, int func, int type );
	// Non-Return value forwards
	void executeForward( int type );
	void executeForward( int type, int d1, int d2, int d3 );
	// Return value forwards
	META_RES executeForward( int type, int d1 );
	META_RES executeForward( int type, int d1, int d2 );
	META_RES executeForward( int type, int d1, const char *d2 );
	META_RES executeForward( int type, int d1, char *d2, char *d3 );
	META_RES executeForward( int type, int d1, char d2[1024] );
	META_RES executeForward( int type, cell state, int e, int ent, int host, int hostflags, int player, cell pSet );
};

extern Forwards g_forwards;

#endif // vexdum_fwds_h
