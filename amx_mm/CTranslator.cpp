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
#include <time.h>
#include "amxmod.h"
#include "CFile.h"

CTranslator g_translator;
CTranslator::CDictionary *g_dictionary;

/*  ishex
 *
 *  Test if character "c" is a hexadecimal digit ("0".."9" or "a".."f").
 */
int ishex(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}


/*  litchar
 *
 *  Return current literal character and increase the pointer to point
 *  just behind this literal character.
 *
 *  Note: standard "escape sequences" are suported, but the backslash may be
 *        replaced by another character; the syntax '\ddd' is supported,
 *        but ddd must be decimal!
 */
static char litchar(char **lptr) {
  char c = 0;
  const char *cptr;

  cptr = *lptr;
  if(*cptr != '^') {  /* no escape character */
        c = *cptr;
        cptr += 1;
  }
  else {
    cptr += 1;
    if(*cptr == '^') {
      c = *cptr;          /* \\ == \ (the escape character itself) */
      cptr += 1;
    }
    else {
      switch(*cptr) {
      case 'a':         /* \a == audible alarm */
        c = 7;
        cptr += 1;
        break;
      case 'b':         /* \b == backspace */
        c = 8;
        cptr += 1;
        break;
      case 'e':         /* \e == escape */
        c = 27;
        cptr += 1;
        break;
      case 'f':         /* \f == form feed */
        c = 12;
        cptr += 1;
        break;
      case 'n':         /* \n == NewLine character */
        c = 10;
        cptr += 1;
        break;
      case 'r':         /* \r == carriage return */
        c = 13;
        cptr += 1;
        break;
      case 't':         /* \t == horizontal TAB */
        c = 9;
        cptr += 1;
        break;
      case 'v':         /* \v == vertical TAB */
        c = 11;
        cptr += 1;
        break;
      case 'x':
        cptr += 1;
        c = 0;
        while(ishex(*cptr)) {
          if(isdigit(*cptr)) {
            c = (c<<4) + (*cptr - '0');
          }
          else {
            c = (c<<4) + (tolower(*cptr) - 'a' + 10);
          }
          cptr++;
        } /* while */
        if(*cptr == ';') {
          cptr++;       /* swallow a trailing ';' */
        }
        break;
      case '\'':        /* \' == ' (single quote) */
      case '"':         /* \" == " (single quote) */
      case '%':         /* \% == % (percent) */
        c = *cptr;
        cptr += 1;
        break;
      default:
        if(isdigit(*cptr)) {   /* \ddd */
          c = 0;
          while(*cptr >= '0' && *cptr <= '9') { /* decimal! */
            c = c*10 + *cptr++ - '0';
          }
          if(*cptr == ';') {
            cptr++;     /* swallow a trailing ';' */
          }
        }
        else {
          // error(27);    /* invalid character constant */
        } /* if */
      } /* switch */
    } /* if */
  } /* if */
  *lptr = (char*)cptr;
  return c;
}

bool CTranslator::CDictionary::loadDictionary() {
  int i;
  i = addFile(name.str());
  return (i ? true : false);
}


