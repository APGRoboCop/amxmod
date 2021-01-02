// Module API By AMXMODX Dev Team
// The authors of AMX Mod X are:
// David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
// Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
// Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
// Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.


#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "amx_module.h"

/********************************** METAMOD ***********************************/

#ifdef USE_METAMOD

enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;

DLL_FUNCTIONS *g_pFunctionTable;
DLL_FUNCTIONS *g_pFunctionTable_Post;
enginefuncs_t *g_pengfuncsTable;
enginefuncs_t *g_pengfuncsTable_Post;
NEW_DLL_FUNCTIONS *g_pNewFunctionsTable;
NEW_DLL_FUNCTIONS *g_pNewFunctionsTable_Post;

// GetEntityAPI2 functions
static DLL_FUNCTIONS g_EntityAPI_Table =
{
#ifdef FN_GameDLLInit
  FN_GameDLLInit,
#else
  NULL,
#endif
#ifdef FN_DispatchSpawn
  FN_DispatchSpawn,
#else
  NULL,
#endif
#ifdef FN_DispatchThink
  FN_DispatchThink,
#else
  NULL,
#endif
#ifdef FN_DispatchUse
  FN_DispatchUse,
#else
  NULL,
#endif
#ifdef FN_DispatchTouch
  FN_DispatchTouch,
#else
  NULL,
#endif
#ifdef FN_DispatchBlocked
  FN_DispatchBlocked,
#else
  NULL,
#endif
#ifdef FN_DispatchKeyValue
  FN_DispatchKeyValue,
#else
  NULL,
#endif
#ifdef FN_DispatchSave
  FN_DispatchSave,
#else
  NULL,
#endif
#ifdef FN_DispatchRestore
  FN_DispatchRestore,
#else
  NULL,
#endif
#ifdef FN_DispatchObjectCollsionBox
  FN_DispatchObjectCollsionBox,
#else
  NULL,
#endif
#ifdef FN_SaveWriteFields
  FN_SaveWriteFields,
#else
  NULL,
#endif
#ifdef FN_SaveReadFields
  FN_SaveReadFields,
#else
  NULL,
#endif
#ifdef FN_SaveGlobalState
  FN_SaveGlobalState,
#else
  NULL,
#endif
#ifdef FN_RestoreGlobalState
  FN_RestoreGlobalState,
#else
  NULL,
#endif
#ifdef FN_ResetGlobalState
  FN_ResetGlobalState,
#else
  NULL,
#endif
#ifdef FN_ClientConnect
  FN_ClientConnect,
#else
  NULL,
#endif
#ifdef FN_ClientDisconnect
  FN_ClientDisconnect,
#else
  NULL,
#endif
#ifdef FN_ClientKill
  FN_ClientKill,
#else
  NULL,
#endif
#ifdef FN_ClientPutInServer
  FN_ClientPutInServer,
#else
  NULL,
#endif
#ifdef FN_ClientCommand
  FN_ClientCommand,
#else
  NULL,
#endif
#ifdef FN_ClientUserInfoChanged
  FN_ClientUserInfoChanged,
#else
  NULL,
#endif
#ifdef FN_ServerActivate
  FN_ServerActivate,
#else
  NULL,
#endif
#ifdef FN_ServerDeactivate
  FN_ServerDeactivate,
#else
  NULL,
#endif
#ifdef FN_PlayerPreThink
  FN_PlayerPreThink,
#else
  NULL,
#endif
#ifdef FN_PlayerPostThink
  FN_PlayerPostThink,
#else
  NULL,
#endif
#ifdef FN_StartFrame
  FN_StartFrame,
#else
  NULL,
#endif
#ifdef FN_ParmsNewLevel
  FN_ParmsNewLevel,
#else
  NULL,
#endif
#ifdef FN_ParmsChangeLevel
  FN_ParmsChangeLevel,
#else
  NULL,
#endif
#ifdef FN_GetGameDescription
  FN_GetGameDescription,
#else
  NULL,
#endif
#ifdef FN_PlayerCustomization
  FN_PlayerCustomization,
#else
  NULL,
#endif
#ifdef FN_SpectatorConnect
  FN_SpectatorConnect,
#else
  NULL,
#endif
#ifdef FN_SpectatorDisconnect
  FN_SpectatorDisconnect,
#else
  NULL,
#endif
#ifdef FN_SpectatorThink
  FN_SpectatorThink,
#else
  NULL,
#endif
#ifdef FN_Sys_Error
  FN_Sys_Error,
#else
  NULL,
#endif
#ifdef FN_PM_Move
  FN_PM_Move,
#else
  NULL,
#endif
#ifdef FN_PM_Init
  FN_PM_Init,
#else
  NULL,
#endif
#ifdef FN_PM_FindTextureType
  FN_PM_FindTextureType,
#else
  NULL,
#endif
#ifdef FN_SetupVisibility
  FN_SetupVisibility,
#else
  NULL,
#endif
#ifdef FN_UpdateClientData
  FN_UpdateClientData,
#else
  NULL,
#endif
#ifdef FN_AddToFullPack
  FN_AddToFullPack,
#else
  NULL,
#endif
#ifdef FN_CreateBaseline
  FN_CreateBaseline,
#else
  NULL,
#endif
#ifdef FN_RegisterEncoders
  FN_RegisterEncoders,
#else
  NULL,
#endif
#ifdef FN_GetWeaponData
  FN_GetWeaponData,
#else
  NULL,
#endif
#ifdef FN_CmdStart
  FN_CmdStart,
#else
  NULL,
#endif
#ifdef FN_CmdEnd
  FN_CmdEnd,
#else
  NULL,
#endif
#ifdef FN_ConnectionlessPacket
  FN_ConnectionlessPacket,
#else
  NULL,
#endif
#ifdef FN_GetHullBounds
  FN_GetHullBounds,
#else
  NULL,
#endif
#ifdef FN_CreateInstancedBaselines
  FN_CreateInstancedBaselines,
#else
  NULL,
#endif
#ifdef FN_InconsistentFile
  FN_InconsistentFile,
#else
  NULL,
#endif
#ifdef FN_AllowLagCompensation
  FN_AllowLagCompensation
#else
  NULL
#endif
}; // g_EntityAPI2_Table

