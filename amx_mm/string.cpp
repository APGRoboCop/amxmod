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
#include <ctype.h>
#include "amxmod.h"
#include "format.h"

/*
#define PARAMS() (params[0]/sizeof(cell))
#define ML() (g_plugins.findPluginFast(amx)->isML())

#define CHECKPARAMS(n) do { \
    if(ML() && PARAMS()<n) \
    { \
      print_srvconsole("[AMX] Not enough parameters (this would not even compile without the Translator)\n"); \
      amx_RaiseError(amx,AMX_ERR_NATIVE,"AMX",__FUNCTION__); \
      return 0; \
    } \
  } while(0)

// Well, this is just for Nano to display the right colors ^^
#ifdef __NOTDEFINED__
""
#endif
*/

const char* stristr(const char* str, const char* substr) {
  register char *needle = (char *)substr;
  register char *prevloc = (char *)str;
  register char *haystack = (char *)str;

  while(*haystack) {
    if(tolower(*haystack) == tolower(*needle)) {
      haystack++;
      if(!*++needle) {
        return prevloc;
      }
    }
    else {
      haystack = ++prevloc;
      needle = (char *)substr;
    }
  }
  return NULL;
}

static char g_outbuf[4096];

char* format_amxstring(AMX *amx, cell *params, int parm, int &len) {
  cell *src = get_amxaddr(amx, params[parm++]);

  if(g_translator.isTranslation(src)) {
    char *s = (char*)g_plugins.findPluginFast(amx)->getDictionary()->getMessage(*src, len);
	  len = atcprintf(g_outbuf, sizeof(g_outbuf)-1, s, amx, params, &parm);
  }
  else {
    len = atcprintf(g_outbuf, sizeof(g_outbuf)-1, src, amx, params, &parm);
  }

	return g_outbuf;
}

int amxstring_len(cell* a) {
  register int c = 0;

  while(a[c]) {
    ++c;
  }
  return c;
}