bool CTranslator::CDictionary::addFile(const char *file, int pass, bool primary) {
  File fp(build_pathname("%s/%s.txt", get_localinfo("amx_langdir","$basedir/lang"), file), "rt");
  if(!fp) {
    if(pass == 1) {
      logtranslatorerror("%s file not found (file \"%s.txt\")\n", AMX_TRANS, file);
    }
    return false;
  }
  char line[1025];
  int linecnt;
  int i = 0, j = 0, k = 0;
  int curpass = pass;
  char *b;
  char *c;
  char *d;
  char *e;
  bool common = false;
  //char *unterm = "unterminated string (file \"%s.txt\") (line %d)\n";
  //char *malf = "malformed line (file \"%s.txt\") (line %d)\n";

  do {
    fp.restart();
    linecnt = 0;
    while(fp.getline(line, 1024)) {
      ++linecnt;
      if(primary && linecnt == 1 && !common) {
        linecnt = 0;
        common = true;
        fp.restart();
        strcpy(line, "+ \"common\"");
      }
      c = line;
      while(*c && (*c == ' ' || *c == '\t')) {
        ++c;
      }
      switch(*c) {
      case '\0': // nothing interesting
      case '\r': // nothing interesting
      case '\n': // nothing interesting
      case ';' :
        break;
      case '+' : // 'include'
        while(*c && *c != '"') {
          *c++;
        }
        if(!*c) {
          if(curpass == 1) {
            logtranslatorerror("%s missing filename to be included (file \"%s.txt\") (line %d)\n", AMX_TRANS, file, linecnt);
          }
          continue;
        }
        c++;
        d = c;
        while(*c && *c != '"') {
          c++;
        }
        if(!*c) {
          if(curpass == 1) {
            logtranslatorerror("%s unterminated string (file \"%s.txt\") (line %d)\n", AMX_TRANS, file, linecnt);
          }
          continue;
        }
        *c = '\0';
        addFile(d, curpass, false);
        break;
      case '"' : // english
        e = line;
        c++;
        while(*c && *c != '"') {
          *e++ = litchar(&c);
        }
        if(!*c) {
          if(curpass == 1) {
            logtranslatorerror("%s unterminated string (file \"%s.txt\") (line %d)\n", AMX_TRANS, file, linecnt);
          }
          continue;
        }
        *e = '\0';
        if(curpass == 1) {
          addTempString(line);
        }
        else {
          k = getStringId(line);
        }
        break;
      default : // some other language
        while(*c && (*c == ' '||*c == '\t' || *c == '.')) {
          c++;
        }
        if(!*c) {
          if(curpass == 1) {
            logtranslatorerror("%s malformed line (file \"%s.txt\") (line %d)\n", AMX_TRANS, file, linecnt);
          }
          continue;
        }
        d = c;
        while(*c && *c != ' ' && *c != '\t' && *c != '"' && *c != ':') {
          c++;
        }
        if(!*c) {
          if(curpass == 1) {
            logtranslatorerror("%s malformed line (file \"%s.txt\") (line %d)\n", AMX_TRANS, file, linecnt);
          }
          continue;
        }
        *c = '\0';
        c++;
        while(*c && *c != '"') {
          *c++;
        }
        if(!*c) {
          if(curpass == 1) {
            logtranslatorerror("%s malformed line (file \"%s.txt\") (line %d)\n", AMX_TRANS, file, linecnt);
          }
          continue;
        }
        e = b = c;
        c++;
        while(*c && *c != '"') {
          *e++ = litchar(&c);
        }
        *e = '\0';
        if(!*c) {
          if(curpass == 1) {
            logtranslatorerror("%s unterminated string (file \"%s.txt\") (line %d)\n", AMX_TRANS, file, linecnt);
          }
          continue;
        }
        if(curpass == 1) {
          i = g_translator.addLang(d);
          if(curpass == 1 && i == -1) {
            logtranslatorerror("%s too many languages (lang \"%s\")\n", AMX_TRANS, d);
          }
        }
        else {
          i = g_translator.getLangId(d);
          if(i != -1) {
            CLangString *ls = &(languages[i].coll[k]);
            ls->string.set(b);
            ls->length = ls->string.size();
          }
        }
        *e = '\0';
        break;
      }
    }
    if(primary) {
      if(curpass == 1) {
        CTempString *s;
        CTempString *p;
        lCounter = g_translator.getLangs();
        lSupport = new int [lCounter];
        languages = new CLangCollection [lCounter];
        for(i = 0; i < lCounter; ++i) {
          languages[i].coll = new CLangString [tCounter];
          memset(languages[i].coll, 0, tCounter * sizeof(CLangString));
        }
        i = 0;
        for(s = tmphead; s; s = s->next) {
          languages[0].coll[i].string.set(s->string.str());
          languages[0].coll[i].length = s->string.size();
#ifdef LOOKUP_HASH
          languages[0].coll[i].hash = s->hash;
#endif
          ++i;
        }
        for(s = tmphead; s; s = p) {
          p = s->next;
          delete s;
        }
        common = false;
      }
      else {
        const char *amx_langdebug = cvar_amx_langdebug->string;
        const char* find;
        for(i = 0; i < lCounter; ++i) {
          find = strstr(amx_langdebug, g_translator.getLang(i));
          k = 0;
          for(j = 0; j < tCounter; ++j) {
            if(languages[i].coll[j].length) {
              k++;
            }
            else {
              if(find) {
                logtranslatorerror("Missing translation in '%s': \"%s\" (file \"%s\")\n",g_translator.getLang(i),languages[0].coll[j].string.str(),file);
              }
            }
          }
          lSupport[i] = k;
        }
        //g_translator.updateSupports();
      }
    }
  }
  while(primary && ++curpass <= 2);
  return true;
}

