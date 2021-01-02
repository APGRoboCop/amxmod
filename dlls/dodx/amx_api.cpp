#include <extdll.h>
#include <meta_api.h>
#include <dllapi.h>
#include <h_export.h>
#include "dodxmod.h"

/******************************* AMX API **************************************/

void AMXAttach() {
  MF_AddNatives(stats_natives);
  MF_AddNatives(base_natives);
  g_map.Init();
}

void AMXPluginsLoaded() {
  g_pengfuncsTable_Post->pfnWriteByte = NULL;
  g_pengfuncsTable_Post->pfnWriteChar = NULL;
  g_pengfuncsTable_Post->pfnWriteShort = NULL;
  g_pengfuncsTable_Post->pfnWriteLong = NULL;
  g_pengfuncsTable_Post->pfnWriteAngle = NULL;
  g_pengfuncsTable_Post->pfnWriteCoord = NULL;
  g_pengfuncsTable_Post->pfnWriteString = NULL;
  g_pengfuncsTable_Post->pfnWriteEntity = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;

  g_forwards.findForwards();
}

void AMXDetach() {
  g_rank.clear();
  g_grenades.clear();
  if(gpMetaGlobals) {
    g_rank.unloadCalc();
  }
}

/******************************* MODULE API ***********************************/

cvar_t init_dodstats_maxsize ={"dodstats_maxsize","3500", 0 , 3500.0};
cvar_t init_dodstats_reset ={"dodstats_reset","0"};
cvar_t init_dodstats_rank ={"dodstats_rank","0"};
cvar_t init_dodstats_rankbots ={"dodstats_rankbots","1"};
cvar_t init_dodstats_pause = {"dodstats_pause","0"};
cvar_t *dodstats_maxsize;
cvar_t *dodstats_reset;
cvar_t *dodstats_rank;
cvar_t *dodstats_rankbots;
cvar_t *dodstats_pause;

void MetaAttach() {
  g_rank.ClearCode();

  CVAR_REGISTER(&init_dodstats_maxsize);
  CVAR_REGISTER(&init_dodstats_reset);
  CVAR_REGISTER(&init_dodstats_rank);
  CVAR_REGISTER(&init_dodstats_rankbots);
  CVAR_REGISTER(&init_dodstats_pause);
  dodstats_maxsize = CVAR_GET_POINTER(init_dodstats_maxsize.name);
  dodstats_reset = CVAR_GET_POINTER(init_dodstats_reset.name);
  dodstats_rank = CVAR_GET_POINTER(init_dodstats_rank.name);
  dodstats_rankbots = CVAR_GET_POINTER(init_dodstats_rankbots.name);
  dodstats_pause = CVAR_GET_POINTER(init_dodstats_pause.name);

  REG_SVR_COMMAND("dodstats_savetext", dodstats_savetext_command);
  REG_SVR_COMMAND("dodstats_loadtext", dodstats_loadtext_command);
  REG_SVR_COMMAND("dodstats_savesv", dodstats_savesv_command);
}

Forwards g_forwards;

function_event_call modMsgsEnd[MAX_REG_MSGS];
function_event_call modMsgs[MAX_REG_MSGS];
void (*function)(void*);
void (*endfunction)(void*);
CPlayer* mPlayer;
CPlayer players[33];
CMapInfo g_map;


//bool rankBots;
int mState;
int mPlayerIndex;
int AlliesScore;
int AxisScore;

int gmsgCurWeapon;
int gmsgObjScore;
int gmsgTeamScore;
int gmsgRoundState;
int gmsgHealth;
int gmsgResetHUD;
int gmsgPTeam;
int gmsgAmmoX;
int gmsgAmmoShort;
int gmsgScoreShort;

RankSystem g_rank;
Grenades g_grenades;

bool g_LoadText;
char g_LoadTextFile[256];

/************************* Begin Hooked Functions *****************************/

