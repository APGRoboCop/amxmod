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

// By AMXMODX Dev Team
entity_state_t g_es;
KVD_Wrapper g_kvd;
clientdata_t g_cd;
usercmd_t g_uc;

// By AMXMODX Dev Team (adapted by AMX Mod Team)
NATIVE(dllfunc) {
	int func = params[1];

	entity_state_t *es;
	cell *cParam;
	int index1;
	int index2;
	int iParam1;
	int iParam2;
	int iParam3;
	char *str1 = "";
	char *str2 = "";
	char *str3 = "";
	int iLen;
	vec3_t Vec1;
	vec3_t Vec2;
	unsigned char *pset;

  switch(func) {
	case DLLFunc_GameInit:					// void )			( void );
		MDLL_GameDLLInit();
		return 1;
	case DLLFunc_Spawn:						// int  )			(edict_t *pent);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			return MDLL_Spawn(INDEXENT2(index1));
		}
		break;
	case DLLFunc_Think:						// void )			(edict_t *pent);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_Think(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_Use:						// void )			(edict_t *pentUsed, edict_t *pentOther);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			cParam = MF_GetAmxAddr(amx, params[3]);
			index2 = cParam[0];
			if(MF_IsEntity(index2)) {
				MDLL_Use(INDEXENT2(index1), INDEXENT2(index2));
			}
		}
		return 1;
	case DLLFunc_Touch:						// void )			(edict_t *pentTouched, edict_t *pentOther);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			cParam = MF_GetAmxAddr(amx, params[3]);
			index2 = cParam[0];
			if(MF_IsEntity(index2)) {
				MDLL_Touch(INDEXENT2(index1), INDEXENT2(index2));
			}
		}
		return 1;
	case DLLFunc_Blocked:					// void )			(edict_t *pentBlocked, edict_t *pentOther);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			cParam = MF_GetAmxAddr(amx, params[3]);
			index2 = cParam[0];
			if(MF_IsEntity(index2)) {
				MDLL_Blocked(INDEXENT2(index1), INDEXENT2(index2));
			}
		}
		return 1;
	case DLLFunc_KeyValue:					// void )			(edict_t *pentKeyvalue, KeyValueData *pkvd);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			cParam = MF_GetAmxAddr(amx, params[3]);
			KVD_Wrapper *kvdw;
			if(*cParam == 0)
				kvdw = &g_kvd;
			else
				kvdw = reinterpret_cast<KVD_Wrapper *>(*cParam);
			MDLL_KeyValue(INDEXENT2(index1), kvdw->kvd);
		}
		return 1;
	case DLLFunc_SetAbsBox:					// void )			(edict_t *pent);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			gpGamedllFuncs->dllapi_table->pfnSetAbsBox(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_ClientConnect:				// bool )			(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			str1 = stringVault.put(MF_GetAmxString(amx, params[3], 0, iLen));
			str2 = stringVault.put(MF_GetAmxString(amx, params[4], 0, iLen));
			iParam1 = MDLL_ClientConnect(INDEXENT2(index1), str1, str2, str3);
			MF_SetAmxString(amx, params[5], str3, params[6]);
			return iParam1;
		}
		break;
	case DLLFunc_ClientDisconnect:			// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_ClientDisconnect(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_ClientKill:					// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_ClientKill(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_ClientPutInServer:			// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_ClientPutInServer(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_ClientCommand:				// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_ClientCommand(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_ServerDeactivate:			// void )			( void );
		MDLL_ServerDeactivate();
		return 1;
	case DLLFunc_PlayerPreThink:				// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_PlayerPreThink(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_PlayerPostThink:			// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_PlayerPostThink(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_StartFrame:					// void )			( void );
		MDLL_StartFrame();
		return 1;
	case DLLFunc_ParmsNewLevel:				// void )			( void );
		MDLL_ParmsNewLevel();
		return 1;
	case DLLFunc_ParmsChangeLevel:			// void )			( void );
		MDLL_ParmsChangeLevel();
		return 1;
	case DLLFunc_GetGameDescription:	 		// const char *)	( void );
		return MF_SetAmxString(amx, params[2], MDLL_GetGameDescription(), params[3]);
	case DLLFunc_SpectatorConnect:			// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_SpectatorConnect(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_SpectatorDisconnect:		// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_SpectatorDisconnect(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_SpectatorThink:				// void )			(edict_t *pEntity);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_SpectatorThink(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_Sys_Error:					// void )			(const char *error_string);
		str1 = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
		MDLL_Sys_Error(str1);
		return 1;
	case DLLFunc_PM_FindTextureType:			// char )			(char *name);
		str1 = MF_GetAmxString(amx, params[2], 0, iLen);
		return MDLL_PM_FindTextureType(str1);
	case DLLFunc_RegisterEncoders:			// void )			( void );
		MDLL_RegisterEncoders();
		return 1;
	case DLLFunc_GetHullBounds:				// int  )			(int hullnumber, float *mins, float *maxs);
		cParam = MF_GetAmxAddr(amx, params[2]);
		iParam1 = MDLL_GetHullBounds(cParam[0], Vec1, Vec2);
		cParam = MF_GetAmxAddr(amx, params[3]);
		cParam[0] = FloatToCell(Vec1.x);
		cParam[1] = FloatToCell(Vec1.y);
		cParam[2] = FloatToCell(Vec1.z);
		cParam = MF_GetAmxAddr(amx, params[4]);
		cParam[0] = FloatToCell(Vec2.x);
		cParam[1] = FloatToCell(Vec2.y);
		cParam[2] = FloatToCell(Vec2.z);
		return iParam1;
	case DLLFunc_CreateInstancedBaselines:	// void )			( void );
		MDLL_CreateInstancedBaselines();
		return 1;
	case DLLFunc_pfnAllowLagCompensation:	// int  )			( void );
		return MDLL_AllowLagCompensation();
	case MetaFunc_CallGameEntity:			// bool	)			(plid_t plid, const char *entStr,entvars_t *pev);
		str1 = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
		cParam = MF_GetAmxAddr(amx, params[3]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			return gpMetaUtilFuncs->pfnCallGameEntity(PLID, str1, VARS(INDEXENT2(index1)));
		}
		break;
	case DLLFunc_ClientUserInfoChanged:		// void	)			(edict *pEntity, char *infobuffer);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_ClientUserInfoChanged(INDEXENT2(index1),GET_INFOKEYBUFFER(INDEXENT2(index1)));
		}
		return 1;
	case DLLFunc_UpdateClientData:			// void )			(const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			cParam = MF_GetAmxAddr(amx, params[3]);
			iParam1 = cParam[0];

			clientdata_t *cd;

			if((params[0] / sizeof(cell)) == 4) {
				cell *ptr = MF_GetAmxAddr(amx, params[4]);
				if(*ptr == 0) {
					cd = &g_cd;
				}
				else {
					cd = reinterpret_cast<clientdata_s *>(*ptr);
				}
			}
			else {
				cd = &g_cd;
			}

			MDLL_UpdateClientData(INDEXENT2(index1), iParam1, cd);
		}
		return 1;
	case DLLFunc_AddToFullPack:				// int  )			(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet);
		cParam = MF_GetAmxAddr(amx, params[4]);
		index1 = cParam[0];

		if(MF_IsEntity(index1)) {

			cParam = MF_GetAmxAddr(amx, params[5]);
			index2 = cParam[0];

			if(MF_IsEntity(index2)) {

				cParam = MF_GetAmxAddr(amx, params[2]);
				if(*cParam == 0) {
					es = &g_es;
				}
				else {
					es = reinterpret_cast<entity_state_t *>(*cParam);
				}

				cParam = MF_GetAmxAddr(amx, params[3]);
				iParam1 = cParam[0];

				cParam = MF_GetAmxAddr(amx, params[6]);
				iParam2 = cParam[0];

				cParam = MF_GetAmxAddr(amx, params[7]);
				iParam3 = cParam[0];

				cParam = MF_GetAmxAddr(amx, params[8]);
				pset = reinterpret_cast<unsigned char *>(*cParam);

				return MDLL_AddToFullPack(es, iParam1, INDEXENT2(index1), INDEXENT2(index2), iParam2, iParam3, pset);
			}
		}
		break;
	case DLLFunc_CmdStart:					// void )			(const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			usercmd_t *uc;
			cParam = MF_GetAmxAddr(amx, params[3]);

			if(*cParam == 0) {
				uc = &g_uc;
			}
			else {
				uc = reinterpret_cast<usercmd_t *>(*cParam);
			}

			cParam = MF_GetAmxAddr(amx, params[4]);
			iParam1 = cParam[0];

			MDLL_CmdStart(INDEXENT2(index1), uc, iParam1);
		}
		return 1;
	case DLLFunc_CmdEnd:						// void )			(const edict_t *player);
		cParam = MF_GetAmxAddr(amx, params[2]);
		index1 = cParam[0];
		if(MF_IsEntity(index1)) {
			MDLL_CmdEnd(INDEXENT2(index1));
		}
		return 1;
	case DLLFunc_CreateBaseline:				// void )			(int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs);
		cParam = MF_GetAmxAddr(amx, params[5]);
		index1 = cParam[0];

		if(MF_IsEntity(index1)) {

			cParam = MF_GetAmxAddr(amx, params[2]);
			iParam1 = cParam[0];
			cParam = MF_GetAmxAddr(amx, params[3]);
			iParam2 = cParam[0];

			cParam = MF_GetAmxAddr(amx, params[4]);

			if(*cParam == 0) {
				es = &g_es;
			}
			else {
				es = reinterpret_cast<entity_state_t *>(*cParam);
			}

			cParam = MF_GetAmxAddr(amx, params[6]);
			iParam3 = cParam[0];

			cParam = MF_GetAmxAddr(amx, params[7]);
			Vec1.x = CellToFloat(cParam[0]);
			Vec1.y = CellToFloat(cParam[1]);
			Vec1.z = CellToFloat(cParam[2]);

			cParam = MF_GetAmxAddr(amx, params[8]);
			Vec2.x = CellToFloat(cParam[0]);
			Vec2.y = CellToFloat(cParam[1]);
			Vec2.z = CellToFloat(cParam[2]);

			MDLL_CreateBaseline(iParam1, iParam2, es, INDEXENT2(index1), iParam3, Vec1, Vec2);
		}
		return 1;
	default:
		MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid DLLFunc_ parameter");
		break;
  }
  return 0;
}

