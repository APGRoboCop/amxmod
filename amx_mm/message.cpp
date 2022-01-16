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

enginefuncs_t *g_pEngTable = nullptr;

/* 2 param */
NATIVE(register_message) {
  int msg = params[1];

  if (msg < 1 || msg >= MAX_REG_MSGS) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid msg type %d", msg);
    return -1;
  }

  int len, iFunction;
  char* sTemp = get_amxstring(amx, params[2], 0, len);

  if(amx_FindPublic(amx, sTemp, &iFunction) != AMX_ERR_NONE) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", sTemp);
    return -1;
  }

  CPluginMngr::CPlugin* plugin = g_plugins.findPluginFast(amx);
  EventsMngr::ClMsgHook* a = g_events.registerMessageHook(plugin, iFunction, msg);

  if(a == nullptr) return -1;
  
  return iFunction;
}

/* 2 param */
NATIVE(unregister_message) {
  int msg = params[1];

  if (msg < 1 || msg >= MAX_REG_MSGS) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid msg type %d", msg);
    return 0;
  }

  int iFunction = params[2];
  if (iFunction < 0) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid function handle");
    return 0;
  }
  CPluginMngr::CPlugin* plugin = g_plugins.findPluginFast(amx);
  
  return g_events.unregisterMessageHook(plugin, iFunction, msg);
}

/* 2 param */
NATIVE(set_msg_arg_int) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  int argpos = params[1];
  if(g_events.setArg(params[1], params[2]) == 0) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid msg arg pos %d", argpos);
    return 0;
  }
  return 1;
}

/* 2 param */
NATIVE(set_msg_arg_string) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  int argpos = params[1];
  int ilen;
  if(g_events.setArg(params[1], get_amxstring(amx, params[2], 0, ilen)) == 0) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid msg arg pos %d", argpos);
    return 0;
  }
  return 1;
}

/* 2 param */
NATIVE(set_msg_arg_float) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  int argpos = params[1];
  if(g_events.setArg(params[1], CellToFloat(params[2])) == 0) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid msg arg %d", argpos);
    return 0;
  }
  return 1;
}

/* 1 param */
NATIVE(get_msg_arg_int) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  return g_events.getArgInteger(params[1]);
}

/* 3 param */
NATIVE(get_msg_arg_string) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  return set_amxstring(amx, params[2], g_events.getArgString(params[1]), *get_amxaddr(amx, params[3]));
}

/* 1 param */
NATIVE(get_msg_arg_float) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  return FloatToCell(g_events.getArgFloat(params[1]));
}

/* 1 param */
NATIVE(get_msg_arg_type) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  return g_events.getArgType(params[1]);
}

NATIVE(get_msg_args) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  return g_events.getArgNum();
}

/* 2 param */
NATIVE(get_msg_origin) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  cell *vRet = get_amxaddr(amx, params[1]);
  float* origin = g_events.getOrigin(); 
  vRet[0] = FloatToCell(origin[0]);
  vRet[1] = FloatToCell(origin[1]);
  vRet[2] = FloatToCell(origin[2]);
  return 1;
}

/* 1 param */
NATIVE(set_msg_origin) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  cell *vInput = get_amxaddr(amx, params[1]);
  float vVal[3] = {
    CellToFloat(vInput[0]),
    CellToFloat(vInput[1]),
    CellToFloat(vInput[2])
  };
  return g_events.setOrigin(vVal);
}

/* 2 param */
NATIVE(set_msg_entity) {
  if(!g_events.isInMessageHook()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of a message hook");
    return 0;
  }

  int ent = params[1];
  if(is_entity(ent, amx)) {
    return g_events.setEntity(ent);
  }
  return 0;
}

/* 4 param */
NATIVE(message_begin) {
  int numparam = *params/sizeof(cell);
  Vector vecOrigin;
  cell *cpOrigin;

  switch(params[1]) {
    case MSG_BROADCAST: case MSG_ALL: case MSG_SPEC: case MSG_INIT:
      MESSAGE_BEGIN(params[1], params[2]);
      break;
    case MSG_PVS: case MSG_PAS: case MSG_PVS_R: case MSG_PAS_R:
      if(numparam < 3) {
        amx_logerror(amx, AMX_ERR_NATIVE, "Invalid number of parameters");
        return 0;
      }
      cpOrigin = get_amxaddr(amx, params[3]);
      vecOrigin.x = *cpOrigin;
      vecOrigin.y = *(cpOrigin + 1);
      vecOrigin.z = *(cpOrigin + 2);
      MESSAGE_BEGIN(params[1], params[2], vecOrigin);
      break;
    case MSG_ONE: case MSG_ONE_UNRELIABLE:
      if(numparam < 4) {
        amx_logerror(amx, AMX_ERR_NATIVE, "Invalid number of parameters");
        return 0;
      }
      MESSAGE_BEGIN(params[1], params[2], nullptr, INDEXENT(params[4]));
      break;
  }
  return 1;
}

NATIVE(message_end) {
  MESSAGE_END();
  return 1;
}

/* 1 param */
NATIVE(write_byte) {
  WRITE_BYTE(params[1]);
  return 1;
}

/* 1 param */
NATIVE(write_char) {
  WRITE_CHAR(params[1]);
  return 1;
}

/* 1 param */
NATIVE(write_short) {
  WRITE_SHORT(params[1]);
  return 1;
}