// GetEntityAPI2_Post functions
static DLL_FUNCTIONS g_EntityAPI_Post_Table =
{
#ifdef FN_GameDLLInit_Post
  FN_GameDLLInit_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchSpawn_Post
  FN_DispatchSpawn_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchThink_Post
  FN_DispatchThink_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchUse_Post
  FN_DispatchUse_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchTouch_Post
  FN_DispatchTouch_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchBlocked_Post
  FN_DispatchBlocked_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchKeyValue_Post
  FN_DispatchKeyValue_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchSave_Post
  FN_DispatchSave_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchRestore_Post
  FN_DispatchRestore_Post,
#else
  NULL,
#endif
#ifdef FN_DispatchObjectCollsionBox_Post
  FN_DispatchObjectCollsionBox_Post,
#else
  NULL,
#endif
#ifdef FN_SaveWriteFields_Post
  FN_SaveWriteFields_Post,
#else
  NULL,
#endif
#ifdef FN_SaveReadFields_Post
  FN_SaveReadFields_Post,
#else
  NULL,
#endif
#ifdef FN_SaveGlobalState_Post
  FN_SaveGlobalState_Post,
#else
  NULL,
#endif
#ifdef FN_RestoreGlobalState_Post
  FN_RestoreGlobalState_Post,
#else
  NULL,
#endif
#ifdef FN_ResetGlobalState_Post
  FN_ResetGlobalState_Post,
#else
  NULL,
#endif
#ifdef FN_ClientConnect_Post
  FN_ClientConnect_Post,
#else
  NULL,
#endif
#ifdef FN_ClientDisconnect_Post
  FN_ClientDisconnect_Post,
#else
  NULL,
#endif
#ifdef FN_ClientKill_Post
  FN_ClientKill_Post,
#else
  NULL,
#endif
#ifdef FN_ClientPutInServer_Post
  FN_ClientPutInServer_Post,
#else
  NULL,
#endif
#ifdef FN_ClientCommand_Post
  FN_ClientCommand_Post,
#else
  NULL,
#endif
#ifdef FN_ClientUserInfoChanged_Post
  FN_ClientUserInfoChanged_Post,
#else
  NULL,
#endif
#ifdef FN_ServerActivate_Post
  FN_ServerActivate_Post,
#else
  NULL,
#endif
#ifdef FN_ServerDeactivate_Post
  FN_ServerDeactivate_Post,
#else
  NULL,
#endif
#ifdef FN_PlayerPreThink_Post
  FN_PlayerPreThink_Post,
#else
  NULL,
#endif
#ifdef FN_PlayerPostThink_Post
  FN_PlayerPostThink_Post,
#else
  NULL,
#endif
#ifdef FN_StartFrame_Post
  FN_StartFrame_Post,
#else
  NULL,
#endif
#ifdef FN_ParmsNewLevel_Post
  FN_ParmsNewLevel_Post,
#else
  NULL,
#endif
#ifdef FN_ParmsChangeLevel_Post
  FN_ParmsChangeLevel_Post,
#else
  NULL,
#endif
#ifdef FN_GetGameDescription_Post
  FN_GetGameDescription_Post,
#else
  NULL,
#endif
#ifdef FN_PlayerCustomization_Post
  FN_PlayerCustomization_Post,
#else
  NULL,
#endif
#ifdef FN_SpectatorConnect_Post
  FN_SpectatorConnect_Post,
#else
  NULL,
#endif
#ifdef FN_SpectatorDisconnect_Post
  FN_SpectatorDisconnect_Post,
#else
  NULL,
#endif
#ifdef FN_SpectatorThink_Post
  FN_SpectatorThink_Post,
#else
  NULL,
#endif
#ifdef FN_Sys_Error_Post
  FN_Sys_Error_Post,
#else
  NULL,
#endif
#ifdef FN_PM_Move_Post
  FN_PM_Move_Post,
#else
  NULL,
#endif
#ifdef FN_PM_Init_Post
  FN_PM_Init_Post,
#else
  NULL,
#endif
#ifdef FN_PM_FindTextureType_Post
  FN_PM_FindTextureType_Post,
#else
  NULL,
#endif
#ifdef FN_SetupVisibility_Post
  FN_SetupVisibility_Post,
#else
  NULL,
#endif
#ifdef FN_UpdateClientData_Post
  FN_UpdateClientData_Post,
#else
  NULL,
#endif
#ifdef FN_AddToFullPack_Post
  FN_AddToFullPack_Post,
#else
  NULL,
#endif
#ifdef FN_CreateBaseline_Post
  FN_CreateBaseline_Post,
#else
  NULL,
#endif
#ifdef FN_RegisterEncoders_Post
  FN_RegisterEncoders_Post,
#else
  NULL,
#endif
#ifdef FN_GetWeaponData_Post
  FN_GetWeaponData_Post,
#else
  NULL,
#endif
#ifdef FN_CmdStart_Post
  FN_CmdStart_Post,
#else
  NULL,
#endif
#ifdef FN_CmdEnd_Post
  FN_CmdEnd_Post,
#else
  NULL,
#endif
#ifdef FN_ConnectionlessPacket_Post
  FN_ConnectionlessPacket_Post,
#else
  NULL,
#endif
#ifdef FN_GetHullBounds_Post
  FN_GetHullBounds_Post,
#else
  NULL,
#endif
#ifdef FN_CreateInstancedBaselines_Post
  FN_CreateInstancedBaselines_Post,
#else
  NULL,
#endif
#ifdef FN_InconsistentFile_Post
  FN_InconsistentFile_Post,
#else
  NULL,
#endif
#ifdef FN_AllowLagCompensation
  FN_AllowLagCompensation,
#else
  NULL,
#endif
}; // g_EntityAPI2_Table

