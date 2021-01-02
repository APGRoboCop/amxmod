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

// Sets a model for a player.
NATIVE(set_user_model) {
  int id = params[1];
  if(MF_IsPlayer(id, 1)) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);
    int iLen;
    char* szModel = "";
    if((*params / sizeof(cell)) == 2) {
      szModel = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
    }
    if(strcmp(szModel, "") == 0) {
      if(!pPlayer->modeled) {
        return 0;
      }
      *pPlayer->modelname = 0;
      pPlayer->modeled = false;
      pPlayer->fModelSet = 0.0f;
      pPlayer->SetModel = 0;
      if(g_bmod_cstrike) {
        if(countModelled(false) >= MAX_MODELLED) {
          pPlayer->fModelSet = gpGlobals->time + RANDOM_FLOAT(0.02f, 0.10f);
          pPlayer->UnSetModel = true;
        }
        else {
          pPlayer->UnSetModel = false;
          MDLL_ClientUserInfoChanged(pPlayer->pEdict, GET_INFOKEYBUFFER(pPlayer->pEdict));
        }
        if(countModelled(true) == 0 && g_forwards.forwardsExist(FF_ClientPostThink) == false) {
          g_pFunctionTable->pfnPlayerPostThink = NULL;
        }
      }
      else {
        MDLL_ClientUserInfoChanged(pPlayer->pEdict, GET_INFOKEYBUFFER(pPlayer->pEdict));
        if(countModelled(false) == 0 && g_forwards.forwardsExist(FF_ClientPostThink) == false) {
          g_pFunctionTable->pfnPlayerPostThink = NULL;
        }
      }
      if(countModelled(false) == 0) {
        g_pFunctionTable->pfnClientUserInfoChanged = NULL;
      }
      return 1;
    }
    if(g_pFunctionTable->pfnPlayerPostThink == NULL) {
      g_pFunctionTable->pfnPlayerPostThink = PlayerPostThink;
    }
    if(g_pFunctionTable->pfnClientUserInfoChanged == NULL) {
      g_pFunctionTable->pfnClientUserInfoChanged = ClientUserInfoChanged;
    }
    strcpy(pPlayer->modelname, szModel);
    pPlayer->modeled = true;
    if(g_bmod_cstrike) {
      pPlayer->UnSetModel = false;
      pPlayer->SetModel = 2;
      if(countModelled(false) >= MAX_MODELLED) {
        pPlayer->fModelSet = gpGlobals->time + RANDOM_FLOAT(0.02f, 0.10f);
      }
      else {
        SET_CLIENT_KEYVALUE(pPlayer->id, GET_INFOKEYBUFFER(pPlayer->pEdict), "model", pPlayer->modelname);
      }
    }
    else {
      SET_CLIENT_KEYVALUE(pPlayer->id, GET_INFOKEYBUFFER(pPlayer->pEdict), "model", pPlayer->modelname);
    }
		return 1;
  }
  return 0;
}

// Set A TFC Player's model. This works differently than CS.
NATIVE(TFC_SetModel) {
  int id = params[1];
  if(MF_IsPlayer(id, 1)) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);
    int iLen;
    char* szModel = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
    char* szSkin = stringVault.put(MF_GetAmxString(amx, params[3], 0, iLen));

    KeyValueData pkvd;
    pkvd.szClassName = "player";
    pkvd.szKeyName = "replacement_model";
    pkvd.szValue = szModel;
    pkvd.fHandled = FALSE;
    MDLL_KeyValue(pPlayer->pEdict, &pkvd);

    KeyValueData pkvd2;
    pkvd2.szClassName = "player";
    pkvd2.szKeyName = "replacement_model_skin";
    pkvd2.szValue = szSkin;
    pkvd2.fHandled = FALSE;
    MDLL_KeyValue(pPlayer->pEdict, &pkvd2);

    SET_CLIENT_KEYVALUE(pPlayer->id, GET_INFOKEYBUFFER(pPlayer->pEdict), "model", szModel);
		return 1;
  }
  return 0;
}

