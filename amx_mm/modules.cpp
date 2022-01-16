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
#ifdef LINUX
  #include <malloc.h>
  #include <stdlib.h>
  #include <sys/mman.h>
#endif
#include <ctime>
#include "amxmod.h"
#include "osdep.h"      // sleep, etc
#include "CFile.h"
#include "modules_autoload.h"
#include "optimizer.h"

CList<CModule> g_modules;
CList<CScript,AMX*> g_loadedscripts;

CModule *g_calledModule = nullptr;

#ifdef  __cplusplus
  extern "C" {
#endif
  extern const char* no_function; // stupid work around
#ifdef  __cplusplus
}
#endif

void report_error(int code, char* fmt, ...) {
  va_list argptr;
  char string[256];
  *string = 0;
  va_start (argptr, fmt);
  vsnprintf (string, 255, fmt, argptr);
  string[255] = 0;
  va_end (argptr);
  if(*string) {
    print_srvconsole(string);
    print_srvconsole("[AMX] Make sure that modules are compatible with AMX %s\n", AMX_VERSION);
    print_srvconsole("[AMX] Please fix the problem then start the server again\n");
    char date[32];
    time_t td; time(&td);
    strftime(date, 31, "error%Y%m%d.log", localtime(&td));
    amx_log(date, string);
  }
  sleep(5);
  exit(code);
}

void amx_logerror(AMX *amx, int err, const char *error, ...) {
  // Check whether amx_debug CVAR is active
  if(cvar_amx_debug->value) {
    char string[1024];

    static char errorMsg[512];

    va_list arglst;
    va_start(arglst, error);
    vsnprintf(errorMsg, 511, error, arglst);
    va_end(arglst);

    // Generic Error Messages
    if(strcmp(error, "") == 0) {
      static char *msg[] = {
      /* AMX_ERR_NONE      */ "(none)",
      /* AMX_ERR_EXIT      */ "Forced exit",
      /* AMX_ERR_ASSERT    */ "Assertion failed",
      /* AMX_ERR_STACKERR  */ "Stack/heap collision (insufficient stack size)",
      /* AMX_ERR_BOUNDS    */ "Array index out of bounds",
      /* AMX_ERR_MEMACCESS */ "Invalid memory access",
      /* AMX_ERR_INVINSTR  */ "Invalid instruction",
      /* AMX_ERR_STACKLOW  */ "Stack underflow",
      /* AMX_ERR_HEAPLOW   */ "Heap underflow",
      /* AMX_ERR_CALLBACK  */ "No (valid) native function callback",
      /* AMX_ERR_NATIVE    */ "Native function failed",
      /* AMX_ERR_DIVIDE    */ "Divide by zero",
      /* AMX_ERR_SLEEP     */ "(sleep mode)",
      /* 13 */                "(reserved)",
      /* 14 */                "(reserved)",
      /* 15 */                "(reserved)",
      /* AMX_ERR_MEMORY    */ "Out of memory",
      /* AMX_ERR_FORMAT    */ "Invalid/unsupported P-code file format",
      /* AMX_ERR_VERSION   */ "File is for a newer version of the AMX",
      /* AMX_ERR_NOTFOUND  */ "File or function is not found",
      /* AMX_ERR_INDEX     */ "Invalid index parameter (bad entry point)",
      /* AMX_ERR_DEBUG     */ "Debugger cannot run",
      /* AMX_ERR_INIT      */ "AMX not initialized (or doubly initialized)",
      /* AMX_ERR_USERDATA  */ "Unable to set user data field (table full)",
      /* AMX_ERR_INIT_JIT  */ "Cannot initialize the JIT",
      /* AMX_ERR_PARAMS    */ "Parameter error",
      };
      // Using JIT = no correct line number
      #if defined JITORNOT
      if(amx->usejit) {
      #endif
        snprintf(string, 1023, "[AMX Error] Plugin: %s - Native: %s - Error: %s\n", g_plugins.findPluginFast(amx)->getName(), (const char *)(amx->userdata[2]), msg[err]);
      #if defined JITORNOT
      }
      else {
        snprintf(string, 1023, "[AMX Error] Plugin: %s - Native: %s - Error: %s - Line: %d\n", g_plugins.findPluginFast(amx)->getName(), (const char *)(amx->userdata[2]), msg[err], amx->curline);
      }
      #endif
    }
    // Custom Error Messages
    else {
      // Find native report_error for custom error messages reported by plugins
      if(strcmp((const char *)(amx->userdata[2]), "report_error") == 0) {
        snprintf(string, 1023, "[AMX Error] Plugin: %s - Error: %s\n", g_plugins.findPluginFast(amx)->getName(), errorMsg);
      }
      else {
        // Using JIT = no correct line number
        #if defined JITORNOT
        if(amx->usejit) {
        #endif
          snprintf(string, 1023, "[AMX Error] Plugin: %s - Native: %s - Error: %s\n", g_plugins.findPluginFast(amx)->getName(), (const char *)(amx->userdata[2]), errorMsg);
        #if defined JITORNOT
        }
        else {
          snprintf(string, 1023, "[AMX Error] Plugin: %s - Native: %s - Error: %s - Line: %d\n", g_plugins.findPluginFast(amx)->getName(), (const char *)(amx->userdata[2]), errorMsg, amx->curline);
        }
        #endif
      }
    }
    // Do we need to log this error?
    if(cvar_amx_debug->value >= 2) {
      char date[32];
      time_t td; time(&td);
      strftime(date, 31, "error%Y%m%d.log", localtime(&td));
      amx_log(date, string);
    }
    // Send error to HLDS console
    else {
      print_srvconsole(string);
    }
  }
}