struct sUserMsg {
  const char* name;
  int* id;
  funEventCall func;
  bool endmsg;
} g_user_msg[] = {
  { "CurWeapon",&gmsgCurWeapon,Client_CurWeapon,false },
  { "ObjScore",&gmsgObjScore,Client_ObjScore,false },
  { "TeamScore",&gmsgTeamScore,Client_TeamScore,false },
  { "RoundState",&gmsgRoundState,Client_RoundState,false },
  { "Health",&gmsgHealth,Client_Health_End,true },
  { "ResetHUD",&gmsgResetHUD,Client_ResetHUD_End,true },
  { "PTeam",&gmsgPTeam,NULL,false },
  { "AmmoX",&gmsgAmmoX,Client_AmmoX,false},
  { "AmmoShort",&gmsgAmmoShort,Client_AmmoShort,false},
  { "ScoreShort",&gmsgScoreShort,NULL,false },
  { 0,0,0,false }
};

int RegUserMsg_Post(const char *pszName, int iSize){
  for (int i = 0; g_user_msg[ i ].name; ++i ){
    if ( strcmp( g_user_msg[ i ].name , pszName  ) == 0 ){
      int id = META_RESULT_ORIG_RET( int );

      *g_user_msg[ i ].id = id;

      if ( g_user_msg[ i ].endmsg )
        modMsgsEnd[ id  ] = g_user_msg[ i ].func;
      else
        modMsgs[ id  ] = g_user_msg[ i ].func;

      break;
    }
  }

  RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
  int id;
  for(int i = 0; g_user_msg[ i ].name; ++i )  {
    if(( *g_user_msg[ i ].id == 0 ) && ( id = GET_USER_MSG_ID(PLID, g_user_msg[ i ].name, NULL) ) != 0) {
      *g_user_msg[ i ].id = id;

      if( g_user_msg[ i ].endmsg ) {
        modMsgsEnd[ id ] = g_user_msg[ i ].func;
      }
      else {
        modMsgs[ id ] = g_user_msg[ i ].func;
      }
    }
  }

  const char* path =  MF_GetLocalInfo("dodstats_score","$pluginsdir/dodstats.amx");
  if(path && *path) {
    char error[128];
    g_rank.loadCalc(MF_BuildPathname("%s", path), error);
  }
  if(!g_rank.begin()) {
    g_rank.loadRank(MF_BuildPathname("%s", MF_GetLocalInfo("dodstats","$modulesdir/dodstats.dat")));
  }

  RETURN_META(MRES_IGNORED);
}

void ServerActivate_Post(edict_t *pEdictList, int edictCount, int clientMax) {
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    GET_PLAYER_POINTER_I(i)->Init( i , pEdictList + i );
  }

  gmsgCurWeapon = GET_USER_MSG_ID(PLID, "CurWeapon", NULL );
  gmsgObjScore = GET_USER_MSG_ID(PLID, "ObjScore", NULL );
  gmsgTeamScore = GET_USER_MSG_ID(PLID, "TeamScore", NULL );
  gmsgRoundState = GET_USER_MSG_ID(PLID, "RoundState", NULL );
  gmsgHealth = GET_USER_MSG_ID(PLID, "Health", NULL );
  gmsgResetHUD = GET_USER_MSG_ID(PLID, "ResetHUD", NULL );
  gmsgPTeam = GET_USER_MSG_ID(PLID, "PTeam", NULL );
  gmsgAmmoX = GET_USER_MSG_ID(PLID, "AmmoX", NULL );
  gmsgAmmoShort = GET_USER_MSG_ID(PLID, "AmmoShort", NULL );
  gmsgScoreShort = GET_USER_MSG_ID(PLID, "ScoreShort", NULL );

  g_LoadText = false;

  RETURN_META(MRES_IGNORED);
}

void PlayerPreThink_Post(edict_t *pEntity) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  if ( !pPlayer->ingame )
    RETURN_META(MRES_IGNORED);

  if ( pPlayer->staminaSet ) {
    if ( pEntity->v.fuser4 > pPlayer->staminaMax )
      pEntity->v.fuser4 = pPlayer->staminaMax;
    else if ( pEntity->v.fuser4 < pPlayer->staminaMin )
      pEntity->v.fuser4 = pPlayer->staminaMin;
  }

  if ( !isModuleActive() )
    RETURN_META(MRES_IGNORED);

  if (pPlayer->clearStats && pPlayer->clearStats < gpGlobals->time && pPlayer->ingame){
    pPlayer->clearStats = 0.0f;
    if ( !ignoreBots(pPlayer->index/*pEntity*/) ){
      pPlayer->rank->updatePosition( &pPlayer->life );
    }
    pPlayer->restartStats(false);
  }

  if (pPlayer->sendScore && pPlayer->sendScore < gpGlobals->time){
    pPlayer->sendScore = 0.0f;
    if(g_forwards.forwardsExist(FF_ScoreInfo) == true) {
      g_forwards.executeForward(FF_ScoreInfo, pPlayer->index, pPlayer->lastScore, pPlayer->savedScore);
    }
  }

  RETURN_META(MRES_IGNORED);
}

