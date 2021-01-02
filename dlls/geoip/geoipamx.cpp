/*
 * Copyright (c) 2002-2011 - Aleksander Naszko, Julien Laurent
 *
 *    This file is part of AMX Mod.
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
 */

#include <stdio.h>
#include "geoipamx.h"

GeoIP *GI = NULL;

inline void removeport(char *ip)
{
  char *tmp = strchr(ip, ':');
  if(!tmp) {
    return;
  }
  *tmp = '\0';
}


NATIVE(geoip_code2_cpp) {
  if(GI) {
    int len;
    char *ip = MF_GetAmxString(amx,params[1],0,len);
    removeport(ip);
    const char *code = GeoIP_country_code_by_addr(GI,ip);
    if(code != NULL) {
      MF_SetAmxString(amx,params[2],code,2);
      return 1;
    }
  }
  MF_SetAmxString(amx,params[2],"",1);
  return 0;
}

NATIVE(geoip_code3_cpp) {
  if(GI) {
    int len;
    char *ip = MF_GetAmxString(amx,params[1],0,len);
    removeport(ip);
    const char *code = GeoIP_country_code3_by_addr(GI,ip);
    if(code != NULL) {
      MF_SetAmxString(amx,params[2],code,3);
      return 1;
    }
  }
  MF_SetAmxString(amx,params[2],"",1);
  return 0;
}

NATIVE(geoip_country_cpp) {
  if(GI) {
    int len;
    char *ip = MF_GetAmxString(amx,params[1],0,len);
    removeport(ip);
    const char *code = GeoIP_country_name_by_addr(GI,ip);
    if(code != NULL) {
      MF_SetAmxString(amx,params[2],code,44);
      return 1;
    }
  }
  MF_SetAmxString(amx,params[2],"",1);
  return 0;
}

AMX_NATIVE_INFO geoip_natives[] = {
  {"geoip_code2",   geoip_code2_cpp},
  {"geoip_code3",   geoip_code3_cpp},
  {"geoip_country", geoip_country_cpp},
  { NULL,           NULL }
};

void AMXAttach() {
  const char* path = MF_BuildPathname("%s/GeoIP.dat", MF_GetLocalInfo("amx_modulesdir","$basedir/modules"));

  GI = GeoIP_open(path, GEOIP_MEMORY_CACHE);
  if(GI == NULL) {
    MF_LogError("[GeoIP] Error: could not open file '%s'\n", path);
    return;
  }
  MF_PrintSrvConsole("[GeoIP] Successfully loaded database : %s\n", GeoIP_database_info(GI));
  MF_AddNatives(geoip_natives);
}

void AMXDetach() {
  if(GI != NULL) {
    GeoIP_delete(GI);
    GI = NULL;
  }
}
