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

#include <extdll.h>
#include <meta_api.h>
#include "amxmod.h"
#include "CEvent.h"

// *****************************************************
// class EventsMngr
// *****************************************************
EventsMngr::EventsMngr() { 
  currentMsgType = -1;
	memset(modMsgsFunCall, 0, sizeof(modMsgsFunCall));
	memset(modMsgsHookFunCall, 0, sizeof(modMsgsHookFunCall));
	memset(iMessageBlock, BLOCK_NOT, sizeof(iMessageBlock)); 
}

EventsMngr::~EventsMngr() { 
	clearEvents();
	clearMessageHooks();
}

EventsMngr::ClEvent::ClEvent(CPluginMngr::CPlugin* amxplugin, int function, int flags) {	
	plugin = amxplugin;
	func = function;
	stamp = 0.0f;
	next = nullptr;
	done = false;
	alive = true;
	dead = true;
	ignorebots = false;
	world = (flags & 1) ? true : false; //a
	player = (flags & 2) ? true : false; //b
	once = (flags & 4) ? true : false; //c
	if(flags & 24) {
    dead = (flags & 8) ? true : false; //d
		alive = (flags & 16) ? true : false; //e
	}
	ignorebots = (flags & 32) ? true : false; //f
	memset(cond, 0, sizeof(cond));	
}

EventsMngr::ClMsgHook::ClMsgHook(CPluginMngr::CPlugin* amxplugin, int function) {	
	plugin = amxplugin;
	func = function;
}

void EventsMngr::ClEvent::registerFilter(char* filter) {
	if(filter == nullptr) {
    return;
  }
	char* value = filter;

	while(isdigit(*value)) {
		++value;
  }
	if(*value == 0) {
    return;
  }
	cond_t* b = new cond_t;

	if(b == nullptr) {
    return;
  }

	b->type = *value;

	*value++ = 0;

	b->sValue.set(value);
	b->fValue = atof(value);
	b->iValue = atoi(value);

	int i = atoi(filter);
	if(i >= 0 && i < MAX_PARSE_VALUES) {
		b->next = cond[i];
		cond[i] = b;
	}
	else {
    delete b;
  }
}

EventsMngr::ClEvent* EventsMngr::registerEvent(CPluginMngr::CPlugin* p, int f, int flags, int pos) {
	if (pos < 0 || pos >= MAX_AMX_REG_MSG)
		return nullptr;

  ClEvent* a = new ClEvent(p, f, flags);
	if(a == nullptr) {
    return nullptr;
  }
	ClEvent** end = &modMsgsFunCall[pos];
	while(*end) {
    end = &(*end)->next;
  }
	return *end = a;
}

EventsMngr::ClMsgHook* EventsMngr::registerMessageHook(CPluginMngr::CPlugin* p, int f, int pos) {
	if (pos < 1 || pos >= MAX_REG_MSGS)
		return nullptr;

  ClMsgHook* a = new ClMsgHook(p, f);
	if(a == nullptr) {
    return nullptr;
  }
	ClMsgHook** end = &modMsgsHookFunCall[pos];
	while(*end) {
    end = &(*end)->next;
  }
	return *end = a;
}

int EventsMngr::unregisterMessageHook(CPluginMngr::CPlugin* p, int f, int pos) {
	if (pos < 1 || pos >= MAX_REG_MSGS)
		return 0;

	ClMsgHook** end = &modMsgsHookFunCall[pos];
	while(*end) {
    if( (*end)->plugin == p && (*end)->func == f ) {
		  ClMsgHook* aa = (*end)->next;
		  delete *end;
		  *end = aa;
      return 1;
		}
    end = &(*end)->next;
	}

  return 0;
}