void print_srvconsole(char *fmt, ...) {
  va_list argptr;
  char buf[256];
  va_start(argptr, fmt);
  vsnprintf(buf, 255, fmt, argptr);
  buf[255] = 0;
  va_end(argptr);
  SERVER_PRINT(buf);
}

void amx_log(char *szfile, char *message) {
  FILE* fp;
  const char* filename;
  if(strchr(szfile, '/') || strchr(szfile, '\\')) {
    filename = build_pathname("%s", szfile);
    create_path((char*)filename);
  }
  else filename = build_pathname("%s/%s", g_log_dir.str(), szfile);

  bool first_time = true;
  if((fp = fopen(filename, "r")) != nullptr) {
    first_time = false;
    fclose(fp);
  }
  if((fp = fopen(filename, "a")) != nullptr) {
    char date[32];
    time_t td; time(&td);
    strftime(date, 31, "%m/%d/%Y - %H:%M:%S", localtime(&td));
    if(first_time) {
      fprintf(fp,"L %s: Log file started (file \"%s\") (game \"%s\") (amx \"%s\")\n",
        date, filename, g_mod_name.str(), Plugin_info.version);
      print_srvconsole("L %s: Log file started (file \"%s\") (game \"%s\") (amx \"%s\")\n",
        date, filename, g_mod_name.str(), Plugin_info.version);
    }
    fprintf(fp, "L %s - %s: %s", date, STRING(gpGlobals->mapname), message);
    fclose(fp);
    print_srvconsole("L %s: %s", date, message);
    if(cvar_amx_log_to_hl_logs->value == 1) {
      ALERT(at_logged, "%s", message);
    }
  }
}

void logerror(const char *error, ...) {
  char errorMsg[256];
  va_list arglst;
  va_start(arglst, error);
  vsnprintf(errorMsg, 255, error, arglst);
  va_end(arglst);
  char date[32];
  time_t td; time(&td);
  strftime(date, 31, "error%Y%m%d.log", localtime(&td));
  amx_log(date, errorMsg);
}

void logtranslatorerror(const char *error, ...) {
  char errorMsg[1024];
  va_list arglst;
  va_start(arglst, error);
  vsnprintf(errorMsg, 1023, error, arglst);
  va_end(arglst);
  char date[32];
  time_t td; time(&td);
  strftime(date, 31, "error_lang%Y%m%d.log", localtime(&td));
  amx_log(date, errorMsg);
}

void* alloc_amxmemory(void** p, int size) {
  *p = new unsigned char[size];
  return *p;
}

void free_amxmemory(void** ptr) {
  delete[] (unsigned char *)(*ptr);
  *ptr = nullptr;
}

