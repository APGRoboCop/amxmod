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

#ifndef vexdum_enum_h
#define vexdum_enum_h

// Entvars Integer
enum {
	EV_gamestate,
	EV_oldbuttons,
	EV_groupinfo,
	EV_iuser1,
	EV_iuser2,
	EV_iuser3,
	EV_iuser4,
	EV_weaponanim,
	EV_pushmsec,
	EV_bInDuck,
	EV_flTimeStepSound,
	EV_flSwimTime,
	EV_flDuckTime,
	EV_iStepLeft,
	EV_movetype,
	EV_solid,
	EV_skin,
	EV_body,
	EV_effects,
	EV_light_level,
	EV_sequence,
	EV_gaitsequence,
	EV_modelindex,
	EV_playerclass,
	EV_waterlevel,
	EV_watertype,
	EV_spawnflags,
	EV_flags,
	EV_colormap,
	EV_team,
	EV_fixangle,
	EV_weapons,
	EV_rendermode,
	EV_renderfx,
	EV_button,
	EV_impulse,
	EV_deadflag,
};

// Entvars Float
enum {
	EV_impacttime,
	EV_starttime,
	EV_idealpitch,
	EV_pitch_speed,
	EV_ideal_yaw,
	EV_yaw_speed,
	EV_ltime,
	EV_nextthink,
	EV_gravity,
	EV_friction,
	EV_frame,
	EV_animtime,
	EV_framerate,
	EV_health,
	EV_frags,
	EV_takedamage,
	EV_max_health,
	EV_teleport_time,
	EV_armortype,
	EV_armorvalue,
	EV_dmg_take,
	EV_dmg_save,
	EV_dmg,
	EV_dmgtime,
	EV_speed,
	EV_air_finished,
	EV_pain_finished,
	EV_radsuit_finished,
	EV_scale,
	EV_renderamt,
	EV_maxspeed,
	EV_fov,
	EV_flFallVelocity,
	EV_fuser1,
	EV_fuser2,
	EV_fuser3,
	EV_fuser4,
};

// Entvars Vector
enum {
	EV_origin,
	EV_oldorigin,
	EV_velocity,
	EV_basevelocity,
	EV_clbasevelocity,
	EV_movedir,
	EV_angles,
	EV_avelocity,
	EV_punchangle,
	EV_v_angle,
	EV_endpos,
	EV_startpos,
	EV_absmin,
	EV_absmax,
	EV_mins,
	EV_maxs,
	EV_size,
	EV_rendercolor,
	EV_view_ofs,
	EV_vuser1,
	EV_vuser2,
	EV_vuser3,
	EV_vuser4,
};

// Entvars Edict
enum {
	EV_chain,
	EV_dmg_inflictor,
	EV_enemy,
	EV_aiment,
	EV_owner,
	EV_groundentity,
	EV_pContainingEntity,
	EV_euser1,
	EV_euser2,
	EV_euser3,
	EV_euser4,
};

// Entvars String
enum {
	EV_classname,
	EV_globalname,
	EV_model,
	EV_target,
	EV_targetname,
	EV_netname,
	EV_message,
	EV_noise,
	EV_noise1,
	EV_noise2,
	EV_noise3,
	EV_viewmodel,
	EV_weaponmodel,
};

// Entvars Byte
enum {
	EV_controller0,
	EV_controller1,
	EV_controller2,
	EV_controller3,
	EV_blending0,
	EV_blending1,
};

// Globalvars Integer
enum {
	GV_trace_hitgroup,
	GV_trace_flags,
	GV_msg_entity,
	GV_cdAudioTrack,
	GV_maxClients,
	GV_maxEntities,
};

// Globalvars Float
enum {
	GV_time,			// Bollocky Linux crap
	GV_frametime,
	GV_force_retouch,
	GV_deathmatch,
	GV_coop,
	GV_teamplay,
	GV_serverflags,
	GV_found_secrets,
	GV_trace_allsolid,
	GV_trace_startsolid,
	GV_trace_fraction,
	GV_trace_plane_dist,
	GV_trace_inopen,
	GV_trace_inwater,
};

// Globalvars Vector
enum {
	GV_v_forward,
	GV_v_up,
	GV_v_right,
	GV_trace_endpos,
	GV_trace_plane_normal,
	GV_vecLandmarkOffset,
};

// Globalvars Edict
enum {
	GV_trace_ent,
};

// Globalvars String
enum {
	GV_mapname,
	GV_startspot,
};

// TraceLine Integer
enum {
	TR_fAllSolid,		// if true, plane is not valid
	TR_fStartSolid,		// if true, the initial point was in a solid area
	TR_fInOpen,
	TR_fInWater,
	TR_iHitgroup,		// 0 == generic, non zero is specific body part
};

// TraceLine Float
enum {
	TR_flFraction,		// time completed, 1.0 = didn't hit anything
	TR_flPlaneDist,
};

// TraceLine Vector
enum {
	TR_vecEndPos,		// final position
	TR_vecPlaneNormal,	// surface normal at impact
};

// TraceLine Edict
enum {
	TR_pHit,			// entity the surface is on
};

