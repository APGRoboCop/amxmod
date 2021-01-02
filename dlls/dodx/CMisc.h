/*
 * Copyright (c) 2003-2011 Lukasz Wlasinski
 *
 *    Some code was taken from AMX Mod X, credits for this code go to them.
 *    The authors of AMX Mod X are:
 *    David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
 *	  Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
 *		Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
 *	  Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.
 *
 *    This file is part of DoD XMod.
 *
 *    DoD XMod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    DoD XMod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with DoD XMod; if not, write to the Free Software Foundation,
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
 */

#ifndef CMISC_H
#define CMISC_H

#include "CRank.h"

#define DODMAX_WEAPONS 42

#ifndef LINUX
#define LINUXOFFSET	0
#else
#define LINUXOFFSET	5
#endif

// By AMXMODX Dev Team
#define PDOFFSET_WDEPLOY	229	 + LINUXOFFSET // weapon deploy
#define PDOFFSET_CLASS		366  + LINUXOFFSET // player class
#define PDOFFSET_RCLASS		367  + LINUXOFFSET // random class
#define PDOFFSET_SCORE		475  + LINUXOFFSET // score
#define PDOFFSET_DEATHS		476  + LINUXOFFSET // deaths
#define PDOFFSET_TEAMNAME	1396 + (LINUXOFFSET * sizeof(char)) // team name

enum {
	DODW_AMERKNIFE = 1,
	DODW_GERKNIFE,
	DODW_COLT,
	DODW_LUGER,
	DODW_GARAND,
	DODW_SCOPED_KAR,
	DODW_THOMPSON,
	DODW_STG44,
	DODW_SPRINGFIELD,
	DODW_KAR,
	DODW_BAR,
	DODW_MP40,
	DODW_HANDGRENADE,
	DODW_STICKGRENADE,
	DODW_STICKGRENADE_EX,
	DODW_HANDGRENADE_EX,
	DODW_MG42,
	DODW_30_CAL,
	DODW_SPADE,
	DODW_M1_CARBINE,
	DODW_MG34,
	DODW_GREASEGUN,
	DODW_FG42,
	DODW_K43,
	DODW_ENFIELD,
	DODW_STEN,
	DODW_BREN,
	DODW_WEBLEY,
	DODW_BAZOOKA,
	DODW_PANZERSCHRECK,
	DODW_PIAT,
};

// *****************************************************
// class CPlayer
// *****************************************************

class CPlayer {
private:
	char ip[32];
public:
	edict_t* pEdict;
	int index;
	int aiming;
	int current;
	int wpnModel;

	int savedScore;
	int lastScore;
	float sendScore;
	int staminaMin;
	int staminaMax;
	bool staminaSet;

	bool fuseSet;
	int fuseType; // 1<<0 - for new , 1<<1 - for catched
	float nadeFuse;

	bool ingame;
	float clearStats;

	int health;
	
	struct PlayerWeapon : public Stats {
		char*		name;
		int			ammo;
		int			clip;
	};

	PlayerWeapon	weapons[DODMAX_WEAPONS];
	PlayerWeapon	attackers[33];
	PlayerWeapon	victims[33];
	Stats			weaponsLife[DODMAX_WEAPONS]; // DEC-Weapon (Life) stats
	Stats			weaponsRnd[DODMAX_WEAPONS]; // DEC-Weapon (Round) stats
	Stats			life;
	Stats			round;

	RankSystem::RankStats*	rank;

	void Init(  int pi, edict_t* pe );
	void Connect(const char* name,const char* ip );
	void PutInServer();
	void Disconnect();
	void saveKill(CPlayer* pVictim, int weapon, int hs, int tk);
	void saveHit(CPlayer* pVictim, int weapon, int damage, int aiming);
	void saveShot(int weapon);
	void updateScore(int weapon, int score);
	void restartStats(bool all = true);
	void killPlayer();
	void setTeamName( char *szName );
	inline bool IsAlive(){
		return ((pEdict->v.deadflag==DEAD_NO)&&(pEdict->v.health>0));
	}
};

// *****************************************************
// class Grenades
// *****************************************************

class Grenades
{
  struct Obj 
  {
    CPlayer* player;
    edict_t* grenade;
    float time;
    int type;
    Obj* next;
  } *head;


public:
  Grenades() { head = 0; }
  ~Grenades() { clear(); }
  void put( edict_t* grenade, float time, int type, CPlayer* player  );
  bool find( edict_t* enemy, CPlayer** p, int& type );
  void clear();
};

// *****************************************************
// class Mortar
// *****************************************************

class Mortar
{
  struct Obj 
  {
    CPlayer* player;
    edict_t* mortar;
    float delTime; // kiedy usunac
    Obj* next;
  } *head;


public:
	int AreMortars;
	int CNameOffset; //monster_mortar offset in string base
	float NextCheck;
	int AreNew;
	Mortar() { head = 0; }
	~Mortar() { clear(); }
	void put( edict_t* mortar, float delTime );
	void del();
	bool find( edict_t* enemy , CPlayer** p );
	void update();
	void clear();
};

// *****************************************************
// class CMapInfo
// *****************************************************

class CMapInfo
{
public:
	edict_t* pEdict;
	bool initialized;

	int detect_axis_paras;
	int detect_allies_paras;
	int detect_allies_country;

	void Init();
};

// *****************************************************
// class Forward
// *****************************************************

class Forward
{
	struct AmxCall {
		AMX *amx;
		int iFunctionIdx;
		AmxCall* next;
		AmxCall( AMX *a , int i, AmxCall* n ): amx(a), iFunctionIdx(i), next(n) {}
	} *head;
public:
	Forward() { head = 0; }
	~Forward() { clear(); }
	void clear();
	void put( AMX *a , int i );
	void exec(int p1,int p2,int p3,int p4,int p5,int p6);
	void exec(int p1,int p2,int p3,int p4,int p5);
	void exec(int p1,int p2,int p3);
	void exec(int p1,int p2);
};

#endif // CMISC_H