int load_amxscript(AMX *amx, void **program, const char *filename, char error[128], int wantjit) {
  AMX_HEADER hdr;
  int err;
  FILE *fp;

  memset(amx, 0, sizeof(*amx));
  *program = nullptr;
  *error = 0;

#if defined JITORNOT
  char *jit = (char*)get_localinfo("amx_usejit", "config");
  if(!strcmp(jit, "never")) {
    amx->usejit = 0;
  }
  else if(!strcmp(jit, "always")) {
    amx->usejit = 1;
  }
  else {
    amx->usejit = wantjit;
  }
#endif

  if((fp = fopen(filename, "rb")) == nullptr) {
    strcpy(error, "Plugin file open error");
    return (amx->error = AMX_ERR_NOTFOUND);
  }

  fread(&hdr, sizeof(hdr), 1, fp);
  amx_Align16(&hdr.magic);

  if(hdr.magic != AMX_MAGIC) {
    strcpy(error, "Invalid plugin");
    return (amx->error = AMX_ERR_FORMAT);
  }

  amx_Align32((uint32_t *)&hdr.stp);
  amx_Align32((uint32_t *)&hdr.size);

  if((*program  = new unsigned char[(int)hdr.stp]) == nullptr) {
  //if ( (*program = malloc( (int)hdr.stp )) == 0 )
    strcpy(error, "Failed to allocate memory");
    return (amx->error = AMX_ERR_MEMORY);
  }

  rewind(fp);
  fread(*program, 1, (size_t)hdr.size, fp);
  fclose(fp);

  #if defined JITORNOT
  if(amx->usejit) {
  #endif
  if(g_opt_level != 65536) {
		SetupOptimizer(amx); // By AMXMODX Dev Team
	}
	#if defined JITORNOT
	}
	#endif

  if((err = amx_Init(amx, *program)) != AMX_ERR_NONE) {
    sprintf(error, "Load error %d (invalid file format or version)", err);
    return (amx->error = AMX_ERR_INIT);
  }

//#if defined JITORNOT // v2006.3
#if defined JIT || defined JITORNOT
  amx->flags &= (~AMX_FLAG_DEBUG); //v2006.3
	amx->sysreq_d = 0; //v2006.3
	#if defined JITORNOT
  if(amx->usejit) {
  #endif
  #if defined JIT || defined JITORNOT
    unsigned char *np = new unsigned char[amx->code_size];
    unsigned char *rt = new unsigned char[amx->reloc_size];
    #if defined JITORNOT
      amx->jiterror = AMX_ERR_NONE;
    #endif
    if(!np || (!rt && amx->reloc_size > 0)) {
      delete[] np;
      delete[] rt;
      strcpy(error,"Failed to initialize plugin before JIT");
      #if defined JITORNOT
        amx->usejit = 0;
        amx->jiterror = AMX_ERR_INIT;
      #elif defined JIT
        return (amx->error = AMX_ERR_INIT);
      #endif
    }
    else {
      if(amx_InitJIT(amx, rt, np) == AMX_ERR_NONE) {
      //amx->base = (unsigned char FAR *)realloc( np, amx->code_size );
        #ifdef LINUX
          amx->base = (unsigned char *)memalign(sysconf(_SC_PAGESIZE), amx->code_size);
          mprotect((void *)amx->base, amx->code_size, PROT_READ|PROT_WRITE|PROT_EXEC);
        #else
          amx->base = new unsigned char[amx->code_size];
        #endif

        if(amx->base) {
          memcpy( amx->base , np , amx->code_size );
        }
        delete[] np;
        delete[] rt;
        delete[] (unsigned char *)(*program);
        (*program) = amx->base;
        if(*program == 0) {
          strcpy(error, "JIT ok, but then failed to allocate memory");
          #if defined JITORNOT
            amx->usejit = 0;
            amx->jiterror = AMX_ERR_MEMORY;
          #elif defined JIT
            return (amx->error = AMX_ERR_MEMORY);
          #endif
        }
      }
      else {
        delete[] np;
        delete[] rt;
        strcpy(error, "JIT failed to initialize plugin");
        #if defined JITORNOT
          amx->usejit = 0;
          amx->jiterror = AMX_ERR_INIT_JIT;
      }
      #elif defined JIT
        return (amx->error = AMX_ERR_INIT_JIT);
      }
      #endif
    }
  #if defined JITORNOT
  }
  #endif
  #endif // JIT
#endif // JITORNOT

  CScript* aa = new CScript(amx, *program, filename);
  if(aa == nullptr) {
    strcpy(error, "Failed to allocate memory");
    return (amx->error = AMX_ERR_MEMORY);
  }

  g_loadedscripts.put(aa);
  return set_amxnatives(amx, error);
}