// By AMXMODX Dev Team (adapted by AMX Mod Team)
NATIVE(get_es) {
	cell* cVal = MF_GetAmxAddr(amx, params[1]);;
	entity_state_t *es;
	if(*cVal == 0) {
		es = &g_es;
	}
	else {
		es = reinterpret_cast<entity_state_t *>(*cVal);
	}

  int iSet = params[2];

  switch(iSet) {
	case ES_EntityType:
		return es->entityType;
	case ES_Number:
		return es->number;
	case ES_MsgTime:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->msg_time);
		break;
	case ES_MessageNum:
		return es->messagenum;
		break;
	case ES_Origin:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->origin.x);
		cVal[1] = FloatToCell(es->origin.y);
		cVal[2] = FloatToCell(es->origin.z);
		break;
	case ES_Angles:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->angles.x);
		cVal[1] = FloatToCell(es->angles.y);
		cVal[2] = FloatToCell(es->angles.z);
		break;
	case ES_ModelIndex:
		return es->modelindex;
	case ES_Sequence:
		return es->sequence;
	case ES_Frame:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->frame);
		break;
	case ES_ColorMap:
		return es->colormap;
	case ES_Skin:
		return es->skin;
	case ES_Solid:
		return es->solid;
	case ES_Effects:
		return es->effects;
	case ES_Scale:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->scale);
		break;
	case ES_eFlags:
		return es->eflags;
	case ES_RenderMode:
		return es->rendermode;
	case ES_RenderAmt:
		return es->renderamt;
	case ES_RenderColor:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = es->rendercolor.r;
		cVal[1] = es->rendercolor.g;
		cVal[2] = es->rendercolor.b;
		break;
	case ES_RenderFx:
		return es->renderfx;
	case ES_MoveType:
		return es->movetype;
	case ES_AnimTime:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->animtime);
		break;
	case ES_FrameRate:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->framerate);
		break;
	case ES_Body:
		return es->body;
	case ES_Controller:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = es->controller[0];
		cVal[1] = es->controller[1];
		cVal[2] = es->controller[2];
		cVal[3] = es->controller[3];
		break;
	case ES_Blending:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = es->blending[0];
		cVal[1] = es->blending[1];
		cVal[2] = es->blending[2];
		cVal[3] = es->blending[3];
		break;
	case ES_Velocity:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->velocity.x);
		cVal[1] = FloatToCell(es->velocity.y);
		cVal[2] = FloatToCell(es->velocity.z);
		break;
	case ES_Mins:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->mins.x);
		cVal[1] = FloatToCell(es->mins.y);
		cVal[2] = FloatToCell(es->mins.z);
		break;
	case ES_Maxs:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->maxs.x);
		cVal[1] = FloatToCell(es->maxs.y);
		cVal[2] = FloatToCell(es->maxs.z);
		break;
	case ES_AimEnt:
		return es->aiment;
	case ES_Owner:
		return es->owner;
	case ES_Friction:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->friction);
		break;
	case ES_Gravity:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->gravity);
		break;
	case ES_Team:
		return es->team;
	case ES_PlayerClass:
		return es->playerclass;
	case ES_Health:
		return es->health;
	case ES_Spectator:
		return es->spectator ? 1 : 0;
	case ES_WeaponModel:
		return es->weaponmodel;
	case ES_GaitSequence:
		return es->gaitsequence;
	case ES_BaseVelocity:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->basevelocity.x);
		cVal[1] = FloatToCell(es->basevelocity.y);
		cVal[2] = FloatToCell(es->basevelocity.z);
		break;
	case ES_UseHull:
		return es->usehull;
	case ES_OldButtons:
		return es->oldbuttons;
	case ES_OnGround:
		return es->onground;
	case ES_iStepLeft:
		return es->iStepLeft;
	case ES_flFallVelocity:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->flFallVelocity);
		break;
	case ES_FOV:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->fov);
		break;
	case ES_WeaponAnim:
		return es->weaponanim;
	case ES_StartPos:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->startpos.x);
		cVal[1] = FloatToCell(es->startpos.y);
		cVal[2] = FloatToCell(es->startpos.z);
		break;
	case ES_EndPos:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->endpos.x);
		cVal[1] = FloatToCell(es->endpos.y);
		cVal[2] = FloatToCell(es->endpos.z);
		break;
	case ES_ImpactTime:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->impacttime);
		break;
	case ES_StartTime:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->starttime);
		break;
	case ES_iUser1:
		return es->iuser1;
	case ES_iUser2:
		return es->iuser2;
	case ES_iUser3:
		return es->iuser3;
	case ES_iUser4:
		return es->iuser4;
	case ES_fUser1:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->fuser1);
		break;
	case ES_fUser2:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->fuser2);
		break;
	case ES_fUser3:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->fuser3);
		break;
	case ES_fUser4:
		cVal = MF_GetAmxAddr(amx, params[3]);
		*cVal = FloatToCell(es->fuser4);
		break;
	case ES_vUser1:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->vuser1.x);
		cVal[1] = FloatToCell(es->vuser1.y);
		cVal[2] = FloatToCell(es->vuser1.z);
		break;
	case ES_vUser2:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->vuser2.x);
		cVal[1] = FloatToCell(es->vuser2.y);
		cVal[2] = FloatToCell(es->vuser2.z);
		break;
	case ES_vUser3:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->vuser3.x);
		cVal[1] = FloatToCell(es->vuser3.y);
		cVal[2] = FloatToCell(es->vuser3.z);
		break;
	case ES_vUser4:
		cVal = MF_GetAmxAddr(amx, params[3]);
		cVal[0] = FloatToCell(es->vuser4.x);
		cVal[1] = FloatToCell(es->vuser4.y);
		cVal[2] = FloatToCell(es->vuser4.z);
		break;
	default:
		MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid ES_ parameter");
		return 0;
  }
	return 1;
}