void ServerDeactivate() {
  for(int i = 1;i<=gpGlobals->maxClients; ++i){
    CPlayer *pPlayer = GET_PLAYER_POINTER_I(i);
    if(pPlayer->ingame) pPlayer->Disconnect();
  }

  if(g_LoadText == true) {
    if(g_rank.rank_text_to_binary(g_LoadTextFile) == true) {
      MF_PrintSrvConsole("[AMX] Successfully converted \"%s\" to dodstats.dat\n", g_LoadTextFile);
    }
    else {
      MF_PrintSrvConsole("[AMX] Couldn't convert \"%s\" to dodstats.dat\n", g_LoadTextFile);
      g_rank.saveRank(MF_BuildPathname("%s", MF_GetLocalInfo("dodstats","$modulesdir/dodstats.dat")));
    }
  }
  else {
    g_rank.saveRank(MF_BuildPathname("%s", MF_GetLocalInfo("dodstats","$modulesdir/dodstats.dat")));
  }

  g_rank.unloadCalc();

  g_rank.clear();
  g_grenades.clear();
  g_forwards.clear();

  g_map.Init();

  g_pengfuncsTable_Post->pfnWriteByte = NULL;
  g_pengfuncsTable_Post->pfnWriteChar = NULL;
  g_pengfuncsTable_Post->pfnWriteShort = NULL;
  g_pengfuncsTable_Post->pfnWriteLong = NULL;
  g_pengfuncsTable_Post->pfnWriteAngle = NULL;
  g_pengfuncsTable_Post->pfnWriteCoord = NULL;
  g_pengfuncsTable_Post->pfnWriteString = NULL;
  g_pengfuncsTable_Post->pfnWriteEntity = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;
  
  RETURN_META(MRES_IGNORED);
}

BOOL ClientConnect_Post( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  ){
  GET_PLAYER_POINTER(pEntity)->Connect(pszName,pszAddress);
  RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

void ClientDisconnect( edict_t *pEntity ) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  if(pPlayer->ingame)
    pPlayer->Disconnect();
  RETURN_META(MRES_IGNORED);
}

void ClientPutInServer_Post( edict_t *pEntity ) {
  GET_PLAYER_POINTER(pEntity)->PutInServer();
  RETURN_META(MRES_IGNORED);
}

void ClientUserInfoChanged_Post( edict_t *pEntity, char *infobuffer ) {
  CPlayer *pPlayer = GET_PLAYER_POINTER(pEntity);
  const char* name = INFOKEY_VALUE(infobuffer,"name");
  const char* oldname = STRING(pEntity->v.netname);

  if ( pPlayer->ingame){
    if ( strcmp(oldname,name) ) {
      if (!dodstats_rank->value)
        pPlayer->rank = g_rank.findEntryInRank( name, name );
      else
        pPlayer->rank->setName( name );
    }
  }
  else if ( MF_IsPlayerBot(pPlayer->index) ) {
    pPlayer->Connect( name , "127.0.0.1" );
    pPlayer->PutInServer();
  }

  RETURN_META(MRES_IGNORED);
}

