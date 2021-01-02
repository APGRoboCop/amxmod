/*
 * Copyright (c) 2002-2011
 *
 *    This file is part of AMX Mod.
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
 
/******************************** NATIVES *************************************/

#define AMX_MODULES_NUM 7

#define AMX_MODULES_CSSTATS_NUM 2
#define AMX_MODULES_DODXMOD_NUM 6

static char *amxModulesName[] = {
	"VexdUM",
	"mysql",
	"csstats",
	"fun",
	"geoip",
	"sockets",
	"dodxmod"
};

static int amxModulesNativesEndPosition[] = {
	84,
	96,
	108,
	136,
	139,
	144,
	186
};

static char *amxModulesNatives[] = {
"set_user_model", /* VEXDUM */ // vexdum.cpp
"CS_SetModel",
"CS_ClearModel",
"TFC_SetModel",
"TFC_ClearModel",
"attach_view",
"get_speak",
"set_speak",
"set_lights",
"PointContents",
"DispatchKeyValue",
"DispatchSpawn",
"trace_line",
"trace_hull",
"trace_normal",
"trace_texture",
"trace_result",
"precache_event",
"playback_event",
"get_info_keybuffer",
"get_keyvalue",
"copy_keyvalue",
"set_client_keyvalue",
"get_decal_index",
"fake_touch",
"set_gamedescription",
"get_offset_int",  
"set_offset_int",   
"create_bot", // vexdum_bot.cpp
"remove_bot",
"control_bot", 
"radius_damage", // vexdum_dmg.cpp
"take_damage", 
"client_death", 
"entity_get_int", // vexdum_ent.cpp
"entity_set_int",
"entity_get_float",
"entity_set_float",
"entity_get_vector",
"entity_set_vector",
"entity_get_edict",
"entity_set_edict",
"entity_get_string",
"entity_set_string",
"entity_get_byte",
"entity_set_byte",
"create_entity",
"entity_set_model",
"entity_set_origin",
"entity_set_size",
"find_entity",
"find_entity_sphere",
"remove_entity",
"get_num_ents",
"get_maxentities",   
"is_entity",
"drop_to_floor",
"entity_use",
"call_think",
"global_get_int", // vexdum_globals.cpp
"global_get_float",
"global_get_vector",	
"global_get_edict",
"global_get_string",	
"dot_points", // vexdum_misc.cpp
"dot_product",
"normalize",
"can_see",
"in_view_cone",
"pointer_to_index",
"index_to_pointer",
"make_vectors",
"traceline_get_int", // vexdum_tr.cpp
"traceline_set_int",
"traceline_get_float",
"traceline_set_float",
"traceline_get_vector",
"traceline_set_vector",
"traceline_get_edict",
"traceline_set_edict",
"dllfunc", // vexdum_minimeta.cpp
"get_es",
"set_es",
"forward_return",
"mysql_connect", /* MYSQL */
"mysql_query",
"mysql_nextrow",
"mysql_getfield",
"mysql_getresult",
"mysql_insert_id",
"mysql_affected_rows", 
"mysql_num_fields",
"mysql_num_rows",
"mysql_field_name",
"mysql_close",
"mysql_error",
"get_stats", /* CSSTATS */
"get_stats2",
"get_statsnum",
"get_user_astats",
"get_user_rstats",
"get_user_lstats",
"get_user_stats",
"get_user_stats2",
"get_user_vstats",
"get_user_wrstats",
"get_user_wstats",
"reset_user_wstats",
"get_user_deaths_cs", /* FUN */
"get_user_footsteps",
"get_user_godmode",
"get_user_gravity",
"get_user_hitzones",
"get_user_maxspeed",
"get_user_money",
"get_user_noclip",
"get_user_team_cs",
"get_client_listen",
"give_item",
"set_user_armor",
"set_user_deaths_cs",
"set_user_footsteps",
"set_user_frags",
"set_user_godmode",
"set_user_gravity",
"set_user_health",
"set_user_hitzones",
"set_user_maxspeed",
"set_user_money",
"set_user_noclip",
"set_user_origin",
"set_user_rendering",
"set_user_team_cs",
"set_client_listen",
"strip_user_weapons",
"user_spawn",
"geoip_code2", /* GEOIP */
"geoip_code3",
"geoip_country",
"socket_open", /* SOCKETS */
"socket_close",	
"socket_change",
"socket_recv",	
"socket_send",	
"dod_get_wpnname", /* DODXMOD */ // NBase.cpp
"dod_get_wpnlogname",
"dod_wpnlog_to_name",
"dod_wpnlog_to_id",
"dod_is_melee",
"dod_get_team_score",
"dod_get_user_score",
"dod_get_user_class",
"dod_get_user_team",
"dod_get_user_weapon",
"detect_allies_team",
"detect_allies_paras",
"detect_axis_paras",
"set_user_stamina",
"reset_user_stamina",
"dod_get_pronestate",
"dod_user_kill",
"nade_set_fuse",
"nade_reset_fuse",
"dod_set_user_class",
"dod_set_user_team",
"dod_get_next_class",
"dod_is_randomclass",
"dod_get_user_deaths",
"dod_set_user_deaths",
"dod_set_user_score",
"dod_get_user_kills",
"dod_set_user_kills",
"dod_get_user_ammo",
"dod_set_user_ammo",
"dod_is_deployed", 
"get_stats", // NRank.cpp
"get_statsnum",   
"get_user_astats",  
"get_user_rstats",  
"get_user_lstats",  
"get_user_stats",
"get_user_vstats",  
"get_user_wlstats", 
"get_user_wrstats", 
"get_user_wstats",  
"reset_user_wstats"
};

/******************************* FORWARDS *************************************/

#define AMX_MODULES_NUM_FW 3

#define AMX_MODULES_CSSTATS_NUM_FW 1
#define AMX_MODULES_DODXMOD_NUM_FW 2

static char *amxModulesName_FW[] = {
	"VexdUM",
	"csstats",
	"dodxmod"
};

static int amxModulesForwardsEndPosition[] = {
	17,
	25,
  29
};

static char *amxModulesForwards[] = {
"client_kill", /* VEXDUM */
"client_postthink",
"client_prethink",
"emitsound",
"emitambientsound",
"entity_think",
"entity_touch",
"entity_spawn",
"server_frame",
"set_model",
"traceline",
"monster_hurt",
"setclientkeyvalue",
"keyvalue",
"alertmessage",
"gamedescription",
"addtofullpack",
"damage_info", /* CSSTATS */
"death_info",
"grenade_throw",
"bomb_planting",
"bomb_planted",
"bomb_exploded",
"bomb_defusing",
"bomb_defused",
"damage_info", /* DODXMOD */
"death_info",
"score_info",
"grenade_throw"
};
