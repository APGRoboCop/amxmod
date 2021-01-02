// Module API By AMXMODX Dev Team
// The authors of AMX Mod X are:
// David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
// Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
// Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
// Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.

#ifndef amx_module_h
#define amx_module_h

#include <modules.h>
#include "moduleconfig.h"


#ifdef USE_METAMOD
#include <extdll.h>
#include <meta_api.h>
#include "osdep.h"


void UTIL_LogPrintf(char *fmt, ...);
void UTIL_HudMessage(CBaseEntity *pEntity, const hudtextparms_t &textparms, const char *pMessage);
short FixedSigned16(float value, float scale);
unsigned short FixedUnsigned16(float value, float scale);

#ifdef FN_META_QUERY
void FN_META_QUERY(void);
#endif // FN_META_QUERY

#ifdef FN_META_ATTACH
void FN_META_ATTACH(void);
#endif // FN_META_ATTACH

#ifdef FN_META_DETACH
void FN_META_DETACH(void);
#endif // FN_META_DETACH





#ifdef FN_GameDLLInit
void FN_GameDLLInit(void);
#endif // FN_GameDLLInit

#ifdef FN_DispatchSpawn
int FN_DispatchSpawn(edict_t *pent);
#endif // FN_DispatchSpawn

#ifdef FN_DispatchThink
void FN_DispatchThink(edict_t *pent);
#endif // FN_DispatchThink

#ifdef FN_DispatchUse
void FN_DispatchUse(edict_t *pentUser, edict_t *pentOther);
#endif // FN_DispatchUse

#ifdef FN_DispatchTouch
void FN_DispatchTouch(edict_t *pentTouched, edict_t *pentOther);
#endif // FN_DispatchTouch

#ifdef FN_DispatchBlocked
void FN_DispatchBlocked(edict_t *pentBlocked, edict_t *pentOther);
#endif // FN_DispatchBlocked

#ifdef FN_DispatchKeyValue
void FN_DispatchKeyValue(edict_t *pentKeyvalue, KeyValueData *pkvd);
#endif // FN_DispatchKeyValue

#ifdef FN_DispatchSave
void FN_DispatchSave(edict_t *pent, SAVERESTOREDATA *pSaveData);
#endif // FN_DispatchSave

#ifdef FN_DispatchRestore
int FN_DispatchRestore(edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity);
#endif // FN_DispatchRestore

#ifdef FN_DispatchObjectCollsionBox
void FN_DispatchObjectCollsionBox(edict_t *pent);
#endif // FN_DispatchObjectCollsionBox

#ifdef FN_SaveWriteFields
void FN_SaveWriteFields(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount);
#endif // FN_SaveWriteFields

#ifdef FN_SaveReadFields
void FN_SaveReadFields(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount);
#endif // FN_SaveReadFields

#ifdef FN_SaveGlobalState
void FN_SaveGlobalState(SAVERESTOREDATA *pSaveData);
#endif // FN_SaveGlobalState

#ifdef FN_RestoreGlobalState
void FN_RestoreGlobalState(SAVERESTOREDATA *pSaveData);
#endif // FN_RestoreGlobalState

#ifdef FN_ResetGlobalState
void FN_ResetGlobalState(void);
#endif // FN_ResetGlobalState

#ifdef FN_ClientConnect
BOOL FN_ClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]);
#endif // FN_ClientConnect

#ifdef FN_ClientDisconnect
void FN_ClientDisconnect(edict_t *pEntity);
#endif // FN_ClientDisconnect

#ifdef FN_ClientKill
void FN_ClientKill(edict_t *pEntity);
#endif // FN_ClientKill

#ifdef FN_ClientPutInServer
void FN_ClientPutInServer(edict_t *pEntity);
#endif // FN_ClientPutInServer

#ifdef FN_ClientCommand
void FN_ClientCommand(edict_t *pEntity);
#endif // FN_ClientCommand

#ifdef FN_ClientUserInfoChanged
void FN_ClientUserInfoChanged(edict_t *pEntity, char *infobuffer);
#endif // FN_ClientUserInfoChanged

#ifdef FN_ServerActivate
void FN_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
#endif // FN_ServerActivate

#ifdef FN_ServerDeactivate
void FN_ServerDeactivate(void);
#endif // FN_ServerDeactivate

#ifdef FN_PlayerPreThink
void FN_PlayerPreThink(edict_t *pEntity);
#endif // FN_PlayerPreThink

#ifdef FN_PlayerPostThink
void FN_PlayerPostThink(edict_t *pEntity);
#endif // FN_PlayerPostThink

#ifdef FN_StartFrame
void FN_StartFrame(void);
#endif // FN_StartFrame

#ifdef FN_ParmsNewLevel
void FN_ParmsNewLevel(void);
#endif // FN_ParmsNewLevel

#ifdef FN_ParmsChangeLevel
void FN_ParmsChangeLevel(void);
#endif // FN_ParmsChangeLevel

#ifdef FN_GetGameDescription
const char *FN_GetGameDescription(void);
#endif // FN_GetGameDescription

#ifdef FN_PlayerCustomization
void FN_PlayerCustomization(edict_t *pEntity, customization_t *pCust);
#endif // FN_PlayerCustomization

#ifdef FN_SpectatorConnect
void FN_SpectatorConnect(edict_t *pEntity);
#endif // FN_SpectatorConnect

#ifdef FN_SpectatorDisconnect
void FN_SpectatorDisconnect(edict_t *pEntity);
#endif // FN_SpectatorDisconnect

#ifdef FN_SpectatorThink
void FN_SpectatorThink(edict_t *pEntity);
#endif // FN_SpectatorThink

#ifdef FN_Sys_Error
void FN_Sys_Error(const char *error_string);
#endif // FN_Sys_Error

#ifdef FN_PM_Move
void FN_PM_Move(struct playermove_s *ppmove, int server);
#endif // FN_PM_Move

#ifdef FN_PM_Init
void FN_PM_Init(struct playermove_s *ppmove);
#endif // FN_PM_Init

#ifdef FN_PM_FindTextureType
char FN_PM_FindTextureType(char *name);
#endif // FN_PM_FindTextureType

#ifdef FN_SetupVisibility
void FN_SetupVisibility(edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas);
#endif // FN_SetupVisibility

