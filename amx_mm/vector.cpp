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

#define ANGLEVECTORS_FORWARD	1
#define ANGLEVECTORS_RIGHT		2
#define ANGLEVECTORS_UP       3

/* 2 param */
NATIVE(get_distance) {
  cell *cpVec1 = get_amxaddr(amx, params[1]);
  cell *cpVec2 = get_amxaddr(amx, params[2]);
  Vector vec1 = Vector((float)cpVec1[0], (float)cpVec1[1], (float)cpVec1[2]);
  Vector vec2 = Vector((float)cpVec2[0], (float)cpVec2[1], (float)cpVec2[2]);
  int iDist = (int)((vec1 - vec2).Length());
  return iDist;
}

// By AMXMODX Dev Team
/* 2 param */
NATIVE(get_distance_f) {
  cell *cpVec1 = get_amxaddr(amx, params[1]);
  cell *cpVec2 = get_amxaddr(amx, params[2]);
  Vector vec1 = Vector((float)CellToFloat(cpVec1[0]), (float)CellToFloat(cpVec1[1]), (float)CellToFloat(cpVec1[2]));
  Vector vec2 = Vector((float)CellToFloat(cpVec2[0]), (float)CellToFloat(cpVec2[1]), (float)CellToFloat(cpVec2[2]));
  float fDist = (float) (vec1 - vec2).Length();
  return FloatToCell(fDist);
}

// VelocityByAim, this function will take the aimvectors of an entity, and create a velocity
// of iVelocity in the direction of the aimvec.
NATIVE(VelocityByAim) {
  int ent = params[1];
  if(is_entity(ent, amx)) {
    edict_t* pEntity = INDEXENT(ent);
    int iVel = params[2];
    cell *vRet = get_amxaddr(amx, params[3]);
    Vector vVal = Vector(0, 0, 0);
    MAKE_VECTORS(pEntity->v.v_angle);
    vVal = gpGlobals->v_forward * iVel;

    vRet[0] = FloatToCell(vVal[0]);
    vRet[1] = FloatToCell(vVal[1]);
    vRet[2] = FloatToCell(vVal[2]);
  }
  return 1;
}

// VecToAngles, this is a half-life function for making a vector out of
// angles.
NATIVE(vector_to_angle) {
  cell* vInput = get_amxaddr(amx, params[1]);

  float vVec[3] = {
    CellToFloat(vInput[0]),
    CellToFloat(vInput[1]),
    CellToFloat(vInput[2])
  };

  float vAngle[3] = {0.0, 0.0, 0.0};
  VEC_TO_ANGLES(vVec, vAngle);
  cell* vRet = get_amxaddr(amx, params[2]);
  vRet[0] = FloatToCell(vAngle[0]);
  vRet[1] = FloatToCell(vAngle[1]);
  vRet[2] = FloatToCell(vAngle[2]);

  return 1;
}

// Convert Angles to Vectors
NATIVE(angle_to_vector) {
  cell* vInput = get_amxaddr(amx, params[1]);

  float v_angles[3] = {
    CellToFloat(vInput[0]),
    CellToFloat(vInput[1]),
    CellToFloat(vInput[2])
  };

  Vector v_forward, v_right, v_up, v_return;
  g_engfuncs.pfnAngleVectors(v_angles, v_forward, v_right, v_up);
  int iSet = params[2];
  cell* vRet = get_amxaddr(amx, params[3]);

  switch(iSet) {
    case ANGLEVECTORS_FORWARD:
      v_return = v_forward;
      break;
    case ANGLEVECTORS_RIGHT:
      v_return = v_right;
      break;
    case ANGLEVECTORS_UP:
      v_return = v_up;
      break;
    default:
      amx_logerror(amx, AMX_ERR_NATIVE, "Invalid ANGLEVECTORS_ parameter");
      return 0;
      break;
  }

  vRet[0] = FloatToCell(v_return[0]);
  vRet[1] = FloatToCell(v_return[1]);
  vRet[2] = FloatToCell(v_return[2]);

  return 1;
}

// VecLength, this gives you the length of a vector (float[3] type).
NATIVE(vector_length) {
  cell* vInput = get_amxaddr(amx,params[1]);

  float vVec[3] = {
    CellToFloat(vInput[0]),
    CellToFloat(vInput[1]),
    CellToFloat(vInput[2])
  };

  Vector Vec = vVec;
  float flLength = Vec.Length();

  return FloatToCell(flLength);
}

// VecDist, this gives you the distance between 2 vectors (float[3] type).
NATIVE(vector_distance) {
  cell* vInput1 = get_amxaddr(amx, params[1]);
  float vVec1[3] = {
    CellToFloat(vInput1[0]),
    CellToFloat(vInput1[1]),
    CellToFloat(vInput1[2])
  };

  cell* vInput2 = get_amxaddr(amx, params[2]);
  float vVec2[3] = {
    CellToFloat(vInput2[0]),
    CellToFloat(vInput2[1]),
    CellToFloat(vInput2[2])
  };

  Vector Vec1 = vVec1;
  Vector Vec2 = vVec2;
  float flLength = (Vec1 - Vec2).Length();

  return FloatToCell(flLength);
}

AMX_NATIVE_INFO vector_Natives[] = {
  {"get_distance",		get_distance},
	{"get_distance_f",	get_distance_f},
	{"VelocityByAim",	  VelocityByAim},
	{"vector_to_angle",	vector_to_angle},
	{"angle_to_vector",	angle_to_vector},
	{"vector_length",		vector_length},
	{"vector_distance",	vector_distance},
  { NULL, NULL }
};