static enginefuncs_t g_EngineFuncs_Table =
{
#ifdef FN_PrecacheModel
  FN_PrecacheModel,
#else
  NULL,
#endif
#ifdef FN_PrecacheSound
  FN_PrecacheSound,
#else
  NULL,
#endif
#ifdef FN_SetModel
  FN_SetModel,
#else
  NULL,
#endif
#ifdef FN_ModelIndex
  FN_ModelIndex,
#else
  NULL,
#endif
#ifdef FN_ModelFrames
  FN_ModelFrames,
#else
  NULL,
#endif
#ifdef FN_SetSize
  FN_SetSize,
#else
  NULL,
#endif
#ifdef FN_ChangeLevel
  FN_ChangeLevel,
#else
  NULL,
#endif
#ifdef FN_GetSpawnParms
  FN_GetSpawnParms,
#else
  NULL,
#endif
#ifdef FN_SaveSpawnParms
  FN_SaveSpawnParms,
#else
  NULL,
#endif
#ifdef FN_VecToYaw
  FN_VecToYaw,
#else
  NULL,
#endif
#ifdef FN_VecToAngles
  FN_VecToAngles,
#else
  NULL,
#endif
#ifdef FN_MoveToOrigin
  FN_MoveToOrigin,
#else
  NULL,
#endif
#ifdef FN_ChangeYaw
  FN_ChangeYaw,
#else
  NULL,
#endif
#ifdef FN_ChangePitch
  FN_ChangePitch,
#else
  NULL,
#endif
#ifdef FN_FindEntityByString
  FN_FindEntityByString,
#else
  NULL,
#endif
#ifdef FN_GetEntityIllum
  FN_GetEntityIllum,
#else
  NULL,
#endif
#ifdef FN_FindEntityInSphere
  FN_FindEntityInSphere,
#else
  NULL,
#endif
#ifdef FN_FindClientInPVS
  FN_FindClientInPVS,
#else
  NULL,
#endif
#ifdef FN_EntitiesInPVS
  FN_EntitiesInPVS,
#else
  NULL,
#endif
#ifdef FN_MakeVectors
  FN_MakeVectors,
#else
  NULL,
#endif
#ifdef FN_AngleVectors
  FN_AngleVectors,
#else
  NULL,
#endif
#ifdef FN_CreateEntity
  FN_CreateEntity,
#else
  NULL,
#endif
#ifdef FN_RemoveEntity
  FN_RemoveEntity,
#else
  NULL,
#endif
#ifdef FN_CreateNamedEntity
  FN_CreateNamedEntity,
#else
  NULL,
#endif
#ifdef FN_MakeStatic
  FN_MakeStatic,
#else
  NULL,
#endif
#ifdef FN_EntIsOnFloor
  FN_EntIsOnFloor,
#else
  NULL,
#endif
#ifdef FN_DropToFloor
  FN_DropToFloor,
#else
  NULL,
#endif
#ifdef FN_WalkMove
  FN_WalkMove,
#else
  NULL,
#endif
#ifdef FN_SetOrigin
  FN_SetOrigin,
#else
  NULL,
#endif
#ifdef FN_EmitSound
  FN_EmitSound,
#else
  NULL,
#endif
#ifdef FN_EmitAmbientSound
  FN_EmitAmbientSound,
#else
  NULL,
#endif
#ifdef FN_TraceLine
  FN_TraceLine,
#else
  NULL,
#endif
#ifdef FN_TraceToss
  FN_TraceToss,
#else
  NULL,
#endif
#ifdef FN_TraceMonsterHull
  FN_TraceMonsterHull,
#else
  NULL,
#endif
#ifdef FN_TraceHull
  FN_TraceHull,
#else
  NULL,
#endif
#ifdef FN_TraceModel
  FN_TraceModel,
#else
  NULL,
#endif
#ifdef FN_TraceTexture
  FN_TraceTexture,
#else
  NULL,
#endif
#ifdef FN_TraceSphere
  FN_TraceSphere,
#else
  NULL,
#endif
#ifdef FN_GetAimVector
  FN_GetAimVector,
#else
  NULL,
#endif
#ifdef FN_ServerCommand
  FN_ServerCommand,
#else
  NULL,
#endif
#ifdef FN_ServerExecute
  FN_ServerExecute,
#else
  NULL,
#endif
#ifdef FN_engClientCommand
  FN_engClientCommand,
#else
  NULL,
#endif
#ifdef FN_ParticleEffect
  FN_ParticleEffect,
#else
  NULL,
#endif
#ifdef FN_LightStyle
  FN_LightStyle,
#else
  NULL,
#endif
#ifdef FN_DecalIndex
  FN_DecalIndex,
#else
  NULL,
#endif
#ifdef FN_PointContents
  FN_PointContents,
#else
  NULL,
#endif
#ifdef FN_MessageBegin
  FN_MessageBegin,
#else
  NULL,
#endif
#ifdef FN_MessageEnd
  FN_MessageEnd,
#else
  NULL,
#endif
#ifdef FN_WriteByte
  FN_WriteByte,
#else
  NULL,
#endif
#ifdef FN_WriteChar
  FN_WriteChar,
#else
  NULL,
#endif
#ifdef FN_WriteShort
  FN_WriteShort,
#else
  NULL,
#endif
#ifdef FN_WriteLong
  FN_WriteLong,
#else
  NULL,
#endif
#ifdef FN_WriteAngle
  FN_WriteAngle,
#else
  NULL,
#endif
#ifdef FN_WriteCoord
  FN_WriteCoord,
#else
  NULL,
#endif
#ifdef FN_WriteString
  FN_WriteString,
#else
  NULL,
#endif
#ifdef FN_WriteEntity
  FN_WriteEntity,
#else
  NULL,
#endif
#ifdef FN_CVarRegister
  FN_CVarRegister,
#else
  NULL,
#endif
#ifdef FN_CVarGetFloat
  FN_CVarGetFloat,
#else
  NULL,
#endif
#ifdef FN_CVarGetString
  FN_CVarGetString,
#else
  NULL,
#endif
#ifdef FN_CVarSetFloat
  FN_CVarSetFloat,
#else
  NULL,
#endif
#ifdef FN_CVarSetString
  FN_CVarSetString,
#else
  NULL,
#endif
#ifdef FN_AlertMessage
  FN_AlertMessage,
#else
  NULL,
#endif
#ifdef FN_EngineFprintf
  FN_EngineFprintf,
#else
  NULL,
#endif
#ifdef FN_PvAllocEntPrivateData
  FN_PvAllocEntPrivateData,
#else
  NULL,
#endif
#ifdef FN_PvEntPrivateData
  FN_PvEntPrivateData,
#else
  NULL,
#endif
#ifdef FN_FreeEntPrivateData
  FN_FreeEntPrivateData,
#else
  NULL,
#endif
#ifdef FN_SzFromIndex
  FN_SzFromIndex,
#else
  NULL,
#endif
#ifdef FN_AllocString
  FN_AllocString,
#else
  NULL,
#endif
#ifdef FN_GetVarsOfEnt
  FN_GetVarsOfEnt,
#else
  NULL,
#endif
#ifdef FN_PEntityOfEntOffset
  FN_PEntityOfEntOffset,
#else
  NULL,
#endif
#ifdef FN_EntOffsetOfPEntity
  FN_EntOffsetOfPEntity,
#else
  NULL,
#endif
#ifdef FN_IndexOfEdict
  FN_IndexOfEdict,
#else
  NULL,
#endif
#ifdef FN_PEntityOfEntIndex
  FN_PEntityOfEntIndex,
#else
  NULL,
#endif
#ifdef FN_FindEntityByVars
  FN_FindEntityByVars,
#else
  NULL,
#endif
#ifdef FN_GetModelPtr
  FN_GetModelPtr,
#else
  NULL,
#endif
#ifdef FN_RegUserMsg
  FN_RegUserMsg,
#else
  NULL,
#endif
#ifdef FN_AnimationAutomove
  FN_AnimationAutomove,
#else
  NULL,
#endif
#ifdef FN_GetBonePosition
  FN_GetBonePosition,
#else
  NULL,
#endif
#ifdef FN_FunctionFromName
  FN_FunctionFromName,
#else
  NULL,
#endif
#ifdef FN_NameForFunction
  FN_NameForFunction,
#else
  NULL,
#endif
#ifdef FN_ClientPrintf
  FN_ClientPrintf,
#else
  NULL,
#endif
#ifdef FN_ServerPrint
  FN_ServerPrint,
#else
  NULL,
#endif
#ifdef FN_Cmd_Args
  FN_Cmd_Args,
#else
  NULL,
#endif
#ifdef FN_Cmd_Argv
  FN_Cmd_Argv,
#else
  NULL,
#endif
#ifdef FN_Cmd_Argc
  FN_Cmd_Argc,
#else
  NULL,
#endif
#ifdef FN_GetAttachment
  FN_GetAttachment,
#else
  NULL,
#endif
#ifdef FN_CRC32_Init
  FN_CRC32_Init,
#else
  NULL,
#endif
#ifdef FN_CRC32_ProcessBuffer
  FN_CRC32_ProcessBuffer,
#else
  NULL,
#endif
#ifdef FN_CRC32_ProcessByte
  FN_CRC32_ProcessByte,
#else
  NULL,
#endif
#ifdef FN_CRC32_Final
  FN_CRC32_Final,
#else
  NULL,
#endif
#ifdef FN_RandomLong
  FN_RandomLong,
#else
  NULL,
#endif
#ifdef FN_RandomFloat
  FN_RandomFloat,
#else
  NULL,
#endif
#ifdef FN_SetView
  FN_SetView,
#else
  NULL,
#endif
#ifdef FN_Time
  FN_Time,
#else
  NULL,
#endif
#ifdef FN_CrosshairAngle
  FN_CrosshairAngle,
#else
  NULL,
#endif
#ifdef FN_LoadFileForMe
  FN_LoadFileForMe,
#else
  NULL,
#endif
#ifdef FN_FreeFile
  FN_FreeFile,
#else
  NULL,
#endif
#ifdef FN_EndSection
  FN_EndSection,
#else
  NULL,
#endif
#ifdef FN_CompareFileTime
  FN_CompareFileTime,
#else
  NULL,
#endif
#ifdef FN_GetGameDir
  FN_GetGameDir,
#else
  NULL,
#endif
#ifdef FN_Cvar_RegisterVariable
  FN_Cvar_RegisterVariable,
#else
  NULL,
#endif
#ifdef FN_FadeClientVolume
  FN_FadeClientVolume,
#else
  NULL,
#endif
#ifdef FN_SetClientMaxspeed
  FN_SetClientMaxspeed,
#else
  NULL,
#endif
#ifdef FN_CreateFakeClient
  FN_CreateFakeClient,
#else
  NULL,
#endif
#ifdef FN_RunPlayerMove
  FN_RunPlayerMove,
#else
  NULL,
#endif
#ifdef FN_NumberOfEntities
  FN_NumberOfEntities,
#else
  NULL,
#endif
#ifdef FN_GetInfoKeyBuffer
  FN_GetInfoKeyBuffer,
#else
  NULL,
#endif
#ifdef FN_InfoKeyValue
  FN_InfoKeyValue,
#else
  NULL,
#endif
#ifdef FN_SetKeyValue
  FN_SetKeyValue,
#else
  NULL,
#endif
#ifdef FN_SetClientKeyValue
  FN_SetClientKeyValue,
#else
  NULL,
#endif
#ifdef FN_IsMapValid
  FN_IsMapValid,
#else
  NULL,
#endif
#ifdef FN_StaticDecal
  FN_StaticDecal,
#else
  NULL,
#endif
#ifdef FN_PrecacheGeneric
  FN_PrecacheGeneric,
#else
  NULL,
#endif
#ifdef FN_GetPlayerUserId
  FN_GetPlayerUserId,
#else
  NULL,
#endif
#ifdef FN_BuildSoundMsg
  FN_BuildSoundMsg,
#else
  NULL,
#endif
#ifdef FN_IsDedicatedServer
  FN_IsDedicatedServer,
#else
  NULL,
#endif
#ifdef FN_CVarGetPointer
  FN_CVarGetPointer,
#else
  NULL,
#endif
#ifdef FN_GetPlayerWONId
  FN_GetPlayerWONId,
#else
  NULL,
#endif
#ifdef FN_Info_RemoveKey
  FN_Info_RemoveKey,
#else
  NULL,
#endif
#ifdef FN_GetPhysicsKeyValue
  FN_GetPhysicsKeyValue,
#else
  NULL,
#endif
#ifdef FN_SetPhysicsKeyValue
  FN_SetPhysicsKeyValue,
#else
  NULL,
#endif
#ifdef FN_GetPhysicsInfoString
  FN_GetPhysicsInfoString,
#else
  NULL,
#endif
#ifdef FN_PrecacheEvent
  FN_PrecacheEvent,
#else
  NULL,
#endif
#ifdef FN_PlaybackEvent
  FN_PlaybackEvent,
#else
  NULL,
#endif
#ifdef FN_SetFatPVS
  FN_SetFatPVS,
#else
  NULL,
#endif
#ifdef FN_SetFatPAS
  FN_SetFatPAS,
#else
  NULL,
#endif
#ifdef FN_CheckVisibility
  FN_CheckVisibility,
#else
  NULL,
#endif
#ifdef FN_DeltaSetField
  FN_DeltaSetField,
#else
  NULL,
#endif
#ifdef FN_DeltaUnsetField
  FN_DeltaUnsetField,
#else
  NULL,
#endif
#ifdef FN_DeltaAddEncoder
  FN_DeltaAddEncoder,
#else
  NULL,
#endif
#ifdef FN_GetCurrentPlayer
  FN_GetCurrentPlayer,
#else
  NULL,
#endif
#ifdef FN_CanSkipPlayer
  FN_CanSkipPlayer,
#else
  NULL,
#endif
#ifdef FN_DeltaFindField
  FN_DeltaFindField,
#else
  NULL,
#endif
#ifdef FN_DeltaSetFieldByIndex
  FN_DeltaSetFieldByIndex,
#else
  NULL,
#endif
#ifdef FN_DeltaUnsetFieldByIndex
  FN_DeltaUnsetFieldByIndex,
#else
  NULL,
#endif
#ifdef FN_SetGroupMask
  FN_SetGroupMask,
#else
  NULL,
#endif
#ifdef FN_engCreateInstancedBaseline
  FN_engCreateInstancedBaseline,
#else
  NULL,
#endif
#ifdef FN_Cvar_DirectSet
  FN_Cvar_DirectSet,
#else
  NULL,
#endif
#ifdef FN_ForceUnmodified
  FN_ForceUnmodified,
#else
  NULL,
#endif
#ifdef FN_GetPlayerStats
  FN_GetPlayerStats,
#else
  NULL,
#endif
#ifdef FN_AddServerCommand
  FN_AddServerCommand,
#else
  NULL,
#endif
#ifdef FN_Voice_GetClientListening
  FN_Voice_GetClientListening,
#else
  NULL,
#endif
#ifdef FN_Voice_SetClientListening
  FN_Voice_SetClientListening,
#else
  NULL,
#endif
#ifdef FN_GetPlayerAuthId
  FN_GetPlayerAuthId
#else
  NULL
#endif
}; // g_EngineFuncs_Table


