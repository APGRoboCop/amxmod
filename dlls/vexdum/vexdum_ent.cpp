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

// Get an integer from an entities entvars.
NATIVE(entity_get_int) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    int iVal = 0;

    switch(iSet) {
		case EV_gamestate:
			iVal = pEntity->v.gamestate;
			break;
		case EV_oldbuttons:
			iVal = pEntity->v.oldbuttons;
			break;
		case EV_groupinfo:
			iVal = pEntity->v.groupinfo;
			break;
		case EV_iuser1:
			iVal = pEntity->v.iuser1;
			break;
		case EV_iuser2:
			iVal = pEntity->v.iuser2;
			break;
		case EV_iuser3:
			iVal = pEntity->v.iuser3;
			break;
		case EV_iuser4:
			iVal = pEntity->v.iuser4;
			break;
		case EV_weaponanim:
			iVal = pEntity->v.weaponanim;
			break;
		case EV_pushmsec:
			iVal = pEntity->v.pushmsec;
			break;
		case EV_bInDuck:
			iVal = pEntity->v.bInDuck;
			break;
		case EV_flTimeStepSound:
			iVal = pEntity->v.flTimeStepSound;
			break;
		case EV_flSwimTime:
			iVal = pEntity->v.flSwimTime;
			break;
		case EV_flDuckTime:
			iVal = pEntity->v.flDuckTime;
			break;
		case EV_iStepLeft:
			iVal = pEntity->v.iStepLeft;
			break;
		case EV_movetype:
			iVal = pEntity->v.movetype;
			break;
		case EV_solid:
			iVal = pEntity->v.solid;
			break;
		case EV_skin:
			iVal = pEntity->v.skin;
			break;
		case EV_body:
			iVal = pEntity->v.body;
			break;
		case EV_effects:
			iVal = pEntity->v.effects;
			break;
		case EV_light_level:
			iVal = pEntity->v.light_level;
			break;
		case EV_sequence:
			iVal = pEntity->v.sequence;
			break;
		case EV_gaitsequence:
			iVal = pEntity->v.gaitsequence;
			break;
		case EV_modelindex:
			iVal = pEntity->v.modelindex;
			break;
		case EV_playerclass:
			iVal = pEntity->v.playerclass;
			break;
		case EV_waterlevel:
			iVal = pEntity->v.waterlevel;
			break;
		case EV_watertype:
			iVal = pEntity->v.watertype;
			break;
		case EV_spawnflags:
			iVal = pEntity->v.spawnflags;
			break;
		case EV_flags:
			iVal = pEntity->v.flags;
			break;
		case EV_colormap:
			iVal = pEntity->v.colormap;
			break;
		case EV_team:
			iVal = pEntity->v.team;
			break;
		case EV_fixangle:
			iVal = pEntity->v.fixangle;
			break;
		case EV_weapons:
			iVal = pEntity->v.weapons;
			break;
		case EV_rendermode:
			iVal = pEntity->v.rendermode;
			break;
		case EV_renderfx:
			iVal = pEntity->v.renderfx;
			break;
		case EV_button:
			iVal = pEntity->v.button;
			break;
		case EV_impulse:
			iVal = pEntity->v.impulse;
			break;
		case EV_deadflag:
			iVal = pEntity->v.deadflag;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
    return iVal;
  }
  return 0;
}