// Clear's a TFC player's model... Almost the same as setting the model.
// HACKHACK: Is there a better way of doing this? what if the player is random?
NATIVE(TFC_ClearModel) {
  int id = params[1];
  if(MF_IsPlayer(id, 1)) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);
    char szModel[32];

    switch(pPlayer->pEdict->v.playerclass) {
    case 1: //Scout
      strcpy(szModel, "Scout");
      break;
    case 2: //Sniper
      strcpy(szModel, "Sniper");
      break;
    case 3: //Soldier
      strcpy(szModel, "Soldier");
      break;
    case 4: //demoman
      strcpy(szModel, "Demo");
      break;
    case 5: //Medic
      strcpy(szModel, "Medic");
      break;
    case 6: //HWGuy
      strcpy(szModel, "HvyWeapon");
      break;
    case 7: //Pyro
      strcpy(szModel, "Pyro");
      break;
    case 8: //Spy
      strcpy(szModel, "Spy");
      break;
    case 9: //Engineer
      strcpy(szModel, "Engineer");
      break;
    case 11: //Hunted
      strcpy(szModel, "Civilian");
      break;
    default:
      break;
    }

    KeyValueData pkvd;
    pkvd.szClassName = "player";
    pkvd.szKeyName = "replacement_model";
    pkvd.szValue = szModel;
    pkvd.fHandled = FALSE;
    MDLL_KeyValue(pPlayer->pEdict, &pkvd);

    KeyValueData pkvd2;
    pkvd2.szClassName = "player";
    pkvd2.szKeyName = "replacement_model_skin";
    pkvd2.szValue = "0";
    pkvd2.fHandled = FALSE;
    MDLL_KeyValue(pPlayer->pEdict, &pkvd2);

    SET_CLIENT_KEYVALUE(pPlayer->id, GET_INFOKEYBUFFER(pPlayer->pEdict), "model", szModel);
		return 1;
  }
  return 0;
}

// Gets the contents of a point. Return values for this are probably in const.h.
NATIVE(PointContents) {
  cell *vInput = MF_GetAmxAddr(amx,params[1]);

  float vVal[3] = {
    CellToFloat(vInput[0]),
    CellToFloat(vInput[1]),
    CellToFloat(vInput[2])
  };

  return POINT_CONTENTS(vVal);
}

// DispatchKeyValue, sets a key-value pair for a newly created entity.
// Use DispatchSpawn after doing ALL DispatchKeyValues on an entity.
NATIVE(DispatchKeyValue) {
	if((*params/sizeof(cell)) == 3) {
    cell *cVal = MF_GetAmxAddr(amx, params[1]);
		int ent = *cVal;
    if(MF_IsEntity(ent)) {
      edict_t* pEntity = INDEXENT(ent);
      int iLen;
      char* szKey = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
      char* szVal = stringVault.put(MF_GetAmxString(amx, params[3], 0, iLen));
      KeyValueData pkvd;
      pkvd.szClassName = (char *)STRING(pEntity->v.classname);
      pkvd.szKeyName = szKey;
      pkvd.szValue = szVal;
      pkvd.fHandled = false;
      MDLL_KeyValue(pEntity, &pkvd);
      return 1;
    }
	}
	else {
    if(g_inKeyValue) {
      // By AMXMODX Dev Team
  		int iLen;
  		char* szKey = stringVault.put(MF_GetAmxString(amx, params[1], 0, iLen));
      char* szVal = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
  		g_pkvd->szKeyName = szKey;
  		g_pkvd->szValue = szVal;
      return 1;
    }
	}
  return 0;
}

// DispatchSpawn. Call this after doing any DispatchKeyValues you are
// doing on an entity your creating.
NATIVE(DispatchSpawn) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    MDLL_Spawn(pEntity);
		return 1;
  }
  return 0;
}