static enginefuncs_t g_EngineFuncs_Post_Table =
{
#ifdef FN_PrecacheModel_Post
  FN_PrecacheModel_Post,
#else
  NULL,
#endif
#ifdef FN_PrecacheSound_Post
  FN_PrecacheSound_Post,
#else
  NULL,
#endif
#ifdef FN_SetModel_Post
  FN_SetModel_Post,
#else
  NULL,
#endif
#ifdef FN_ModelIndex_Post
  FN_ModelIndex_Post,
#else
  NULL,
#endif
#ifdef FN_ModelFrames_Post
  FN_ModelFrames_Post,
#else
  NULL,
#endif
#ifdef FN_SetSize_Post
  FN_SetSize_Post,
#else
  NULL,
#endif
#ifdef FN_ChangeLevel_Post
  FN_ChangeLevel_Post,
#else
  NULL,
#endif
#ifdef FN_GetSpawnParms_Post
  FN_GetSpawnParms_Post,
#else
  NULL,
#endif
#ifdef FN_SaveSpawnParms_Post
  FN_SaveSpawnParms_Post,
#else
  NULL,
#endif
#ifdef FN_VecToYaw_Post
  FN_VecToYaw_Post,
#else
  NULL,
#endif
#ifdef FN_VecToAngles_Post
  FN_VecToAngles_Post,
#else
  NULL,
#endif
#ifdef FN_MoveToOrigin_Post
  FN_MoveToOrigin_Post,
#else
  NULL,
#endif
#ifdef FN_ChangeYaw_Post
  FN_ChangeYaw_Post,
#else
  NULL,
#endif
#ifdef FN_ChangePitch_Post
  FN_ChangePitch_Post,
#else
  NULL,
#endif
#ifdef FN_FindEntityByString_Post
  FN_FindEntityByString_Post,
#else
  NULL,
#endif
#ifdef FN_GetEntityIllum_Post
  FN_GetEntityIllum_Post,
#else
  NULL,
#endif
#ifdef FN_FindEntityInSphere_Post
  FN_FindEntityInSphere_Post,
#else
  NULL,
#endif
#ifdef FN_FindClientInPVS_Post
  FN_FindClientInPVS_Post,
#else
  NULL,
#endif
#ifdef FN_EntitiesInPVS_Post
  FN_EntitiesInPVS_Post,
#else
  NULL,
#endif
#ifdef FN_MakeVectors_Post
  FN_MakeVectors_Post,
#else
  NULL,
#endif
#ifdef FN_AngleVectors_Post
  FN_AngleVectors_Post,
#else
  NULL,
#endif
#ifdef FN_CreateEntity_Post
  FN_CreateEntity_Post,
#else
  NULL,
#endif
#ifdef FN_RemoveEntity_Post
  FN_RemoveEntity_Post,
#else
  NULL,
#endif
#ifdef FN_CreateNamedEntity_Post
  FN_CreateNamedEntity_Post,
#else
  NULL,
#endif
#ifdef FN_MakeStatic_Post
  FN_MakeStatic_Post,
#else
  NULL,
#endif
#ifdef FN_EntIsOnFloor_Post
  FN_EntIsOnFloor_Post,
#else
  NULL,
#endif
#ifdef FN_DropToFloor_Post
  FN_DropToFloor_Post,
#else
  NULL,
#endif
#ifdef FN_WalkMove_Post
  FN_WalkMove_Post,
#else
  NULL,
#endif
#ifdef FN_SetOrigin_Post
  FN_SetOrigin_Post,
#else
  NULL,
#endif
#ifdef FN_EmitSound_Post
  FN_EmitSound_Post,
#else
  NULL,
#endif
#ifdef FN_EmitAmbientSound_Post
  FN_EmitAmbientSound_Post,
#else
  NULL,
#endif
#ifdef FN_TraceLine_Post
  FN_TraceLine_Post,
#else
  NULL,
#endif
#ifdef FN_TraceToss_Post
  FN_TraceToss_Post,
#else
  NULL,
#endif
#ifdef FN_TraceMonsterHull_Post
  FN_TraceMonsterHull_Post,
#else
  NULL,
#endif
#ifdef FN_TraceHull_Post
  FN_TraceHull_Post,
#else
  NULL,
#endif
#ifdef FN_TraceModel_Post
  FN_TraceModel_Post,
#else
  NULL,
#endif
#ifdef FN_TraceTexture_Post
  FN_TraceTexture_Post,
#else
  NULL,
#endif
#ifdef FN_TraceSphere_Post
  FN_TraceSphere_Post,
#else
  NULL,
#endif
#ifdef FN_GetAimVector_Post
  FN_GetAimVector_Post,
#else
  NULL,
#endif
#ifdef FN_ServerCommand_Post
  FN_ServerCommand_Post,
#else
  NULL,
#endif
#ifdef FN_ServerExecute_Post
  FN_ServerExecute_Post,
#else
  NULL,
#endif
#ifdef FN_engClientCommand_Post
  FN_engClientCommand_Post,
#else
  NULL,
#endif
#ifdef FN_ParticleEffect_Post
  FN_ParticleEffect_Post,
#else
  NULL,
#endif
#ifdef FN_LightStyle_Post
  FN_LightStyle_Post,
#else
  NULL,
#endif
#ifdef FN_DecalIndex_Post
  FN_DecalIndex_Post,
#else
  NULL,
#endif
#ifdef FN_PointContents_Post
  FN_PointContents_Post,
#else
  NULL,
#endif
#ifdef FN_MessageBegin_Post
  FN_MessageBegin_Post,
#else
  NULL,
#endif
#ifdef FN_MessageEnd_Post
  FN_MessageEnd_Post,
#else
  NULL,
#endif
#ifdef FN_WriteByte_Post
  FN_WriteByte_Post,
#else
  NULL,
#endif
#ifdef FN_WriteChar_Post
  FN_WriteChar_Post,
#else
  NULL,
#endif
#ifdef FN_WriteShort_Post
  FN_WriteShort_Post,
#else
  NULL,
#endif
#ifdef FN_WriteLong_Post
  FN_WriteLong_Post,
#else
  NULL,
#endif
#ifdef FN_WriteAngle_Post
  FN_WriteAngle_Post,
#else
  NULL,
#endif
#ifdef FN_WriteCoord_Post
  FN_WriteCoord_Post,
#else
  NULL,
#endif
#ifdef FN_WriteString_Post
  FN_WriteString_Post,
#else
  NULL,
#endif
#ifdef FN_WriteEntity_Post
  FN_WriteEntity_Post,
#else
  NULL,
#endif
#ifdef FN_CVarRegister_Post
  FN_CVarRegister_Post,
#else
  NULL,
#endif
#ifdef FN_CVarGetFloat_Post
  FN_CVarGetFloat_Post,
#else
  NULL,
#endif
#ifdef FN_CVarGetString_Post
  FN_CVarGetString_Post,
#else
  NULL,
#endif
#ifdef FN_CVarSetFloat_Post
  FN_CVarSetFloat_Post,
#else
  NULL,
#endif
#ifdef FN_CVarSetString_Post
  FN_CVarSetString_Post,
#else
  NULL,
#endif
#ifdef FN_AlertMessage_Post
  FN_AlertMessage_Post,
#else
  NULL,
#endif
#ifdef FN_EngineFprintf_Post
  FN_EngineFprintf_Post,
#else
  NULL,
#endif
#ifdef FN_PvAllocEntPrivateData_Post
  FN_PvAllocEntPrivateData_Post,
#else
  NULL,
#endif
#ifdef FN_PvEntPrivateData_Post
  FN_PvEntPrivateData_Post,
#else
  NULL,
#endif
#ifdef FN_FreeEntPrivateData_Post
  FN_FreeEntPrivateData_Post,
#else
  NULL,
#endif
#ifdef FN_SzFromIndex_Post
  FN_SzFromIndex_Post,
#else
  NULL,
#endif
#ifdef FN_AllocString_Post
  FN_AllocString_Post,
#else
  NULL,
#endif
#ifdef FN_GetVarsOfEnt_Post
  FN_GetVarsOfEnt_Post,
#else
  NULL,
#endif
#ifdef FN_PEntityOfEntOffset_Post
  FN_PEntityOfEntOffset_Post,
#else
  NULL,
#endif
#ifdef FN_EntOffsetOfPEntity_Post
  FN_EntOffsetOfPEntity_Post,
#else
  NULL,
#endif
#ifdef FN_IndexOfEdict_Post
  FN_IndexOfEdict_Post,
#else
  NULL,
#endif
#ifdef FN_PEntityOfEntIndex_Post
  FN_PEntityOfEntIndex_Post,
#else
  NULL,
#endif
#ifdef FN_FindEntityByVars_Post
  FN_FindEntityByVars_Post,
#else
  NULL,
#endif
#ifdef FN_GetModelPtr_Post
  FN_GetModelPtr_Post,
#else
  NULL,
#endif
#ifdef FN_RegUserMsg_Post
  FN_RegUserMsg_Post,
#else
  NULL,
#endif
#ifdef FN_AnimationAutomove_Post
  FN_AnimationAutomove_Post,
#else
  NULL,
#endif
#ifdef FN_GetBonePosition_Post
  FN_GetBonePosition_Post,
#else
  NULL,
#endif
#ifdef FN_FunctionFromName_Post
  FN_FunctionFromName_Post,
#else
  NULL,
#endif
#ifdef FN_NameForFunction_Post
  FN_NameForFunction_Post,
#else
  NULL,
#endif
#ifdef FN_ClientPrintf_Post
  FN_ClientPrintf_Post,
#else
  NULL,
#endif
#ifdef FN_ServerPrint_Post
  FN_ServerPrint_Post,
#else
  NULL,
#endif
#ifdef FN_Cmd_Args_Post
  FN_Cmd_Args_Post,
#else
  NULL,
#endif
#ifdef FN_Cmd_Argv_Post
  FN_Cmd_Argv_Post,
#else
  NULL,
#endif
#ifdef FN_Cmd_Argc_Post
  FN_Cmd_Argc_Post,
#else
  NULL,
#endif
#ifdef FN_GetAttachment_Post
  FN_GetAttachment_Post,
#else
  NULL,
#endif
#ifdef FN_CRC32_Init_Post
  FN_CRC32_Init_Post,
#else
  NULL,
#endif
#ifdef FN_CRC32_ProcessBuffer_Post
  FN_CRC32_ProcessBuffer_Post,
#else
  NULL,
#endif
#ifdef FN_CRC32_ProcessByte_Post
  FN_CRC32_ProcessByte_Post,
#else
  NULL,
#endif
#ifdef FN_CRC32_Final_Post
  FN_CRC32_Final_Post,
#else
  NULL,
#endif
#ifdef FN_RandomLong_Post
  FN_RandomLong_Post,
#else
  NULL,
#endif
#ifdef FN_RandomFloat_Post
  FN_RandomFloat_Post,
#else
  NULL,
#endif
#ifdef FN_SetView_Post
  FN_SetView_Post,
#else
  NULL,
#endif
#ifdef FN_Time_Post
  FN_Time_Post,
#else
  NULL,
#endif
#ifdef FN_CrosshairAngle_Post
  FN_CrosshairAngle_Post,
#else
  NULL,
#endif
#ifdef FN_LoadFileForMe_Post
  FN_LoadFileForMe_Post,
#else
  NULL,
#endif
#ifdef FN_FreeFile_Post
  FN_FreeFile_Post,
#else
  NULL,
#endif
#ifdef FN_EndSection_Post
  FN_EndSection_Post,
#else
  NULL,
#endif
#ifdef FN_CompareFileTime_Post
  FN_CompareFileTime_Post,
#else
  NULL,
#endif
#ifdef FN_GetGameDir_Post
  FN_GetGameDir_Post,
#else
  NULL,
#endif
#ifdef FN_Cvar_RegisterVariable_Post
  FN_Cvar_RegisterVariable_Post,
#else
  NULL,
#endif
#ifdef FN_FadeClientVolume_Post
  FN_FadeClientVolume_Post,
#else
  NULL,
#endif
#ifdef FN_SetClientMaxspeed_Post
  FN_SetClientMaxspeed_Post,
#else
  NULL,
#endif
#ifdef FN_CreateFakeClient_Post
  FN_CreateFakeClient_Post,
#else
  NULL,
#endif
#ifdef FN_RunPlayerMove_Post
  FN_RunPlayerMove_Post,
#else
  NULL,
#endif
#ifdef FN_NumberOfEntities_Post
  FN_NumberOfEntities_Post,
#else
  NULL,
#endif
#ifdef FN_GetInfoKeyBuffer_Post
  FN_GetInfoKeyBuffer_Post,
#else
  NULL,
#endif
#ifdef FN_InfoKeyValue_Post
  FN_InfoKeyValue_Post,
#else
  NULL,
#endif
#ifdef FN_SetKeyValue_Post
  FN_SetKeyValue_Post,
#else
  NULL,
#endif
#ifdef FN_SetClientKeyValue_Post
  FN_SetClientKeyValue_Post,
#else
  NULL,
#endif
#ifdef FN_IsMapValid_Post
  FN_IsMapValid_Post,
#else
  NULL,
#endif
#ifdef FN_StaticDecal_Post
  FN_StaticDecal_Post,
#else
  NULL,
#endif
#ifdef FN_PrecacheGeneric_Post
  FN_PrecacheGeneric_Post,
#else
  NULL,
#endif
#ifdef FN_GetPlayerUserId_Post
  FN_GetPlayerUserId_Post,
#else
  NULL,
#endif
#ifdef FN_BuildSoundMsg_Post
  FN_BuildSoundMsg_Post,
#else
  NULL,
#endif
#ifdef FN_IsDedicatedServer_Post
  FN_IsDedicatedServer_Post,
#else
  NULL,
#endif
#ifdef FN_CVarGetPointer_Post
  FN_CVarGetPointer_Post,
#else
  NULL,
#endif
#ifdef FN_GetPlayerWONId_Post
  FN_GetPlayerWONId_Post,
#else
  NULL,
#endif
#ifdef FN_Info_RemoveKey_Post
  FN_Info_RemoveKey_Post,
#else
  NULL,
#endif
#ifdef FN_GetPhysicsKeyValue_Post
  FN_GetPhysicsKeyValue_Post,
#else
  NULL,
#endif
#ifdef FN_SetPhysicsKeyValue_Post
  FN_SetPhysicsKeyValue_Post,
#else
  NULL,
#endif
#ifdef FN_GetPhysicsInfoString_Post
  FN_GetPhysicsInfoString_Post,
#else
  NULL,
#endif
#ifdef FN_PrecacheEvent_Post
  FN_PrecacheEvent_Post,
#else
  NULL,
#endif
#ifdef FN_PlaybackEvent_Post
  FN_PlaybackEvent_Post,
#else
  NULL,
#endif
#ifdef FN_SetFatPVS_Post
  FN_SetFatPVS_Post,
#else
  NULL,
#endif
#ifdef FN_SetFatPAS_Post
  FN_SetFatPAS_Post,
#else
  NULL,
#endif
#ifdef FN_CheckVisibility_Post
  FN_CheckVisibility_Post,
#else
  NULL,
#endif
#ifdef FN_DeltaSetField_Post
  FN_DeltaSetField_Post,
#else
  NULL,
#endif
#ifdef FN_DeltaUnsetField_Post
  FN_DeltaUnsetField_Post,
#else
  NULL,
#endif
#ifdef FN_DeltaAddEncoder_Post
  FN_DeltaAddEncoder_Post,
#else
  NULL,
#endif
#ifdef FN_GetCurrentPlayer_Post
  FN_GetCurrentPlayer_Post,
#else
  NULL,
#endif
#ifdef FN_CanSkipPlayer_Post
  FN_CanSkipPlayer_Post,
#else
  NULL,
#endif
#ifdef FN_DeltaFindField_Post
  FN_DeltaFindField_Post,
#else
  NULL,
#endif
#ifdef FN_DeltaSetFieldByIndex_Post
  FN_DeltaSetFieldByIndex_Post,
#else
  NULL,
#endif
#ifdef FN_DeltaUnsetFieldByIndex_Post
  FN_DeltaUnsetFieldByIndex_Post,
#else
  NULL,
#endif
#ifdef FN_SetGroupMask_Post
  FN_SetGroupMask_Post,
#else
  NULL,
#endif
#ifdef FN_engCreateInstancedBaseline_Post
  FN_engCreateInstancedBaseline_Post,
#else
  NULL,
#endif
#ifdef FN_Cvar_DirectSet_Post
  FN_Cvar_DirectSet_Post,
#else
  NULL,
#endif
#ifdef FN_ForceUnmodified_Post
  FN_ForceUnmodified_Post,
#else
  NULL,
#endif
#ifdef FN_GetPlayerStats_Post
  FN_GetPlayerStats_Post,
#else
  NULL,
#endif
#ifdef FN_AddServerCommand_Post
  FN_AddServerCommand_Post,
#else
  NULL,
#endif
#ifdef FN_Voice_GetClientListening_Post
  FN_Voice_GetClientListening_Post,
#else
  NULL,
#endif
#ifdef FN_Voice_SetClientListening_Post
  FN_Voice_SetClientListening_Post,
#else
  NULL,
#endif
#ifdef FN_GetPlayerAuthId_Post
  FN_GetPlayerAuthId_Post
#else
  NULL
#endif
}; // g_EngineFuncs_Post_Table