// Set an integer in entvars. mostly the same as get_integer. look there for comments.
NATIVE(entity_set_int) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    int iVal = params[3];

    switch(iSet) {
		case EV_gamestate:
			pEntity->v.gamestate = iVal;
			break;
		case EV_oldbuttons:
			pEntity->v.oldbuttons = iVal;
			break;
		case EV_groupinfo:
			pEntity->v.groupinfo = iVal;
			break;
		case EV_iuser1:
			pEntity->v.iuser1 = iVal;
			break;
		case EV_iuser2:
			pEntity->v.iuser2 = iVal;
			break;
		case EV_iuser3:
			pEntity->v.iuser3 = iVal;
			break;
		case EV_iuser4:
			pEntity->v.iuser4 = iVal;
			break;
		case EV_weaponanim:
			pEntity->v.weaponanim = iVal;
			break;
		case EV_pushmsec:
			pEntity->v.pushmsec = iVal;
			break;
		case EV_bInDuck:
			pEntity->v.bInDuck = iVal;
			break;
		case EV_flTimeStepSound:
			pEntity->v.flTimeStepSound = iVal;
			break;
		case EV_flSwimTime:
			pEntity->v.flSwimTime = iVal;
			break;
		case EV_flDuckTime:
			pEntity->v.flDuckTime = iVal;
			break;
		case EV_iStepLeft:
			pEntity->v.iStepLeft = iVal;
			break;
		case EV_movetype:
			pEntity->v.movetype = iVal;
			break;
		case EV_solid:
			pEntity->v.solid = iVal;
			break;
		case EV_skin:
			pEntity->v.skin = iVal;
			break;
		case EV_body:
			pEntity->v.body = iVal;
			break;
		case EV_effects:
			pEntity->v.effects = iVal;
			break;
		case EV_light_level:
			pEntity->v.light_level = iVal;
			break;
		case EV_sequence:
			pEntity->v.sequence = iVal;
			break;
		case EV_gaitsequence:
			pEntity->v.gaitsequence = iVal;
			break;
		case EV_modelindex:
			pEntity->v.modelindex = iVal;
			break;
		case EV_playerclass:
			pEntity->v.playerclass = iVal;
			break;
		case EV_waterlevel:
			pEntity->v.waterlevel = iVal;
			break;
		case EV_watertype:
			pEntity->v.watertype = iVal;
			break;
		case EV_spawnflags:
			pEntity->v.spawnflags = iVal;
			break;
		case EV_flags:
			pEntity->v.flags = iVal;
			break;
		case EV_colormap:
			pEntity->v.colormap = iVal;
			break;
		case EV_team:
			pEntity->v.team = iVal;
			break;
		case EV_fixangle:
			pEntity->v.fixangle = iVal;
			break;
		case EV_weapons:
			pEntity->v.weapons = iVal;
			break;
		case EV_rendermode:
			pEntity->v.rendermode = iVal;
			break;
		case EV_renderfx:
			pEntity->v.renderfx = iVal;
			break;
		case EV_button:
			pEntity->v.button = iVal;
			break;
		case EV_impulse:
			pEntity->v.impulse = iVal;
			break;
		case EV_deadflag:
			pEntity->v.deadflag = iVal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
  }
	return 1;
}