#ifdef FN_UpdateClientData
void FN_UpdateClientData(const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
#endif // FN_UpdateClientData

#ifdef FN_AddToFullPack
int FN_AddToFullPack(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet);
#endif // FN_AddToFullPack

#ifdef FN_CreateBaseline
void FN_CreateBaseline(int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs);
#endif // FN_CreateBaseline

#ifdef FN_RegisterEncoders
void FN_RegisterEncoders(void);
#endif // FN_RegisterEncoders

#ifdef FN_GetWeaponData
int FN_GetWeaponData(struct edict_s *player, struct weapon_data_s *info);
#endif // FN_GetWeaponData

#ifdef FN_CmdStart
void FN_CmdStart(const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed);
#endif // FN_CmdStart

#ifdef FN_CmdEnd
void FN_CmdEnd(const edict_t *player);
#endif // FN_CmdEnd

#ifdef FN_ConnectionlessPacket
int FN_ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
#endif // FN_ConnectionlessPacket

#ifdef FN_GetHullBounds
int FN_GetHullBounds(int hullnumber, float *mins, float *maxs);
#endif // FN_GetHullBounds

#ifdef FN_CreateInstancedBaselines
void FN_CreateInstancedBaselines(void);
#endif // FN_CreateInstancedBaselines

#ifdef FN_InconsistentFile
int FN_InconsistentFile(const edict_t *player, const char *filename, char *disconnect_message);
#endif // FN_InconsistentFile

#ifdef FN_AllowLagCompensation
int FN_AllowLagCompensation(void);
#endif // FN_AllowLagCompensation




#ifdef FN_GameDLLInit_Post
void FN_GameDLLInit_Post(void);
#endif // FN_GameDLLInit_Post

#ifdef FN_DispatchSpawn_Post
int FN_DispatchSpawn_Post(edict_t *pent);
#endif // FN_DispatchSpawn_Post

#ifdef FN_DispatchThink_Post
void FN_DispatchThink_Post(edict_t *pent);
#endif // FN_DispatchThink_Post

#ifdef FN_DispatchUse_Post
void FN_DispatchUse_Post(edict_t *pentUser, edict_t *pentOther);
#endif // FN_DispatchUse_Post

#ifdef FN_DispatchTouch_Post
void FN_DispatchTouch_Post(edict_t *pentTouched, edict_t *pentOther);
#endif // FN_DispatchTouch_Post

#ifdef FN_DispatchBlocked_Post
void FN_DispatchBlocked_Post(edict_t *pentBlocked, edict_t *pentOther);
#endif // FN_DispatchBlocked_Post

#ifdef FN_DispatchKeyValue_Post
void FN_DispatchKeyValue_Post(edict_t *pentKeyvalue, KeyValueData *pkvd);
#endif // FN_DispatchKeyValue_Post

#ifdef FN_DispatchSave_Post
void FN_DispatchSave_Post(edict_t *pent, SAVERESTOREDATA *pSaveData);
#endif // FN_DispatchSave_Post

#ifdef FN_DispatchRestore_Post
int FN_DispatchRestore_Post(edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity);
#endif // FN_DispatchRestore_Post

#ifdef FN_DispatchObjectCollsionBox_Post
void FN_DispatchObjectCollsionBox_Post(edict_t *pent);
#endif // FN_DispatchObjectCollsionBox_Post

#ifdef FN_SaveWriteFields_Post
void FN_SaveWriteFields_Post(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount);
#endif // FN_SaveWriteFields_Post

#ifdef FN_SaveReadFields_Post
void FN_SaveReadFields_Post(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount);
#endif // FN_SaveReadFields_Post

#ifdef FN_SaveGlobalState_Post
void FN_SaveGlobalState_Post(SAVERESTOREDATA *pSaveData);
#endif // FN_SaveGlobalState_Post

#ifdef FN_RestoreGlobalState_Post
void FN_RestoreGlobalState_Post(SAVERESTOREDATA *pSaveData);
#endif // FN_RestoreGlobalState_Post

#ifdef FN_ResetGlobalState_Post
void FN_ResetGlobalState_Post(void);
#endif // FN_ResetGlobalState_Post

#ifdef FN_ClientConnect_Post
BOOL FN_ClientConnect_Post(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]);
#endif // FN_ClientConnect_Post

#ifdef FN_ClientDisconnect_Post
void FN_ClientDisconnect_Post(edict_t *pEntity);
#endif // FN_ClientDisconnect_Post

#ifdef FN_ClientKill_Post
void FN_ClientKill_Post(edict_t *pEntity);
#endif // FN_ClientKill_Post

#ifdef FN_ClientPutInServer_Post
void FN_ClientPutInServer_Post(edict_t *pEntity);
#endif // FN_ClientPutInServer_Post

#ifdef FN_ClientCommand_Post
void FN_ClientCommand_Post(edict_t *pEntity);
#endif // FN_ClientCommand_Post

#ifdef FN_ClientUserInfoChanged_Post
void FN_ClientUserInfoChanged_Post(edict_t *pEntity, char *infobuffer);
#endif // FN_ClientUserInfoChanged_Post

#ifdef FN_ServerActivate_Post
void FN_ServerActivate_Post(edict_t *pEdictList, int edictCount, int clientMax);
#endif // FN_ServerActivate_Post

#ifdef FN_ServerDeactivate_Post
void FN_ServerDeactivate_Post(void);
#endif // FN_ServerDeactivate_Post

#ifdef FN_PlayerPreThink_Post
void FN_PlayerPreThink_Post(edict_t *pEntity);
#endif // FN_PlayerPreThink_Post

#ifdef FN_PlayerPostThink_Post
void FN_PlayerPostThink_Post(edict_t *pEntity);
#endif // FN_PlayerPostThink_Post

#ifdef FN_StartFrame_Post
void FN_StartFrame_Post(void);
#endif // FN_StartFrame_Post

#ifdef FN_ParmsNewLevel_Post
void FN_ParmsNewLevel_Post(void);
#endif // FN_ParmsNewLevel_Post

#ifdef FN_ParmsChangeLevel_Post
void FN_ParmsChangeLevel_Post(void);
#endif // FN_ParmsChangeLevel_Post

#ifdef FN_GetGameDescription_Post
const char *FN_GetGameDescription_Post(void);
#endif // FN_GetGameDescription_Post

#ifdef FN_PlayerCustomization_Post
void FN_PlayerCustomization_Post(edict_t *pEntity, customization_t *pCust);
#endif // FN_PlayerCustomization_Post

#ifdef FN_SpectatorConnect_Post
void FN_SpectatorConnect_Post(edict_t *pEntity);
#endif // FN_SpectatorConnect_Post

#ifdef FN_SpectatorDisconnect_Post
void FN_SpectatorDisconnect_Post(edict_t *pEntity);
#endif // FN_SpectatorDisconnect_Post

#ifdef FN_SpectatorThink_Post
void FN_SpectatorThink_Post(edict_t *pEntity);
#endif // FN_SpectatorThink_Post

#ifdef FN_Sys_Error_Post
void FN_Sys_Error_Post(const char *error_string);
#endif // FN_Sys_Error_Post

#ifdef FN_PM_Move_Post
void FN_PM_Move_Post(struct playermove_s *ppmove, int server);
#endif // FN_PM_Move_Post

#ifdef FN_PM_Init_Post
void FN_PM_Init_Post(struct playermove_s *ppmove);
#endif // FN_PM_Init_Post

#ifdef FN_PM_FindTextureType_Post
char FN_PM_FindTextureType_Post(char *name);
#endif // FN_PM_FindTextureType_Post

#ifdef FN_SetupVisibility_Post
void FN_SetupVisibility_Post(edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas);
#endif // FN_SetupVisibility_Post