static NEW_DLL_FUNCTIONS g_NewFuncs_Table =
{
#ifdef FN_OnFreeEntPrivateData
  FN_OnFreeEntPrivateData,
#else
  NULL,
#endif
#ifdef FN_GameShutdown
  FN_GameShutdown,
#else
  NULL,
#endif
#ifdef FN_ShouldCollide
  ShouldCollide,
#else
  NULL,
#endif
};


static NEW_DLL_FUNCTIONS g_NewFuncs_Post_Table =
{
#ifdef FN_OnFreeEntPrivateData_Post
  FN_OnFreeEntPrivateData_Post,
#else
  NULL,
#endif
#ifdef FN_GameShutdown_Post
  FN_GameShutdown_Post,
#else
  NULL,
#endif
#ifdef FN_ShouldCollide_Post
  ShouldCollide_Post,
#else
  NULL,
#endif
};

meta_globals_t *gpMetaGlobals;    // metamod globals
gamedll_funcs_t *gpGamedllFuncs;  // gameDLL function tables
mutil_funcs_t *gpMetaUtilFuncs;   // metamod utility functions

plugin_info_t Plugin_info = {
  META_INTERFACE_VERSION,
  MODULE_NAME,
  MODULE_VERSION,
  MODULE_DATE,
  MODULE_AUTHOR,
  MODULE_URL,
  MODULE_NAME,
  PT_ANYTIME,
  PT_ANYTIME
};

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion) {
  LOG_DEVELOPER(PLID, "called: GetEntityAPI2; version=%d", *interfaceVersion);
  if(!pFunctionTable) {
    LOG_ERROR(PLID, "GetEntityAPI2 called with null pFunctionTable");
    return(FALSE);
  }
  else if(*interfaceVersion != INTERFACE_VERSION) {
    LOG_ERROR(PLID, "GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
    *interfaceVersion = INTERFACE_VERSION;
    return(FALSE);
  }
  memcpy(pFunctionTable, &g_EntityAPI_Table, sizeof(DLL_FUNCTIONS));
  g_pFunctionTable=pFunctionTable;
  return(TRUE);
}

C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion) {
  LOG_DEVELOPER(PLID, "called: GetEntityAPI2_Post; version=%d", *interfaceVersion);
  if(!pFunctionTable) {
    LOG_ERROR(PLID, "GetEntityAPI2_Post called with null pFunctionTable");
    return(FALSE);
  }
  else if(*interfaceVersion != INTERFACE_VERSION) {
    LOG_ERROR(PLID, "GetEntityAPI2_Post version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
    *interfaceVersion = INTERFACE_VERSION;
    return(FALSE);
  }
  memcpy(pFunctionTable, &g_EntityAPI_Post_Table, sizeof(DLL_FUNCTIONS));
  g_pFunctionTable_Post=pFunctionTable;
  return(TRUE);
}

C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion) {
  LOG_DEVELOPER(PLID, "called: GetEngineFunctions; version=%d", *interfaceVersion);
  if(!pengfuncsFromEngine) {
    LOG_ERROR(PLID, "GetEngineFunctions called with null pengfuncsFromEngine");
    return(FALSE);
  }
  else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
    LOG_ERROR(PLID, "GetEngineFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
    *interfaceVersion = ENGINE_INTERFACE_VERSION;
    return(FALSE);
  }
  memcpy(pengfuncsFromEngine, &g_EngineFuncs_Table, sizeof(enginefuncs_t));
  g_pengfuncsTable=pengfuncsFromEngine;
  return TRUE;
}