int set_amxnatives(AMX* amx, char error[128]) {
  amx_Register(amx, string_Natives, -1);
  amx_Register(amx, float_Natives, -1);
  amx_Register(amx, file_Natives, -1);
  amx_Register(amx, amxmod_Natives, -1);
  amx_Register(amx, power_Natives, -1);
  amx_Register(amx, time_Natives, -1);
  amx_Register(amx, vault_Natives, -1);
  amx_Register(amx, math_Natives, -1);
  amx_Register(amx, vector_Natives, -1);
  amx_Register(amx, message_Natives, -1);
  
  int iFunc;
  if(amx_FindPublic(amx, "disable_natives_check", &iFunc) == AMX_ERR_NONE) {
    amx->flags |= AMX_FLAG_INNATCHK;
    amx->flags |= AMX_FLAG_NONATCHK;
    cell ret = 0;
	  int err;
    if((err = amx_Exec(amx, &ret, iFunc, 0)) != AMX_ERR_NONE) {
			amx_logerror(amx, err, "Error in disable_natives_check execution.");
		}
  }

  int i;
  int modulesLoaded = 0;

  CList<CModule>::iterator a;
  CList<AMX_NATIVE_INFO*>::iterator cc;
  for(a = g_modules.begin(); a; ++a) {
    for(cc = (*a).natives.begin(); cc; ++cc) {
      amx_Register(amx, *cc, -1);
    }
    if(g_autoloading_modules != -1) {
      for(i = 0; i < AMX_MODULES_NUM; ++i) {
        if(modulesLoaded & (1<<i)) continue;
        if(stristr((*a).getName(),amxModulesName[i])) {
          modulesLoaded |= (1<<i);
          break;
        }
      }
    }
  }

  /*amx_Register(amx, string_Natives, -1);
  amx_Register(amx, float_Natives, -1);
  amx_Register(amx, file_Natives, -1);
  amx_Register(amx, amxmod_Natives, -1);
  amx_Register(amx, power_Natives, -1);
  amx_Register(amx, time_Natives, -1);
  amx_Register(amx, vault_Natives, -1);
  amx_Register(amx, math_Natives, -1);
  amx_Register(amx, vector_Natives, -1);
  amx_Register(amx, message_Natives, -1);*/

  
  if(amx->flags & AMX_FLAG_INNATCHK) {
    amx->flags &= ~AMX_FLAG_INNATCHK;
  }

  int iError = amx_Register(amx, core_Natives, -1);
  if(g_autoloading_modules == -1) {
    if(iError != AMX_ERR_NONE) {
      sprintf(error, "Native not found: %s - Check your modules.ini and your modules folder.", no_function);
      return (amx->error = AMX_ERR_NATIVE);
    }
    return AMX_ERR_NONE;
  }

  int j, lastEndPos = 0, nextEndPos = 0, loopNum = 0;
  bool found;
  if(g_autoload_modules) {
    while(iError != AMX_ERR_NONE && loopNum < AMX_MODULES_NUM) {
      found = false;
      for(i = 0; i < AMX_MODULES_NUM && found == false; ++i) {
        if(modulesLoaded & (1<<i)) continue;
        if(i == AMX_MODULES_CSSTATS_NUM && !g_bmod_cstrike) continue;
        if(i == AMX_MODULES_DODXMOD_NUM && !g_bmod_dod) continue;
        lastEndPos = (i > 0) ? amxModulesNativesEndPosition[i-1] : 0;
        nextEndPos = amxModulesNativesEndPosition[i];
        for(j = lastEndPos; j < nextEndPos; ++j) {
					// the last native in the code of the plugin is the first tested here (nofunction), so it must
					// be a native declared in the autoload enum, otherwise the plugin/module won't load
          if(!strcmp(no_function, amxModulesNatives[j])) {
            if(loadModule(amxModulesName[i], g_pluginsloaded ? PT_STARTUP : PT_ANYTIME, true) > 0) {
              modulesLoaded |= (1<<i);
              a = g_modules.begin();
              for(cc = (*a).natives.begin(); cc; ++cc) {
                amx_Register(amx, *cc, -1);
              }
            }
            found = true;
            break;
          }
        }
      }
      ++loopNum;
      iError = amx_Register(amx, nullptr,0);
    }
    //FORWARDS
    int iFunc;
    for(i = 0; i < AMX_MODULES_NUM_FW; ++i) {
      if(modulesLoaded & (1<<i)) continue;
      if(i == AMX_MODULES_CSSTATS_NUM_FW && !g_bmod_cstrike) continue;
      if(i == AMX_MODULES_DODXMOD_NUM_FW && !g_bmod_dod) continue;
      lastEndPos = (i > 0) ? amxModulesForwardsEndPosition[i-1] : 0;
      nextEndPos = amxModulesForwardsEndPosition[i];
      for(j = lastEndPos; j < nextEndPos; ++j) {
        if(amx_FindPublic(amx, amxModulesForwards[j], &iFunc) == AMX_ERR_NONE) {
          if(loadModule(amxModulesName_FW[i], g_pluginsloaded ? PT_STARTUP : PT_ANYTIME, true) > 0) {
            modulesLoaded |= (1<<i);
            a = g_modules.begin();
            for(cc = (*a).natives.begin(); cc; ++cc) {
              amx_Register(amx, *cc, -1);
            }
          }
          break;
        }
      }
    }
  }

  if(iError != AMX_ERR_NONE) {
    if(g_autoloading_modules == 0) {
      found = false;
      lastEndPos = 0;
      nextEndPos = 0;
      for(i = 0; i < AMX_MODULES_NUM && found == false; ++i) {
        if(modulesLoaded & (1<<i)) continue;
        if(i == AMX_MODULES_CSSTATS_NUM && !g_bmod_cstrike) continue;
        if(i == AMX_MODULES_DODXMOD_NUM && !g_bmod_dod) continue;
        lastEndPos = (i > 0) ? amxModulesNativesEndPosition[i-1] : 0;
        nextEndPos = amxModulesNativesEndPosition[i];
        for(j = lastEndPos; j < nextEndPos; ++j) {
          if(!strcmp(no_function, amxModulesNatives[j])) {
            #ifndef LINUX
            sprintf(error, "Native not found: %s - Enable \"%s.dll\" in your modules.ini.", no_function, amxModulesName[i]);
            #else
            sprintf(error, "Native not found: %s - Enable \"%s_i386.so\" in your modules.ini.", no_function, amxModulesName[i]);
            #endif
            found = true;
            break;
          }
        }
      }
      if(found == false) {
        sprintf(error, "Native not found: %s - Check your modules.ini and your modules folder.", no_function);
      }
    }
    return (amx->error = AMX_ERR_NATIVE);
  }

  return AMX_ERR_NONE;
}