// Gets a float out of entvars.
NATIVE(entity_get_float) {
	int ent = params[1];
	float fVal = 0.0;
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];

    switch(iSet) {
		case EV_impacttime:
			fVal = pEntity->v.impacttime;
			break;
		case EV_starttime:
			fVal = pEntity->v.starttime;
			break;
		case EV_idealpitch:
			fVal = pEntity->v.idealpitch;
			break;
		case EV_pitch_speed:
			fVal = pEntity->v.pitch_speed;
			break;
		case EV_ideal_yaw:
			fVal = pEntity->v.ideal_yaw;
			break;
		case EV_yaw_speed:
			fVal = pEntity->v.yaw_speed;
			break;
		case EV_ltime:
			fVal = pEntity->v.ltime;
			break;
		case EV_nextthink:
			fVal = pEntity->v.nextthink;
			break;
		case EV_gravity:
			fVal = pEntity->v.gravity;
			break;
		case EV_friction:
			fVal = pEntity->v.friction;
			break;
		case EV_frame:
			fVal = pEntity->v.frame;
			break;
		case EV_animtime:
			fVal = pEntity->v.animtime;
			break;
		case EV_framerate:
			fVal = pEntity->v.framerate;
			break;
		case EV_health:
			fVal = pEntity->v.health;
			break;
		case EV_frags:
			fVal = pEntity->v.frags;
			break;
		case EV_takedamage:
			fVal = pEntity->v.takedamage;
			break;
		case EV_max_health:
			fVal = pEntity->v.max_health;
			break;
		case EV_teleport_time:
			fVal = pEntity->v.teleport_time;
			break;
		case EV_armortype:
			fVal = pEntity->v.armortype;
			break;
		case EV_armorvalue:
			fVal = pEntity->v.armorvalue;
			break;
		case EV_dmg_take:
			fVal = pEntity->v.dmg_take;
			break;
		case EV_dmg_save:
			fVal = pEntity->v.dmg_save;
			break;
		case EV_dmg:
			fVal = pEntity->v.dmg;
			break;
		case EV_dmgtime:
			fVal = pEntity->v.dmgtime;
			break;
		case EV_speed:
			fVal = pEntity->v.speed;
			break;
		case EV_air_finished:
			fVal = pEntity->v.air_finished;
			break;
		case EV_pain_finished:
			fVal = pEntity->v.pain_finished;
			break;
		case EV_radsuit_finished:
			fVal = pEntity->v.radsuit_finished;
			break;
		case EV_scale:
			fVal = pEntity->v.scale;
			break;
		case EV_renderamt:
			fVal = pEntity->v.renderamt;
			break;
		case EV_maxspeed:
			fVal = pEntity->v.maxspeed;
			break;
		case EV_fov:
			fVal = pEntity->v.fov;
			break;
		case EV_flFallVelocity:
			fVal = pEntity->v.flFallVelocity;
			break;
		case EV_fuser1:
			fVal = pEntity->v.fuser1;
			break;
		case EV_fuser2:
			fVal = pEntity->v.fuser2;
			break;
		case EV_fuser3:
			fVal = pEntity->v.fuser3;
			break;
		case EV_fuser4:
			fVal = pEntity->v.fuser4;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
  }
	return FloatToCell(fVal);
}

// Almost the same as Get_float, look there for comments.
NATIVE(entity_set_float) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    float fVal = CellToFloat(params[3]);

    switch(iSet) {
		case EV_impacttime:
			pEntity->v.impacttime = fVal;
			break;
		case EV_starttime:
			pEntity->v.starttime = fVal;
			break;
		case EV_idealpitch:
			pEntity->v.idealpitch = fVal;
			break;
		case EV_pitch_speed:
			pEntity->v.pitch_speed = fVal;
			break;
		case EV_ideal_yaw:
			pEntity->v.ideal_yaw = fVal;
			break;
		case EV_yaw_speed:
			pEntity->v.yaw_speed = fVal;
			break;
		case EV_ltime:
			pEntity->v.ltime = fVal;
			break;
		case EV_nextthink:
			pEntity->v.nextthink = fVal;
			break;
		case EV_gravity:
			pEntity->v.gravity = fVal;
			break;
		case EV_friction:
			pEntity->v.friction = fVal;
			break;
		case EV_frame:
			pEntity->v.frame = fVal;
			break;
		case EV_animtime:
			pEntity->v.animtime = fVal;
			break;
		case EV_framerate:
			pEntity->v.framerate = fVal;
			break;
		case EV_health:
			pEntity->v.health = fVal;
			break;
		case EV_frags:
			pEntity->v.frags = fVal;
			break;
		case EV_takedamage:
			pEntity->v.takedamage = fVal;
			break;
		case EV_max_health:
			pEntity->v.max_health = fVal;
			break;
		case EV_teleport_time:
			pEntity->v.teleport_time = fVal;
			break;
		case EV_armortype:
			pEntity->v.armortype = fVal;
			break;
		case EV_armorvalue:
			pEntity->v.armorvalue = fVal;
			break;
		case EV_dmg_take:
			pEntity->v.dmg_take = fVal;
			break;
		case EV_dmg_save:
			pEntity->v.dmg_save = fVal;
			break;
		case EV_dmg:
			pEntity->v.dmg = fVal;
			break;
		case EV_dmgtime:
			pEntity->v.dmgtime = fVal;
			break;
		case EV_speed:
			pEntity->v.speed = fVal;
			break;
		case EV_air_finished:
			pEntity->v.air_finished = fVal;
			break;
		case EV_pain_finished:
			pEntity->v.pain_finished = fVal;
			break;
		case EV_radsuit_finished:
			pEntity->v.radsuit_finished = fVal;
			break;
		case EV_scale:
			pEntity->v.scale = fVal;
			break;
		case EV_renderamt:
			pEntity->v.renderamt = fVal;
			break;
		case EV_maxspeed:
			pEntity->v.maxspeed = fVal;
			break;
		case EV_fov:
			pEntity->v.fov = fVal;
			break;
		case EV_flFallVelocity:
			pEntity->v.flFallVelocity = fVal;
			break;
		case EV_fuser1:
			pEntity->v.fuser1 = fVal;
			break;
		case EV_fuser2:
			pEntity->v.fuser2 = fVal;
			break;
		case EV_fuser3:
			pEntity->v.fuser3 = fVal;
			break;
		case EV_fuser4:
			pEntity->v.fuser4 = fVal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
  }
	return 1;
}