C_DLLEXPORT int GetEngineFunctions_Post(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion) {
  LOG_DEVELOPER(PLID, "called: GetEngineFunctions_Post; version=%d", *interfaceVersion);
  if(!pengfuncsFromEngine) {
    LOG_ERROR(PLID, "GetEngineFunctions_Post called with null pengfuncsFromEngine");
    return(FALSE);
  }
  else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
    LOG_ERROR(PLID, "GetEngineFunctions_Post version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
    *interfaceVersion = ENGINE_INTERFACE_VERSION;
    return(FALSE);
  }
  memcpy(pengfuncsFromEngine, &g_EngineFuncs_Post_Table, sizeof(enginefuncs_t));
  g_pengfuncsTable_Post=pengfuncsFromEngine;
  return TRUE;

}

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion) {
  LOG_DEVELOPER(PLID, "called: GetNewDLLFunctions; version=%d", *interfaceVersion);
  if(!pNewFunctionTable) {
    LOG_ERROR(PLID, "GetNewDLLFunctions called with null pNewFunctionTable");
    return(FALSE);
  }
  else if(*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
    LOG_ERROR(PLID, "GetNewDLLFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, NEW_DLL_FUNCTIONS_VERSION);
    *interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
    return(FALSE);
  }
  memcpy(pNewFunctionTable, &g_NewFuncs_Table, sizeof(NEW_DLL_FUNCTIONS));
  g_pNewFunctionsTable=pNewFunctionTable;
  return TRUE;
}

C_DLLEXPORT int GetNewDLLFunctions_Post( NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion ) {
  LOG_DEVELOPER(PLID, "called: GetNewDLLFunctions_Post; version=%d", *interfaceVersion);
  if(!pNewFunctionTable) {
    LOG_ERROR(PLID, "GetNewDLLFunctions_Post called with null pNewFunctionTable");
    return(FALSE);
  }
  else if(*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
    LOG_ERROR(PLID, "GetNewDLLFunctions_Post version mismatch; requested=%d ours=%d", *interfaceVersion, NEW_DLL_FUNCTIONS_VERSION);
    *interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
    return(FALSE);
  }
  memcpy(pNewFunctionTable, &g_NewFuncs_Post_Table, sizeof(NEW_DLL_FUNCTIONS));
  g_pNewFunctionsTable_Post=pNewFunctionTable;
  return TRUE;
}


static META_FUNCTIONS g_MetaFunctions_Table =
{
  NULL,
  NULL,
  GetEntityAPI2,
  GetEntityAPI2_Post,
  GetNewDLLFunctions,
  GetNewDLLFunctions_Post,
  GetEngineFunctions,
  GetEngineFunctions_Post
};

C_DLLEXPORT int Meta_Query(char *ifvers, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs) {
  if(!pMetaUtilFuncs) {
    UTIL_LogPrintf("[%s] ERROR: Meta_Query called with null pMetaUtilFuncs\n", Plugin_info.logtag);
    return(FALSE);
  }
  gpMetaUtilFuncs = pMetaUtilFuncs;
  *pPlugInfo = &Plugin_info;
  if(!FStrEq(ifvers, Plugin_info.ifvers)) {
    int mmajor=0, mminor=0, pmajor=0, pminor=0;
    LOG_MESSAGE(PLID, "WARNING: meta-interface version mismatch; requested=%s ours=%s", Plugin_info.logtag, ifvers);
    sscanf(ifvers, "%d:%d", &mmajor, &mminor);
    sscanf(META_INTERFACE_VERSION, "%d:%d", &pmajor, &pminor);
    if(pmajor > mmajor || (pmajor==mmajor && pminor > mminor)) {
      LOG_ERROR(PLID, "metamod version is too old for this module; update metamod");
      return(FALSE);
    }
    else if(pmajor < mmajor) {
      LOG_ERROR(PLID, "metamod version is incompatible with this module; please find a newer version of this module");
      return(FALSE);
    }
    else if(pmajor==mmajor && pminor < mminor) {
      LOG_MESSAGE(PLID, "WARNING: metamod version is newer than expected; consider finding a newer version of this module");
    }
    else {
      LOG_ERROR(PLID, "unexpected version comparison; metavers=%s, mmajor=%d, mminor=%d; plugvers=%s, pmajor=%d, pminor=%d", ifvers, mmajor, mminor, META_INTERFACE_VERSION, pmajor, pminor);
    }
  }
  #ifdef FN_META_QUERY
  FN_META_QUERY();
  #endif  // FN_META_QUERY
  return TRUE;
}


C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs) {
  if(now > Plugin_info.loadable) {
    LOG_ERROR(PLID, "Can't load module right now");
    return(FALSE);
  }
  if(!pMGlobals) {
    LOG_ERROR(PLID, "Meta_Attach called with null pMGlobals");
    return(FALSE);
  }
  gpMetaGlobals=pMGlobals;
  if(!pFunctionTable) {
    LOG_ERROR(PLID, "Meta_Attach called with null pFunctionTable");
    return(FALSE);
  }
  memcpy(pFunctionTable, &g_MetaFunctions_Table, sizeof(META_FUNCTIONS));
  gpGamedllFuncs=pGamedllFuncs;

  #ifdef FN_META_ATTACH
  FN_META_ATTACH();
  #endif  // FN_META_ATTACH
  return TRUE;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason) {
  if(now > Plugin_info.unloadable && reason != PNL_CMD_FORCED) {
    LOG_ERROR(PLID, "Can't unload plugin right now");
    return(FALSE);
  }

  #ifdef FN_META_DETACH
  FN_META_DETACH();
  #endif  // FN_META_DETACH

  return TRUE;
}