// trace_line, Traces a Line in space. This is useful for making beams, or checking if
// an entity is at the end of a line.
// The return value is either the end of the line (where it crashed into an object,
// or the end you supplied).
NATIVE(trace_line) {
  int ent = params[1];

  cell* fStart = MF_GetAmxAddr(amx, params[2]);
  float vStart[3] = {
    CellToFloat(fStart[0]),
    CellToFloat(fStart[1]),
    CellToFloat(fStart[2])
  };

  cell* fEnd = MF_GetAmxAddr(amx, params[3]);
  float vEnd[3] = {
    CellToFloat(fEnd[0]),
    CellToFloat(fEnd[1]),
    CellToFloat(fEnd[2])
  };

  cell* vRet = MF_GetAmxAddr(amx, params[4]);
  TraceResult tr;

  if(ent == -1) {
    TRACE_LINE(vStart, vEnd, ignore_monsters, NULL, &tr);
  }
  else {
    TRACE_LINE(vStart, vEnd, dont_ignore_monsters, INDEXENT(ent), &tr);
  }

  edict_t *pHit = tr.pHit;
  vRet[0] = FloatToCell(tr.vecEndPos[0]);
  vRet[1] = FloatToCell(tr.vecEndPos[1]);
  vRet[2] = FloatToCell(tr.vecEndPos[2]);

  if(FNullEnt(pHit)) {
    return -1;
  }

  return ENTINDEX(pHit);
}

// By AMXMODX Dev Team
// trace_hull
NATIVE(trace_hull) {
  int iResult = 0;
  TraceResult tr;

  cell* fOrigin = MF_GetAmxAddr(amx, params[1]);
  float vOrigin[3] = {
    CellToFloat(fOrigin[0]),
    CellToFloat(fOrigin[1]),
    CellToFloat(fOrigin[2])
  };

  TRACE_HULL(vOrigin, vOrigin, params[4], params[2], params[3] > 0 ? INDEXENT(params[3]) : NULL , &tr);

  if(tr.fStartSolid) {
    iResult += 1;
  }
  if(tr.fAllSolid) {
    iResult += 2;
  }
  if(!tr.fInOpen) {
    iResult += 4;
  }

  return iResult;
}

// trace_normal. This is just like TraceLine, but it gives back the
// normal of whatever plane it hit. Use with TraceLine and you can have
// a point on a plane, with its normal, good for creating things that attach
// to walls (like tripmines).
NATIVE(trace_normal) {
  int ent = params[1];

  cell* fStart = MF_GetAmxAddr(amx, params[2]);
  float vStart[3] = {
    CellToFloat(fStart[0]),
    CellToFloat(fStart[1]),
    CellToFloat(fStart[2])
  };

  cell* fEnd = MF_GetAmxAddr(amx, params[3]);
  float vEnd[3] = {
    CellToFloat(fEnd[0]),
    CellToFloat(fEnd[1]),
    CellToFloat(fEnd[2])
  };

  cell* vRet = MF_GetAmxAddr(amx, params[4]);
  TraceResult tr;
  TRACE_LINE(vStart, vEnd, dont_ignore_monsters, INDEXENT(ent), &tr);
  vRet[0] = FloatToCell(tr.vecPlaneNormal[0]);
  vRet[1] = FloatToCell(tr.vecPlaneNormal[1]);
  vRet[2] = FloatToCell(tr.vecPlaneNormal[2]);

  if(tr.flFraction >= 1.0) {
    return 0;
  }

  return 1;
}