// Gets a vector from entvars. I use a float[3] to describe half life vectors,
// as this is how half-life handles vectors. also angle vectors and the such are
// less then 1, so you need floats. All functions that i have that work with vectors
// use the float[3], this makes it easier.
NATIVE(entity_get_vector) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    cell* vRet = MF_GetAmxAddr(amx, params[3]);
    Vector vVal = Vector(0, 0, 0);

    switch(iSet) {
		case EV_origin:
			vVal = pEntity->v.origin;
			break;
		case EV_oldorigin:
			vVal = pEntity->v.oldorigin;
			break;
		case EV_velocity:
			vVal = pEntity->v.velocity;
			break;
		case EV_basevelocity:
			vVal = pEntity->v.basevelocity;
			break;
		case EV_clbasevelocity:
			vVal = pEntity->v.clbasevelocity;
			break;
		case EV_movedir:
			vVal = pEntity->v.movedir;
			break;
		case EV_angles:
			vVal = pEntity->v.angles;
			break;
		case EV_avelocity:
			vVal = pEntity->v.avelocity;
			break;
		case EV_punchangle:
			vVal = pEntity->v.punchangle;
			break;
		case EV_v_angle:
			vVal = pEntity->v.v_angle;
			break;
		case EV_endpos:
			vVal = pEntity->v.endpos;
			break;
		case EV_startpos:
			vVal = pEntity->v.startpos;
			break;
		case EV_absmin:
			vVal = pEntity->v.absmin;
			break;
		case EV_absmax:
			vVal = pEntity->v.absmax;
			break;
		case EV_mins:
			vVal = pEntity->v.mins;
			break;
		case EV_maxs:
			vVal = pEntity->v.maxs;
			break;
		case EV_size:
			vVal = pEntity->v.size;
			break;
		case EV_rendercolor:
			vVal = pEntity->v.rendercolor;
			break;
		case EV_view_ofs:
			vVal = pEntity->v.view_ofs;
			break;
		case EV_vuser1:
			vVal = pEntity->v.vuser1;
			break;
		case EV_vuser2:
			vVal = pEntity->v.vuser2;
			break;
		case EV_vuser3:
			vVal = pEntity->v.vuser3;
			break;
		case EV_vuser4:
			vVal = pEntity->v.vuser4;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
    vRet[0] = FloatToCell(vVal[0]);
    vRet[1] = FloatToCell(vVal[1]);
    vRet[2] = FloatToCell(vVal[2]);
  }
	return 1;
}

