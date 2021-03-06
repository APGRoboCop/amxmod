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

#ifndef vexdum_defs_h
#define vexdum_defs_h

#define SPEAK_NORMAL          0
#define SPEAK_MUTED           1
#define SPEAK_ALL             2
#define SPEAK_LISTENALL       4

#define BLOCK_NOT             0
#define BLOCK_ONCE            1
#define BLOCK_SET             2

#define MAX_MODELLED          14

#define ANGLE_VECTORS         (*g_engfuncs.pfnAngleVectors)
#define SET_CLIENT_LISTEN     (*g_engfuncs.pfnVoice_SetClientListening)
#define CREATE_FAKE_CLIENT    (*g_engfuncs.pfnCreateFakeClient)
#define RUNPLAYERMOVE         (*g_engfuncs.pfnRunPlayerMove)
#define GETPLAYERAUTHID       (*g_engfuncs.pfnGetPlayerAuthId)
#define GETCLIENTKEYVALUE			(*g_engfuncs.pfnInfoKeyValue)

#define GET_PLAYER_POINTER(e)	(&players[ENTINDEX(e)])
#define GET_PLAYER_POINTER_I(i)	(&players[i])

#define CHECK_BOT(n, rtn) \
  if(CVAR_GET_POINTER("amx_debug")->value) { \
    if(MF_IsPlayerBot(n) == 0) { \
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "%d is not a bot", n); \
      return rtn; \
    } \
	}

#endif // vexdum_defs_h