int unload_amxscript(AMX* amx, void** program) {
  optimizer_s *opt = (optimizer_s *)amx->usertags[UT_OPTIMIZER];
  if(opt) {
    delete opt;
  }

  CList<CScript,AMX*>::iterator a = g_loadedscripts.find(amx);
  if(a) a.remove();

  delete[] (unsigned char *)(*program);
  //free( *program );
  *program = nullptr;
  return AMX_ERR_NONE;
}


AMX* get_amxscript(int id, void** code, const char** filename) {
  CList<CScript,AMX*>::iterator a = g_loadedscripts.begin();
  while(a && id--) ++a;

  if(a) {
    *filename = (*a).getName();
    *code = (*a).getCode();
    return (*a).getAMX();
  }
  return nullptr;
}

const char* get_amxscriptname(AMX* amx) {
  CList<CScript,AMX*>::iterator a = g_loadedscripts.find(amx);
  return a ? (*a).getName() : "";
}

void get_modname(char* buffer) {
  strcpy(buffer, g_mod_name.str());
}

// By AMXMODX Dev Team
int dirExists(char dir[128]) {
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

void create_path(char *path) {
  char sDir[256];
  sDir[0] = '\0';
  char* sPath = path;
  int i = 0;
  while(*sPath) {
#ifndef LINUX
    if(*sPath == '\\') {
#else
    if(*sPath == '/') {
#endif
      if(i < 256) {
        sDir[i] = '\0';
        if(!dirExists(sDir))
#ifndef LINUX
          mkdir(sDir);
#else
          mkdir(sDir, 0700);
#endif
      }
    }
    if(i < 256) sDir[i++] = *sPath;

    ++sPath;
  }
}

char* build_pathname(char *fmt, ...) {
  static char string[256];

  int b;
  int a = b = snprintf(string, 255,
#ifndef LINUX
    "%s\\"
#else
    "%s/"
#endif
    , g_mod_name.str());

  va_list argptr;
  va_start (argptr, fmt);
  a += vsnprintf(&string[a], 255 - a, fmt, argptr);
  string[a] = 0;
  va_end (argptr);

  char* path = &string[b];

  while(*path) {
#ifndef LINUX
    if(*path == '/') *path = '\\';
#else
    if(*path == '\\') *path = '/';
#endif
    ++path;
  }

  return string;
}

char* build_pathname_addons(char *fmt, ...) {
  static char string[256];

  va_list argptr;
  va_start(argptr, fmt);
  vsnprintf(string, 255, fmt, argptr);
  va_end(argptr);

  char* path = string;

  while(*path) {
#ifndef LINUX
    if(*path == '/') *path = '\\';
#else
    if(*path == '\\') *path = '/';
#endif
    ++path;
  }

  return string;
}

int add_amxnatives(AMX_NATIVE_INFO* natives) {
  if(!g_calledModule) {
    return AMX_ERR_NATIVE;
  }

  AMX_NATIVE_INFO** aa = new AMX_NATIVE_INFO*(natives);
  if(aa == nullptr) return AMX_ERR_NATIVE;

  g_calledModule->natives.put(aa);
  return AMX_ERR_NONE;
}

bool is_plugin_running(AMX *amx) {
  CList<CScript,AMX*>::iterator a = g_loadedscripts.find(amx);
  if(!a) return false;

  CPluginMngr::CPlugin *plugin = g_plugins.findPluginFast(amx);
  return (plugin ? (plugin->isValid() && !plugin->isPaused()) : false);
}


/*bool validFile(const char* file) {
  const char* a = 0;
  while(*file) {
    if(*file++ == '.') a = file;
  }
#ifndef LINUX
  return (a && !strcmp(a, "dll"));
#else
  return (a && !strcmp(a, "so"));
#endif
}*/

int loadModule(const char* moduleName, PLUG_LOADTIME now, bool addExt) {

  char* pathname;
  if(addExt) {
#ifndef LINUX
    pathname = build_pathname("%s/%s.dll", get_localinfo("amx_modulesdir", "$basedir/modules"), moduleName);
#else
    pathname = build_pathname("%s/%s_i386.so", get_localinfo("amx_modulesdir", "$basedir/modules"), moduleName);
#endif
  }
  else {
    pathname = build_pathname("%s/%s", get_localinfo("amx_modulesdir", "$basedir/modules"), moduleName);
  }

  FILE* moduleFile = fopen(pathname, "rb");
  if(!moduleFile) {
    return 0;
  }
  fclose(moduleFile);

  CList<CModule>::iterator a = g_modules.find(pathname);

  if(a) {
    return 0;
  }

  CModule* cc = new CModule(pathname);

  if(cc == nullptr) return 0;

  cc->queryModule();

  bool error = true;

  switch(cc->getStatusValue()) {
    case MODULE_BADLOAD:
      report_error(1, "[AMX] Module is not a valid library (file \"%s\")\n", pathname);
      break;
    case MODULE_NOINFO:
      report_error(1, "[AMX] Couldn't find info. about module (file \"%s\")\n", pathname);
      break;
    case MODULE_NOQUERY:
      report_error(1, "[AMX] Couldn't find \"AMX_Query\" (file \"%s\")\n", pathname);
      break;
    case MODULE_NOATTACH:
      report_error(1, "[AMX] Couldn't find \"AMX_Attach\" (file \"%s\")\n", pathname);
      break;
    case MODULE_OLD:
      report_error(1, "[AMX] Module has a different interface version (file \"%s\")\n", pathname);
      break;
    default:
      error = false;
      break;
  }

  g_modules.put(cc);

  if(error) return 0;

  if(cc->isMetamod()) {
    char* metamodPathname;
    if(addExt) {
#ifndef LINUX
      metamodPathname = build_pathname_addons("%s/%s.dll", get_localinfo("amx_modulesdir", "$basedir/modules"), moduleName);
#else
      metamodPathname = build_pathname_addons("%s/%s_i386.so", get_localinfo("amx_modulesdir", "$basedir/modules"), moduleName);
#endif
    }
    else {
      metamodPathname = build_pathname_addons("%s/%s", get_localinfo("amx_modulesdir", "$basedir/modules"), moduleName);
    }

    cc->attachModuleToMetamod(metamodPathname, now);

    cc->attachModule();

    if(cc->getStatusValue() == MODULE_LOADED) {
      return 2; // metamod module
    }

    return 0;
  }

  cc->attachModule();

  if(cc->getStatusValue() == MODULE_LOADED) {
    return 1; // amx module
  }

  return 0;
}

void loadModules(const char* filename, PLUG_LOADTIME now) {
  FILE* fp = fopen(build_pathname("%s", filename), "rt");

  if(!fp) {
    logerror("[AMX] Modules list not found (file \"%s\")\n", filename);
    return;
  }

  char line[256], moduleName[256];
  size_t len;

  while(!feof(fp)) {
    line[0] = '\0';
    fgets(line, 255, fp);

    if(line[0] == ';' || line[0] == '/' || line[0] == '\n') {
      continue;
    }

    *moduleName = 0;
    if(sscanf(line, "%s", moduleName) == EOF) { // By AMXMODX Dev Team
      continue;
    }

    if(!isalnum(*moduleName)) {
      continue;
    }

    len = strlen(moduleName);
    #ifndef LINUX
    if(len > 4 && strncmp(&moduleName[len-4], ".dll", 4) == 0) {
      loadModule(moduleName, now, false);
    }
    #else
    if(len > 8 && strncmp(&moduleName[len-8], "_i386.so", 8) == 0) {
      loadModule(moduleName, now, false);
    }
    #endif
    else {
      loadModule(moduleName, now, true);
    }
  }

  fclose(fp);
}

int countLoadedModules() {
  int loaded = 0;
  CList<CModule>::iterator a = g_modules.begin();
  while(a) {
    if((*a).getStatusValue() == MODULE_LOADED) {
      ++loaded;
    }
    ++a;
  }
  return loaded;
}

void detachModules() {
  CList<CModule>::iterator a = g_modules.begin();

  while(a) {
    (*a).detachModule();
    a.remove();
  }
}

void detachReloadModules() {
  CList<CModule>::iterator a = g_modules.begin();

  while(a) {
    if((*a).isReloadable() && !(*a).isMetamod()) {
      (*a).detachModule();
      a.remove();
      continue;
    }

    ++a;
  }
}

const char* strip_name(const char* a) {
  const char* ret = a;
  while(*a) {
    if(*a == '/' || *a == '\\') {
      ret = ++a;
      continue;
    }
    ++a;
  }
  return ret;
}

int loadModuleByMetamod(const char *modulePath, void **moduleHandle, PLUG_LOADTIME now) {
  int err = 0;
  if((err = LOAD_PLUGIN(PLID, modulePath, now, moduleHandle)) || !*moduleHandle) {
    LOG_MESSAGE(PLID, "Can't attach Metamod-Module \"%s\"", modulePath);
    return 0;
  }
  return 1;
}

int unloadModuleByMetamod(void *moduleHandle) {
  if(UNLOAD_PLUGIN_BY_HANDLE(PLID, (void *)moduleHandle, PT_ANYTIME, PNL_PLUGIN)) {
    return 0;
  }
  return 1;
}

void modulesPluginsLoaded() {
  CList<CModule>::iterator a = g_modules.begin();
  while(a) {
    (*a).pluginsLoaded();
    ++a;
  }
}

bool is_player(int id, int ingame, AMX *amx) {
  if(id > 0 && id <= gpGlobals->maxClients) {
    if(!GET_PLAYER_POINTER_I(id)->initialized) {
      return 0;
    }
    if(ingame && !GET_PLAYER_POINTER_I(id)->ingame) {
      //amx_logerror(amx, AMX_ERR_NATIVE, "Player %d not in-game", id);
      return 0;
    }
  }
  else {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid player id %d", id);
    return 0;
  }
  return 1;
}

bool is_entity(int id, AMX *amx) {
  if(id < 0 && id > gpGlobals->maxEntities) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Entity %d out of range", id);
    return 0;
  }
  else {
    if(id != 0 && FNullEnt(INDEXENT(id))) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Invalid entity id %d", id);
      return 0;
    }
  }
  return 1;
}