// Set is close to get, these functions are pretty self-explanatory.
NATIVE(entity_set_vector) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    cell* vInput = MF_GetAmxAddr(amx, params[3]);

    float vVal[3] = {
      CellToFloat(vInput[0]),
      CellToFloat(vInput[1]),
      CellToFloat(vInput[2])
    };

    switch(iSet) {
		case EV_origin:
			pEntity->v.origin = vVal;
			break;
		case EV_oldorigin:
			pEntity->v.oldorigin = vVal;
			break;
		case EV_velocity:
			pEntity->v.velocity = vVal;
			break;
		case EV_basevelocity:
			pEntity->v.basevelocity = vVal;
			break;
		case EV_clbasevelocity:
			pEntity->v.clbasevelocity = vVal;
			break;
		case EV_movedir:
			pEntity->v.movedir = vVal;
			break;
		case EV_angles:
			pEntity->v.angles = vVal;
			break;
		case EV_avelocity:
			pEntity->v.avelocity = vVal;
			break;
		case EV_punchangle:
			pEntity->v.punchangle = vVal;
			break;
		case EV_v_angle:
			pEntity->v.v_angle = vVal;
			break;
		case EV_endpos:
			pEntity->v.endpos = vVal;
			break;
		case EV_startpos:
			pEntity->v.startpos = vVal;
			break;
		case EV_absmin:
			pEntity->v.absmin = vVal;
			break;
		case EV_absmax:
			pEntity->v.absmax = vVal;
			break;
		case EV_mins:
			pEntity->v.mins = vVal;
			break;
		case EV_maxs:
			pEntity->v.maxs = vVal;
			break;
		case EV_size:
			pEntity->v.size = vVal;
			break;
		case EV_rendercolor:
			pEntity->v.rendercolor = vVal;
			break;
		case EV_view_ofs:
			pEntity->v.view_ofs = vVal;
			break;
		case EV_vuser1:
			pEntity->v.vuser1 = vVal;
			break;
		case EV_vuser2:
			pEntity->v.vuser2 = vVal;
			break;
		case EV_vuser3:
			pEntity->v.vuser3 = vVal;
			break;
		case EV_vuser4:
			pEntity->v.vuser4 = vVal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
  }
	return 1;
}

// Get an edict pointer (or index, amx does not have pointers) from entvars.
NATIVE(entity_get_edict) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    int pTarget;

    switch(iSet) {
		case EV_chain:
			pTarget = ENTINDEX(pEntity->v.chain);
			break;
		case EV_dmg_inflictor:
			pTarget = ENTINDEX(pEntity->v.dmg_inflictor);
			break;
		case EV_enemy:
			pTarget = ENTINDEX(pEntity->v.enemy);
			break;
		case EV_aiment:
			pTarget = ENTINDEX(pEntity->v.aiment);
			break;
		case EV_owner:
			pTarget = ENTINDEX(pEntity->v.owner);
			break;
		case EV_groundentity:
			pTarget = ENTINDEX(pEntity->v.groundentity);
			break;
		case EV_pContainingEntity:
			pTarget = ENTINDEX(pEntity->v.pContainingEntity);
			break;
		case EV_euser1:
			pTarget = ENTINDEX(pEntity->v.euser1);
			break;
		case EV_euser2:
			pTarget = ENTINDEX(pEntity->v.euser2);
			break;
		case EV_euser3:
			pTarget = ENTINDEX(pEntity->v.euser3);
			break;
		case EV_euser4:
			pTarget = ENTINDEX(pEntity->v.euser4);
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }

    if(FNullEnt(pTarget)) {
      return 0;
    }
    return pTarget;
  }
  return 0;
}

// Set edict is almost the same as get, look there for comments.
NATIVE(entity_set_edict) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    int ent2 = params[3];
    edict_t* pEntity = INDEXENT(ent);
    edict_t* pTarget = INDEXENT(ent2);
    int iSet = params[2];

    switch(iSet) {
		case EV_chain:
			pEntity->v.chain = pTarget;
			break;
		case EV_dmg_inflictor:
			pEntity->v.dmg_inflictor = pTarget;
			break;
		case EV_enemy:
			pEntity->v.enemy = pTarget;
			break;
		case EV_aiment:
			pEntity->v.aiment = pTarget;
			break;
		case EV_owner:
			pEntity->v.owner = pTarget;
			break;
		case EV_groundentity:
			pEntity->v.groundentity = pTarget;
			break;
		case EV_pContainingEntity:
			pEntity->v.pContainingEntity = pTarget;
			break;
		case EV_euser1:
			pEntity->v.euser1 = pTarget;
			break;
		case EV_euser2:
			pEntity->v.euser2 = pTarget;
			break;
		case EV_euser3:
			pEntity->v.euser3 = pTarget;
			break;
		case EV_euser4:
			pEntity->v.euser4 = pTarget;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
  }
	return 1;
}