// By AMXMODX Dev Team
enum
{
	DLLFunc_GameInit,					// void )			( void );
	DLLFunc_Spawn,						// int  )			(edict_t *pent);
	DLLFunc_Think,						// void )			(edict_t *pent);
	DLLFunc_Use,						// void )			(edict_t *pentUsed, edict_t *pentOther);
	DLLFunc_Touch,						// void )			(edict_t *pentTouched, edict_t *pentOther);
	DLLFunc_Blocked,					// void )			(edict_t *pentBlocked, edict_t *pentOther);
	DLLFunc_KeyValue,					// void )			(edict_t *pentKeyvalue, KeyValueData *pkvd);
	DLLFunc_SetAbsBox,					// void )			(edict_t *pent);
	DLLFunc_ClientConnect,				// bool )			(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]);
	DLLFunc_ClientDisconnect,			// void )			(edict_t *pEntity);
	DLLFunc_ClientKill,					// void )			(edict_t *pEntity);
	DLLFunc_ClientPutInServer,			// void )			(edict_t *pEntity);
	DLLFunc_ClientCommand,				// void )			(edict_t *pEntity);
	DLLFunc_ServerDeactivate,			// void )			( void );
	DLLFunc_PlayerPreThink,				// void )			(edict_t *pEntity);
	DLLFunc_PlayerPostThink,			// void )			(edict_t *pEntity);
	DLLFunc_StartFrame,					// void )			( void );
	DLLFunc_ParmsNewLevel,				// void )			( void );
	DLLFunc_ParmsChangeLevel,			// void )			( void );
	DLLFunc_GetGameDescription,	 		// const char *)	( void );
	DLLFunc_SpectatorConnect,			// void )			(edict_t *pEntity);
	DLLFunc_SpectatorDisconnect,		// void )			(edict_t *pEntity);
	DLLFunc_SpectatorThink,				// void )			(edict_t *pEntity);
	DLLFunc_Sys_Error,					// void )			(const char *error_string);
	DLLFunc_PM_FindTextureType,			// char )			(char *name);
	DLLFunc_RegisterEncoders,			// void )			( void );
	DLLFunc_GetHullBounds,				// int  )			(int hullnumber, float *mins, float *maxs);
	DLLFunc_CreateInstancedBaselines,	// void )			( void );
	DLLFunc_pfnAllowLagCompensation,	// int  )			( void );
	MetaFunc_CallGameEntity,			// bool	)			(plid_t plid, const char *entStr,entvars_t *pev);
	DLLFunc_ClientUserInfoChanged,		// void	)			(edict *pEntity, char *infobuffer);
	DLLFunc_UpdateClientData,			// void )			(const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
	DLLFunc_AddToFullPack,				// int  )			(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet);
	DLLFunc_CmdStart,					// void )			(const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed);
	DLLFunc_CmdEnd,						// void )			(const edict_t *player);
	DLLFunc_CreateBaseline				// void )			(int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs);
};

enum {
	ES_EntityType,		// int
	ES_Number,			// int
	ES_MsgTime,			// float
	ES_MessageNum,		// int
	ES_Origin,			// float array[3]
	ES_Angles,			// float array[3]
	ES_ModelIndex,		// int
	ES_Sequence,		// int
	ES_Frame,			// float
	ES_ColorMap,		// int
	ES_Skin,			// short
	ES_Solid,			// short
	ES_Effects,			// int
	ES_Scale,			// float
	ES_eFlags,			// byte
	ES_RenderMode,		// int
	ES_RenderAmt,		// int
	ES_RenderColor,		// byte array[3], RGB value
	ES_RenderFx,		// int
	ES_MoveType,		// int
	ES_AnimTime,		// float
	ES_FrameRate,		// float
	ES_Body,			// int
	ES_Controller,		// byte array[4]
	ES_Blending,		// byte array[4]
	ES_Velocity,		// float array[3]
	ES_Mins,			// float array[3]
	ES_Maxs,			// float array[3]
	ES_AimEnt,			// int
	ES_Owner,			// int
	ES_Friction,		// float
	ES_Gravity,			// float
	ES_Team,			// int
	ES_PlayerClass,		// int
	ES_Health,			// int
	ES_Spectator,		// bool
	ES_WeaponModel,		// int
	ES_GaitSequence,	// int
	ES_BaseVelocity,	// float array[3]
	ES_UseHull,			// int
	ES_OldButtons,		// int
	ES_OnGround,		// int
	ES_iStepLeft,		// int
	ES_flFallVelocity,	// float
	ES_FOV,				// float
	ES_WeaponAnim,		// int
	ES_StartPos,		// float array[3]
	ES_EndPos,			// float array[3]
	ES_ImpactTime,		// float
	ES_StartTime,		// float
	ES_iUser1,			// int
	ES_iUser2,			// int
	ES_iUser3,			// int
	ES_iUser4,			// int
	ES_fUser1,			// float
	ES_fUser2,			// float
	ES_fUser3,			// float
	ES_fUser4,			// float
	ES_vUser1,			// float array[3]
	ES_vUser2,			// float array[3]
	ES_vUser3,			// float array[3]
	ES_vUser4			// float array[3]
};

#define	FMV_STRING	1
#define FMV_FLOAT		2
#define FMV_CELL		3
// End of By AMXMODX Dev Team

#endif // vexdum_enum_h