#ifdef FN_UpdateClientData_Post
void FN_UpdateClientData_Post(const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
#endif // FN_UpdateClientData_Post

#ifdef FN_AddToFullPack_Post
int FN_AddToFullPack_Post(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet);
#endif // FN_AddToFullPack_Post

#ifdef FN_CreateBaseline_Post
void FN_CreateBaseline_Post(int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs);
#endif // FN_CreateBaseline_Post

#ifdef FN_RegisterEncoders_Post
void FN_RegisterEncoders_Post(void);
#endif // FN_RegisterEncoders_Post

#ifdef FN_GetWeaponData_Post
int FN_GetWeaponData_Post(struct edict_s *player, struct weapon_data_s *info);
#endif // FN_GetWeaponData_Post

#ifdef FN_CmdStart_Post
void FN_CmdStart_Post(const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed);
#endif // FN_CmdStart_Post

#ifdef FN_CmdEnd_Post
void FN_CmdEnd_Post(const edict_t *player);
#endif // FN_CmdEnd_Post

#ifdef FN_ConnectionlessPacket_Post
int FN_ConnectionlessPacket_Post(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
#endif // FN_ConnectionlessPacket_Post

#ifdef FN_GetHullBounds_Post
int FN_GetHullBounds_Post(int hullnumber, float *mins, float *maxs);
#endif // FN_GetHullBounds_Post

#ifdef FN_CreateInstancedBaselines_Post
void FN_CreateInstancedBaselines_Post(void);
#endif // FN_CreateInstancedBaselines_Post

#ifdef FN_InconsistentFile_Post
int FN_InconsistentFile_Post(const edict_t *player, const char *filename, char *disconnect_message);
#endif // FN_InconsistentFile_Post

#ifdef FN_AllowLagCompensation_Post
int FN_AllowLagCompensation_Post(void);
#endif // FN_AllowLagCompensation_Post



#ifdef FN_PrecacheModel
int FN_PrecacheModel(char *s);
#endif // FN_PrecacheModel

#ifdef FN_PrecacheSound
int FN_PrecacheSound(char *s);
#endif // FN_PrecacheSound

#ifdef FN_SetModel
void FN_SetModel(edict_t *e, const char *m);
#endif // FN_SetModel

#ifdef FN_ModelIndex
int FN_ModelIndex(const char *m);
#endif // FN_ModelIndex

#ifdef FN_ModelFrames
int FN_ModelFrames(int modelIndex);
#endif // FN_ModelFrames

#ifdef FN_SetSize
void FN_SetSize(edict_t *e, const float *rgflMin, const float *rgflMax);
#endif // FN_SetSize

#ifdef FN_ChangeLevel
void FN_ChangeLevel(char *s1, char *s2);
#endif // FN_ChangeLevel

#ifdef FN_GetSpawnParms
void FN_GetSpawnParms(edict_t *ent);
#endif // FN_GetSpawnParms

#ifdef FN_SaveSpawnParms
void FN_SaveSpawnParms(edict_t *ent);
#endif // FN_SaveSpawnParms

#ifdef FN_VecToYaw
float FN_VecToYaw(const float *rgflVector);
#endif // FN_VecToYaw

#ifdef FN_VecToAngles
void FN_VecToAngles(const float *rgflVectorIn, float *rgflVectorOut);
#endif // FN_VecToAngles

#ifdef FN_MoveToOrigin
void FN_MoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType);
#endif // FN_MoveToOrigin

#ifdef FN_ChangeYaw
void FN_ChangeYaw(edict_t *ent);
#endif // FN_ChangeYaw

#ifdef FN_ChangePitch
void FN_ChangePitch(edict_t *ent);
#endif // FN_ChangePitch

#ifdef FN_FindEntityByString
edict_t *FN_FindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
#endif // FN_FindEntityByString

#ifdef FN_GetEntityIllum
int FN_GetEntityIllum(edict_t *pEnt);
#endif // FN_GetEntityIllum

#ifdef FN_FindEntityInSphere
edict_t *FN_FindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad);
#endif // FN_FindEntityInSphere

#ifdef FN_FindClientInPVS
edict_t *FN_FindClientInPVS(edict_t *pEdict);
#endif // FN_FindClientInPVS

#ifdef FN_EntitiesInPVS
edict_t *FN_EntitiesInPVS(edict_t *pplayer);
#endif // FN_EntitiesInPVS

#ifdef FN_MakeVectors
void FN_MakeVectors(const float *rgflVector);
#endif // FN_MakeVectors

#ifdef FN_AngleVectors
void FN_AngleVectors(const float *rgflVector, float *forward, float *right, float *up);
#endif // FN_AngleVectors

#ifdef FN_CreateEntity
edict_t *FN_CreateEntity(void);
#endif // FN_CreateEntity

#ifdef FN_RemoveEntity
void FN_RemoveEntity(edict_t *e);
#endif // FN_RemoveEntity

#ifdef FN_CreateNamedEntity
edict_t *FN_CreateNamedEntity(int className);
#endif // FN_CreateNamedEntity

#ifdef FN_MakeStatic
void FN_MakeStatic(edict_t *ent);
#endif // FN_MakeStatic

#ifdef FN_EntIsOnFloor
int FN_EntIsOnFloor(edict_t *ent);
#endif // FN_EntIsOnFloor

#ifdef FN_DropToFloor
int FN_DropToFloor(edict_t *ent);
#endif // FN_DropToFloor

#ifdef FN_WalkMove
int FN_WalkMove(edict_t *ent, float yaw, float dist, int iMode);
#endif // FN_WalkMove

#ifdef FN_SetOrigin
void FN_SetOrigin(edict_t *e, const float *rgflOrigin);
#endif // FN_SetOrigin

#ifdef FN_EmitSound
void FN_EmitSound(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
#endif // FN_EmitSound

#ifdef FN_EmitAmbientSound
void FN_EmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);
#endif // FN_EmitAmbientSound

#ifdef FN_TraceLine
void FN_TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
#endif // FN_TraceLine

#ifdef FN_TraceToss
void FN_TraceToss(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr);
#endif // FN_TraceToss

#ifdef FN_TraceMonsterHull
int FN_TraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
#endif // FN_TraceMonsterHull

#ifdef FN_TraceHull
void FN_TraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
#endif // FN_TraceHull

#ifdef FN_TraceModel
void FN_TraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
#endif // FN_TraceModel

#ifdef FN_TraceTexture
const char *FN_TraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2 );
#endif // FN_TraceTexture

#ifdef FN_TraceSphere
void FN_TraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);
#endif // FN_TraceSphere

#ifdef FN_GetAimVector
void FN_GetAimVector(edict_t *ent, float speed, float *rgflReturn);
#endif // FN_GetAimVector

#ifdef FN_ServerCommand
void FN_ServerCommand(char *str);
#endif // FN_ServerCommand

#ifdef FN_ServerExecute
void FN_ServerExecute(void);
#endif // FN_ServerExecute

#ifdef FN_engClientCommand
void FN_engClientCommand(edict_t *pEdict, char *szFmt, ...);
#endif // FN_engClientCommand

#ifdef FN_ParticleEffect
void FN_ParticleEffect(const float *org, const float *dir, float color, float count);
#endif // FN_ParticleEffect

#ifdef FN_LightStyle
void FN_LightStyle(int style, char *val);
#endif // FN_LightStyle

#ifdef FN_DecalIndex
int FN_DecalIndex(const char *name);
#endif // FN_DecalIndex

#ifdef FN_PointContents
int FN_PointContents(const float *rgflVector);
#endif // FN_PointContents

#ifdef FN_MessageBegin
void FN_MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
#endif // FN_MessageBegin

#ifdef FN_MessageEnd
void FN_MessageEnd(void);
#endif // FN_MessageEnd

#ifdef FN_WriteByte
void FN_WriteByte(int iValue);
#endif // FN_WriteByte

#ifdef FN_WriteChar
void FN_WriteChar(int iValue);
#endif // FN_WriteChar

#ifdef FN_WriteShort
void FN_WriteShort(int iValue);
#endif // FN_WriteShort

#ifdef FN_WriteLong
void FN_WriteLong(int iValue);
#endif // FN_WriteLong

#ifdef FN_WriteAngle
void FN_WriteAngle(float flValue);
#endif // FN_WriteAngle

#ifdef FN_WriteCoord
void FN_WriteCoord(float flValue);
#endif // FN_WriteCoord