// Strings. I remember now that weaponmodel and viewmodel are strings as well,
// even though half-life declares them as integers. (a half-life string is just a
// typedefed integer.).
NATIVE(entity_get_string) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    int szVal = 0;

    switch(iSet) {
		case EV_classname:
			szVal = pEntity->v.classname;
			break;
		case EV_globalname:
			szVal = pEntity->v.globalname;
			break;
		case EV_model:
			szVal = pEntity->v.model;
			break;
		case EV_target:
			szVal = pEntity->v.target;
			break;
		case EV_targetname:
			szVal = pEntity->v.targetname;
			break;
		case EV_netname:
			szVal = pEntity->v.netname;
			break;
		case EV_message:
			szVal = pEntity->v.message;
			break;
		case EV_noise:
			szVal = pEntity->v.noise;
			break;
		case EV_noise1:
			szVal = pEntity->v.noise1;
			break;
		case EV_noise2:
			szVal = pEntity->v.noise2;
			break;
		case EV_noise3:
			szVal = pEntity->v.noise3;
			break;
		case EV_viewmodel:
			szVal = pEntity->v.viewmodel;
			break;
		case EV_weaponmodel:
			szVal = pEntity->v.weaponmodel;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
    return MF_SetAmxString(amx, params[3], STRING(szVal), params[4]);
  }
  return 0;
}

// Almost the same as Get_String, look there for comments.
NATIVE(entity_set_string) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    int iLen;
    int szVal = MAKE_STRING(stringVault.put(MF_GetAmxString(amx, params[3], 0, iLen)));

    switch(iSet) {
		case EV_classname:
			pEntity->v.classname = szVal;
			break;
		case EV_globalname:
			pEntity->v.globalname = szVal;
			break;
		case EV_model:
			pEntity->v.model = szVal;
			break;
		case EV_target:
			pEntity->v.target = szVal;
			break;
		case EV_targetname:
			pEntity->v.targetname = szVal;
			break;
		case EV_netname:
			pEntity->v.netname = szVal;
			break;
		case EV_message:
			pEntity->v.message = szVal;
			break;
		case EV_noise:
			pEntity->v.noise = szVal;
			break;
		case EV_noise1:
			pEntity->v.noise1 = szVal;
			break;
		case EV_noise2:
			pEntity->v.noise2 = szVal;
			break;
		case EV_noise3:
			pEntity->v.noise3 = szVal;
			break;
		case EV_viewmodel:
			pEntity->v.viewmodel = szVal;
			break;
		case EV_weaponmodel:
			pEntity->v.weaponmodel = szVal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
  }
	return 1;
}

// Bytes, these were used for some things, mostly useless, but might be useful.
// They are arrays, but we just use numbers in naming of the enum variables to
// let us pass get different positions in the array.
NATIVE(entity_get_byte) {
	int ent = params[1];
	int iVal = 0;
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];

    switch(iSet) {
		case EV_controller0:
			iVal = pEntity->v.controller[0];
			break;
		case EV_controller1:
			iVal = pEntity->v.controller[1];
			break;
		case EV_controller2:
			iVal = pEntity->v.controller[2];
			break;
		case EV_controller3:
			iVal = pEntity->v.controller[3];
			break;
		case EV_blending0:
			iVal = pEntity->v.blending[0];
			break;
		case EV_blending1:
			iVal = pEntity->v.blending[1];
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
  }
	return iVal;
}

