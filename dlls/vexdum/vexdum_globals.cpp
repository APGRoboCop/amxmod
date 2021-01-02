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

// Get a integer from globalvars.
NATIVE(global_get_int) {
	int iSet = params[1];
	int iVal = 0;

	switch(iSet) {
		case GV_trace_hitgroup:
			iVal = gpGlobals->trace_hitgroup;
			break;
		case GV_trace_flags:
			iVal = gpGlobals->trace_flags;
			break;
		case GV_msg_entity:
			iVal = gpGlobals->msg_entity;
			break;
		case GV_cdAudioTrack:
			iVal = gpGlobals->cdAudioTrack;
			break;
		case GV_maxClients:
			iVal = gpGlobals->maxClients;
			break;
		case GV_maxEntities:
			iVal = gpGlobals->maxEntities;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid GV_ parameter");
			return 0;
			break;
	}

	return iVal;
}

// Get a float from globalvars.
NATIVE(global_get_float) {
	int iSet = params[1];
	float fVal = 0.0;

	switch(iSet) {
		case GV_time:
			fVal = gpGlobals->time;
			break;
		case GV_frametime:
			fVal = gpGlobals->frametime;
			break;
		case GV_force_retouch:
			fVal = gpGlobals->force_retouch;
			break;
		case GV_deathmatch:
			fVal = gpGlobals->deathmatch;
			break;
		case GV_coop:
			fVal = gpGlobals->coop;
			break;
		case GV_teamplay:
			fVal = gpGlobals->teamplay;
			break;
		case GV_serverflags:
			fVal = gpGlobals->serverflags;
			break;
		case GV_found_secrets:
			fVal = gpGlobals->found_secrets;
			break;
		case GV_trace_allsolid:
			fVal = gpGlobals->trace_allsolid;
			break;
		case GV_trace_startsolid:
			fVal = gpGlobals->trace_startsolid;
			break;
		case GV_trace_fraction:
			fVal = gpGlobals->trace_fraction;
			break;
		case GV_trace_plane_dist:
			fVal = gpGlobals->trace_plane_dist;
			break;
		case GV_trace_inopen:
			fVal = gpGlobals->trace_inopen;
			break;
		case GV_trace_inwater:
			fVal = gpGlobals->trace_inwater;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid GV_ parameter");
			return 0;
			break;
	}

	return FloatToCell(fVal);
}

// Get a vector from globalvars.
NATIVE(global_get_vector) {
	int iSet = params[1];
	cell* vRet = MF_GetAmxAddr(amx, params[2]);
	Vector vVal = Vector(0, 0, 0);

	switch(iSet) {
		case GV_v_forward:
			vVal = gpGlobals->v_forward;
			break;
		case GV_v_up:
			vVal = gpGlobals->v_up;
			break;
		case GV_v_right:
			vVal = gpGlobals->v_right;
			break;
		case GV_trace_endpos:
			vVal = gpGlobals->trace_endpos;
			break;
		case GV_trace_plane_normal:
			vVal = gpGlobals->trace_plane_normal;
			break;
		case GV_vecLandmarkOffset:
			vVal = gpGlobals->vecLandmarkOffset;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid GV_ parameter");
			return 0;
			break;
	}
	vRet[0] = FloatToCell(vVal[0]);
	vRet[1] = FloatToCell(vVal[1]);
	vRet[2] = FloatToCell(vVal[2]);

	return 1;
}

// Get a edict pointer from globalvars.
NATIVE(global_get_edict) {
	int iSet = params[1];
	int pTarget;

	switch(iSet) {
		case GV_trace_ent:
			pTarget = ENTINDEX(gpGlobals->trace_ent);
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid GV_ parameter");
			return 0;
			break;
	}

	if(FNullEnt(pTarget)) {
		return 0;
	}

	return pTarget;
}

// Get a string from globalvars.
NATIVE(global_get_string) {
	int iSet = params[1];
	int szVal = 0;

	switch(iSet) {
		case GV_mapname:
			szVal = gpGlobals->mapname;
			break;
		case GV_startspot:
			szVal = gpGlobals->startspot;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid GV_ parameter");
			return 0;
			break;
	}

	return MF_SetAmxString(amx, params[2], STRING(szVal), params[3]);
}

AMX_NATIVE_INFO VexdUM_Globals[] = {
	{"global_get_int",	global_get_int},
	{"global_get_float",	global_get_float},
	{"global_get_vector",	global_get_vector},
	{"global_get_edict",	global_get_edict},
	{"global_get_string",	global_get_string},

	{NULL,					NULL},
};