bool is_bot(int id, AMX *amx) {
  if(id > 0 && id <= gpGlobals->maxClients) {
    if(!GET_PLAYER_POINTER_I(id)->IsBot()) {
      amx_logerror(amx, AMX_ERR_NATIVE, "%d isn't a Bot", id);
      return 0;
    }
  }
  else {
    amx_logerror(amx, AMX_ERR_NATIVE, "Bot %d invalid", id);
    return 0;
  }
  return 1;
}

// By AMXMODX Dev Team
void MODFUNC_LogError(const char *fmt, ...) {
  char msg[1024];
  va_list arglst;
  va_start(arglst, fmt);
  vsnprintf(msg, sizeof(msg) - 1, fmt, arglst);
  va_end(arglst);
  logerror(msg);
}

// By AMXMODX Dev Team
int MODFUNC_FindAmxScriptByName(const char *name) {
  CPluginMngr::iterator iter = g_plugins.begin();
  bool found = false;
  int i = 0;
  while(iter) {
    if (stricmp((*iter).getName(), name) == 0) {
      found = true;
      break;
    }
    ++iter;
    ++i;
  }
  if(!found) {
    return -1;
  }
  return i;
}

// By AMXMODX Dev Team
int MODFUNC_FindAmxScriptByAmx(const AMX *amx) {
  CPluginMngr::iterator iter = g_plugins.begin();
  bool found = false;
  int i = 0;
  while(iter) {
    if(amx == (*iter).getAMX()) {
      found = true;
      break;
    }
    ++iter;
    ++i;
  }
  if(!found) {
    return -1;
  }
  return i;
}