void MessageBegin_Post(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) {
  if (ed){
    mPlayerIndex = ENTINDEX(ed);
    mPlayer = GET_PLAYER_POINTER_I(mPlayerIndex);
  } else {
    mPlayerIndex = 0;
    mPlayer = NULL;
  }
  mState = 0;
  if ( msg_type < 0 || msg_type >= MAX_REG_MSGS )
    msg_type = 0;
  function=modMsgs[msg_type];
  endfunction=modMsgsEnd[msg_type];

  if(function) {
    g_pengfuncsTable_Post->pfnWriteByte = WriteByte_Post;
    g_pengfuncsTable_Post->pfnWriteChar = WriteChar_Post;
    g_pengfuncsTable_Post->pfnWriteShort = WriteShort_Post;
    g_pengfuncsTable_Post->pfnWriteLong = WriteLong_Post;
    g_pengfuncsTable_Post->pfnWriteAngle = WriteAngle_Post;
    g_pengfuncsTable_Post->pfnWriteCoord = WriteCoord_Post;
    g_pengfuncsTable_Post->pfnWriteString = WriteString_Post;
    g_pengfuncsTable_Post->pfnWriteEntity = WriteEntity_Post;
    g_pengfuncsTable_Post->pfnMessageEnd = MessageEnd_Post;
  }
  if(endfunction) {
    g_pengfuncsTable_Post->pfnMessageEnd = MessageEnd_Post;
  }

  RETURN_META(MRES_IGNORED);
}

void WriteByte_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}
void WriteChar_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}
void WriteShort_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}
void WriteLong_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}
void WriteAngle_Post(float flValue) {
  if (function) (*function)((void *)&flValue);
  RETURN_META(MRES_IGNORED);
}
void WriteCoord_Post(float flValue) {
  if (function) (*function)((void *)&flValue);
  RETURN_META(MRES_IGNORED);
}
void WriteString_Post(const char *sz) {
  if (function) (*function)((void *)sz);
  RETURN_META(MRES_IGNORED);
}
void WriteEntity_Post(int iValue) {
  if (function) (*function)((void *)&iValue);
  RETURN_META(MRES_IGNORED);
}

void MessageEnd_Post(void) {
  if (endfunction) (*endfunction)(NULL);
  g_pengfuncsTable_Post->pfnWriteByte = NULL;
  g_pengfuncsTable_Post->pfnWriteChar = NULL;
  g_pengfuncsTable_Post->pfnWriteShort = NULL;
  g_pengfuncsTable_Post->pfnWriteLong = NULL;
  g_pengfuncsTable_Post->pfnWriteAngle = NULL;
  g_pengfuncsTable_Post->pfnWriteCoord = NULL;
  g_pengfuncsTable_Post->pfnWriteString = NULL;
  g_pengfuncsTable_Post->pfnWriteEntity = NULL;
  g_pengfuncsTable_Post->pfnMessageEnd = NULL;
  RETURN_META(MRES_IGNORED);
}

void SetModel_Post(edict_t *e, const char *m){
  if ( !e->v.owner || !e->v.dmgtime )
    RETURN_META(MRES_IGNORED);

  int owner = ENTINDEX(e->v.owner);
  if ( owner && owner<33 && m[7]=='w' && m[8]=='_' ){
    int w_id = 0;

    CPlayer* pPlayer = GET_PLAYER_POINTER_I(owner);
    bool newNade = ( pPlayer->current == 13 || pPlayer->current == 14 ) ? true : false;

    if ( m[9]=='g' && m[10]=='r' && m[11]=='e' && m[12]=='n' )
      newNade ? w_id = 13 : w_id = 16; // grenade
    else if ( m[9]=='m' && m[10]=='i' )
      newNade ? w_id = 36 : w_id = 16 ; // mills
    else if ( m[9]=='s' && m[10]=='t' && m[11]=='i')
      newNade ? w_id = 14 : w_id = 15; // stick

    if ( !w_id )
      RETURN_META(MRES_IGNORED);

    g_grenades.put(e, 6, w_id, pPlayer);

    if ( isModuleActive() ) {
      pPlayer->saveShot(w_id);
    }

    if(g_forwards.forwardsExist(FF_GrenadeThrow) == true) {
      g_forwards.executeForward(FF_GrenadeThrow, pPlayer->index, ENTINDEX(e), w_id);
    }

    /* fuse start */
    if ( pPlayer->fuseSet ){
      if ( newNade ){
        if ( pPlayer->fuseType & 1<<0 ){
          e->v.dmgtime += pPlayer->nadeFuse - 5.0;
        }
      }
      else{ // caught
        bool ownNade = ( (pPlayer->pEdict->v.team == 1 && pPlayer->current == 16) || (pPlayer->pEdict->v.team == 2 && pPlayer->current == 15) ) ? true:false;
        if ( ownNade ){
          float fExp = e->v.dmgtime - gpGlobals->time;
          e->v.dmgtime += pPlayer->nadeFuse - fExp;
        }
      }
    }
    /* fuse end */
  }

  RETURN_META(MRES_IGNORED);
}