// Like Get_Byte, but sets.
NATIVE(entity_set_byte) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iSet = params[2];
    int iVal = params[3];

    if(iVal > 255) {
      iVal = 255;
    }
    if(iVal < 0) {
      iVal = 0;
    }

    switch(iSet) {
		case EV_controller0:
			pEntity->v.controller[0] = iVal;
			break;
		case EV_controller1:
			pEntity->v.controller[1] = iVal;
			break;
		case EV_controller2:
			pEntity->v.controller[2] = iVal;
			break;
		case EV_controller3:
			pEntity->v.controller[3] = iVal;
			break;
		case EV_blending0:
			pEntity->v.blending[0] = iVal;
			break;
		case EV_blending1:
			pEntity->v.blending[1] = iVal;
			break;
		default:
			MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid EV_ parameter");
			return 0;
			break;
    }
  }
	return 1;
}

// CreateEntity. Makes an entity.
NATIVE(create_entity) {
	int iLen;
	int szName = MAKE_STRING(stringVault.put(MF_GetAmxString(amx, params[1], 0, iLen)));

	edict_t* pEntity = CREATE_NAMED_ENTITY(szName);

	if(FNullEnt(pEntity)) {
		MF_AmxLogError(amx, AMX_ERR_NATIVE, "Couldn't create entity");
		return 0;
	}

	return ENTINDEX(pEntity);
}

// Sets a model on an entity. Dont set models in entvars, it wont
// update the engine of the model change.
NATIVE(entity_set_model) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iLen;
    char *szVal = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
    SET_MODEL(pEntity, szVal);
  }
	return 1;
}

// Set origin for entities. Use this instead of entvars, as it updates the engine.
// It also does a SetSize, this way we can set the size at the same time as origin.
NATIVE(entity_set_origin) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    cell* vInput = MF_GetAmxAddr(amx, params[2]);
    float vOrig[3] = {
      CellToFloat(vInput[0]),
      CellToFloat(vInput[1]),
      CellToFloat(vInput[2])
    };
    SET_ORIGIN(pEntity, vOrig);
  }
	return 1;
}

NATIVE(entity_set_size) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    cell* vInput1 = MF_GetAmxAddr(amx, params[2]);
    cell* vInput2 = MF_GetAmxAddr(amx, params[3]);

    float vMin[3] = {
      CellToFloat(vInput1[0]),
      CellToFloat(vInput1[1]),
      CellToFloat(vInput1[2])
    };
    float vMax[3] = {
      CellToFloat(vInput2[0]),
      CellToFloat(vInput2[1]),
      CellToFloat(vInput2[2])
    };
    SET_SIZE(pEntity, vMin, vMax);
  }
	return 1;
}

// FindEntity, use this in a while loop. will return -1 when theres no entities left.
// It searches by "type" (params[3]).
NATIVE(find_entity) {
	int ent = params[1];
	edict_t* pEntity;

	if(ent <= 0) {
		pEntity = NULL;
	}
	else {
		pEntity = INDEXENT(ent);
		
		if(FNullEnt(pEntity)) {
			return -1;
		}
	}

  int type;
  if((*params / sizeof(cell)) == 2) {
    type = 0;
  }
  else {
    type = params[3];
  }

  char search[16];
	switch(type)
	{
		case 0:
      strcpy(search, "classname");
      break;
		case 1:
      strcpy(search, "globalname");
      break;
		case 2:
      strcpy(search, "model");
      break;
		case 3:
      strcpy(search, "target");
      break;
		case 4:
      strcpy(search, "targetname");
      break;
		case 5:
      strcpy(search, "netname");
      break;
		case 6:
      strcpy(search, "message");
      break;
		case 7:
      strcpy(search, "noise");
      break;
		case 8:
      strcpy(search, "noise1");
      break;
		case 9:
      strcpy(search, "noise2");
      break;
		case 10:
      strcpy(search, "noise3");
      break;
		case 11:
      strcpy(search, "viewmodel");
      break;
		case 12:
      strcpy(search, "weaponmodel");
      break;
		default: 
      strcpy(search, "classname");
      break;
	}
	
	int iLen;
	char *szVal = MF_GetAmxString(amx, params[2], 0, iLen);
	int iEnt = ENTINDEX(FIND_ENTITY_BY_STRING(pEntity, search, szVal));

	if(!iEnt || FNullEnt(iEnt)) {
		return -1;
	}
	return iEnt;
}