// By AMXMODX Dev Team
int MODFUNC_IsPlayerValid(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return (GET_PLAYER_POINTER_I(id)->initialized) ? 1 : 0;
}

// By AMXMODX Dev Team
int MODFUNC_IsPlayerHLTV(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return (GET_PLAYER_POINTER_I(id)->pEdict->v.flags & FL_PROXY) ? 1 : 0;
}

// By AMXMODX Dev Team
int MODFUNC_IsPlayerBot(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->IsBot() ? 1 : 0;
}

// By AMXMODX Dev Team
int MODFUNC_IsPlayerConnecting(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  CPlayer * pPlayer = GET_PLAYER_POINTER_I(id);
  return (!pPlayer->ingame && pPlayer->initialized && (GETPLAYERUSERID(pPlayer->pEdict) > 0)) ? 1 : 0;
}

// By AMXMODX Dev Team
int MODFUNC_IsPlayerAuthorized(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->authorized ? 1 : 0;
}

// By AMXMODX Dev Team
int MODFUNC_IsPlayerInGame(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->ingame ? 1 : 0;
}

// By AMXMODX Dev Team
int MODFUNC_IsPlayerAlive(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->IsAlive() ? 1 : 0;
}

// By AMXMODX Dev Team
const char * MODFUNC_GetPlayerName(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return nullptr;

  return GET_PLAYER_POINTER_I(id)->name.str();
}