C_DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals) {
  memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
  gpGlobals = pGlobals;
}

#endif  // #ifdef USE_METAMOD

/******************************** AMX API *************************************/

#undef DLLEXPORT
#ifndef LINUX
#define DLLEXPORT   __declspec(dllexport)
#else
#define DLLEXPORT
#define WINAPI
#endif

#undef C_DLLEXPORT
#define C_DLLEXPORT extern "C" DLLEXPORT

pfnamx_engine_g* g_engAmxFunc;
pfnmodule_engine_g* g_engModuleFunc;

static module_info_s module_info = {
  MODULE_NAME,
  MODULE_AUTHOR,
  MODULE_VERSION,
  AMX_INTERFACE_VERSION,
#ifdef MODULE_RELOAD
  RELOAD_MODULE
#else
  STATIC_MODULE
#endif
};

C_DLLEXPORT int AMX_Query(module_info_s** info) {
  *info=&module_info;
  #ifdef FN_AMX_QUERY
  FN_AMX_QUERY();
  #endif // FN_AMX_QUERY
  return 1;
}

C_DLLEXPORT int AMX_Attach(pfnamx_engine_g* amxeng, pfnmodule_engine_g* meng) {
  g_engAmxFunc=amxeng;
  g_engModuleFunc=meng;
  #ifdef FN_AMX_ATTACH
  FN_AMX_ATTACH();
  #endif // FN_AMX_ATACH
  return 1;
}