int EventsMngr::parserInit(int msg_type, float* tim, CPlayer *pPlayer, int index, int msg_dest, const float* origin) {
  if(msg_type < 0 || msg_type > MAX_AMX_REG_MSG)
		return 0;
		
  currentMsgType = msg_type;
  currentMsgDest = msg_dest;
  if(origin) {
    currentMsgOrigin[0] = origin[0];
    currentMsgOrigin[1] = origin[1];
    currentMsgOrigin[2] = origin[2];
  }
  else {
    currentMsgOrigin[0] = 0.0;
    currentMsgOrigin[1] = 0.0;
    currentMsgOrigin[2] = 0.0;
  }
  parseNotDone = false;
  inMessageHook = false;
  timer = tim;
  
  if((parseMsgHookFun = modMsgsHookFunCall[msg_type]) != nullptr) {
    inMessageHook = true;
    parseVault[parsePos = 0].type = MSG_ARG_SHORT;
    parseVault[parsePos].iValue = parseVault[parsePos].iNewValue = index;
  }

  if((parseFun = modMsgsFunCall[msg_type]) == nullptr) {
    return 0;
  }
  for(EventsMngr::ClEvent*p = parseFun; p; p = p->next) {
    if(p->done) {
      continue;
    }
    if(!p->plugin->isExecutable(p->func)) {
      p->done = true;
      continue;
    }
    if(pPlayer) {
      if(!p->player || (pPlayer->IsAlive() ? !p->alive : !p->dead) || (pPlayer->IsBot() && p->ignorebots)) {
        p->done = true;
        continue;
      }
    }
    else if(!p->world) {
      p->done = true;
      continue;
    }
    if(p->once && p->stamp == (float)(*timer)) {
      p->done = true;
      continue;
    }
    parseNotDone = true;
  }
  if(parseNotDone) {
    parseVault[parsePos = 0].type = MSG_ARG_SHORT;
    parseVault[parsePos].iValue = parseVault[parsePos].iNewValue = index;
    return 1;
  }
  return 2;
}

const char* EventsMngr::getArgString(int a) {
	if(a < 0 || a > parsePos) return "";

	static char var[32];
	switch(parseVault[a].type) {
		case MSG_ARG_BYTE:
		case MSG_ARG_CHAR:
    case MSG_ARG_SHORT:
    case MSG_ARG_LONG:
    case MSG_ARG_ENTITY:
			sprintf(var, "%d", inMessageHook ? parseVault[a].iNewValue : parseVault[a].iValue);
			return var;
		case MSG_ARG_STRING: return inMessageHook ? parseVault[a].sNewValue : parseVault[a].sValue;
		default:
			sprintf(var, "%g", inMessageHook ? parseVault[a].fNewValue : parseVault[a].fValue);
			return var;
	}
}

int EventsMngr::setArg(int a, const char* str) {
	if(a < 1 || a > parsePos) return 0;

	parseVault[a].sNewValue = (char*)STRING(ALLOC_STRING(str));
	return 1;
}

int EventsMngr::getArgInteger(int a) {
	if(a < 0 || a > parsePos) return 0;

	switch(parseVault[a].type) {
		case MSG_ARG_BYTE:
		case MSG_ARG_CHAR:
    case MSG_ARG_SHORT:
    case MSG_ARG_LONG:
    case MSG_ARG_ENTITY: 
      return inMessageHook ? parseVault[a].iNewValue : parseVault[a].iValue;
		case MSG_ARG_STRING: return atoi(inMessageHook ? parseVault[a].sNewValue : parseVault[a].sValue);
		default: return (int) (inMessageHook ? parseVault[a].fNewValue : parseVault[a].fValue); 
	}
}

int EventsMngr::setArg(int a, int value) {
	if(a < 1 || a > parsePos) return 0;

	parseVault[a].iNewValue = value;
	return 1;
}

float EventsMngr::getArgFloat(int a) {
	if(a < 0 || a > parsePos) return 0.0f;

	switch(parseVault[a].type) {
		case MSG_ARG_BYTE:
		case MSG_ARG_CHAR:
    case MSG_ARG_SHORT:
    case MSG_ARG_LONG:
    case MSG_ARG_ENTITY:
      return inMessageHook ? parseVault[a].iNewValue : parseVault[a].iValue;
		case MSG_ARG_STRING: return atof(inMessageHook ? parseVault[a].sNewValue : parseVault[a].sValue);
		default: return inMessageHook ? parseVault[a].fNewValue : parseVault[a].fValue;
	}
}

int EventsMngr::setArg(int a, float value) {
	if(a < 1 || a > parsePos) return 0;

	parseVault[a].fNewValue = value;
	return 1;
}