// By AMXMODX Dev Team
const char * MODFUNC_GetPlayerIP(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return nullptr;

  return GET_PLAYER_POINTER_I(id)->ip.str();
}

// By AMXMODX Dev Team
float MODFUNC_GetPlayerTime(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0.0f;

  return GET_PLAYER_POINTER_I(id)->time;
}

// By AMXMODX Dev Team
float MODFUNC_GetPlayerPlayTime(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0.0f;

  return GET_PLAYER_POINTER_I(id)->playtime;
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerCurweapon(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->current;
}

// By AMXMODX Dev Team
const char *MODFUNC_GetPlayerTeam(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return nullptr;

  return GET_PLAYER_POINTER_I(id)->team.str();
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerTeamID(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->teamId;
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerFrags(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return (int)(GET_PLAYER_POINTER_I(id)->pEdict->v.frags);
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerDeaths(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->deaths;
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerHealth(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return (int)(GET_PLAYER_POINTER_I(id)->pEdict->v.health);
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerArmor(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return (int)(GET_PLAYER_POINTER_I(id)->pEdict->v.armorvalue);
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerMenu(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->menu;
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerKeys(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->keys;
}

// By AMXMODX Dev Team
int MODFUNC_GetPlayerFlags(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return 0;

  return GET_PLAYER_POINTER_I(id)->flags[0];
}

// By AMXMODX Dev Team
edict_t* MODFUNC_GetPlayerEdict(int id) {
  if(id < 1 || id > gpGlobals->maxClients)
    return nullptr;

  return GET_PLAYER_POINTER_I(id)->pEdict;
}

// By AMXMODX Dev Team
int MODFUNC_SetPlayerTeamInfo(int player, int teamid, const char *teamname) {
  if(player < 1 || player > gpGlobals->maxClients)
    return 0;

  CPlayer *pPlayer = GET_PLAYER_POINTER_I(player);

  if(!pPlayer->ingame) {
    return 0;
  }

  pPlayer->teamId = teamid;
  if(teamname != nullptr) {
    pPlayer->team.set(teamname);
  }

  return 1;
}

// By AMXMODX Dev Team
void *MODFUNC_PlayerPropAddr(int id, int prop) {
  if(id < 1 || id > gpGlobals->maxClients)
    return nullptr;

  CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

  switch(prop) {
    case Player_Name:
      return &pPlayer->name;
    case Player_Ip:
      return &pPlayer->ip;
    case Player_Team:
      return &pPlayer->team;
    case Player_Ingame:
      return &pPlayer->ingame;
    case Player_Authorized:
      return &pPlayer->authorized;
    case Player_Time:
      return &pPlayer->time;
    case Player_Playtime:
      return &pPlayer->playtime;
    case Player_ClearMenuTime:
      return &pPlayer->clearmenutime;
    case Player_Weapons:
      return &pPlayer->weapons[0];
    case Player_CurrentWeapon:
      return &pPlayer->current;
    case Player_TeamID:
      return &pPlayer->teamId;
    case Player_Deaths:
      return &pPlayer->deaths;
    case Player_Aiming:
      return &pPlayer->aiming;
    case Player_Menu:
      return &pPlayer->menu;
    case Player_Keys:
      return &pPlayer->keys;
    case Player_Flags:
      return &pPlayer->flags[0];
    default:
      return nullptr;
  }

  return nullptr;
}

void * operator new(size_t size) {
  return(calloc(1, size));
}

void * operator new[](size_t size) {
  return(calloc(1, size));
}

void operator delete(void *ptr) {
  if(ptr) free(ptr);
}

void operator delete[](void *ptr) {
  if(ptr) free(ptr);
}