// Finds an entity within a sphere.
// Use this in a while loop. will return -1 when theres no entities left.
NATIVE(find_entity_sphere) {
	int ent = params[1];
	edict_t* pEntity;

	if(ent <= 0) {
		pEntity = NULL;
	}

	else {
		pEntity = INDEXENT(ent);
		
		if(FNullEnt(pEntity)) {
			return -1;
		}
	}
	cell *vInput = MF_GetAmxAddr(amx, params[2]);

	float vOrig[3] = {
		CellToFloat(vInput[0]),
		CellToFloat(vInput[1]),
		CellToFloat(vInput[2])
	};

	float fRad = CellToFloat(params[3]);
	int iEnt = ENTINDEX(FIND_ENTITY_IN_SPHERE(pEntity, vOrig, fRad));

	if(!iEnt || FNullEnt(iEnt)) {
		return -1;
	}

	return iEnt;
}

// RemoveEntity, this removes an entity from the world.
// Could also be done setting the flag FL_KILLME in entvars.
NATIVE(remove_entity) {
	int ent = params[1];
  
	if(ent > 0 && ent <= gpGlobals->maxClients) {
		MF_AmxLogError(amx, AMX_ERR_NATIVE, "Not allowed to remove ID %d, it's a player/bot", ent);
		return 1;
	}
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    REMOVE_ENTITY(pEntity);
    //pEntity->v.flags = FL_KILLME;
  }
	return 1;
}

// Return current number of entities in the map
NATIVE(get_num_ents) { 
	return NUMBER_OF_ENTITIES(); 
}

NATIVE(get_maxentities) {
	return gpGlobals->maxEntities;
}

NATIVE(is_entity) {
	edict_t* pEntity = INDEXENT(params[1]);

	if(FNullEnt(pEntity)) {
		return 0;
	}

	return 1;
}

NATIVE(drop_to_floor) {
	int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    return DROP_TO_FLOOR(pEntity);
  }
  return 0;
}

NATIVE(entity_use) {
	int ent1 = params[1];
	int ent2 = params[2];
	if(MF_IsEntity(ent1) && MF_IsEntity(ent2)) {
    edict_t* pUsed = INDEXENT(ent1);
    edict_t* pOther = INDEXENT(ent2);
    MDLL_Use(pUsed, pOther);
  }
	return 1;	
}

// By AMXMODX Dev Team
NATIVE(call_think) {
	int ent = params[1];
	if(MF_IsEntity(ent)) {
    edict_t* pEnt = INDEXENT(ent);
    MDLL_Think(pEnt);
  }
	return 1;
}

// Native list.
AMX_NATIVE_INFO VexdUM_Ent[] = {
	{ "entity_get_int",     entity_get_int },
	{ "entity_set_int",     entity_set_int },
	{ "entity_get_float",   entity_get_float },
	{ "entity_set_float",   entity_set_float },
	{ "entity_get_vector",  entity_get_vector },
	{ "entity_set_vector",  entity_set_vector },
	{ "entity_get_edict",   entity_get_edict },
	{ "entity_set_edict",   entity_set_edict },
	{ "entity_get_string",  entity_get_string },
	{ "entity_set_string",  entity_set_string },
	{ "entity_get_byte",    entity_get_byte },
	{ "entity_set_byte",    entity_set_byte },
	{ "create_entity",      create_entity },
	{ "entity_set_model",   entity_set_model },
	{ "entity_set_origin",  entity_set_origin },
	{ "entity_set_size",    entity_set_size },
	{ "find_entity",        find_entity },
	{ "find_entity_sphere", find_entity_sphere },
	{ "remove_entity",      remove_entity },
	{ "get_num_ents",       get_num_ents },
	{ "get_maxentities",    get_maxentities },
	{ "is_entity",          is_entity },
	{ "drop_to_floor",      drop_to_floor },
	{ "entity_use",         entity_use },
	{ "call_think",         call_think },

	{NULL,                 NULL}
};