void EventsMngr::executeEvents() {
	int err;

	for(ClEvent* p = parseFun; p; p = p->next) {
		if(p->done) {
			p->done = false;
			continue;
		}

		p->stamp = *timer;

		if((err = amx_Exec(p->plugin->getAMX(), nullptr, p->func, 1, parseVault[0].iValue)) != AMX_ERR_NONE) {
			amx_logerror(p->plugin->getAMX(), err, "");
		}
	}
	currentMsgType = -1;
}

void EventsMngr::executeMessageHooks() {
	int err;
	cell ret = 0;

	for(ClMsgHook* p = parseMsgHookFun; p; p = p->next) {
	  if((err = amx_Exec(p->plugin->getAMX(), &ret, p->func, 3, currentMsgType, currentMsgDest, parseVault[0].iValue)) != AMX_ERR_NONE) {
      amx_logerror(p->plugin->getAMX(), err, "");
	  }
    if(ret) {
      break;
    }
	}

  if(ret == 0) {
    writeMessage();
  }

  inMessageHook = false;
}

void EventsMngr::writeMessage() {
  switch(currentMsgDest) {
    case MSG_BROADCAST: case MSG_ALL: case MSG_SPEC: case MSG_INIT:
      MESSAGE_BEGIN(currentMsgDest, currentMsgType);
      break;
    case MSG_PVS: case MSG_PAS: case MSG_PVS_R: case MSG_PAS_R:
      MESSAGE_BEGIN(currentMsgDest, currentMsgType, currentMsgOrigin);
      break;
    case MSG_ONE: case MSG_ONE_UNRELIABLE:
      MESSAGE_BEGIN(currentMsgDest, currentMsgType, nullptr, INDEXENT(parseVault[0].iNewValue));
      break;
  }

  for(int i = 1; i <= parsePos; ++i) {
    switch(parseVault[i].type) {
      case MSG_ARG_BYTE:
        WRITE_BYTE(parseVault[i].iNewValue);
        break;
  		case MSG_ARG_CHAR:
  		  WRITE_CHAR(parseVault[i].iNewValue);
        break;
      case MSG_ARG_SHORT:
        WRITE_SHORT(parseVault[i].iNewValue);
        break;
      case MSG_ARG_LONG:
        WRITE_LONG(parseVault[i].iNewValue);
        break;
      case MSG_ARG_ENTITY:
        WRITE_ENTITY(parseVault[i].iNewValue);
        break;
      case MSG_ARG_ANGLE:
        WRITE_ANGLE(parseVault[i].fNewValue);
        break;
      case MSG_ARG_COORD:
        WRITE_COORD(parseVault[i].fNewValue);
        break;
  		case MSG_ARG_STRING:
  		  WRITE_STRING(parseVault[i].sNewValue);
        break;
  		default: break;
    }
  }

  MESSAGE_END();
}

void EventsMngr::parseValue(int iValue, MsgArgType argType) {
  if(!parseNotDone && !inMessageHook) {
    return;
  }
  parseVault[++parsePos].type = argType;
  parseVault[parsePos].iValue = parseVault[parsePos].iNewValue = iValue;
  if(parseNotDone) {
    bool skip;
    for(ClEvent* p = parseFun; p; p = p->next) {
      if(p->done || !p->cond[parsePos]) {
        continue;
      }
      skip = false;
      ClEvent::cond_t* a = p->cond[parsePos];
      do {
        switch(a->type) {
        case '=':
          if(a->iValue == iValue) {
            skip = true;
          }
          break;
        case '!':
          if(a->iValue != iValue) {
            skip = true;
          }
          break;
        case '&':
          if(iValue & a->iValue) {
            skip = true;
          }
          break;
        case '<':
          if(iValue < a->iValue) {
            skip = true;
          }
          break;
        case '>':
          if(iValue > a->iValue) {
            skip = true;
          }
          break;
        }
        if(skip) {
          break;
        }
      }
      while((a = a->next));
      if(skip) {
        continue;
      }
      p->done = true;
    }
  }
}