#ifdef FN_WriteString
void FN_WriteString(const char *sz);
#endif // FN_WriteString

#ifdef FN_WriteEntity
void FN_WriteEntity(int iValue);
#endif // FN_WriteEntity

#ifdef FN_CVarRegister
void FN_CVarRegister(cvar_t *pCvar);
#endif // FN_CVarRegister

#ifdef FN_CVarGetFloat
float FN_CVarGetFloat(const char *szVarName);
#endif // FN_CVarGetFloat

#ifdef FN_CVarGetString
const char *FN_CVarGetString(const char *szVarName);
#endif // FN_CVarGetString

#ifdef FN_CVarSetFloat
void FN_CVarSetFloat(const char *szVarName, float flValue);
#endif // FN_CVarSetFloat

#ifdef FN_CVarSetString
void FN_CVarSetString(const char *szVarName, const char *szValue);
#endif // FN_CVarSetString

#ifdef FN_AlertMessage
void FN_AlertMessage(ALERT_TYPE atype, char *szFmt, ...);
#endif // FN_AlertMessage

#ifdef FN_EngineFprintf
void FN_EngineFprintf(FILE *pfile, char *szFmt, ...);
#endif // FN_EngineFprintf

#ifdef FN_PvAllocEntPrivateData
void *FN_PvAllocEntPrivateData(edict_t *pEdict, int32 cb);
#endif // FN_PvAllocEntPrivateData

#ifdef FN_PvEntPrivateData
void *FN_PvEntPrivateData(edict_t *pEdict);
#endif // FN_PvEntPrivateData

#ifdef FN_FreeEntPrivateData
void FN_FreeEntPrivateData(edict_t *pEdict);
#endif // FN_FreeEntPrivateData

#ifdef FN_SzFromIndex
const char *FN_SzFromIndex(int iString);
#endif // FN_SzFromIndex

#ifdef FN_AllocString
int FN_AllocString(const char *szValue);
#endif // FN_AllocString

#ifdef FN_GetVarsOfEnt
struct entvars_s *FN_GetVarsOfEnt(edict_t *pEdict);
#endif // FN_GetVarsOfEnt

#ifdef FN_PEntityOfEntOffset
edict_t *FN_PEntityOfEntOffset(int iEntOffset);
#endif // FN_PEntityOfEntOffset

#ifdef FN_EntOffsetOfPEntity
int FN_EntOffsetOfPEntity(const edict_t *pEdict);
#endif // FN_EntOffsetOfPEntity

#ifdef FN_IndexOfEdict
int FN_IndexOfEdict(const edict_t *pEdict);
#endif // FN_IndexOfEdict

#ifdef FN_PEntityOfEntIndex
edict_t *FN_PEntityOfEntIndex(int iEntIndex);
#endif // FN_PEntityOfEntIndex

#ifdef FN_FindEntityByVars
edict_t *FN_FindEntityByVars(struct entvars_s *pvars);
#endif // FN_FindEntityByVars

#ifdef FN_GetModelPtr
void *FN_GetModelPtr(edict_t *pEdict);
#endif // FN_GetModelPtr

#ifdef FN_RegUserMsg
int FN_RegUserMsg(const char *pszName, int iSize);
#endif // FN_RegUserMsg

#ifdef FN_AnimationAutomove
void FN_AnimationAutomove(const edict_t *pEdict, float flTime);
#endif // FN_AnimationAutomove

#ifdef FN_GetBonePosition
void FN_GetBonePosition(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles);
#endif // FN_GetBonePosition

#ifdef FN_FunctionFromName
unsigned long FN_FunctionFromName(const char *pName);
#endif // FN_FunctionFromName

#ifdef FN_NameForFunction
const char *FN_NameForFunction(unsigned long function);
#endif // FN_NameForFunction

#ifdef FN_ClientPrintf
void FN_ClientPrintf(edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg);
#endif // FN_ClientPrintf

#ifdef FN_ServerPrint
void FN_ServerPrint(const char *szMsg);
#endif // FN_ServerPrint

#ifdef FN_Cmd_Args
const char *FN_Cmd_Args(void);
#endif // FN_Cmd_Args

#ifdef FN_Cmd_Argv
const char *FN_Cmd_Argv(int argc);
#endif // FN_Cmd_Argv

#ifdef FN_Cmd_Argc
int FN_Cmd_Argc(void);
#endif // FN_Cmd_Argc

#ifdef FN_GetAttachment
void FN_GetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles );
#endif // FN_GetAttachment

#ifdef FN_CRC32_Init
void FN_CRC32_Init(CRC32_t *pulCRC);
#endif // FN_CRC32_Init

#ifdef FN_CRC32_ProcessBuffer
void FN_CRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len);
#endif // FN_CRC32_ProcessBuffer

#ifdef FN_CRC32_ProcessByte
void FN_CRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch);
#endif // FN_CRC32_ProcessByte

#ifdef FN_CRC32_Final
CRC32_t FN_CRC32_Final(CRC32_t pulCRC);
#endif // FN_CRC32_Final

#ifdef FN_RandomLong
long FN_RandomLong(long lLow, long lHigh);
#endif // FN_RandomLong

#ifdef FN_RandomFloat
float FN_RandomFloat(float flLow, float flHigh);
#endif // FN_RandomFloat

#ifdef FN_SetView
void FN_SetView(const edict_t *pClient, const edict_t *pViewent);
#endif // FN_SetView

#ifdef FN_Time
float FN_Time(void);
#endif // FN_Time

#ifdef FN_CrosshairAngle
void FN_CrosshairAngle(const edict_t *pClient, float pitch, float yaw);
#endif // FN_CrosshairAngle

#ifdef FN_LoadFileForMe
byte *FN_LoadFileForMe(char *filename, int *pLength);
#endif // FN_LoadFileForMe

#ifdef FN_FreeFile
void FN_FreeFile(void *buffer);
#endif // FN_FreeFile

#ifdef FN_EndSection
void FN_EndSection(const char *pszSectionName);
#endif // FN_EndSection

#ifdef FN_CompareFileTime
int FN_CompareFileTime(char *filename1, char *filename2, int *iCompare);
#endif // FN_CompareFileTime

#ifdef FN_GetGameDir
void FN_GetGameDir(char *szGetGameDir);
#endif // FN_GetGameDir

#ifdef FN_Cvar_RegisterVariable
void FN_Cvar_RegisterVariable(cvar_t *variable);
#endif // FN_Cvar_RegisterVariable

#ifdef FN_FadeClientVolume
void FN_FadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
#endif // FN_FadeClientVolume

#ifdef FN_SetClientMaxspeed
void FN_SetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed);
#endif // FN_SetClientMaxspeed

#ifdef FN_CreateFakeClient
edict_t *FN_CreateFakeClient(const char *netname);
#endif // FN_CreateFakeClient

#ifdef FN_RunPlayerMove
void FN_RunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec);
#endif // FN_RunPlayerMove

#ifdef FN_NumberOfEntities
int FN_NumberOfEntities(void);
#endif // FN_NumberOfEntities

#ifdef FN_GetInfoKeyBuffer
char *FN_GetInfoKeyBuffer(edict_t *e);
#endif // FN_GetInfoKeyBuffer

#ifdef FN_InfoKeyValue
char *FN_InfoKeyValue(char *infobuffer, char *key);
#endif // FN_InfoKeyValue

#ifdef FN_SetKeyValue
void FN_SetKeyValue(char *infobuffer, char *key, char *value);
#endif // FN_SetKeyValue