void CTranslator::updateSupports() {
  CDictionary *d;
  int i, m, n;

  m = getLangs();

  for(i = 0; i < m; ++i) {
    lSupport[i] = 0;
  }

  for(d = head; d; d = d->next) {
    n = d->getLangs();
    for(i = 0; i < n; ++i) {
      lSupport[i] += d->getLangSupport(i);
    }
  }
}

CTranslator::CDictionary * CTranslator::loadDictionary(const char *name) {
  CDictionary **d = &head;
  CDictionary *e = findDictionary(name);
  if(e) {
    return e;
  }
  e = new CDictionary(name);
  if(!e->loadDictionary()) {
    logtranslatorerror("%s Could not load specified file (file \"%s\")\n", AMX_TRANS, name);
  }
  while(*d) {
    d = &(*d)->next;
  }
  *d = e;
  ++dCounter;
  g_translator.updateSupports();
  return *d;
}

CTranslator::CDictionary * CTranslator::findDictionary(const char *name) {
  CDictionary *d = head;
  while(d) {
    if(!strcmp(name, d->name.str())) {
      return d;
    }
    d = d->next;
  }
  return NULL;
}

CTranslator::CDictionary::~CDictionary() {
  int i;
  if(languages) {
    for(i = 0; i < lCounter; ++i) {
      delete [] languages[i].coll;
      languages[i].coll = NULL;
    }
    delete [] languages;
    delete [] lSupport;
    languages = NULL;
  }
}

void CTranslator::clear() {
  CDictionary **a = &head;
  while(*a) {
    unloadDictionary(a);
  }

  int i;
  for(i = 0; i < 255; ++i) {
    if(lang[i]) {
      delete lang[i];
      lang[i] = NULL;
    }
  }
  memset(&lang, 0, sizeof(lang));
  lang[0] = new String("en");
}

void CTranslator::unloadDictionary(CDictionary **d) {
  CDictionary *next = (*d)->next;
  delete *d;
  *d = next;
  --dCounter;
}

const char * CTranslator::CDictionary::getMessage(const char *english, int destid, int &length) {
  int msg;
  int dest = destid;

  msg = getStringId(english);

  if(msg == -1) {
    return english;
  }
  if(dest < 0 || dest > gpGlobals->maxClients) {
    dest = 0;
  }
  return getMessage(dest, msg, length);
}

cell CTranslator::CDictionary::getTranslation(const char *english, int destid, int forcelang) {
  int dest;
  int msg;

  dest = destid;
  msg = getStringId(english);

  if(dest < -1 || dest > gpGlobals->maxClients) {
    dest = 0;
  }

  if(msg == -1) {
    msg = addGarbage(english);
    dest = -2;
    logtranslatorerror("%s unknown message (string \"%s\") (file \"%s\")\n", AMX_TRANS, english, name.str());
  }

  ++msg;

  cell res;

  if(forcelang == -1) {
    res = ((dest&0xFF)<<24) | ((msg&0xFFFF)<<8);
    return res;
  }

  forcelang += 128;
  res = ((forcelang&0xFF)<<24) | ((msg&0xFFFF)<<8);

  return res;
}

const char *CTranslator::CDictionary::getMessage(cell src, int &length) {
  int dest;
  int msg;

  dest = 0xFF & (src>>24);
  msg = 0xFFFF & (src>>8);
  --msg;

  return getMessage(dest, msg, length);
}