// trace_texture. Traces what texture a entity is looking at.
NATIVE(trace_texture) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t *pEntity = INDEXENT(ent);

    int set = params[4];
    char szBuf[64];
    Vector vStart, vEnd;

    MAKE_VECTORS(pEntity->v.v_angle);

    if(set <= 0) {
      vStart = pEntity->v.origin + pEntity->v.view_ofs;
      vEnd = pEntity->v.origin + pEntity->v.view_ofs + gpGlobals->v_forward * 9999;
    }
    else {
      vStart = pEntity->v.origin;
      vEnd = pEntity->v.origin - Vector (0, 0, 2048);
    }

    TraceResult tr;
    TRACE_LINE(vStart, vEnd, ignore_monsters, pEntity, &tr);

    if(tr.pHit) {
      const char *pTextureName = TRACE_TEXTURE(ENT(0), vStart, vEnd);

      if(pTextureName) {
        // strip leading '-0' or '+0~' or '{' or '!'
        if(*pTextureName == '-' || *pTextureName == '+') {
          pTextureName += 2;
        }
        if(*pTextureName == '{' || *pTextureName == '!' || *pTextureName == '~' || *pTextureName == ' ') {
          ++pTextureName;
        }

        strcpy(szBuf, pTextureName);
      }
    }
    return MF_SetAmxString(amx, params[2], szBuf, params[3]);
  }
  return 0;
}

// By AMXMODX Dev Team
NATIVE(trace_result)
{
  int type = params[1];
  cell *cRet;
/*
  TR_AllSolid,      // (int) if true, plane is not valid
  TR_StartSolid,    // (int) if true, the initial point was in a solid area
  TR_InOpen,        // (int)
  TR_InWater,       // (int)
  TR_Fraction,      // (float) time completed, 1.0 = didn't hit anything
  TR_EndPos,        // (vector) final position
  TR_PlaneDist,     // (float)
  TR_PlaneNormal,   // (vector) surface normal at impact
  TR_Hit,           // (entity) entity the surface is on
  TR_Hitgroup       // (int) 0 == generic, non zero is specific body part
*/
  switch(type) {
    case TR_AllSolid:
      return vx_tr->fAllSolid;
    case TR_StartSolid:
      return vx_tr->fStartSolid;
    case TR_InOpen:
      return vx_tr->fInOpen;
    case TR_InWater:
      return vx_tr->fInWater;
    case TR_Hitgroup:
      return vx_tr->iHitgroup;
    case TR_Hit:
      if(!FNullEnt(vx_tr->pHit))
        return ENTINDEX(vx_tr->pHit);
      else
        return -1;
    case TR_Fraction:
      cRet = MF_GetAmxAddr(amx,params[2]);
      cRet[0] = amx_ftoc(vx_tr->flFraction);
      return 1;
    case TR_EndPos:
      cRet = MF_GetAmxAddr(amx,params[2]);
      cRet[0] = amx_ftoc(vx_tr->vecEndPos[0]);
      cRet[1] = amx_ftoc(vx_tr->vecEndPos[1]);
      cRet[2] = amx_ftoc(vx_tr->vecEndPos[2]);
      return 1;
    case TR_PlaneDist:
      cRet = MF_GetAmxAddr(amx,params[2]);
      cRet[0] = amx_ftoc(vx_tr->flPlaneDist);
      return 1;
    case TR_PlaneNormal:
      cRet = MF_GetAmxAddr(amx,params[2]);
      cRet[0] = amx_ftoc(vx_tr->vecPlaneNormal[0]);
      cRet[1] = amx_ftoc(vx_tr->vecPlaneNormal[1]);
      cRet[2] = amx_ftoc(vx_tr->vecPlaneNormal[2]);
      return 1;
  }
  return 0;
}

// attach_view, this allows you to attach a player's view to an entity.
// use attach_view(player, player) to reset view.
NATIVE(attach_view) {
  int id = params[1];
  int ent = params[2];
  if(MF_IsPlayer(id, 1) && MF_IsEntity(ent)) {
    edict_t *pClient = INDEXENT(id);
    edict_t *pViewent = INDEXENT(ent);
    SET_VIEW(pClient, pViewent);
  }
  return 1;
}

// GetSpeak, returns who can speak to who.
NATIVE(get_speak) {
  int id = params[1];
  int target = params[2];

  if(id == 0) {
    bool same = true;
    int spkflags = -1;
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      if(spkflags == -1) {
        spkflags = (int)GET_PLAYER_POINTER_I(i)->speakflags[target];
      }
      else {
        if((int)GET_PLAYER_POINTER_I(i)->speakflags[target] != spkflags) {
          same = false;
        }
      }
    }
    if(same) {
      return (spkflags == 8) ? -1 : spkflags;
    }
    else {
      return -1;
    }
  }
  else {
    if(MF_IsPlayer(id, 1)) {
      int spkflags = (int)GET_PLAYER_POINTER_I(id)->speakflags[target];
      return (spkflags == 8) ? -1 : spkflags;
    }
  }
  return 0;
}

