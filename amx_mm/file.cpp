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

// header file for unlink()
#ifdef LINUX
  #include <unistd.h>
#else
  #include <io.h>
#endif

#ifdef __GNUC__

//#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#endif

NATIVE(read_dir) {
#ifdef __GNUC__
  int a;
  struct dirent *ep;
  DIR *dp;
  char* dirname = build_pathname("%s", get_amxstring(amx, params[1], 0, a));
  create_path(dirname);
  a = params[2];
  if((dp = opendir(dirname)) == NULL) {
    return 0;
  }
  seekdir(dp, a);
  if((ep  = readdir(dp)) != NULL) {
    cell *length = get_amxaddr(amx, params[5]);
    *length = set_amxstring(amx, params[3], ep->d_name, params[4]);
    a = telldir(dp);
  }
  else {
    a = 0;
  }
  closedir(dp);
  return a;
#else
  return 0;
#endif
}

/* 5 param */
NATIVE(read_file) {
  int iLen;
  char* szFile = get_amxstring(amx, params[1], 0, iLen);
  FILE* fp;
  if((fp = fopen(build_pathname("%s", szFile), "r")) == NULL) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Couldn't read file \"%s\"", szFile);
    return 0;
  }
  char buffor[1024];
  int i = 0, iLine = params[2];
  if(iLine == 0) {
    // strip UTF8 BOM
    int c = getc(fp);
    if(c != EOF && c == 239) { // 0xEF
      c = getc(fp);
      if(c != EOF && c == 187) { // 0xBB
        c = getc(fp);
        if(c == EOF || c != 191) { // 0xBF
          rewind(fp);
        }
      }
      else {
        rewind(fp);
      }
    }
    else {
      rewind(fp);
    }
  }
  while((i <= iLine) && fgets(buffor, 1023, fp)) {
    ++i;
  }
  fclose(fp);
  if(i > iLine) {
    int len = strlen(buffor);
    if(buffor[len - 1] == '\n') {
      buffor[--len] = 0;
    }
    if(buffor[len - 1] == '\r') {
      buffor[--len] = 0;
    }
    cell *length = get_amxaddr(amx, params[5]);
    *length = set_amxstring(amx, params[3], buffor, params[4]);
    return i;
  }
  return 0;
}

/* 3 param */
NATIVE(write_file) {
  int i;
  char* sFile = build_pathname("%s", get_amxstring(amx, params[1], 0, i));
  create_path(sFile);
  char* sText = get_amxstring(amx, params[2], 0, i);
  FILE* pFile;
  int iLine = params[3];

  // apending to the end
  if(iLine < 0) {
    if((pFile = fopen(sFile, "a")) == NULL) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Couldn't write file \"%s\"", sFile);
      return 0;
    }
    fputs(sText, pFile);
    fputc('\n', pFile);
    fclose(pFile);
    return 1;
  }

  // creating a new file with a line in a middle
  if((pFile = fopen(sFile, "r")) == NULL) {
    if((pFile = fopen(sFile, "w")) == NULL) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Couldn't write file \"%s\"", sFile);
      return 0;
    }
    for(i = 0; i < iLine; ++i) {
      fputc('\n',pFile);
    }
    fputs(sText, pFile);
    fputc('\n', pFile);
    fclose(pFile);
    return 1;
  }

  // adding a new line in a middle of already existing file
  FILE* pTemp;
  
  bool bAmxTempFile = false;
  if((pTemp = tmpfile()) == NULL) {
    if((pTemp = fopen(build_pathname("amx_tempfile.txt"), "w+")) == NULL) {
      fclose(pFile);
      amx_logerror(amx, AMX_ERR_NATIVE, "Couldn't create temp file");
      return 0;
    }
    bAmxTempFile = true;
  }
  
  char buffor[2048];

  for(i = 0; ; ++i) {
    if(i == iLine) {
      fgets(buffor, 2047, pFile);
      fputs(sText, pTemp);
      fputc('\n', pTemp);
    }
    else if(fgets(buffor, 2047, pFile)) {
      fputs(buffor, pTemp);
    }
    else if(i < iLine) {
      fputc('\n', pTemp);
    }
    else {
      break;
    }
  }

  fclose(pFile);
  rewind(pTemp);
  
  // now rewrite because file can be now smaller...
  if(bAmxTempFile == true) {
    sFile = build_pathname("%s", get_amxstring(amx, params[1], 0, i));
  }
  if((pFile = fopen(sFile, "w")) == NULL) {
    fclose(pTemp);
    amx_logerror(amx, AMX_ERR_NATIVE, "Couldn't write file \"%s\"", sFile);
    if(bAmxTempFile == true) {
      unlink(build_pathname("amx_tempfile.txt"));
    }
    return 0;
  }

  while(fgets(buffor, 2047, pTemp)) {
    fputs(buffor, pFile);
  }
  
  fclose(pTemp);
  fclose(pFile);
  
  if(bAmxTempFile == true) {
    unlink(build_pathname("amx_tempfile.txt"));
  }
  
  return 1;
}