/* 1 param */
NATIVE(write_long) {
  WRITE_LONG(params[1]);
  return 1;
}

/* 1 param */
NATIVE(write_entity) {
  WRITE_ENTITY(params[1]);
  return 1;
}

/* 1 param */
NATIVE(write_angle) {
  WRITE_ANGLE(params[1]);
  return 1;
}

/* 1 param */
NATIVE(write_coord) {
  WRITE_COORD(params[1]);
  return 1;
}

/* 1 param */
NATIVE(write_string) {
  int a;
  WRITE_STRING(get_amxstring(amx, params[1], 3, a));
  return 1;
}

/* 4 param */
NATIVE(emessage_begin) {
  int numparam = *params/sizeof(cell);
  Vector vecOrigin;
  cell *cpOrigin;

  switch(params[1]) {
    case MSG_BROADCAST: case MSG_ALL: case MSG_SPEC: case MSG_INIT:
      g_pEngTable->pfnMessageBegin(params[1], params[2], nullptr, nullptr); // By AMXMODX Dev Team
      break;
    case MSG_PVS: case MSG_PAS: case MSG_PVS_R: case MSG_PAS_R:
      if(numparam < 3) {
        amx_logerror(amx, AMX_ERR_NATIVE, "Invalid number of parameters");
        return 0;
      }
      cpOrigin = get_amxaddr(amx, params[3]);
      vecOrigin.x = *cpOrigin;
      vecOrigin.y = *(cpOrigin + 1);
      vecOrigin.z = *(cpOrigin + 2);
      g_pEngTable->pfnMessageBegin(params[1], params[2], vecOrigin, nullptr); // By AMXMODX Dev Team
      break;
    case MSG_ONE: case MSG_ONE_UNRELIABLE:
      if(numparam < 4) {
        amx_logerror(amx, AMX_ERR_NATIVE, "Invalid number of parameters");
        return 0;
      }
      g_pEngTable->pfnMessageBegin(params[1], params[2], nullptr, INDEXENT(params[4])); // By AMXMODX Dev Team
      break;
  }
  return 1;
}

NATIVE(emessage_end) {
  g_pEngTable->pfnMessageEnd(); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(ewrite_byte) {
  g_pEngTable->pfnWriteByte(params[1]); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(ewrite_char) {
  g_pEngTable->pfnWriteChar(params[1]); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(ewrite_short) {
  g_pEngTable->pfnWriteShort(params[1]); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(ewrite_long) {
  g_pEngTable->pfnWriteLong(params[1]); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(ewrite_entity) {
  g_pEngTable->pfnWriteEntity(params[1]); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(ewrite_angle) {
  g_pEngTable->pfnWriteAngle(params[1]); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(ewrite_coord) {
  g_pEngTable->pfnWriteCoord(params[1]); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(ewrite_string) {
  int a;
  g_pEngTable->pfnWriteString(get_amxstring(amx, params[1], 3, a)); // By AMXMODX Dev Team
  return 1;
}

/* 1 param */
NATIVE(get_msg_block) {
  int msg = params[1];

  if(msg < 1 || msg >= MAX_REG_MSGS) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid msg type %d", msg);
    return 0;
  }

  return g_events.iMessageBlock[msg];
}

/* 2 param */
NATIVE(set_msg_block) {
  int msg = params[1];

  if(msg < 1 || msg >= MAX_REG_MSGS) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid msg type %d", msg);
    return 0;
  }

  int iMessageFlags = params[2];
  g_events.iMessageBlock[msg] = iMessageFlags;

  return 1;
}

AMX_NATIVE_INFO message_Natives[] = {
  { "register_message",       register_message },
  { "unregister_message",     unregister_message },
  { "set_msg_arg_int",        set_msg_arg_int },
  { "set_msg_arg_float",      set_msg_arg_float },
  { "set_msg_arg_string",     set_msg_arg_string },
  { "get_msg_arg_int",        get_msg_arg_int },
  { "get_msg_arg_float",      get_msg_arg_float },
  { "get_msg_arg_string",     get_msg_arg_string },
  { "get_msg_arg_type",       get_msg_arg_type },
  { "get_msg_args",           get_msg_args },
  { "get_msg_origin",         get_msg_origin },
  { "set_msg_origin",         set_msg_origin },
  { "set_msg_entity",         set_msg_entity },
  { "message_begin",          message_begin },
  { "message_end",            message_end },
  { "write_angle",            write_angle },
  { "write_byte",             write_byte },
  { "write_char",             write_char },
  { "write_coord",            write_coord },
  { "write_entity",           write_entity },
  { "write_long",             write_long },
  { "write_short",            write_short },
  { "write_string",           write_string },
  { "emessage_begin",         emessage_begin },
  { "emessage_end",           emessage_end },
  { "ewrite_angle",           ewrite_angle },
  { "ewrite_byte",            ewrite_byte },
  { "ewrite_char",            ewrite_char },
  { "ewrite_coord",           ewrite_coord },
  { "ewrite_entity",          ewrite_entity },
  { "ewrite_long",            ewrite_long },
  { "ewrite_short",           ewrite_short },
  { "ewrite_string",          ewrite_string },
  { "set_msg_block",          set_msg_block },
  { "get_msg_block",          get_msg_block },
  {nullptr, nullptr}
};