cell* get_amxaddr(AMX *amx, cell amx_addr) {
  return (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
}

int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max) {
  cell* dest = (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
  cell* start = dest;
  while(max-- && *source) {
    *dest++ = (cell)*source++;
  }    
  *dest = 0;
  return dest - start;
}

char* get_amxstring(AMX *amx, cell amx_addr, int id, int& len) {
  static char buffor[4][3072];
  register cell* source = (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
  register char* dest = buffor[id];
  char* start = dest;
  
  if(g_translator.isTranslation(source)) {
    return (char*)g_plugins.findPluginFast(amx)->getDictionary()->getMessage(*source, len);
  }
  
  while((*dest++ = (char)*source++)) {
    // nothing
  }
  len = --dest - start;
  return start;
}

extern "C" size_t get_amxstring_r(AMX *amx, cell amx_addr, char *destination, int maxlen)
{
	register cell *source = (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
	register char *dest = destination;
	char *start = dest;

  if(g_translator.isTranslation(source)) {
    int len;
    char *s = (char*)g_plugins.findPluginFast(amx)->getDictionary()->getMessage(*source, len);
	  while(maxlen-- && *source) {
		  *dest++=(char)(*s++);
    }
  }
  else {
	  while(maxlen-- && *source) {
		  *dest++=(char)(*source++);
    }
  }

	*dest = '\0';

	return dest - start;
}

void copy_amxmemory(cell* dest, cell* src, int len) {
	while(len--) {
		*dest++ = *src++;
	}		
}

char* parse_arg(char** line, int& state) {
  static char arg[3072];
  char* dest = arg;
  state = 0;
  while(**line) {
    if(isspace(**line)) {
      if(state == 1) {
        break;
      }
      else if(!state) {
        (*line)++;
        continue;
      }
    }
    else if(state != 2) {
      state = 1;
    }
    if(**line == '"') {
      (*line)++;
      if(state == 2) {
        break;
      }
      state = 2;
      continue;
    }
    *dest++ = *(*line)++;
  }
  *dest = '\0';
  return arg;
}

// By AMXMODX Dev Team
bool fastcellcmp(cell *a, cell *b, cell len) {
	while(len--) {
		if(*a++ != *b++)
			return false;
	}
	return true;
}

// By AMXMODX Dev Team
/* 4 param */
NATIVE(replace) {
  cell *a = get_amxaddr(amx, params[1]);
  cell *b = get_amxaddr(amx, params[3]);
  cell *c = get_amxaddr(amx, params[4]);
  cell *aptr = a;
  int iMain = amxstring_len(a);
  int iWhat = amxstring_len(b);
  int iWith = amxstring_len(c);
  if(iWhat > iMain)
		return 0;
	if(iWhat < 1) {
    amx_logerror(amx, AMX_ERR_NATIVE, "No search string specified");
    return 0;
  }
  if(iMain - iWhat + iWith > params[2]) {
    amx_logerror(amx, AMX_ERR_NATIVE, "replace() buffer not big enough (%d>=%d)", (iMain - iWhat + iWith), params[2]);
    return 0;
  }
  cell browsed = 0;
	while(*a && (browsed <= (iMain-iWhat))) {
		if(*a == *b) {
			if(fastcellcmp(a, b, iWhat)) {
				cell *saveptr = a + iWhat;
				cell restlen = iMain - (browsed + iWhat);
				aptr = a + iWith;
				memmove(aptr, saveptr, (restlen + 1) * sizeof(cell));
				memcpy(a, c, iWith * sizeof(cell));
				return (iMain - iWhat + iWith);
			}
		}
		a++;
		browsed++;
	}
  return 0;
}

/* 2 param */
NATIVE(contain) {
  register cell *a = get_amxaddr(amx, params[2]);
  register cell *b = get_amxaddr(amx, params[1]);
  register cell *c = b;
  cell* str = b;
  cell* substr = a;
  while(*c) {
    if(*c == *a) {
      c++;
      if(!*++a) {
        return b - str;
      }
    }
    else {
      c = ++b;
      a = substr;
    }
  }
  return -1;
}

/* 2 param */
NATIVE(containi) {
  register cell *a = get_amxaddr(amx, params[2]);
  register cell *b = get_amxaddr(amx, params[1]);
  register cell *c = b;
  cell* str = b;
  cell* substr = a;
  while(*c) {
    if(tolower(*c) == tolower(*a)) {
      c++;
      if(!*++a) {
        return b - str;
      }
    }
    else {
      c = ++b;
      a = substr;
    }
  }
  return -1;
}

/* 1 param */
NATIVE(strtonum) {
  int iLen;
  return atoi(get_amxstring(amx, params[1], 0, iLen));
}

/* 3 param */
NATIVE(numtostr) {
  char szTemp[32];
  sprintf(szTemp, "%d", (int)params[1]);
  return set_amxstring(amx, params[2], szTemp, params[3]);
}

/* 4 param */
NATIVE(add) {
  cell *dest = get_amxaddr(amx, params[1]);
  cell *start = dest;
  int c = params[2];
  int d = params[4];

  if(!g_translator.isTranslation(amx, params[3])) {
    cell *src = get_amxaddr(amx, params[3]);
    while(*dest && c--) {
      ++dest;
    }
    if(d) {
      while(c-- && d-- && *src) {
        *dest++ = *src++;
      }      
      *dest = 0;
      return (dest - start);
    }
    while(c-- && *src) {
      *dest++=*src++;
    }      
    *dest = 0;
    return (dest - start);
  }
  else {
    g_translator.setDest(-3);
    int i;
    char *src = get_amxstring(amx, params[3], 0, i);
    while(*dest && c--) {
      ++dest;
    }
    if(d) {
      while(c-- && d-- && *src) {
        *dest++=(cell)*src++;
      }      
      *dest = 0;
      return (dest - start);
    }
    while(c-- && *src) {
      *dest++ = (cell)*src++;
    }      
    *dest = 0;
    return (dest - start);    
  }
}

/* 4 param */
NATIVE(copy) {
  cell *dest = get_amxaddr(amx, params[1]);
  cell *start = dest;
  int c = params[2];
  if(!g_translator.isTranslation(amx, params[3])) {
    cell *src = get_amxaddr(amx, params[3]);
    while(c-- && *src) {
      *dest++ = *src++;
    }      
    *dest = 0;
    return (dest - start);
  }
  else {
    g_translator.setDest(-4);
    int i;
    char *src = get_amxstring(amx, params[3], 0, i);
    while(c-- && *src) {
      *dest++ = (cell)*src++;
    }      
    *dest = 0;
    return (dest - start);
  }
}

/* 4 param */
NATIVE(copyc) {
  cell *src = get_amxaddr(amx, params[3]);
  cell *dest = get_amxaddr(amx, params[1]);
  cell *start = dest;
  int c = params[2];
  cell ch = params[4];
  while(c-- && *src && *src != ch) {
    *dest++ = *src++;
  }    
  *dest = 0;
  return (dest - start);
}

/* 4 param */
NATIVE(setc) {
  cell *src = get_amxaddr(amx, params[1]);
  int c = params[2];
  cell ch = params[3];
  while(c--) {
    *src++ = ch;
  }    
  return 1;
}

/* 3 param */
NATIVE(equal) {
  int c;
/*
  if(ML())
    c = PARAMS()>2 ? (int)*get_amxaddr(amx, params[3]) : 0;
  else
*/
  c = params[3];
  if(!g_translator.isTranslation(amx, params[1])) {
    cell *a = get_amxaddr(amx, params[1]);
    if(!g_translator.isTranslation(amx, params[2])) {
      cell *b = get_amxaddr(amx, params[2]);
      if(c) {
        while(--c && *a && (*a == *b)) {
          ++a, ++b;
        }
        return (*a - *b) ? 0 : 1;
      }
      int ret;
      while(!(ret = *a - *b) && *b) {
        ++a, ++b;
      }
      return ret ? 0 : 1;
    }
    else {
      int l;
      g_translator.setDest(-2);
      char *b = get_amxstring(amx, params[2], 0, l);
      if(c) {
        while(--c && *a && (*a == (cell)*b)) {
          ++a, ++b;
        }
        return (*a - (cell)*b) ? 0 : 1;
      }
      int ret;
      while(!(ret = *a - (cell)*b) && *b) {
        ++a, ++b;
      }
      return ret ? 0 : 1;
    }
  }
  else {
    int k;
    g_translator.setDest(-2);
    char *a = get_amxstring(amx, params[1], 1, k);
    if(!g_translator.isTranslation(amx, params[2])) {
      cell *b = get_amxaddr(amx, params[2]);
      if(c) {
        while(--c && *a && ((cell)*a == *b)) {
          ++a, ++b;
        }
        return ((cell)*a - *b) ? 0 : 1;
      }
      int ret;
      while(!(ret = (cell)*a - *b) && *b) {
        ++a, ++b;
      }
      return ret ? 0 : 1;
    }
    else {
      int l;
      char *b = get_amxstring(amx, params[2], 0, l);
      if(c) {
        while(--c && *a && (*a == *b)) {
          ++a, ++b;
        }
        return (*a - *b) ? 0 : 1;
      }
      int ret;
      while(!(ret = *a - *b) && *b) {
        ++a, ++b;
      }
      return ret ? 0 : 1;
    }
  }
}

/* 3 param */
NATIVE(equali) {
  int f,l,i;
  int c;
/*
  if(ML())
    c = PARAMS()>2 ? (int)*get_amxaddr(amx, params[3]) : 0;
  else
*/
  c = params[3];
  if(!g_translator.isTranslation(amx, params[1])) {
    cell *a = get_amxaddr(amx, params[1]);
    if(!g_translator.isTranslation(amx, params[2])) {
      cell *b = get_amxaddr(amx, params[2]);
      if(c) {
        do {
          f = tolower(*a++);
          l = tolower(*b++);
        }
        while(--c && l && f && f == l);
        return(f - l) ? 0 : 1;
      }
      do {
        f = tolower(*a++);
        l = tolower(*b++);
      }
      while(f && f == l);
      return (f - l) ? 0 : 1;
    }
    else {
      g_translator.setDest(-5);
      char *b = get_amxstring(amx, params[2], 0, i);
      if(c) {
        do {
          f = tolower(*a++);
          l = tolower(*b++);
        }
        while(--c && l && f && f == l);
        return(f - l) ? 0 : 1;
      }
      do {
        f = tolower(*a++);
        l = tolower(*b++);
      }
      while(f && f == l);
      return (f - l) ? 0 : 1;  
    }
  }
  else {
    g_translator.setDest(-5);
    char *a = get_amxstring(amx, params[1], 0, i);
    if(!g_translator.isTranslation(amx, params[2])) {
      cell *b = get_amxaddr(amx, params[2]);
      if(c) {
        do{
          f = tolower(*a++);
          l = tolower(*b++);
        }
        while(--c && l && f && f == l);
        return(f - l) ? 0 : 1;
      }
      do {
        f = tolower(*a++);
        l = tolower(*b++);
      }
      while(f && f == l);
      return (f - l) ? 0 : 1;
    }
    else {
      char *b = get_amxstring(amx, params[2], 0, i);
      if(c) {
        do {
          f = tolower(*a++);
          l = tolower(*b++);
        }
        while(--c && l && f && f == l);
        return(f - l) ? 0 : 1;
      }
      do {
        f = tolower(*a++);
        l = tolower(*b++);
      }
      while(f && f == l);
      return (f - l) ? 0 : 1;  
    }  
  }
}

static cell g_cpbuf[4096];

// By AMXMODX Dev Team
/* 3 param */
NATIVE(formatex) {
	cell *buf = get_amxaddr(amx, params[1]);
	size_t maxlen = static_cast<size_t>(params[2]);
  cell *fmt = get_amxaddr(amx, params[3]);
	int param = 4;
	size_t total;
  if(g_translator.isTranslation(fmt)) {
    int len;
    char *s = (char*)g_plugins.findPluginFast(amx)->getDictionary()->getMessage(*fmt, len);
	  total = atcprintf(buf, maxlen, s, amx, params, &param);
  }
  else {
    total = atcprintf(buf, maxlen, fmt, amx, params, &param);
  }
	return static_cast<cell>(total);
}

// By AMXMODX Dev Team
/* 3 param */
NATIVE(format) {
  g_translator.setDest(-1);
	cell *buf = get_amxaddr(amx, params[1]);
	cell *fmt = get_amxaddr(amx, params[3]);
	size_t maxlen = params[2];
	cell addr_start = params[1];
	cell addr_end = params[1] + maxlen * sizeof(cell);
	cell max = params[0] / sizeof(cell);
	bool copy = false;
	for(cell i = 3; i <= max; ++i) {
		if(params[i] >= addr_start && params[i] <= addr_end) {
			copy = true;
			break;
		}
	}
	if(copy) {
		buf = g_cpbuf;
  }
	int param = 4;
	size_t total;
  if(g_translator.isTranslation(fmt)) {
    int len;
    char *s = (char*)g_plugins.findPluginFast(amx)->getDictionary()->getMessage(*fmt, len);
	  total = atcprintf(buf, maxlen, s, amx, params, &param);
  }
  else {
    total = atcprintf(buf, maxlen, fmt, amx, params, &param);
  }
	if(copy) {
		cell *old = get_amxaddr(amx, params[1]);
		memcpy(old, g_cpbuf, (total+1) * sizeof(cell));
	}
	return total;
}

/* 3 param */
NATIVE(parse) {
  int inum = *params/sizeof(cell), iarg = 2, c;
  char* arg, *parse = get_amxstring(amx, params[1], 0, c);
  cell *cptr;
  int state;
  while(*parse) {
    arg = parse_arg(&parse,state);
    if(state) {
      if(inum <= iarg) {
        return((iarg-2)>>1);
      }
      cptr = get_amxaddr(amx, params[iarg++]);
      c = *get_amxaddr(amx, params[iarg++]);
      while(c-- && *arg) {
        *cptr++ = (cell)*arg++;
      }        
      *cptr=0;
    }
  }

  return((iarg-2)>>1);
}

/* 1 param */
NATIVE(strtolower) {
  cell *cptr = get_amxaddr(amx, params[1]);
  cell *begin = cptr;
  while(*cptr) {
    *cptr = tolower(*cptr);
    cptr++;
  }
  return cptr - begin;
}

/* 1 param */
NATIVE(strtoupper) {
  cell *cptr = get_amxaddr(amx, params[1]);
  cell *begin = cptr;
  while(*cptr) {
    *cptr = toupper(*cptr);
    cptr++;
  }
  return cptr - begin;
}

/* 1 param */
NATIVE(amx_strlen) {
	int len;
	char *str = get_amxstring(amx, params[1], 0, len);
	return strlen(str);
}

char* format_arguments(AMX *amx, int parm, int &len) {
  unsigned char *data = amx->base + (int)((AMX_HEADER *)amx->base)->dat;
  cell value = * (cell *)(data + (int)amx->frm + (parm + 3)*sizeof(cell));
  cell *src = (cell *)(data + (int)value);
  ++parm;
  
  if(g_translator.isTranslation(src)) {
    char *s = (char*)g_plugins.findPluginFast(amx)->getDictionary()->getMessage(*src, len);
	  len = atcprintf(g_outbuf, sizeof(g_outbuf)-1, s, amx, &parm);
  }
  else {
    len = atcprintf(g_outbuf, sizeof(g_outbuf)-1, src, amx, &parm);
  }

	return g_outbuf;
}

NATIVE(format_args) {
  int len;
  int pos = params[3];
  if(pos < 0){
    amx_logerror(amx, AMX_ERR_NATIVE, "Pos has to be a positive number");
    return 0;
  }

  switch(*params / sizeof(cell)) {
    case 5: {
      int forcelang = params[5];
      if(forcelang == -1) {
        g_translator.setDest(params[4]);
      }
      else {
        if(forcelang < -1 || forcelang >= g_translator.getLangs()) {
          amx_logerror(amx, AMX_ERR_NATIVE, "Translator: forcelang parameter is incorrect");
          return 0;
        }
        int s = g_translator.getLangSupport(forcelang);
        if(!s) {
          amx_logerror(amx, AMX_ERR_NATIVE, "Translator: forcelang parameter matches a language that has no support");
          return 0;
        }
        g_translator.setDest(128 + forcelang);
      }
      break;
		}
    case 4:
      g_translator.setDest(params[4]);
      break;
    default:
      g_translator.setDest(0);
  }

  char* string = format_arguments(amx, pos ,len); // indexed from 0
  return set_amxstring(amx, params[1], string, params[2]);
}

NATIVE(is_digit) {
  return isdigit(params[1]);
}

NATIVE(is_alnum) {
  return isalnum(params[1]);
}

NATIVE(is_space) {
  return isspace(params[1]);
}

NATIVE(is_alpha) {
  return isalpha(params[1]);
}

NATIVE(trim) {
  cell *a = get_amxaddr(amx, params[1]);
  cell *aptr = a;
  
  while(*a && (*a == ' ' || *a == '\n' || *a == '\t' || *a == '\r' || *a == '\f' || *a == '\v')) {
    a++; 
  }

  int iLen = amxstring_len(a);
  if(a != aptr) {
    memmove(aptr, a, (iLen + 1) * sizeof(cell));
  }

  int i;
  for(i = iLen - 1; i >= 0; --i) {
    if(aptr[i] == ' ' || aptr[i] == '\n' || aptr[i] == '\t' || aptr[i] == '\r' || *a == '\f' || *a == '\v') {
      aptr[i] = 0;
    }
    else {
      break;
    }
  }

  return (i+1);
}

AMX_NATIVE_INFO string_Natives[] = {
  { "add",        add },
  { "contain",    contain },
  { "containi",   containi },
  { "copy",       copy },
  { "copyc",      copyc },
  { "equal",      equal },
  { "equali",     equali },
  { "format",     format },
  { "formatex",   formatex },
  { "format_args",format_args },
  { "isdigit",    is_digit },
  { "isalnum",    is_alnum },
  { "isspace",    is_space },
  { "isalpha",    is_alpha },
  { "numtostr",   numtostr },
  { "num_to_str", numtostr },
  { "parse",      parse },
  { "replace",    replace },
  { "setc",       setc },
  { "strlen",     amx_strlen },
  { "strtolower", strtolower },
  { "strtonum",   strtonum },
  { "strtoupper", strtoupper },
  { "str_to_num", strtonum },
  { "trim",       trim },

  { NULL, NULL }
};