const char *CTranslator::CDictionary::getMessage(int dest, int msg, int &length) {
  int lang, fblang;
  int dest2 = dest;
  int forcelang = -1;
  char *err = "ERROR: no destination specified in a translation for";
  if(dest2 == 0xFE) {
    length = gLength[msg];
    return gString[msg].str();
  }
  else if(dest2 == 0xFF) {
    dest2 = g_translator.getDest();
    switch(dest2) {
    case -1: // format()
      logtranslatorerror("%s %s 'format' (string \"%s\") (file \"%s\")\n", AMX_TRANS, err, languages[0].coll[msg].string.str(), name.str());
      dest2 = 0;
      break;
    case -2: // equal()
      logtranslatorerror("%s %s 'equal' (string \"%s\") (file \"%s\")\n", AMX_TRANS, err, languages[0].coll[msg].string.str(), name.str());
      dest2 = 0;
      break;
    case -3: // add()
      logtranslatorerror("%s %s 'add' (string \"%s\") (file \"%s\")\n", AMX_TRANS, err, languages[0].coll[msg].string.str(), name.str());
      dest2 = 0;
      break;
    case -4: // copy()
      logtranslatorerror("%s %s 'copy' (string \"%s\") (file \"%s\")\n", AMX_TRANS, err, languages[0].coll[msg].string.str(), name.str());
      dest2 = 0;
      break;
    case -5: // equali()
      logtranslatorerror("%s %s 'equali' (string \"%s\") (file \"%s\")\n", AMX_TRANS, err, languages[0].coll[msg].string.str(), name.str());
      dest2 = 0;
      break;
    case 100: // register_plugin()
      forcelang = 0; // english
      break;
    case 101: // get_plugin()
      forcelang = 0;
      break;
    case 102: // get_*cmd() (*: cl, con, srv)
      forcelang = 0;
      break;
    }
  }

  if((dest2 - 128) >= 0) {
    forcelang = dest2 - 128;
  }

  if(forcelang == -1) {
    if(dest2) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(dest2);
      lang = pPlayer->langid;
      if(lang < 0 || lang >= lCounter || !lSupport[lang]) {
        lang = pPlayer->langid2;
        fblang = 0;
        if(lang < 0 || lang >= lCounter || !lSupport[lang]) {
          lang = 0;
        }
      }
      else {
        fblang = pPlayer->langid2;
        if(fblang < 0 || fblang >= lCounter || !lSupport[fblang]) {
          fblang = 0;
        }
      }
    }
    else {
      lang = g_translator.getServerLang();
      if(lang >= lCounter || !lSupport[lang]) {
        lang = 0;
      }
      fblang = 0;
    }
  }
  else {
    lang = forcelang;
    fblang = 0;
  }

  length = languages[lang].coll[msg].length;

  if(!length) {
    const char *amx_langdebug = cvar_amx_langdebug->string;
    const char* find = strstr(amx_langdebug, g_translator.getLang(lang));
    if(find) {
      logtranslatorerror("%s no translation for specified message (string \"%s\") (file \"%s\") (language \"%s\")\n", AMX_TRANS, languages[0].coll[msg].string.str(), name.str(), g_translator.getLang(lang));
    }
    lang = fblang;
    length = languages[lang].coll[msg].length;
    if(!length) {
      lang = 0;
      length = languages[lang].coll[msg].length;
    }
  }

  return languages[lang].coll[msg].string.str();
}

int CTranslator::addLang(const char *code) {
  int i;
  for(i = 0; lang[i] && i < 254; ++i) {
    if(!strcmp(lang[i]->str(), code)) {
      return i;
    }
  }

  if(i < 254) {
    lang[i] = new String(code);
    return i;
  }

  return -1;
}

int CTranslator::getLangId(const char *code) {
  int i;
  for(i = 0; lang[i] && i < 255; ++i) {
    if(!stricmp(lang[i]->str(), code)) {
      return i;
    }
  }
  return -1;
}

const char *CTranslator::getLang(int i) {
  if(i >= 0 && i < 255 && lang[i] && strcmp("", lang[i]->str())) {
    return lang[i]->str();
  }
  return NULL;
}

int CTranslator::getLangs() {
  int i;
  for(i = 0; lang[i] && i < 255; ++i){/* nothing */}
  return i;
}