void EventsMngr::parseValue(float flValue, MsgArgType argType) {
  if(!parseNotDone && !inMessageHook) {
    return;
  }
  parseVault[++parsePos].type = argType;
  parseVault[parsePos].fValue = parseVault[parsePos].fNewValue = flValue;
  if(parseNotDone) {
    bool skip;
    for(ClEvent* p = parseFun; p; p = p->next) {
      if(p->done || !p->cond[parsePos]) {
        continue;
      }
      skip = false;
      ClEvent::cond_t* a = p->cond[parsePos];
      do {
        switch(a->type) {
        case '=':
          if(a->fValue == flValue) {
            skip = true;
          }
          break;
        case '!':
          if(a->fValue != flValue) {
            skip = true;
          }
          break;
        case '<':
          if(flValue < a->fValue) {
            skip = true;
          }
          break;
        case '>':
          if(flValue > a->fValue) {
            skip = true;
          }
          break;
        }
        if(skip) {
          break;
        }
      }
      while((a = a->next));
      if(skip) {
        continue;
      }
      p->done = true;
    }
  }
}

void EventsMngr::parseValue(const char *sz) {
  if(!parseNotDone && !inMessageHook) {
    return;
  }
  parseVault[++parsePos].type = MSG_ARG_STRING;
  parseVault[parsePos].sValue = parseVault[parsePos].sNewValue = sz;
  if(parseNotDone) {
    bool skip;
    for(ClEvent* p = parseFun; p; p = p->next) {
      if(p->done || !p->cond[parsePos]) {
        continue;
      }
      skip = false;
      ClEvent::cond_t* a = p->cond[parsePos];
      do {
        switch(a->type) {
        case '=':
          if(!strcmp(sz, a->sValue.str())) {
            skip = true;
          }
          break;
        case '!':
          if(strcmp(sz, a->sValue.str())) {
            skip = true;
          }
          break;
        case '&':
          if(strstr(sz, a->sValue.str())) {
            skip = true;
          }
          break;
        }
        if(skip) {
          break;
        }
      }
      while((a = a->next));
      if(skip) {
        continue;
      }
      p->done = true;
    }
  }
}

void EventsMngr::clearEvents() {
	for(int i = 0; i < MAX_AMX_REG_MSG; ++i) {
		ClEvent** b = &modMsgsFunCall[i];
		while(*b) {
			ClEvent* aa = (*b)->next;
			delete *b;
			*b = aa;
		}
	}
}

void EventsMngr::clearMessageHooks() {
	for(int i = 0; i < MAX_REG_MSGS; ++i) {
		ClMsgHook** b = &modMsgsHookFunCall[i];
		while(*b) {
			ClMsgHook* aa = (*b)->next;
			delete *b;
			*b = aa;
		}
	}
}

EventsMngr::ClEvent::~ClEvent() {
	for(int a = 0; a < MAX_PARSE_VALUES; ++a) {
		cond_t** b = &cond[a];
		while(*b) {
			cond_t* nn = (*b)->next;
			delete *b;
			*b = nn;
		}
	}
}

EventsMngr::ClEvent* EventsMngr::getValidEvent(ClEvent* a ) {
	while(a) {
		if(a->done) {
			a->done = false;
			a = a->next;
			continue;
		}
		a->stamp = *timer;
		return a;
	}
	return nullptr;
}

int EventsMngr::getEventId(const char* msg) {
	struct CS_Events {
		const char* name;
		CS_EventsIds id;
	} table[] = {
		{ "CS_DeathMsg", CS_DeathMsg },
//		{ "CS_RoundEnd", CS_RoundEnd },
//		{ "CS_RoundStart", CS_RoundStart },
//		{ "CS_Restart", CS_Restart },
		{ "", CS_Null }
	};
	int pos;
	if((pos = atoi(msg)) != 0) {
		return pos;
	}
	for(pos = 0; table[pos].id != CS_Null; ++pos) {
		if(!strcmp(table[pos].name, msg)) {
			return table[pos].id;
		}
	}
	return pos = GET_USER_MSG_ID(PLID, msg, nullptr);
}

int EventsMngr::getCurrentMsgType() {
	return currentMsgType;
}

int EventsMngr::getArgType(int a) {
  if(a < 1 || a > parsePos) return (int)MSG_ARG_UNKNOWN;

	return (int)parseVault[a].type;
}

float* EventsMngr::getOrigin() {
	return currentMsgOrigin;
}

int EventsMngr::setOrigin(float origin[3]) {
	currentMsgOrigin[0] = origin[0];
	currentMsgOrigin[1] = origin[1];
	currentMsgOrigin[2] = origin[2];
  return 1;
}

int EventsMngr::setEntity(int index) {
  parseVault[0].iNewValue = index;
  return 1;
}