#ifdef FN_SetClientKeyValue
void FN_SetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value);
#endif // FN_SetClientKeyValue

#ifdef FN_IsMapValid
int FN_IsMapValid(char *filename);
#endif // FN_IsMapValid

#ifdef FN_StaticDecal
void FN_StaticDecal(const float *origin, int decalIndex, int entityIndex, int modelIndex);
#endif // FN_StaticDecal

#ifdef FN_PrecacheGeneric
int FN_PrecacheGeneric(char *s);
#endif // FN_PrecacheGeneric

#ifdef FN_GetPlayerUserId
int FN_GetPlayerUserId(edict_t *e );
#endif // FN_GetPlayerUserId

#ifdef FN_BuildSoundMsg
void FN_BuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
#endif // FN_BuildSoundMsg

#ifdef FN_IsDedicatedServer
int FN_IsDedicatedServer(void);
#endif // FN_IsDedicatedServer

#ifdef FN_CVarGetPointer
cvar_t *FN_CVarGetPointer(const char *szVarName);
#endif // FN_CVarGetPointer

#ifdef FN_GetPlayerWONId
unsigned int FN_GetPlayerWONId(edict_t *e);
#endif // FN_GetPlayerWONId

#ifdef FN_Info_RemoveKey
void FN_Info_RemoveKey( char *s, const char *key);
#endif // FN_Info_RemoveKey

#ifdef FN_GetPhysicsKeyValue
const char *FN_GetPhysicsKeyValue(const edict_t *pClient, const char *key);
#endif // FN_GetPhysicsKeyValue

#ifdef FN_SetPhysicsKeyValue
void FN_SetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value);
#endif // FN_SetPhysicsKeyValue

#ifdef FN_GetPhysicsInfoString
const char *FN_GetPhysicsInfoString( const edict_t *pClient);
#endif // FN_GetPhysicsInfoString

#ifdef FN_PrecacheEvent
unsigned short FN_PrecacheEvent(int type, const char *psz);
#endif // FN_PrecacheEvent

#ifdef FN_PlaybackEvent
void FN_PlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
#endif // FN_PlaybackEvent

#ifdef FN_SetFatPVS
unsigned char *FN_SetFatPVS(float *org);
#endif // FN_SetFatPVS

#ifdef FN_SetFatPAS
unsigned char *FN_SetFatPAS(float *org);
#endif // FN_SetFatPAS

#ifdef FN_CheckVisibility
int FN_CheckVisibility(const edict_t *entity, unsigned char *pset);
#endif // FN_CheckVisibility

#ifdef FN_DeltaSetField
void FN_DeltaSetField(struct delta_s *pFields, const char *fieldname);
#endif // FN_DeltaSetField

#ifdef FN_DeltaUnsetField
void FN_DeltaUnsetField(struct delta_s *pFields, const char *fieldname);
#endif // FN_DeltaUnsetField

#ifdef FN_DeltaAddEncoder
void FN_DeltaAddEncoder(char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) );
#endif // FN_DeltaAddEncoder

#ifdef FN_GetCurrentPlayer
int FN_GetCurrentPlayer(void);
#endif // FN_GetCurrentPlayer

#ifdef FN_CanSkipPlayer
int FN_CanSkipPlayer(const edict_t *player);
#endif // FN_CanSkipPlayer

#ifdef FN_DeltaFindField
int FN_DeltaFindField(struct delta_s *pFields, const char *fieldname);
#endif // FN_DeltaFindField

#ifdef FN_DeltaSetFieldByIndex
void FN_DeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber);
#endif // FN_DeltaSetFieldByIndex

#ifdef FN_DeltaUnsetFieldByIndex
void FN_DeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber);
#endif // FN_DeltaUnsetFieldByIndex

#ifdef FN_SetGroupMask
void FN_SetGroupMask(int mask, int op);
#endif // FN_SetGroupMask

#ifdef FN_engCreateInstancedBaseline
int FN_engCreateInstancedBaseline(int classname, struct entity_state_s *baseline);
#endif // FN_engCreateInstancedBaseline

#ifdef FN_Cvar_DirectSet
void FN_Cvar_DirectSet(struct cvar_s *var, char *value);
#endif // FN_Cvar_DirectSet

#ifdef FN_ForceUnmodified
void FN_ForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename);
#endif // FN_ForceUnmodified

#ifdef FN_GetPlayerStats
void FN_GetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss);
#endif // FN_GetPlayerStats

#ifdef FN_AddServerCommand
void FN_AddServerCommand(char *cmd_name, void (*function) (void));
#endif // FN_AddServerCommand

#ifdef FN_Voice_GetClientListening
qboolean FN_Voice_GetClientListening(int iReceiver, int iSender);
#endif // FN_Voice_GetClientListening

#ifdef FN_Voice_SetClientListening
qboolean FN_Voice_SetClientListening(int iReceiver, int iSender, qboolean bListen);
#endif // FN_Voice_SetClientListening

#ifdef FN_GetPlayerAuthId
const char *FN_GetPlayerAuthId(edict_t *e);
#endif // FN_GetPlayerAuthId






#ifdef FN_PrecacheModel_Post
int FN_PrecacheModel_Post(char *s);
#endif // FN_PrecacheModel_Post

#ifdef FN_PrecacheSound_Post
int FN_PrecacheSound_Post(char *s);
#endif // FN_PrecacheSound_Post

#ifdef FN_SetModel_Post
void FN_SetModel_Post(edict_t *e, const char *m);
#endif // FN_SetModel_Post

#ifdef FN_ModelIndex_Post
int FN_ModelIndex_Post(const char *m);
#endif // FN_ModelIndex_Post

#ifdef FN_ModelFrames_Post
int FN_ModelFrames_Post(int modelIndex);
#endif // FN_ModelFrames_Post

#ifdef FN_SetSize_Post
void FN_SetSize_Post(edict_t *e, const float *rgflMin, const float *rgflMax);
#endif // FN_SetSize_Post

#ifdef FN_ChangeLevel_Post
void FN_ChangeLevel_Post(char *s1, char *s2);
#endif // FN_ChangeLevel_Post

#ifdef FN_GetSpawnParms_Post
void FN_GetSpawnParms_Post(edict_t *ent);
#endif // FN_GetSpawnParms_Post

#ifdef FN_SaveSpawnParms_Post
void FN_SaveSpawnParms_Post(edict_t *ent);
#endif // FN_SaveSpawnParms_Post

#ifdef FN_VecToYaw_Post
float FN_VecToYaw_Post(const float *rgflVector);
#endif // FN_VecToYaw_Post

#ifdef FN_VecToAngles_Post
void FN_VecToAngles_Post(const float *rgflVectorIn, float *rgflVectorOut);
#endif // FN_VecToAngles_Post

#ifdef FN_MoveToOrigin_Post
void FN_MoveToOrigin_Post(edict_t *ent, const float *pflGoal, float dist, int iMoveType);
#endif // FN_MoveToOrigin_Post

#ifdef FN_ChangeYaw_Post
void FN_ChangeYaw_Post(edict_t *ent);
#endif // FN_ChangeYaw_Post

#ifdef FN_ChangePitch_Post
void FN_ChangePitch_Post(edict_t *ent);
#endif // FN_ChangePitch_Post