void EmitSound_Post(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch) {
  if ( entity->v.owner ){
    if ( sample[8]=='r' && sample[9]=='o' ){
      CPlayer* pPlayer = GET_PLAYER_POINTER(entity->v.owner);
      int w_id = 0;
      char name[15];
      strcpy(name,STRING(entity->v.classname));
      if ( name[7]=='i' ) w_id = 31; // shell_piat
      else if ( name[7] =='a' ) w_id = 29; // shell_bazooka
      else /*if ( name[7] =='s' )*/ w_id = 30 ; // shell_pschreck
      g_grenades.put(entity,4,w_id,pPlayer);
      // fire_shot in CurWeapon MSG
    }
  }

  RETURN_META(MRES_IGNORED);
}

void TraceLine_Post(const float *v1, const float *v2, int fNoMonsters, edict_t *e, TraceResult *ptr) {
  if(ptr->pHit && (ptr->pHit->v.flags & (FL_CLIENT | FL_FAKECLIENT) ) &&  e && (e->v.flags & (FL_CLIENT | FL_FAKECLIENT))) {
    GET_PLAYER_POINTER(e)->aiming = ptr->iHitgroup;
  }
  RETURN_META(MRES_IGNORED);
}

void DispatchKeyValue_Post( edict_t *pentKeyvalue, KeyValueData *pkvd ){
  if ( !pkvd->szClassName ){
    // info_doddetect
    if ( pkvd->szValue[0]=='i' && pkvd->szValue[5]=='d' ){
      g_map.pEdict = pentKeyvalue;
      g_map.initialized = true;
    }
    RETURN_META(MRES_IGNORED);
  }
  // info_doddetect
  if ( g_map.initialized && pentKeyvalue == g_map.pEdict ){
    if ( pkvd->szKeyName[0]=='d' && pkvd->szKeyName[7]=='a' ){
      if ( pkvd->szKeyName[8]=='l' ){
        switch ( pkvd->szKeyName[14] ){
        case 'c':
          g_map.detect_allies_country=atoi(pkvd->szValue);
          break;
        case 'p':
          g_map.detect_allies_paras=atoi(pkvd->szValue);
          break;
        }
      }
      else if ( pkvd->szKeyName[12]=='p' ) g_map.detect_axis_paras=atoi(pkvd->szValue);
    }
  }
  RETURN_META(MRES_IGNORED);
}

void dodstats_savetext_command() {
  const char* filename = CMD_ARGV(1);
  if(g_rank.save_rank_text(MF_BuildPathname("%s", filename)) == true) {
    MF_PrintSrvConsole("[AMX] dodstats.dat has been saved to \"%s\"\n", filename);
  }
}

void dodstats_loadtext_command() {
  const char* filename = CMD_ARGV(1);
  if(strcmp(filename, "")) {
    const char* path = MF_BuildPathname("%s", filename);
    if(g_rank.is_valid_text_file(path) == false) {
      return;
    }
    snprintf(g_LoadTextFile,255,"%s",path);
    g_LoadText = true;
    MF_PrintSrvConsole("[AMX] \"%s\" will be loaded at the end of the current map\n", g_LoadTextFile);
  }
  else if(!strcmp(filename, "") && g_LoadText == true) {
    g_LoadText = false;
    MF_PrintSrvConsole("[AMX] \"%s\" will not be loaded any more at the end of the current map\n", g_LoadTextFile);
  }
}

void dodstats_savesv_command() {
  const char* filename = CMD_ARGV(1);
  const char* parsertype = CMD_ARGV(2);
  if(g_rank.save_rank_sv(MF_BuildPathname("%s", filename), parsertype ? atoi(parsertype) : 0) == true) {
    MF_PrintSrvConsole("[AMX] dodstats.dat has been saved to \"%s\"\n", filename);
  }
}

/************************* End Hooked Functions *******************************/