// By AMXMODX Dev Team (adapted by AMX Mod Team)
NATIVE(set_es) {
	entity_state_t *es;
	if(params[1] == 0) {
		es = &g_es;
	}
	else {
		es = reinterpret_cast<entity_state_t *>(params[1]);
	}

  int iSet = params[2];

	int iVal;
	cell* cVal = MF_GetAmxAddr(amx, params[3]);

  switch(iSet) {
	case ES_EntityType:
		es->entityType = *cVal;
		break;
	case ES_Number:
		es->number = *cVal;
		break;
	case ES_MsgTime:
		es->msg_time = CellToFloat(*cVal);
		break;
	case ES_MessageNum:
		es->messagenum = *cVal;
		break;
	case ES_Origin:
		es->origin.x = CellToFloat(cVal[0]);
		es->origin.y = CellToFloat(cVal[1]);
		es->origin.z = CellToFloat(cVal[2]);
		break;
	case ES_Angles:
		es->angles.x = CellToFloat(cVal[0]);
		es->angles.y = CellToFloat(cVal[1]);
		es->angles.z = CellToFloat(cVal[2]);
		break;
	case ES_ModelIndex:
		es->modelindex = *cVal;
		break;
	case ES_Sequence:
		es->sequence = *cVal;
		break;
	case ES_Frame:
		es->frame = CellToFloat(*cVal);
		break;
	case ES_ColorMap:
		es->colormap = *cVal;
		break;
	case ES_Skin:
		iVal = *cVal;
		if(iVal > 32767) { iVal = 32767; }
		if(iVal < 0) { iVal = 0; }
		es->skin = iVal;
		break;
	case ES_Solid:
		iVal = *cVal;
		if(iVal > 32767) { iVal = 32767; }
		if(iVal < 0) { iVal = 0; }
		es->solid = iVal;
		break;
	case ES_Effects:
		es->effects = *cVal;
		break;
	case ES_Scale:
		es->scale = CellToFloat(*cVal);
		break;
	case ES_eFlags:
		iVal = *cVal;
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->eflags = iVal;
		break;
	case ES_RenderMode:
		es->rendermode = *cVal;
		break;
	case ES_RenderAmt:
		es->renderamt = *cVal;
		break;
	case ES_RenderColor:
		iVal = cVal[0];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->rendercolor.r = iVal;
		iVal = cVal[1];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->rendercolor.g = iVal;
		iVal = cVal[2];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->rendercolor.b = iVal;
		break;
	case ES_RenderFx:
		es->renderfx = *cVal;
		break;
	case ES_MoveType:
		es->movetype = *cVal;
		break;
	case ES_AnimTime:
		es->animtime = CellToFloat(*cVal);
		break;
	case ES_FrameRate:
		es->framerate = CellToFloat(*cVal);
		break;
	case ES_Body:
		es->body = *cVal;
		break;
	case ES_Controller:
		iVal = cVal[0];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->controller[0] = iVal;
		iVal = cVal[1];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->controller[1] = iVal;
		iVal = cVal[2];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->controller[2] = iVal;
		iVal = cVal[3];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->controller[3] = iVal;
		break;
	case ES_Blending:
		iVal = cVal[0];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->blending[0] = iVal;
		iVal = cVal[1];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->blending[1] = iVal;
		iVal = cVal[2];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->blending[2] = iVal;
		iVal = cVal[3];
		if(iVal > 255) { iVal = 255; }
    if(iVal < 0) { iVal = 0; }
		es->blending[3] = iVal;
		break;
	case ES_Velocity:
		es->velocity.x = CellToFloat(cVal[0]);
		es->velocity.y = CellToFloat(cVal[1]);
		es->velocity.z = CellToFloat(cVal[2]);
		break;
	case ES_Mins:
		es->mins.x = CellToFloat(cVal[0]);
		es->mins.y = CellToFloat(cVal[1]);
		es->mins.z = CellToFloat(cVal[2]);
		break;
	case ES_Maxs:
		es->maxs.x = CellToFloat(cVal[0]);
		es->maxs.y = CellToFloat(cVal[1]);
		es->maxs.z = CellToFloat(cVal[2]);
		break;
	case ES_AimEnt:
		es->aiment = *cVal;
		break;
	case ES_Owner:
		es->owner = *cVal;
		break;
	case ES_Friction:
		es->friction = CellToFloat(*cVal);
		break;
	case ES_Gravity:
		es->gravity = CellToFloat(*cVal);
		break;
	case ES_Team:
		es->team = *cVal;
		break;
	case ES_PlayerClass:
		es->playerclass = *cVal;
		break;
	case ES_Health:
		es->health = *cVal;
		break;
	case ES_Spectator:
		es->spectator = *cVal ? true : false;
		break;
	case ES_WeaponModel:
		es->weaponmodel = *cVal;
		break;
	case ES_GaitSequence:
		es->gaitsequence = *cVal;
		break;
	case ES_BaseVelocity:
		es->basevelocity.x = CellToFloat(cVal[0]);
		es->basevelocity.y = CellToFloat(cVal[1]);
		es->basevelocity.z = CellToFloat(cVal[2]);
		break;
	case ES_UseHull:
		es->usehull = *cVal;
		break;
	case ES_OldButtons:
		es->oldbuttons = *cVal;
		break;
	case ES_OnGround:
		es->onground = *cVal;
		break;
	case ES_iStepLeft:
		es->iStepLeft = *cVal;
		break;
	case ES_flFallVelocity:
		es->flFallVelocity = CellToFloat(*cVal);
		break;
	case ES_FOV:
		es->fov = CellToFloat(*cVal);
		break;
	case ES_WeaponAnim:
		es->weaponanim = *cVal;
		break;
	case ES_StartPos:
		es->startpos.x = CellToFloat(cVal[0]);
		es->startpos.y = CellToFloat(cVal[1]);
		es->startpos.z = CellToFloat(cVal[2]);
		break;
	case ES_EndPos:
		es->endpos.x = CellToFloat(cVal[0]);
		es->endpos.y = CellToFloat(cVal[1]);
		es->endpos.z = CellToFloat(cVal[2]);
		break;
	case ES_ImpactTime:
		es->impacttime = CellToFloat(*cVal);
		break;
	case ES_StartTime:
		es->starttime = CellToFloat(*cVal);
		break;
	case ES_iUser1:
		es->iuser1 = *cVal;
		break;
	case ES_iUser2:
		es->iuser2 = *cVal;
		break;
	case ES_iUser3:
		es->iuser3 = *cVal;
		break;
	case ES_iUser4:
		es->iuser4 = *cVal;
		break;
	case ES_fUser1:
		es->fuser1 = CellToFloat(*cVal);
		break;
	case ES_fUser2:
		es->fuser2 = CellToFloat(*cVal);
		break;
	case ES_fUser3:
		es->fuser3 = CellToFloat(*cVal);
		break;
	case ES_fUser4:
		es->fuser4 = CellToFloat(*cVal);
		break;
	case ES_vUser1:
		es->vuser1.x = CellToFloat(cVal[0]);
		es->vuser1.y = CellToFloat(cVal[1]);
		es->vuser1.z = CellToFloat(cVal[2]);
		break;
	case ES_vUser2:
		es->vuser2.x = CellToFloat(cVal[0]);
		es->vuser2.y = CellToFloat(cVal[1]);
		es->vuser2.z = CellToFloat(cVal[2]);
		break;
	case ES_vUser3:
		es->vuser3.x = CellToFloat(cVal[0]);
		es->vuser3.y = CellToFloat(cVal[1]);
		es->vuser3.z = CellToFloat(cVal[2]);
		break;
	case ES_vUser4:
		es->vuser4.x = CellToFloat(cVal[0]);
		es->vuser4.y = CellToFloat(cVal[1]);
		es->vuser4.z = CellToFloat(cVal[2]);
		break;
	default:
		MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid ES_ parameter");
		return 0;
  }
	return 1;
}

NATIVE(forward_return) {
	int type = params[1];
  switch(type) {
	case FMV_STRING:
		int iLen;
		g_FwdReturnVal.sVal = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
		break;
	case FMV_FLOAT:
		g_FwdReturnVal.fVal = CellToFloat(params[2]);
		break;
	case FMV_CELL:
		g_FwdReturnVal.iVal = params[2];
		break;
	default:
		return 0;
  }
  return 1;
}

// Native list.
AMX_NATIVE_INFO VexdUM_Meta[] = {
	{ "dllfunc",             dllfunc },
	{ "get_es",              get_es },
	{ "set_es",              set_es },
	{ "forward_return",      forward_return },

	{NULL,                   NULL}
};