/* 1 param */
NATIVE(delete_file) {
  int iLen;
  char* sFile = get_amxstring(amx, params[1], 0, iLen);
  return (unlink(build_pathname("%s", sFile) ) ? 0 : 1);
}

/* 1 param */
NATIVE(file_exists) {
  int iLen;
  char* sFile = get_amxstring(amx, params[1], 0, iLen);
  FILE* fp = fopen(build_pathname("%s", sFile), "r");
  if(fp != NULL) {
    fclose(fp);
    return 1;
  }
  return 0;
}

// By AMXMODX Dev Team
/* 1 param */
NATIVE(dir_exists) {
	int iLen;
	char* sDir = get_amxstring(amx, params[1], 0, iLen);
	char* dir = build_pathname("%s", sDir);
#ifndef LINUX
	DWORD attr = GetFileAttributes(dir);
	if(attr == INVALID_FILE_ATTRIBUTES) return 0;
	if(attr == FILE_ATTRIBUTE_DIRECTORY) return 1;
#else
	struct stat s;
	if(stat(dir, &s) != 0) return 0;
	if(S_ISDIR(s.st_mode)) return 1;
#endif
	return 0;
}

// By AMXMODX Dev Team
/* 1 param */
NATIVE(amx_mkdir) {
	int iLen;
	char* sDir = get_amxstring(amx, params[1], 0, iLen);
	char* dir = build_pathname("%s", sDir);

#ifdef LINUX
	return mkdir(dir, 0700);
#else
	return mkdir(dir);
#endif
}

// By AMXMODX Dev Team
/* 2 param */
/*NATIVE(amx_rename) {
	int iLen;
	char *fold = get_amxstring(amx, params[1], 0, iLen);
	char *fnew = get_amxstring(amx, params[2], 1, iLen);

#ifdef LINUX
	return (rename(fold, fnew) == 0);
#else
	return MoveFileA(fold, fnew);
#endif
}*/

/* 1 param */
NATIVE(file_size) {
  int iLen;
  char* sFile = get_amxstring(amx, params[1], 0, iLen);
  FILE* fp = fopen(build_pathname("%s", sFile), "r");
  if(fp != NULL) {
    if(params[0] < 2 || params[2] == 0) {
      fseek(fp, 0, SEEK_END);
      int size = ftell(fp);
      fclose(fp);
      return size;
    }
    else if(params[2] == 1) {
      int a = 0,lines = 0;
      while(a != EOF) {
        ++lines;
        while((a = fgetc(fp)) != '\n' && a != EOF) {
          ;
        }
      }
      //int a, b = '\n';
      //while( (a = fgetc(fp)) != EOF ){
      //  if ( a == '\n')
      //    ++lines;
      //  b = a;
      //}
      //if ( b != '\n' )
      //  ++lines;
      return lines;
    }
    else if(params[2] == 2) {
      fseek(fp, -1, SEEK_END);
      if(fgetc(fp) == '\n') {
        return 1;
      }
      return 0;
    }
  }
  return -1;
}

AMX_NATIVE_INFO file_Natives[] = {
  { "delete_file",    delete_file },
  { "dir_exists",     dir_exists },
  { "file_exists",    file_exists },
  { "file_size",      file_size },
  { "mkdir",          amx_mkdir },
  { "read_dir",       read_dir },
  { "read_file",      read_file },
  { "write_file",     write_file },
  //{ "rename_file",	 	amx_rename},
  { NULL, NULL }
};