#ifdef FN_FindEntityByString_Post
edict_t *FN_FindEntityByString_Post(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
#endif // FN_FindEntityByString_Post

#ifdef FN_GetEntityIllum_Post
int FN_GetEntityIllum_Post(edict_t *pEnt);
#endif // FN_GetEntityIllum_Post

#ifdef FN_FindEntityInSphere_Post
edict_t *FN_FindEntityInSphere_Post(edict_t *pEdictStartSearchAfter, const float *org, float rad);
#endif // FN_FindEntityInSphere_Post

#ifdef FN_FindClientInPVS_Post
edict_t *FN_FindClientInPVS_Post(edict_t *pEdict);
#endif // FN_FindClientInPVS_Post

#ifdef FN_EntitiesInPVS_Post
edict_t *FN_EntitiesInPVS_Post(edict_t *pplayer);
#endif // FN_EntitiesInPVS_Post

#ifdef FN_MakeVectors_Post
void FN_MakeVectors_Post(const float *rgflVector);
#endif // FN_MakeVectors_Post

#ifdef FN_AngleVectors_Post
void FN_AngleVectors_Post(const float *rgflVector, float *forward, float *right, float *up);
#endif // FN_AngleVectors_Post

#ifdef FN_CreateEntity_Post
edict_t *FN_CreateEntity_Post(void);
#endif // FN_CreateEntity_Post

#ifdef FN_RemoveEntity_Post
void FN_RemoveEntity_Post(edict_t *e);
#endif // FN_RemoveEntity_Post

#ifdef FN_CreateNamedEntity_Post
edict_t *FN_CreateNamedEntity_Post(int className);
#endif // FN_CreateNamedEntity_Post

#ifdef FN_MakeStatic_Post
void FN_MakeStatic_Post(edict_t *ent);
#endif // FN_MakeStatic_Post

#ifdef FN_EntIsOnFloor_Post
int FN_EntIsOnFloor_Post(edict_t *ent);
#endif // FN_EntIsOnFloor_Post

#ifdef FN_DropToFloor_Post
int FN_DropToFloor_Post(edict_t *ent);
#endif // FN_DropToFloor_Post

#ifdef FN_WalkMove_Post
int FN_WalkMove_Post(edict_t *ent, float yaw, float dist, int iMode);
#endif // FN_WalkMove_Post

#ifdef FN_SetOrigin_Post
void FN_SetOrigin_Post(edict_t *e, const float *rgflOrigin);
#endif // FN_SetOrigin_Post

#ifdef FN_EmitSound_Post
void FN_EmitSound_Post(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
#endif // FN_EmitSound_Post

#ifdef FN_EmitAmbientSound_Post
void FN_EmitAmbientSound_Post(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);
#endif // FN_EmitAmbientSound_Post

#ifdef FN_TraceLine_Post
void FN_TraceLine_Post(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
#endif // FN_TraceLine_Post

#ifdef FN_TraceToss_Post
void FN_TraceToss_Post(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr);
#endif // FN_TraceToss_Post

#ifdef FN_TraceMonsterHull_Post
int FN_TraceMonsterHull_Post(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
#endif // FN_TraceMonsterHull_Post

#ifdef FN_TraceHull_Post
void FN_TraceHull_Post(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
#endif // FN_TraceHull_Post

#ifdef FN_TraceModel_Post
void FN_TraceModel_Post(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
#endif // FN_TraceModel_Post

#ifdef FN_TraceTexture_Post
const char *FN_TraceTexture_Post(edict_t *pTextureEntity, const float *v1, const float *v2 );
#endif // FN_TraceTexture_Post

#ifdef FN_TraceSphere_Post
void FN_TraceSphere_Post(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);
#endif // FN_TraceSphere_Post

#ifdef FN_GetAimVector_Post
void FN_GetAimVector_Post(edict_t *ent, float speed, float *rgflReturn);
#endif // FN_GetAimVector_Post

#ifdef FN_ServerCommand_Post
void FN_ServerCommand_Post(char *str);
#endif // FN_ServerCommand_Post

#ifdef FN_ServerExecute_Post
void FN_ServerExecute_Post(void);
#endif // FN_ServerExecute_Post

#ifdef FN_engClientCommand_Post
void FN_engClientCommand_Post(edict_t *pEdict, char *szFmt, ...);
#endif // FN_engClientCommand_Post

#ifdef FN_ParticleEffect_Post
void FN_ParticleEffect_Post(const float *org, const float *dir, float color, float count);
#endif // FN_ParticleEffect_Post

#ifdef FN_LightStyle_Post
void FN_LightStyle_Post(int style, char *val);
#endif // FN_LightStyle_Post

#ifdef FN_DecalIndex_Post
int FN_DecalIndex_Post(const char *name);
#endif // FN_DecalIndex_Post

#ifdef FN_PointContents_Post
int FN_PointContents_Post(const float *rgflVector);
#endif // FN_PointContents_Post

#ifdef FN_MessageBegin_Post
void FN_MessageBegin_Post(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
#endif // FN_MessageBegin_Post

#ifdef FN_MessageEnd_Post
void FN_MessageEnd_Post(void);
#endif // FN_MessageEnd_Post

#ifdef FN_WriteByte_Post
void FN_WriteByte_Post(int iValue);
#endif // FN_WriteByte_Post

#ifdef FN_WriteChar_Post
void FN_WriteChar_Post(int iValue);
#endif // FN_WriteChar_Post

#ifdef FN_WriteShort_Post
void FN_WriteShort_Post(int iValue);
#endif // FN_WriteShort_Post

#ifdef FN_WriteLong_Post
void FN_WriteLong_Post(int iValue);
#endif // FN_WriteLong_Post

#ifdef FN_WriteAngle_Post
void FN_WriteAngle_Post(float flValue);
#endif // FN_WriteAngle_Post

#ifdef FN_WriteCoord_Post
void FN_WriteCoord_Post(float flValue);
#endif // FN_WriteCoord_Post

#ifdef FN_WriteString_Post
void FN_WriteString_Post(const char *sz);
#endif // FN_WriteString_Post

#ifdef FN_WriteEntity_Post
void FN_WriteEntity_Post(int iValue);
#endif // FN_WriteEntity_Post

#ifdef FN_CVarRegister_Post
void FN_CVarRegister_Post(cvar_t *pCvar);
#endif // FN_CVarRegister_Post

#ifdef FN_CVarGetFloat_Post
float FN_CVarGetFloat_Post(const char *szVarName);
#endif // FN_CVarGetFloat_Post

#ifdef FN_CVarGetString_Post
const char *FN_CVarGetString_Post(const char *szVarName);
#endif // FN_CVarGetString_Post

#ifdef FN_CVarSetFloat_Post
void FN_CVarSetFloat_Post(const char *szVarName, float flValue);
#endif // FN_CVarSetFloat_Post

#ifdef FN_CVarSetString_Post
void FN_CVarSetString_Post(const char *szVarName, const char *szValue);
#endif // FN_CVarSetString_Post

#ifdef FN_AlertMessage_Post
void FN_AlertMessage_Post(ALERT_TYPE atype, char *szFmt, ...);
#endif // FN_AlertMessage_Post

#ifdef FN_EngineFprintf_Post
void FN_EngineFprintf_Post(FILE *pfile, char *szFmt, ...);
#endif // FN_EngineFprintf_Post

#ifdef FN_PvAllocEntPrivateData_Post
void *FN_PvAllocEntPrivateData_Post(edict_t *pEdict, long cb);
#endif // FN_PvAllocEntPrivateData_Post

#ifdef FN_PvEntPrivateData_Post
void *FN_PvEntPrivateData_Post(edict_t *pEdict);
#endif // FN_PvEntPrivateData_Post

#ifdef FN_FreeEntPrivateData_Post
void FN_FreeEntPrivateData_Post(edict_t *pEdict);
#endif // FN_FreeEntPrivateData_Post

#ifdef FN_SzFromIndex_Post
const char *FN_SzFromIndex_Post(int iString);
#endif // FN_SzFromIndex_Post

#ifdef FN_AllocString_Post
int FN_AllocString_Post(const char *szValue);
#endif // FN_AllocString_Post

#ifdef FN_GetVarsOfEnt_Post
struct entvars_s *FN_GetVarsOfEnt_Post(edict_t *pEdict);
#endif // FN_GetVarsOfEnt_Post

#ifdef FN_PEntityOfEntOffset_Post
edict_t *FN_PEntityOfEntOffset_Post(int iEntOffset);
#endif // FN_PEntityOfEntOffset_Post

#ifdef FN_EntOffsetOfPEntity_Post
int FN_EntOffsetOfPEntity_Post(const edict_t *pEdict);
#endif // FN_EntOffsetOfPEntity_Post

#ifdef FN_IndexOfEdict_Post
int FN_IndexOfEdict_Post(const edict_t *pEdict);
#endif // FN_IndexOfEdict_Post

#ifdef FN_PEntityOfEntIndex_Post
edict_t *FN_PEntityOfEntIndex_Post(int iEntIndex);
#endif // FN_PEntityOfEntIndex_Post

#ifdef FN_FindEntityByVars_Post
edict_t *FN_FindEntityByVars_Post(struct entvars_s *pvars);
#endif // FN_FindEntityByVars_Post

#ifdef FN_GetModelPtr_Post
void *FN_GetModelPtr_Post(edict_t *pEdict);
#endif // FN_GetModelPtr_Post

#ifdef FN_RegUserMsg_Post
int FN_RegUserMsg_Post(const char *pszName, int iSize);
#endif // FN_RegUserMsg_Post

#ifdef FN_AnimationAutomove_Post
void FN_AnimationAutomove_Post(const edict_t *pEdict, float flTime);
#endif // FN_AnimationAutomove_Post

#ifdef FN_GetBonePosition_Post
void FN_GetBonePosition_Post(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles);
#endif // FN_GetBonePosition_Post

#ifdef FN_FunctionFromName_Post
unsigned long FN_FunctionFromName_Post(const char *pName);
#endif // FN_FunctionFromName_Post

#ifdef FN_NameForFunction_Post
const char *FN_NameForFunction_Post(unsigned long function);
#endif // FN_NameForFunction_Post

#ifdef FN_ClientPrintf_Post
void FN_ClientPrintf_Post(edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg);
#endif // FN_ClientPrintf_Post

#ifdef FN_ServerPrint_Post
void FN_ServerPrint_Post(const char *szMsg);
#endif // FN_ServerPrint_Post

#ifdef FN_Cmd_Args_Post
const char *FN_Cmd_Args_Post(void);
#endif // FN_Cmd_Args_Post

#ifdef FN_Cmd_Argv_Post
const char *FN_Cmd_Argv_Post(int argc);
#endif // FN_Cmd_Argv_Post

#ifdef FN_Cmd_Argc_Post
int FN_Cmd_Argc_Post(void);
#endif // FN_Cmd_Argc_Post

#ifdef FN_GetAttachment_Post
void FN_GetAttachment_Post(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles );
#endif // FN_GetAttachment_Post

#ifdef FN_CRC32_Init_Post
void FN_CRC32_Init_Post(CRC32_t *pulCRC);
#endif // FN_CRC32_Init_Post

#ifdef FN_CRC32_ProcessBuffer_Post
void FN_CRC32_ProcessBuffer_Post(CRC32_t *pulCRC, void *p, int len);
#endif // FN_CRC32_ProcessBuffer_Post

#ifdef FN_CRC32_ProcessByte_Post
void FN_CRC32_ProcessByte_Post(CRC32_t *pulCRC, unsigned char ch);
#endif // FN_CRC32_ProcessByte_Post

#ifdef FN_CRC32_Final_Post
CRC32_t FN_CRC32_Final_Post(CRC32_t pulCRC);
#endif // FN_CRC32_Final_Post

#ifdef FN_RandomLong_Post
long FN_RandomLong_Post(long lLow, long lHigh);
#endif // FN_RandomLong_Post

#ifdef FN_RandomFloat_Post
float FN_RandomFloat_Post(float flLow, float flHigh);
#endif // FN_RandomFloat_Post

#ifdef FN_SetView_Post
void FN_SetView_Post(const edict_t *pClient, const edict_t *pViewent);
#endif // FN_SetView_Post

#ifdef FN_Time_Post
float FN_Time_Post(void);
#endif // FN_Time_Post

#ifdef FN_CrosshairAngle_Post
void FN_CrosshairAngle_Post(const edict_t *pClient, float pitch, float yaw);
#endif // FN_CrosshairAngle_Post

#ifdef FN_LoadFileForMe_Post
byte *FN_LoadFileForMe_Post(char *filename, int *pLength);
#endif // FN_LoadFileForMe_Post

#ifdef FN_FreeFile_Post
void FN_FreeFile_Post(void *buffer);
#endif // FN_FreeFile_Post

#ifdef FN_EndSection_Post
void FN_EndSection_Post(const char *pszSectionName);
#endif // FN_EndSection_Post

#ifdef FN_CompareFileTime_Post
int FN_CompareFileTime_Post(char *filename1, char *filename2, int *iCompare);
#endif // FN_CompareFileTime_Post

#ifdef FN_GetGameDir_Post
void FN_GetGameDir_Post(char *szGetGameDir);
#endif // FN_GetGameDir_Post

#ifdef FN_Cvar_RegisterVariable_Post
void FN_Cvar_RegisterVariable_Post(cvar_t *variable);
#endif // FN_Cvar_RegisterVariable_Post

#ifdef FN_FadeClientVolume_Post
void FN_FadeClientVolume_Post(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
#endif // FN_FadeClientVolume_Post

#ifdef FN_SetClientMaxspeed_Post
void FN_SetClientMaxspeed_Post(const edict_t *pEdict, float fNewMaxspeed);
#endif // FN_SetClientMaxspeed_Post

#ifdef FN_CreateFakeClient_Post
edict_t *FN_CreateFakeClient_Post(const char *netname);
#endif // FN_CreateFakeClient_Post

#ifdef FN_RunPlayerMove_Post
void FN_RunPlayerMove_Post(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec);
#endif // FN_RunPlayerMove_Post

#ifdef FN_NumberOfEntities_Post
int FN_NumberOfEntities_Post(void);
#endif // FN_NumberOfEntities_Post

#ifdef FN_GetInfoKeyBuffer_Post
char *FN_GetInfoKeyBuffer_Post(edict_t *e);
#endif // FN_GetInfoKeyBuffer_Post

#ifdef FN_InfoKeyValue_Post
char *FN_InfoKeyValue_Post(char *infobuffer, char *key);
#endif // FN_InfoKeyValue_Post

#ifdef FN_SetKeyValue_Post
void FN_SetKeyValue_Post(char *infobuffer, char *key, char *value);
#endif // FN_SetKeyValue_Post

#ifdef FN_SetClientKeyValue_Post
void FN_SetClientKeyValue_Post(int clientIndex, char *infobuffer, char *key, char *value);
#endif // FN_SetClientKeyValue_Post

#ifdef FN_IsMapValid_Post
int FN_IsMapValid_Post(char *filename);
#endif // FN_IsMapValid_Post

#ifdef FN_StaticDecal_Post
void FN_StaticDecal_Post(const float *origin, int decalIndex, int entityIndex, int modelIndex);
#endif // FN_StaticDecal_Post

#ifdef FN_PrecacheGeneric_Post
int FN_PrecacheGeneric_Post(char *s);
#endif // FN_PrecacheGeneric_Post

#ifdef FN_GetPlayerUserId_Post
int FN_GetPlayerUserId_Post(edict_t *e );
#endif // FN_GetPlayerUserId_Post

#ifdef FN_BuildSoundMsg_Post
void FN_BuildSoundMsg_Post(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
#endif // FN_BuildSoundMsg_Post

#ifdef FN_IsDedicatedServer_Post
int FN_IsDedicatedServer_Post(void);
#endif // FN_IsDedicatedServer_Post

#ifdef FN_CVarGetPointer_Post
cvar_t *FN_CVarGetPointer_Post(const char *szVarName);
#endif // FN_CVarGetPointer_Post

#ifdef FN_GetPlayerWONId_Post
unsigned int FN_GetPlayerWONId_Post(edict_t *e);
#endif // FN_GetPlayerWONId_Post

#ifdef FN_Info_RemoveKey_Post
void FN_Info_RemoveKey_Post( char *s, const char *key);
#endif // FN_Info_RemoveKey_Post

#ifdef FN_GetPhysicsKeyValue_Post
const char *FN_GetPhysicsKeyValue_Post(const edict_t *pClient, const char *key);
#endif // FN_GetPhysicsKeyValue_Post

#ifdef FN_SetPhysicsKeyValue_Post
void FN_SetPhysicsKeyValue_Post(const edict_t *pClient, const char *key, const char *value);
#endif // FN_SetPhysicsKeyValue_Post

#ifdef FN_GetPhysicsInfoString_Post
const char *FN_GetPhysicsInfoString_Post( const edict_t *pClient);
#endif // FN_GetPhysicsInfoString_Post

#ifdef FN_PrecacheEvent_Post
unsigned short FN_PrecacheEvent_Post(int type, const char *psz);
#endif // FN_PrecacheEvent_Post

#ifdef FN_PlaybackEvent_Post
void FN_PlaybackEvent_Post(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
#endif // FN_PlaybackEvent_Post

#ifdef FN_SetFatPVS_Post
unsigned char *FN_SetFatPVS_Post(float *org);
#endif // FN_SetFatPVS_Post

#ifdef FN_SetFatPAS_Post
unsigned char *FN_SetFatPAS_Post(float *org);
#endif // FN_SetFatPAS_Post

#ifdef FN_CheckVisibility_Post
int FN_CheckVisibility_Post(const edict_t *entity, unsigned char *pset);
#endif // FN_CheckVisibility_Post

#ifdef FN_DeltaSetField_Post
void FN_DeltaSetField_Post(struct delta_s *pFields, const char *fieldname);
#endif // FN_DeltaSetField_Post

#ifdef FN_DeltaUnsetField_Post
void FN_DeltaUnsetField_Post(struct delta_s *pFields, const char *fieldname);
#endif // FN_DeltaUnsetField_Post

#ifdef FN_DeltaAddEncoder_Post
void FN_DeltaAddEncoder_Post(char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) );
#endif // FN_DeltaAddEncoder_Post

#ifdef FN_GetCurrentPlayer_Post
int FN_GetCurrentPlayer_Post(void);
#endif // FN_GetCurrentPlayer_Post

#ifdef FN_CanSkipPlayer_Post
int FN_CanSkipPlayer_Post(const edict_t *player);
#endif // FN_CanSkipPlayer_Post

#ifdef FN_DeltaFindField_Post
int FN_DeltaFindField_Post(struct delta_s *pFields, const char *fieldname);
#endif // FN_DeltaFindField_Post

#ifdef FN_DeltaSetFieldByIndex_Post
void FN_DeltaSetFieldByIndex_Post(struct delta_s *pFields, int fieldNumber);
#endif // FN_DeltaSetFieldByIndex_Post

#ifdef FN_DeltaUnsetFieldByIndex_Post
void FN_DeltaUnsetFieldByIndex_Post(struct delta_s *pFields, int fieldNumber);
#endif // FN_DeltaUnsetFieldByIndex_Post

#ifdef FN_SetGroupMask_Post
void FN_SetGroupMask_Post(int mask, int op);
#endif // FN_SetGroupMask_Post

#ifdef FN_engCreateInstancedBaseline_Post
int FN_engCreateInstancedBaseline_Post(int classname, struct entity_state_s *baseline);
#endif // FN_engCreateInstancedBaseline_Post

#ifdef FN_Cvar_DirectSet_Post
void FN_Cvar_DirectSet_Post(struct cvar_s *var, char *value);
#endif // FN_Cvar_DirectSet_Post

#ifdef FN_ForceUnmodified_Post
void FN_ForceUnmodified_Post(FORCE_TYPE type, float *mins, float *maxs, const char *filename);
#endif // FN_ForceUnmodified_Post

#ifdef FN_GetPlayerStats_Post
void FN_GetPlayerStats_Post(const edict_t *pClient, int *ping, int *packet_loss);
#endif // FN_GetPlayerStats_Post

#ifdef FN_AddServerCommand_Post
void FN_AddServerCommand_Post(char *cmd_name, void (*function)(void));
#endif // FN_AddServerCommand_Post

#ifdef FN_Voice_GetClientListening_Post
qboolean FN_Voice_GetClientListening_Post(int iReceiver, int iSender);
#endif // FN_Voice_GetClientListening_Post

#ifdef FN_Voice_SetClientListening_Post
qboolean FN_Voice_SetClientListening_Post(int iReceiver, int iSender, qboolean bListen);
#endif // FN_Voice_SetClientListening_Post

#ifdef FN_GetPlayerAuthId_Post
const char *FN_GetPlayerAuthId_Post(edict_t *e);
#endif // FN_GetPlayerAuthId




#ifdef FN_OnFreeEntPrivateData
void FN_OnFreeEntPrivateData(edict_t *pEnt);
#endif // FN_OnFreeEntPrivateData

#ifdef FN_GameShutdown
void FN_GameShutdown(void);
#endif // FN_GameShutdown

#ifdef FN_ShouldCollide
int FN_ShouldCollide(edict_t *pentTouched, edict_t *pentOther);
#endif // FN_ShouldCollide





#ifdef FN_OnFreeEntPrivateData_Post
void FN_OnFreeEntPrivateData_Post(edict_t *pEnt);
#endif // FN_OnFreeEntPrivateData_Post

#ifdef FN_GameShutdown_Post
void FN_GameShutdown_Post(void);
#endif // FN_GameShutdown_Post

#ifdef FN_ShouldCollide_Post
int FN_ShouldCollide_Post(edict_t *pentTouched, edict_t *pentOther);
#endif // FN_ShouldCollide_Post

#endif // USE_METAMOD


#ifdef FN_AMX_QUERY
void FN_AMX_QUERY(void);
#endif // FN_AMX_QUERY

#ifdef FN_AMX_ATTACH
void FN_AMX_ATTACH(void);
#endif // FN_AMX_ATTACH

#ifdef FN_AMX_DETACH
void FN_AMX_DETACH(void);
#endif // FN_AMX_DETACH

#ifdef FN_AMX_PLUGINSLOADED
void FN_AMX_PLUGINSLOADED(void);
#endif // FN_AMX_PLUGINSLOADED


#endif // #ifndef amx_module_h_
