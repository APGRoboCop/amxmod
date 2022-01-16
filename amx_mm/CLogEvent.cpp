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
#include "CLogEvent.h"

// *****************************************************
// class LogEventsMngr
// *****************************************************
LogEventsMngr::LogEventsMngr() {
  logCurrent = logCounter = 0;
  logcmplist = nullptr;
  arelogevents = false;
  memset(logevents, 0, sizeof(logevents));
}

LogEventsMngr::~LogEventsMngr() {
  clearLogEvents();
}

int LogEventsMngr::CLogCmp::compareCondition(const char* string) {
  if(logid == parent->logCounter) {
    return result;
  }
  logid = parent->logCounter;
  if(in) {
    return result = strstr(string, text.str()) ? 0 : 1;
  }
  return result = strcmp(string,text.str());
}

LogEventsMngr::CLogCmp* LogEventsMngr::registerCondition(char* filter) {
  char* temp = filter;
  //expand "1=message"
  while(isdigit(*filter)) {
    ++filter;
	}
  bool in = (*filter=='&');
  *filter++ = 0;
  int pos = atoi(temp);

	if(pos < 0 || pos >= MAX_LOGARGS) {
    pos = 0;
  }
	CLogCmp* c = logcmplist;

	while(c) {
		if((c->pos == pos) && (c->in == in) && !strcmp(c->text.str(), filter)) {
			return c;
		}
		c = c->next;
	}
	return logcmplist = new CLogCmp(filter, in, pos, logcmplist, this);
}

void LogEventsMngr::CLogEvent::registerFilter(char* filter) {
	CLogCmp *cmp = parent->registerCondition( filter );
	if(cmp == nullptr) {
    return;
  }
	for(LogCond* c = filters; c; c = c->next) {
		if(c->argnum == cmp->pos) {
			c->list = new LogCondEle(cmp, c->list);
			return;
		}
	}
	LogCondEle* aa = new LogCondEle(cmp, nullptr);
	if(aa == nullptr) {
    return;
  }
	filters = new LogCond(cmp->pos, aa, filters);
}

void LogEventsMngr::setLogString(char* frmt, va_list& vaptr) {
	++logCounter;
	int len = vsnprintf(logString, 255, frmt, vaptr);
	if(len == - 1) {
		len = 255;
		logString[len] = 0;
	}
	if(len) {
    logString[--len] = 0;
  }
	logArgc = 0;
}

void LogEventsMngr::setLogString(char* frmt, ...) {
	++logCounter;
	va_list logArgPtr;
	va_start(logArgPtr, frmt);
	int len = vsnprintf(logString, 255, frmt, logArgPtr);
	if(len == -1) {
		len = 255;
		logString[len] = 0;
	}
	va_end(logArgPtr);
	if(len) {
    logString[--len] = 0;
  }
	logArgc = 0;
}

void LogEventsMngr::parseLogString() {
  register const char* b = logString;
  register int a;
  while(*b && logArgc < MAX_LOGARGS) {
    a = 0;
    if(*b == '"') {
      ++b;
      while(*b && *b != '"' && a < 127) {
        logArgs[logArgc][a++] = *b++;
		  }
      logArgs[logArgc++][a] = 0;
      if(*b) {
        b+=2; // thanks to double terminator
      }
    }
    else if(*b == '(') {
      ++b;
      while(*b && *b != ')' && a < 127) {
        logArgs[logArgc][a++] = *b++;
		  }
      logArgs[logArgc++][a] = 0;
      if(*b) {
        b+=2;
      }
    }
    else {
      while(*b && *b != '(' && *b != '"' && a < 127) {
        logArgs[logArgc][a++] = *b++;
		  }
      if(*b) {
        --a;
      }
      logArgs[logArgc++][a] = 0;
    }
  }
}

LogEventsMngr::CLogEvent* LogEventsMngr::registerLogEvent(CPluginMngr::CPlugin* plugin, int func, int pos) {
  if(pos < 1 || pos > MAX_LOGARGS) {
    return nullptr;
  }
  arelogevents = true;
  CLogEvent** d = &logevents[pos];
  while(*d) {
    d = &(*d)->next;
  }
  return *d = new CLogEvent(plugin, func, this);
}

void LogEventsMngr::executeLogEvents() {
  int err;
  bool valid;
  for(CLogEvent* a = logevents[logArgc]; a; a = a->next) {
    valid = true;
    for(CLogEvent::LogCond* b = a->filters; b; b = b->next) {
			valid = false;
			for(CLogEvent::LogCondEle* c = b->list; c; c = c->next) {
				if(c->cmp->compareCondition(logArgs[b->argnum]) == 0) {
					valid = true;
					break;
				}
			}
			if(!valid) {
        break;
      }
		}
		if(valid) {
			if((err = amx_Exec(a->plugin->getAMX(), nullptr, a->func, 0)) != AMX_ERR_NONE) {
				amx_logerror(a->plugin->getAMX(), err, "");
			}
    }
  }
}

void LogEventsMngr::clearLogEvents() {
  logCurrent = logCounter = 0;
  arelogevents = false;
  for(int i = 0; i < MAX_LOGARGS + 1; ++i) {
    CLogEvent **a = &logevents[i];
    while(*a) {
      CLogEvent* bb = (*a)->next;
      delete *a;
      *a = bb;
    }
  }
  clearConditions();
}

void LogEventsMngr::clearConditions() {
  while (logcmplist) {
    CLogCmp* a = logcmplist->next;
    delete logcmplist;
    logcmplist = a;
  }
}

LogEventsMngr::CLogEvent::LogCond::~LogCond() {  
	while(list) {
		LogCondEle* cc = list->next;
		delete list;
		list = cc;
	}
}

LogEventsMngr::CLogEvent::~CLogEvent() {
	while(filters) {
		LogCond* cc = filters->next;
		delete filters;
		filters = cc;
	}
}

LogEventsMngr::CLogEvent *LogEventsMngr::getValidLogEvent(CLogEvent * a) {
	bool valid;
	while(a) {
		valid = true;
		for(CLogEvent::LogCond* b = a->filters; b; b = b->next) {
			valid = false;
			for(CLogEvent::LogCondEle* c = b->list; c; c = c->next) {
				if(c->cmp->compareCondition(logArgs[b->argnum]) == 0) {
					valid = true;
					break;
				}
			}
			if(!valid) {
        break;
      }
		}
		if(!valid) {
			a = a->next;
			continue;
		}
		return a;
	}
	return nullptr;
}