void CTranslator::CDictionary::addTempString(const char *str) {
  CTempString *s;
  CTempString *n;
  CTempString *p = NULL;
#ifdef LOOKUP_HASH
  unsigned long h = getHash(str);
#else
  int cmp;
#endif

  if(!tmphead) {
#ifndef LOOKUP_HASH
    tmphead = new CTempString(str);
#else
    tmphead = new CTempString(str, h);
#endif
    ++tCounter;
    return;
  }

  if(tCounter >= 0xFFFF-1) {
    if(tCounter == 0xFFFF-1) {
      logtranslatorerror("%s could not add translated message (string \"%s\") (file \"%s\")\n", AMX_TRANS, str, name.str());
      ++tCounter;
    }
    return;
  }

  for(s = tmphead; s; s = s->next) {
#ifndef LOOKUP_HASH
    cmp = strcmp(str, s->string.str());
    if(!cmp) {
#else
    if(h == s->hash) {
#endif
      return;
    }
#ifndef LOOKUP_HASH
    if(cmp < 0) {
#else
    if(h < s->hash) {
#endif
#ifndef LOOKUP_HASH
      n = new CTempString(str, s);
#else
      n = new CTempString(str, h, s);
#endif
      if(tmphead == s) {
        tmphead = n;
      }
      else if(p) {
        p->next = n;
      }
      ++tCounter;
      return;
    }
    n = s;
    p = s;
  }
#ifndef LOOKUP_HASH
  s = new CTempString(str);
#else
  s = new CTempString(str, h);
#endif
  n->next = s;
  ++tCounter;
}

#if defined LOOKUP_OPTIMIZED
// Binary (dichotomic) search in (sorted) strings
int CTranslator::CDictionary::findString(const char *str, int from, int to) {
  int idx = (from + to) / 2;
  int res = strcmp(str, languages[0].coll[idx].string.str());

  if(!res) {
    return idx;
  }
  if(from != to) {
    if(res < 0) {
      return findString(str, from, idx);
    }
    return findString(str, idx+1, to);
  }

  return -1;
}

#elif defined LOOKUP_HASH
static unsigned long crc32_table[256] = {
  0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
  0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
  0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
  0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
  0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
  0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
  0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
  0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
  0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
  0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
  0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
  0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
  0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
  0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
  0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
  0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
  0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
  0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
  0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
  0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
  0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
  0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
  0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
  0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
  0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
  0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
  0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
  0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
  0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
  0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
  0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
  0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
  0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
  0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
  0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
  0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
  0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
  0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
  0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
  0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
  0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
  0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
  0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

// Hash
unsigned long CTranslator::CDictionary::getHash(const char *str) {
  unsigned long crc = 0xFFFFFFFF;
  const char *c = str;

  while(*c) {
    crc = ((crc>>8) & 0xFFFFFFFF) ^ crc32_table[(crc^*c++) & 0xFF];
  }
  return (crc & 0xFFFFFFFF);
}

// Binary (dichotomic) search in (sorted) hashes
int CTranslator::CDictionary::findHash(unsigned long hash, int from, int to) {
  int idx = (from + to) / 2;

  if(hash == languages[0].coll[idx].hash) {
    return idx;
  }

  if(from != to) {
    if(hash < languages[0].coll[idx].hash) {
      return findHash(hash, from, idx);
    }
    return findHash(hash, idx + 1, to);
  }

  return -1;
}

int CTranslator::CDictionary::findString(const char *str) {
  return findHash(getHash(str), 0, tCounter - 1);
}
#endif

int CTranslator::CDictionary::getStringId(const char *str) {
  if(!languages){
    return -1;
  }

#if defined LOOKUP_NORMAL
  int i;
  for(i = 0; i < tCounter; ++i) {
    if(!strcmp(str, languages[0].coll[i].string.str())) {
      return i;
    }
  }
  return -1;
#elif defined LOOKUP_OPTIMIZED
  return findString(str, 0, tCounter - 1);
#elif defined LOOKUP_HASH
  return findString(str);
#endif

}

void CTranslator::CDictionary::getString(const char *english, int langid, int &res1, int &res2) {
  int f = getStringId(english);

  if(f == -1) {
    logtranslatorerror("%s unknown message (string \"%s\") (file \"%s\")\n", AMX_TRANS, english, name.str());
    res1 = 0xFF;
    res2 = addGarbage(english);
  }
  else {
    if(!langid) {
      res1 = 0;
      res2 = f;
    }
    else {
      if(!languages[langid].coll[f].length) {
        logtranslatorerror("%s no translation for specified message (string \"%s\") (file \"%s\") (language \"%s\")\n", AMX_TRANS, english, name.str(), g_translator.getLang(langid));
        res1 = 0;
        res2 = f;
      }
      else {
        res1 = langid;
        res2 = f;
      }
    }
  }
}

int CTranslator::CDictionary::addGarbage(const char *str) {
  int i = gCursor;
  gString[i].set(str);
  gLength[i] = gString[i].size();
  ++gCursor;
  if(gCursor == 256) {
    gCursor = 0;
  }
  return i;
}

int CTranslator::CDictionary::getTranslations() {
  return tCounter;
}

char * TR(const char *string) {
  if(g_dictionary) {
    int l;
    return (char*)g_dictionary->getMessage(string, g_translator.getDest(), l);
  }
  return (char*)string;
}
