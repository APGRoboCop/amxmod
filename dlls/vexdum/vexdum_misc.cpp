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

#include "vexdum.h"

NATIVE(dot_points) {
	cell* vecSrc = MF_GetAmxAddr(amx, params[1]);
	cell* vecCheck = MF_GetAmxAddr(amx, params[2]);
	cell* vecDir = MF_GetAmxAddr(amx, params[3]);

	float fNew[3] = {
		CellToFloat(vecSrc[0]),
		CellToFloat(vecSrc[1]),
		CellToFloat(vecSrc[2])
	};
	Vector VvecSrc = fNew;

	fNew[0] = CellToFloat(vecCheck[0]);
	fNew[1] = CellToFloat(vecCheck[1]);
	fNew[2] = CellToFloat(vecCheck[2]);
	Vector VvecCheck = fNew;

	fNew[0] = CellToFloat(vecDir[0]);
	fNew[1] = CellToFloat(vecDir[1]);
	fNew[2] = CellToFloat(vecDir[2]);
	Vector VvecDir = fNew;

	Vector2D vec2LOS;
	vec2LOS = (VvecCheck - VvecSrc).Make2D();
	vec2LOS = vec2LOS.Normalize();

	return FloatToCell(DotProduct(vec2LOS, (VvecDir.Make2D())));
}

NATIVE(dot_product) {
	cell *vecA = MF_GetAmxAddr(amx, params[1]);
	cell *vecB = MF_GetAmxAddr(amx, params[2]);

	float fVecA[3] = {
		CellToFloat(vecA[0]),
		CellToFloat(vecA[1]),
		CellToFloat(vecA[2])
	};

	float fVecB[3] = {
		CellToFloat(vecB[0]),
		CellToFloat(vecB[1]),
		CellToFloat(vecB[2])
	};
	return FloatToCell(DotProduct(fVecA, fVecB));
}

NATIVE(normalize) {
	cell *vec = MF_GetAmxAddr(amx, params[1]);
	float fVec[3] = {
		CellToFloat(vec[0]),
		CellToFloat(vec[1]),
		CellToFloat(vec[2])
	};
	Vector vVec = fVec;

	Vector vNorm = vVec.Normalize();
	cell* vRet = MF_GetAmxAddr(amx, params[2]);

	vRet[0] = FloatToCell(vNorm[0]);
	vRet[1] = FloatToCell(vNorm[1]);
	vRet[2] = FloatToCell(vNorm[2]);

	return 1;
}

NATIVE(can_see) {
	int ent1 = params[1];
	int ent2 = params[2];
  if(MF_IsEntity(ent1) && MF_IsEntity(ent2)) {
    edict_t* pSeeing = INDEXENT(ent1);
    edict_t* pSeen = INDEXENT(ent2);

    if(pSeen->v.flags & EF_NODRAW || pSeen->v.flags & FL_NOTARGET) {
      return 0; 
    }
    Vector vecLookerOrigin = pSeeing->v.origin + pSeeing->v.view_ofs;
    Vector vecTargetOrigin = pSeen->v.origin + pSeen->v.view_ofs;
		
    TraceResult tr;
    TRACE_LINE(vecLookerOrigin,	vecTargetOrigin, FALSE, pSeeing, &tr);

    if(tr.fInOpen && tr.fInWater) { // By AMXMODX Dev Team
      return 0;
    }
    else if((tr.flFraction == 1.0) || (ENTINDEX(tr.pHit) == ENTINDEX(pSeen))) {
      return 1; 
    }
  }
	return 0;
}

NATIVE(in_view_cone) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    cell *vInput = MF_GetAmxAddr(amx, params[2]);

    float fNew[3] = {
      CellToFloat(vInput[0]),
      CellToFloat(vInput[1]),
      CellToFloat(vInput[2])
    };
    Vector vOrigin = fNew;

    Vector2D vec2LOS;
    MAKE_VECTORS(pEntity->v.angles);
    vec2LOS = (vOrigin - pEntity->v.origin).Make2D();
    vec2LOS = vec2LOS.Normalize();

    float flDot = DotProduct(vec2LOS, gpGlobals->v_forward.Make2D());

    if(flDot >= cos(pEntity->v.fov * (M_PI / 360))) { // By AMXMODX Dev Team
      return 1;
    }
    else {
      return 0;
    }
  }
  return 0;
}

NATIVE(pointer_to_index) {
	int pointer = params[1];
	edict_t* pEntity = (edict_t*)pointer;
	return ENTINDEX(pEntity);
}

NATIVE(index_to_pointer) {
	int index = params[1];
	edict_t* pEntity = INDEXENT(index);
	return (int)pEntity;
}

NATIVE(make_vectors) {
  cell *vInput = MF_GetAmxAddr(amx, params[1]);
  float fNew[3] = {
    CellToFloat(vInput[0]),
    CellToFloat(vInput[1]),
    CellToFloat(vInput[2])
  };
  Vector vVector = fNew;
  MAKE_VECTORS(vVector);
  return 1;
}

AMX_NATIVE_INFO VexdUM_Misc[] = {
	{ "dot_points",       dot_points },
	{ "dot_product",      dot_product },
	{ "normalize",        normalize },
	{ "can_see",          can_see },
	{ "in_view_cone",     in_view_cone },
	{ "pointer_to_index", pointer_to_index },
	{ "index_to_pointer", index_to_pointer },
	{ "make_vectors",     make_vectors },

	{NULL,               NULL}
};