// SetSpeak, this sets who can speak to who.
NATIVE(set_speak) {
  int id = params[1];
  int iSpeak = params[2];

  int target;
  if((*params / sizeof(cell)) == 2) {
    target = 0;
  }
  else {
    target = params[3];
  }

  if(g_pengfuncsTable->pfnVoice_SetClientListening == NULL) {
    g_pengfuncsTable->pfnVoice_SetClientListening = Voice_SetClientListening;
  }
    
  if(id == 0) {
    CPlayer *pPlayer;
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      pPlayer = GET_PLAYER_POINTER_I(i);
      if(target == 0) {
        for(int i = 0; i <= gpGlobals->maxClients; ++i) {
          pPlayer->speakflags[i] = iSpeak;
        }
        if(iSpeak == SPEAK_NORMAL) {
          g_pengfuncsTable->pfnVoice_SetClientListening = NULL;
        }
      }
      else {
        if(MF_IsPlayer(target, 1)) {
          pPlayer->speakflags[target] = iSpeak;
          if((int)pPlayer->speakflags[0] != iSpeak) {
            pPlayer->speakflags[0] = 8;
          }
        }
      }
    }
  }
  else {
    if(MF_IsPlayer(id, 1)) {
      CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);
      if(target == 0) {
        for(int i = 0; i <= gpGlobals->maxClients; ++i) {
          pPlayer->speakflags[i] = iSpeak;
        }
      }
      else {
        if(MF_IsPlayer(target, 1)) {
          pPlayer->speakflags[target] = iSpeak;
          if((int)pPlayer->speakflags[0] != iSpeak) {
            pPlayer->speakflags[0] = 8;
          }
        }
      }
      if(disableVoiceClientListening() == true)
        g_pengfuncsTable->pfnVoice_SetClientListening = NULL;
      return 1;
    }
    if(disableVoiceClientListening() == true)
      g_pengfuncsTable->pfnVoice_SetClientListening = NULL;
    return 0;
  }
  if(disableVoiceClientListening() == true)
    g_pengfuncsTable->pfnVoice_SetClientListening = NULL;
  return 1;
}

// SetLights, this sets the lights for the map.
NATIVE(set_lights) {
  int iLen;
  char* szLights = MF_GetAmxString(amx, params[1], 0, iLen);

  if(FStrEq(szLights, "#OFF")) {
    gInfo.bLights = false;
    *gInfo.szLastLights = 0;
    LIGHT_STYLE(0, gInfo.szRealLights);
    
    if(g_forwards.forwardsExist(FF_ServerFrame) == false) {
      g_pFunctionTable_Post->pfnStartFrame = NULL;
    }

    return 1;
  }
  
  if(g_pFunctionTable_Post->pfnStartFrame == NULL) {
    g_pFunctionTable_Post->pfnStartFrame = StartFrame_Post;
  }

  gInfo.bLights = true;
  strcpy(gInfo.szLastLights, szLights);
  LIGHT_STYLE(0, szLights);

  // These make it so that players/weaponmodels look like whatever the lighting is
  // at. otherwise it would color players under the skybox to these values.
  SERVER_COMMAND("sv_skycolor_r 0\n");
  SERVER_COMMAND("sv_skycolor_g 0\n");
  SERVER_COMMAND("sv_skycolor_b 0\n");

  return 1;
}

// Gets a message block.
NATIVE(get_msg_block) {
  int msg = params[1];

  if(msg < 1 || msg >= MAX_REG_MSGS) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid msg id %d", msg);
    return 0;
  }

  return gInfo.iMessageBlock[msg];
}

