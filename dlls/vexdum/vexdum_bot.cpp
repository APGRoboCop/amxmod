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

NATIVE(create_bot) {
	int iLen;
	char* botname = MF_GetAmxString(amx, params[1], 0, iLen);
	edict_t* pEdict = CREATE_FAKE_CLIENT(botname);

	if(FNullEnt(pEdict))	{
		MF_AmxLogError(amx, AMX_ERR_NATIVE, "Couldn't create bot");
		return 0;
	}
	CPlayer *pPlayer = GET_PLAYER_POINTER(pEdict);
	pPlayer->pEdict = pEdict;
	pPlayer->id = ENTINDEX(pEdict);
	CALL_GAME_ENTITY(PLID, "player", VARS(pPlayer->pEdict));
	char *sInfoBuffer = GET_INFOKEYBUFFER(pPlayer->pEdict);
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "rate", "3500");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "cl_updaterate", "20");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "cl_lw", "1");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "cl_lc", "1");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "cl_dlmax", "128");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "_vgui_menus", "0");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "ah", "1");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "dm", "0");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "tracker", "0");
	SET_CLIENT_KEYVALUE(pPlayer->id, sInfoBuffer, "friends", "0");
	pEdict->v.flags |= FL_FAKECLIENT;
	char szRejectReason[128];
	MDLL_ClientConnect(pPlayer->pEdict, botname, "127.0.0.1", szRejectReason);
	MDLL_ClientPutInServer(pPlayer->pEdict);
	RUNPLAYERMOVE(pPlayer->pEdict, pPlayer->pEdict->v.v_angle, 0.0, 0.0, 0, 0, 0, 76);

	return pPlayer->id;
}

NATIVE(remove_bot) {
	int id = params[1];
  if(MF_IsPlayer(id, 1) && MF_IsBot(id)) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);
    SERVER_COMMAND(UTIL_VarArgs("kick \"%s\"\n", STRING(pPlayer->pEdict->v.netname)));
  }
	return 1;
}

NATIVE(control_bot) {
	int id = params[1];
  if(MF_IsPlayer(id, 1) && MF_IsBot(id)) {
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);
    cell *vInput = MF_GetAmxAddr(amx, params[2]);
	
    float vAngles[3] = {
      CellToFloat(vInput[0]),
      CellToFloat(vInput[1]),
      CellToFloat(vInput[2])
    };

    float fMove = CellToFloat(params[3]);
    float sMove = CellToFloat(params[4]);
    float uMove = CellToFloat(params[5]);
    int Buttons = params[6];
    int Impulse = params[7];
    int mSec = params[8];

    RUNPLAYERMOVE(pPlayer->pEdict, vAngles, fMove, sMove, uMove, Buttons, Impulse, mSec);
  }
	return 1;
}

AMX_NATIVE_INFO VexdUM_Bot[] = {
	{ "create_bot",   create_bot },
	{ "remove_bot",   remove_bot },
	{ "control_bot",  control_bot },
	{NULL,            NULL}
};
