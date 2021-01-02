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

#include "CFile.h"
#include <ctype.h>

// *****************************************************
// class File
// *****************************************************
File::File(const char* n, const char* m) {
  fp = fopen(n, m);
}

File::~File() {
  if(fp) {
    fclose(fp);
  }
}

File::operator bool () const {
  return fp && !feof(fp);
}

File& operator<<(File& f, const String& n) {
  if(f) {
    fputs(n.str(), f.fp);
  }
  return f;
}

File& operator<<(File& f, const char* n) {
  if(f) {
    fputs(n, f.fp);
  }
  return f;
}

File& operator<<(File& f, int n) {
  if(f) {
    fprintf(f.fp, "%d", n);
  }
  return f;
}


File& operator<<(File& f, const char& c) {
  if(f) {
    fputc(c, f.fp);
  }
  return f;
}

File& operator>>(File& f, String& n) {
  if(!f) {
    return f;
  }
  char temp[1024];
  fscanf(f.fp, "%s", temp);
  n.set(temp);
  return f;
}

File& operator>>(File& f, char* n) {
  if(f) {
    fscanf(f.fp, "%s", n);
  }
  return f;
}

int File::getline(char* buf, int sz) {
  int a = sz;
  char *origBuf = buf;
  
  if(*this) {
    int c;
    
    // strip UTF8 BOM
    if(ftell((*this).fp) == SEEK_SET) {
      c = getc((*this).fp);
      if(c != EOF && c == 239 ) { // 0xEF
        c = getc((*this).fp);
        if(c != EOF && c == 187 ) { // 0xBB
          c = getc((*this).fp);
          if(c == EOF || c != 191 ) { // 0xBF
            rewind((*this).fp);
          }
        }
        else {
          rewind((*this).fp);
        }
      }
      else {
        rewind((*this).fp);
      }
    }
  
    while(sz-- && (c = getc((*this).fp)) && c != EOF && c != '\n') {
      *buf++ = c;
    }      
    *buf = 0;
  }
  
  while(buf != origBuf) {
		if(*buf == 0x0a || *buf == 0x0d) {
			*buf = 0;
    }
		--buf;
	}
	
  return a - sz;
}

File& File::skipWs() {
  if(!*this) {
    return *this;
  }
  int c;
  while(isspace(c = getc(fp))) {/* nothing */};
  ungetc(c, fp);
  return *this;
}

void File::restart() {
	if(fp) {
		rewind(fp);
	}
}
