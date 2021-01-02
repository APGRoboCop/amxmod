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

#include "vexdum.h"

// Not done: Taz requested this
/* NATIVE(radius_effect) {
	int ent = params[1];
	
	if(check_entity(ent, "radius_effect")) {
		return 0;
	}
	int type = params[2];
	int mag = params[3];
	cell *vInput = MF_GetAmxAddr(amx, params[4]);

	float vOrig[3] = {
		CellToFloat(vInput[0]),
		CellToFloat(vInput[1]),
		CellToFloat(vInput[2])
	};

	int rad = params[5];
	edict_t *pTarget = NULL;

	while((pTarget = UTIL_FindEntityInSphere(pTarget, vOrig, rad)) != NULL) {
		if(type > 1) { // Affect Clients or Bots
			if(pTarget->v.flags & (FL_CLIENT | FL_FAKECLIENT)) {
			
			}
		}
		else if(type < 2) { // Only Affect Clients
			if(pTarget->v.flags & (FL_CLIENT | FL_FAKECLIENT)) {
			
			}
		}
	}

	pTarget = NULL;

	return 1;
}*/

// Damages entities within a certain radius.
NATIVE(radius_damage) {
	int ent = params[1];
	if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);

    float dmg = CellToFloat(params[2]);
    cell *vInput = MF_GetAmxAddr(amx, params[3]);
	
    float vOrig[3] = {
      CellToFloat(vInput[0]),
      CellToFloat(vInput[1]),
      CellToFloat(vInput[2])
    };

    float rad = CellToFloat(params[4]);
    int bit = params[5];
    int iLen;
    char* vxWeapon = stringVault.put(MF_GetAmxString(amx, params[6], 0, iLen));
    int hs = params[7];

    RadiusDamage(pEntity, dmg, vOrig, rad, bit, vxWeapon, hs);
  }
	return 1;
}

NATIVE(take_damage) {
	int id = params[1];
	int ent = params[2];
  if(MF_IsEntity(id) && MF_IsEntity(ent)) {
    edict_t* pTarget = INDEXENT(id);
    edict_t* pEntity = INDEXENT(ent);

    cell *vInput = MF_GetAmxAddr(amx, params[3]);
    float pOrig[3] = {
      CellToFloat(vInput[0]),
      CellToFloat(vInput[1]),
      CellToFloat(vInput[2])
    };
    Vector origin = pOrig;

    float dmg = CellToFloat(params[4]);
    int bit = params[5];
    int iLen;
    char* vxWeapon = stringVault.put(MF_GetAmxString(amx, params[6], 0, iLen));
    int hs = params[7];

    Damage(pTarget, pEntity, origin, dmg, bit, vxWeapon, hs);
  }
	return 1;
}

NATIVE(client_death) {
	int id = params[1];
	int ent = params[2];
  if(MF_IsEntity(id) && MF_IsEntity(ent)) {
    edict_t* pTarget = INDEXENT(id);
    edict_t* pEntity = INDEXENT(ent);

    int iLen;
    char* vxWeapon = stringVault.put(MF_GetAmxString(amx, params[3], 0, iLen));
    int hs = params[4];

    Death(pTarget, pEntity, vxWeapon, hs);
  }
	return 1;
}

AMX_NATIVE_INFO VexdUM_Dmg[] = {
	//{ "radius_effect",	radius_effect },
	{ "radius_damage", radius_damage },
	{ "take_damage",   take_damage },
	{ "client_death",  client_death },

	{NULL,             NULL}
};