// Sets a message block.
NATIVE(set_msg_block) {
  int msg = params[1];

  if(msg < 1 || msg >= MAX_REG_MSGS) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid msg id %d", msg);
    return 0;
  }

  int iMessageFlags = params[2];
  gInfo.iMessageBlock[msg] = iMessageFlags;

  return 1;
}

// Precache an event
NATIVE(precache_event) {
  int iLen;
  char *szEvent = MF_GetAmxString(amx, params[2], 0, iLen);

  return PRECACHE_EVENT(params[1], (char*)STRING(ALLOC_STRING(szEvent)));
}

// get_info_keybuffer(id, buffer[], length);
NATIVE(get_info_keybuffer) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    return MF_SetAmxString(amx, params[2], GET_INFOKEYBUFFER(pEntity), params[3]);
  }
  return 0;
}

// By AMXMODX Dev Team
// Get decal index
NATIVE(get_decal_index) {
  int iLen;
  char *szDecal = MF_GetAmxString(amx, params[1], 0, iLen);

  return DECAL_INDEX(szDecal);
}

// By AMXMODX Dev Team
// Get a keyvalue from an entity
NATIVE(get_keyvalue) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int iLen;
    char* szKey = MF_GetAmxString(amx, params[2], 0, iLen);
    return MF_SetAmxString(amx, params[3], INFOKEY_VALUE(GET_INFOKEYBUFFER(pEntity), szKey), params[4]);
  }
  return 0;
}

// By AMXMODX Dev Team
// Copy keyvalue when KeyValue forward is called
NATIVE(copy_keyvalue) {
  if(g_inKeyValue) {
    if(g_pkvd->szClassName) {
      MF_SetAmxString(amx, params[1], g_pkvd->szClassName, params[2]);
    }
    if(g_pkvd->szKeyName) {
      MF_SetAmxString(amx, params[3], g_pkvd->szKeyName, params[4]);
    }
    if(g_pkvd->szValue) {
      MF_SetAmxString(amx, params[5], g_pkvd->szValue, params[6]);
    }
		return 1;
  }
  return 0;
}

//Set client key value
NATIVE(set_client_keyvalue) {
  int id = params[1];
  if(MF_IsPlayer(id, 0)) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);
    int iLen;
    char* szKey = stringVault.put(MF_GetAmxString(amx, params[2], 0, iLen));
    char* szValue = stringVault.put(MF_GetAmxString(amx, params[3], 0, iLen));
    SET_CLIENT_KEYVALUE(pPlayer->id, GET_INFOKEYBUFFER(pPlayer->pEdict), szKey, szValue);
		return 1;
  }
  return 0;
}

// Creates fake touching between to entities
NATIVE(fake_touch) {
  int ent1 = params[1];
  int ent2 = params[2];
  if(MF_IsEntity(ent1) && MF_IsEntity(ent2)) {
    edict_t* pEntity1 = INDEXENT(ent1);
    edict_t* pEntity2 = INDEXENT(ent2);
    MDLL_Touch(pEntity1, pEntity2);
		return 1;
  }
  return 0;
}

// Sets Game Description
NATIVE(set_gamedescription) {
  if(g_inGameDescription) {
    int iLen;
    char *szTemp = MF_GetAmxString(amx, params[1], 0, iLen);
    snprintf(g_GameDescription,127,"%s",szTemp);
		return 1;
  }
  return 0;
}