C_DLLEXPORT int AMX_PluginsLoaded() {
  #ifdef FN_AMX_PLUGINSLOADED
  FN_AMX_PLUGINSLOADED();
  #endif // FN_AMX_PLUGINSLOADED
  return 1;
}

C_DLLEXPORT int AMX_Detach() {
  #ifdef FN_AMX_DETACH
  FN_AMX_DETACH();
  #endif // FN_AMX_DETACH
  return 1;
}

// This portion of code removes -lstdc++ dependancy - special thanks to Jussi "hullu" Kivilinna
#ifdef LINUX
  #include <malloc.h>
#endif

void * operator new(size_t size) {
  return(calloc(1, size));
}

void * operator new[](size_t size) {
  return(calloc(1, size));
}

void operator delete(void * ptr) {
  if(ptr)
    free(ptr);
}

void operator delete[](void * ptr) {
  if(ptr)
    free(ptr);
}

/**************************** dlls/util.cpp ***********************************/

#ifdef USE_METAMOD

/***************
*
* Copyright (c) 1999, 2000 Valve LLC. All rights reserved.
*
* This product contains software technology licensed from Id
* Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
* All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
***************/


//#include <extdll.h>
#include "sdk_util.h"
#include <cbase.h>

#include <string.h>

#include "osdep.h"

char* UTIL_VarArgs(char *format, ...) {
  va_list argptr;
  static char string[1024];
  va_start(argptr, format);
  vsnprintf(string, sizeof(string), format, argptr);
  va_end(argptr);
  return string;
}

void UTIL_LogPrintf(char *fmt, ...) {
  va_list argptr;
  static char string[1024];
  va_start(argptr, fmt);
  vsnprintf(string, sizeof(string), fmt, argptr);
  va_end(argptr);
  ALERT(at_logged, "%s", string);
}

void UTIL_HudMessage(CBaseEntity *pEntity, const hudtextparms_t &textparms, const char *pMessage) {
  if(!pEntity)
    return;

  MESSAGE_BEGIN( MSG_ONE, SVC_TEMPENTITY, NULL, ENT(pEntity->pev) );
  WRITE_BYTE( TE_TEXTMESSAGE );
  WRITE_BYTE( textparms.channel & 0xFF );

  WRITE_SHORT( FixedSigned16( textparms.x, 1<<13 ) );
  WRITE_SHORT( FixedSigned16( textparms.y, 1<<13 ) );
  WRITE_BYTE( textparms.effect );

  WRITE_BYTE( textparms.r1 );
  WRITE_BYTE( textparms.g1 );
  WRITE_BYTE( textparms.b1 );
  WRITE_BYTE( textparms.a1 );

  WRITE_BYTE( textparms.r2 );
  WRITE_BYTE( textparms.g2 );
  WRITE_BYTE( textparms.b2 );
  WRITE_BYTE( textparms.a2 );

  WRITE_SHORT( FixedUnsigned16( textparms.fadeinTime, 1<<8 ) );
  WRITE_SHORT( FixedUnsigned16( textparms.fadeoutTime, 1<<8 ) );
  WRITE_SHORT( FixedUnsigned16( textparms.holdTime, 1<<8 ) );

  if ( textparms.effect == 2 ) {
    WRITE_SHORT( FixedUnsigned16( textparms.fxTime, 1<<8 ) );
  }

  if ( strlen( pMessage ) < 512 ) {
    WRITE_STRING( pMessage );
  }
  else {
    char tmp[512];
    strncpy( tmp, pMessage, 511 );
    tmp[511] = 0;
    WRITE_STRING( tmp );
  }
  MESSAGE_END();
}

short FixedSigned16( float value, float scale ) {
  int output;

  output = (int) (value * scale);

  if ( output > 32767 )
    output = 32767;

  if ( output < -32768 )
    output = -32768;

  return (short)output;
}

unsigned short FixedUnsigned16( float value, float scale ) {
  int output;

  output = (int) (value * scale);
  if ( output < 0 )
    output = 0;
  if ( output > 0xFFFF )
    output = 0xFFFF;

  return (unsigned short)output;
}

#endif // USE_METAMOD