//playback_event(flags,invoker,eventindex,Float:delay,Float:origin[3],Float:angles[3],Float:fparam1,Float:fparam2,iparam1,iparam2,bparam1,bparam2)
NATIVE(playback_event) {
  int flags;
  edict_t* pInvoker;
  unsigned short eventindex;
  float delay;
  vec3_t origin;
  vec3_t angles;
  float fparam1;
  float fparam2;
  int iparam1;
  int iparam2;
  int bparam1;
  int bparam2;
  flags = params[1];
  pInvoker = INDEXENT(params[2]);
  eventindex = params[3];
  delay = *(float*)&params[4];

  cell* vInput1 = MF_GetAmxAddr(amx, params[5]);
  float vOrigin[3] = {
    CellToFloat(vInput1[0]),
    CellToFloat(vInput1[1]),
    CellToFloat(vInput1[2])
  };

  cell* vInput2 = MF_GetAmxAddr(amx, params[6]);
  float vAngles[3] = {
    CellToFloat(vInput2[0]),
    CellToFloat(vInput2[1]),
    CellToFloat(vInput2[2])
  };

  fparam1 = *(float*)&params[7];
  fparam2 = *(float*)&params[8];
  iparam1 = params[9];
  iparam2 = params[10];
  bparam1 = params[11];
  bparam2 = params[12];
  PLAYBACK_EVENT_FULL(flags, pInvoker, eventindex, delay, vOrigin, vAngles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2);

  return 1;
}

// Gets a pvPrivateData offset for a player (player, offset)
NATIVE(get_offset_int) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int pvSet = params[2];

#if defined LINUX
    int oSet = params[3];
    pvSet += oSet;
#endif

    return *((int *)pEntity->pvPrivateData + pvSet);
  }
  return 0;
}

// Sets a pvPrivateData offset for a player (player, offset, val)
NATIVE(set_offset_int) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int pvSet = params[2];

#if defined LINUX
    int oSet = params[4];
    pvSet += oSet;
#endif

    *((int *)pEntity->pvPrivateData + pvSet) = params[3];
  }
  return 1;
}

// Gets a pvPrivateData offset for a player (player, offset)
NATIVE(get_offset_float) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int pvSet = params[2];

#if defined LINUX
    int oSet = params[3];
    pvSet += oSet;
#endif

    return FloatToCell(*((float *)pEntity->pvPrivateData + pvSet));
  }
  return 0;
}

// Sets a pvPrivateData offset for a player (player, offset, Float:val)
NATIVE(set_offset_float) {
  int ent = params[1];
  if(MF_IsEntity(ent)) {
    edict_t* pEntity = INDEXENT(ent);
    int pvSet = params[2];

#if defined LINUX
    int oSet = params[4];
    pvSet += oSet;
#endif

    *((float *)pEntity->pvPrivateData + pvSet) = CellToFloat(params[3]);
  }
  return 1;
}

// Native list.
AMX_NATIVE_INFO VexdUM_Gen[] = {
  { "set_user_model",   set_user_model },
  { "CS_SetModel",      set_user_model }, //Backward Compatibility
  { "CS_ClearModel",    set_user_model }, //Backward Compatibility
  { "TFC_SetModel",     TFC_SetModel },
  { "TFC_ClearModel",   TFC_ClearModel },

  { "attach_view",    attach_view },
  { "get_speak",      get_speak },
  { "set_speak",      set_speak },
  { "set_lights",     set_lights },

  { "PointContents",    PointContents },

  { "DispatchKeyValue",  DispatchKeyValue },
  { "DispatchSpawn",     DispatchSpawn },

  { "trace_line",       trace_line },
  { "trace_hull",       trace_hull },
  { "trace_normal",     trace_normal },
  { "trace_texture",    trace_texture },
  { "trace_result",     trace_result },

  { "get_msg_block",    get_msg_block },
  { "set_msg_block",    set_msg_block },

  { "precache_event",      precache_event },
  { "playback_event",      playback_event },
  { "get_info_keybuffer",  get_info_keybuffer },
  { "get_keyvalue",        get_keyvalue },
  { "copy_keyvalue",       copy_keyvalue },
  { "set_client_keyvalue", set_client_keyvalue },
  { "get_decal_index",     get_decal_index },
  { "fake_touch",          fake_touch },
  { "set_gamedescription", set_gamedescription },

  { "get_offset_int",   get_offset_int },
  { "set_offset_int",   set_offset_int },
  { "get_offset_float",   get_offset_float },
  { "set_offset_float",   set_offset_float },


  { NULL,       NULL }
};
