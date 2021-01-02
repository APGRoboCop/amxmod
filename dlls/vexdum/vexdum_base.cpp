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

/* Damage System Rev3:
    - Monsters can now take damage
    - Monsters can die by damage
    - Updated the natives to reflect damage to monsters & breakables
    - Fixed odd death bug
    - Added focus on killer when victim dies - Hydralisk
    - Fixed frag for Killer if it was a team kill (-1 frag)
    - Fixed incorrect deathmsg displayed
    - Fixed dying by NPC - KRoT@L
    - Added forward monster_hurt(monster, attacker, damage);
    - Fixed the death not dead bug yet again!
    - Fixed RadiusDamage - KRoT@L

   Damage System Rev2:
  - Armor now taken into consideration
  - Better Damage algorithm
  - Knockback effects
  - New CVAR sv_friendlyfire dependancy
  - RadiusDamage no longer goes through Walls etc
  - New CVAR sv_knockback1 to control the kickback effect
  - New CVAR sv_knockback2 to control the kickback effect
  - Breakables will break under radius_damage - SpLoRyGoN

   Damage System Rev1:
  - Initial Release

   Todo:
      - Make monsters take damage normally do a hp check if < 1 use trigger_hurt
*/

#include "vexdum.h"

BOOL is_breakable(edict_t* pBreak) {
  if(FStrEq("func_breakable", STRING(pBreak->v.classname))
    || (FStrEq("func_pushable", STRING(pBreak->v.classname))
    && (pBreak->v.spawnflags & SF_PUSH_BREAKABLE))) {
    return true;
  }
  return false;
}

BOOL is_monster(edict_t* pMonster) {
  if(pMonster->v.flags & FL_MONSTER) {
    return true;
  }
  return false;
}

// Damage Monsters
void hurt_monster(edict_t* pMonster, float dmg, int bit, const float *origin) {
  int mdmg = (int)pMonster->v.health;
  pMonster->v.health -= dmg;

  // No need to create a trigger_hurt unless we are going to kill the monster ;)
  if((int)pMonster->v.health < 1) {
    int hurt = MAKE_STRING("trigger_hurt");
    char hbit[16];
    char horigin[16];
    snprintf(hbit, 15, "%i", bit);
    snprintf(horigin, 15, "%f %f %f", origin[0], origin[1], origin[2]);

    edict_t* pEntity = CREATE_NAMED_ENTITY(hurt);
    KeyValueData pkvd1;
    pkvd1.szClassName = "trigger_hurt";
    pkvd1.szKeyName = "dmg";
    pkvd1.szValue = "1.0";
    pkvd1.fHandled = FALSE;
    MDLL_KeyValue(pEntity, &pkvd1);

    KeyValueData pkvd2;
    pkvd2.szClassName = "trigger_hurt";
    pkvd2.szKeyName = "damagetype";
    pkvd2.szValue = hbit;
    pkvd2.fHandled = FALSE;
    MDLL_KeyValue(pEntity, &pkvd2);

    KeyValueData pkvd3;
    pkvd3.szClassName = "trigger_hurt";
    pkvd3.szKeyName = "origin";
    pkvd3.szValue = horigin;
    pkvd3.fHandled = FALSE;
    MDLL_KeyValue(pEntity, &pkvd3);

    MDLL_Spawn(pEntity);
    MDLL_Touch(pEntity, pMonster);
    REMOVE_ENTITY(pEntity);
  }
  mdmg -= (int)pMonster->v.health;
  g_forwards.executeForward(FF_MonsterHurt, ENTINDEX(pMonster), ENTINDEX(pMonster->v.dmg_inflictor), mdmg);
}

float ArmorDamage(edict_t* pVictim, float dmg, int bit) {
  float flRatio = 0.2;
  float flBonus = 0.5;
  if(bit & DMG_BLAST) {
    // blasts damage armor more.
    flBonus *= 2;
  }
  if(pVictim->v.armorvalue && !(bit & (DMG_FALL | DMG_DROWN))) { // armor doesn't protect against fall or drown damage!
    float flNew = dmg * flRatio;
    float flArmor = (dmg - flNew) * flBonus;

    // Does this use more armor than we have?
    if(flArmor > pVictim->v.armorvalue) {
      flArmor = pVictim->v.armorvalue;
      flArmor *= (1/flBonus);
      flNew = dmg - flArmor;
      pVictim->v.armorvalue = 0;
    }
    else {
      pVictim->v.armorvalue -= flArmor;
    }
    dmg = flNew;
  }
  // Lets knock the view about abit
  pVictim->v.punchangle.x = -4;
  return dmg;
}

// Radius Damage emulation
void RadiusDamage(edict_t* pAttacker, float dmg, Vector vecSrc, float rad, int bit, const char* weapon, int hs) {
  edict_t *pTarget = NULL;
  TraceResult tr;
  float falloff;
  float dist;
  float newdmg;
  Vector vecSpot;
  Vector vecSee;
  Vector vecPush;

  if(rad > 0.0) {
    falloff = dmg / rad;
  }
  else {
    falloff = 1.0;
  }
  vecSrc.z += 1; // In case grenade is lying on the ground

  while((pTarget = UTIL_FindEntityInSphere(pTarget, vecSrc, rad)) != NULL) {
    // Make sure an entity is allowed to take damage
    if(pTarget->v.takedamage > DAMAGE_NO) {
      vecSpot = (pTarget->v.absmin + pTarget->v.absmax) * 0.5;
      vecSee = (pAttacker->v.absmin + pAttacker->v.absmax) * 0.5;
      TRACE_LINE(vecSee, vecSpot, FALSE, pAttacker, &tr);
      // Explosion can 'see' this entity, so hurt them!
      if(tr.flFraction == 1.0 || ENTINDEX(tr.pHit) == ENTINDEX(pTarget)) {
        // Work out the distance between impact and entity
        dist = (vecSrc - tr.vecEndPos).Length() * falloff;
        // Damage algorithm, its just that easy :)
        newdmg = dmg - dist;
        // Knockback Effect
        if(pTarget->v.flags & (FL_CLIENT | FL_FAKECLIENT) && (bit & (DMG_BLAST | DMG_CLUB | DMG_SHOCK | DMG_SONIC | DMG_ENERGYBEAM | DMG_MORTAR))) {
          vecPush = (pTarget->v.origin - (pAttacker->v.absmin + pAttacker->v.absmax) * 0.5).Normalize();
          if(newdmg < 60.0) {
            pTarget->v.velocity = pTarget->v.velocity + vecPush * newdmg * knockback1->value;
          }
          else {
            pTarget->v.velocity = pTarget->v.velocity + vecPush * newdmg * knockback2->value;
          }
        }
        // Send info to Damage system
        Damage(pTarget, pAttacker, vecSrc, newdmg, bit, weapon, hs);
      }
    }
  }
  pTarget = NULL;
}

// Damage emulation
void Damage(edict_t* pVictim, edict_t* pAttacker, const float *origin, float dmg, int bit, const char* weapon, int hs) {
  // Make sure an entity is allowed to take damage
  if(pVictim->v.takedamage > DAMAGE_NO) {
    // Breakable Check
    if(is_breakable(pVictim) && (int)dmg > 0) {
      MDLL_Use(pVictim, pAttacker);
    }
    // Monster Check
    if(is_monster(pVictim) && (int)dmg > 0) {
      pVictim->v.dmg_inflictor = pAttacker;
      hurt_monster(pVictim, dmg, bit, origin);
    }
    // Player Check
    if(pVictim->v.flags & (FL_CLIENT | FL_FAKECLIENT)) {
      int vTeam = GET_PLAYER_POINTER(pVictim)->team;
      int aTeam = 0;
      if((ENTINDEX(pAttacker) > 0 && ENTINDEX(pAttacker) <= gpGlobals->maxClients) || (ENTINDEX(pAttacker->v.owner) > 0 && ENTINDEX(pAttacker->v.owner) <= gpGlobals->maxClients)) {
        aTeam = GET_PLAYER_POINTER((ENTINDEX(pAttacker) > gpGlobals->maxClients) ? (pAttacker->v.owner) : (pAttacker))->team;
      }
      if((friendlyfire->value) || (vTeam != aTeam)) {
        // Recalculate the damage since we might have armor
        dmg = ArmorDamage(pVictim, dmg, bit);
        // Only allow damage to process if more than 0.5
        if((int)dmg > 0) {
          // Setting to externally flag who last attacked the Victim, pretty neat huh?
          pVictim->v.dmg_inflictor = pAttacker;
          pVictim->v.dmg_take += dmg;
          // Register the Damage Event
          MESSAGE_BEGIN( MSG_ONE, gmsgDamage, NULL, pVictim );
          WRITE_BYTE( (int)pVictim->v.dmg_save );
          WRITE_BYTE( (int)pVictim->v.dmg_take );
          WRITE_LONG( bit );
          WRITE_COORD( origin[0] );
          WRITE_COORD( origin[1] );
          WRITE_COORD( origin[2] );
          MESSAGE_END();

          // Kill the victim
          if((int)dmg >= (int)pVictim->v.health) {
            pVictim->v.health = 0.0;
            // Send info to Death system
            Death(pVictim, pAttacker, weapon, hs);
          }
          // Take health away from victim
          else {
            pVictim->v.health -= dmg;
          }
        }
      }
    }
  }
}

// Death emulation
void Death(edict_t* pVictim, edict_t* pKiller, const char* weapon, int hs) {
  // Make sure an entity is allowed to take damage
  if(pVictim->v.takedamage > DAMAGE_NO) {
    // Breakable Check
    if(is_breakable(pVictim)) {
      MDLL_Use(pVictim, pKiller);
    }
    // Monster Check
    if(is_monster(pVictim)) {
      pVictim->v.dmg_inflictor = pKiller;
      float dmg = pVictim->v.health;
      int bit = DMG_BULLET;
      const float *origin = pVictim->v.origin;
      hurt_monster(pVictim, dmg, bit, origin);
    }
    // Player Check
    if(pVictim->v.flags & (FL_CLIENT | FL_FAKECLIENT)) {
      pVictim->v.dmg_inflictor = pKiller;
      edict_t* inflictor = pKiller->v.owner;

      // See if it is a player attacking with a default weapon
      if(pKiller->v.flags & (FL_CLIENT | FL_FAKECLIENT)) {
        // We only modify the weapon if it = 0, otherwise its been specified
        if(strcmp(weapon, "") == 0) {
          char view_weapon[64];
          // Get the name from the view model
          weapon = STRING(pKiller->v.viewmodel);

          // Strip out the beginning of the viewmodel (models/v_)
          if(strncmp(weapon, "models/v_", 9) == 0) {
            strcpy(view_weapon, weapon + 9);
          }

          // Strip out the end of viewmodel (.mdl)
          view_weapon[strlen(view_weapon) - 4] = '\0';
          weapon = view_weapon;
        }
      }
      // See if its an entity attacking, if so lets find its owner
      else if(ENTINDEX(inflictor) > 0 && ENTINDEX(inflictor) <= gpGlobals->maxClients) {
        // We only modify the weapon if it = 0, otherwise its been specified
        if(strcmp(weapon, "") == 0) {
          weapon = STRING(pKiller->v.classname);
          // Strip out the default part of weapon name (HLSDK)
          if(strncmp(weapon, "weapon_", 7) == 0) {
            weapon += 7;
          }
          else if(strncmp(weapon, "monster_", 8) == 0) {
            weapon += 8;
          }
          else if(strncmp(weapon, "func_", 5) == 0) {
            weapon += 5;
          }
        }
        // Check to see if the victim is the owner
        if(inflictor == pVictim) {
          pKiller = pVictim;
        }
        else {
          pKiller = inflictor;
        }
      }
      // Register the Death Event
      MESSAGE_BEGIN( MSG_ALL, gmsgDeathMsg );
      WRITE_BYTE( (ENTINDEX(pKiller) > gpGlobals->maxClients) ? 0 : ENTINDEX(pKiller) );
      WRITE_BYTE( ENTINDEX(pVictim) );
      WRITE_BYTE( hs );
      WRITE_STRING( weapon );
      MESSAGE_END();
      // Log this kill
      if(pVictim == pKiller) { // killed self
        UTIL_LogPrintf( "\"%s<%i><%s><%s>\" killed self with \"%s\"\n",
          STRING( pVictim->v.netname ),
          GETPLAYERUSERID( pVictim ),
          GETPLAYERAUTHID( pVictim ),
          GET_PLAYER_POINTER(pVictim)->teamname,
          weapon );
      }
      // Killed by another player
      else if(pKiller->v.flags & (FL_CLIENT | FL_FAKECLIENT)) {
        UTIL_LogPrintf( "\"%s<%i><%s><%s>\" killed \"%s<%i><%s><%s>\" with \"%s\"\n",
          STRING( pKiller->v.netname ),
          GETPLAYERUSERID( pKiller ),
          GETPLAYERAUTHID( pKiller ),
          GET_PLAYER_POINTER(pKiller)->teamname,
          STRING( pVictim->v.netname ),
          GETPLAYERUSERID( pVictim ),
          GETPLAYERAUTHID( pVictim ),
          GET_PLAYER_POINTER(pVictim)->teamname,
          weapon );

        if(GET_PLAYER_POINTER(pKiller)->team != GET_PLAYER_POINTER(pVictim)->team) {
          // Give Killer credit for this kill
          pKiller->v.frags += 1;
        }
        else {
          pKiller->v.frags -= 1;
        }
        // Update the scoreboard for the killer
        MESSAGE_BEGIN( MSG_ALL, GET_USER_MSG_ID(PLID, "ScoreInfo", NULL) );
        WRITE_BYTE( ENTINDEX(pKiller) );
        WRITE_SHORT( (int)pKiller->v.frags );
        WRITE_SHORT( GET_PLAYER_POINTER(pKiller)->deaths );
        WRITE_SHORT( 0 );
        WRITE_SHORT( GET_PLAYER_POINTER(pKiller)->team );
        MESSAGE_END();
        // Give Victim back 1 point since they didn't kill themselves
        pVictim->v.frags += 1;
      }
      // Killed by something else?
      else {
        UTIL_LogPrintf( "\"%s<%i><%s><%s>\" killed by \"%s\"\n",
          STRING( pVictim->v.netname ),
          GETPLAYERUSERID( pVictim ),
          GETPLAYERAUTHID( pVictim ),
          GET_PLAYER_POINTER(pVictim)->teamname,
          weapon );
          // Give Victim back 1 point since they didn't commit suicide
          pVictim->v.frags += 1;
      }
      gInfo.iMessageBlock[ gmsgDeathMsg ] = BLOCK_ONCE;
      if(g_pengfuncsTable->pfnClientPrintf == NULL) {
        g_pengfuncsTable->pfnClientPrintf = ClientPrintf;
      }
      if(g_pengfuncsTable->pfnAlertMessage == NULL) {
        g_pengfuncsTable->pfnAlertMessage = AlertMessage;
      }
      gInfo.logBlock = true;
      // Kill the client, since the relevent logging blocks are in place
      MDLL_ClientKill(pVictim);
      g_pengfuncsTable->pfnClientPrintf = NULL;
      if(g_forwards.forwardsExist(FF_AlertMessage) == false) {
        g_pengfuncsTable->pfnAlertMessage = NULL;
      }
      // Show the Victim the killing location
      pVictim->v.iuser3 = (ENTINDEX(pKiller) > gpGlobals->maxClients) ? 0 : ENTINDEX(pKiller);
    }
  }
}
/* End of Damage System Rev3 */

// Log printing
/*void UTIL_LogPrintf(char *fmt, ...) {
  va_list     argptr;
  static char   string[1024];
  va_start    (argptr, fmt);
  vsprintf    (string, fmt, argptr);
  va_end      (argptr);
  // Print to server console
  ALERT(at_logged, "%s", string);
}

char* UTIL_VarArgs(char *fmt, ...) {
  va_list   argptr;
  static char string[1024];
  va_start  (argptr, fmt);
  vsprintf  (string, fmt, argptr);
  va_end    (argptr);
  return string;
}*/

// Find an entity within a sphere for radius damage
edict_t *UTIL_FindEntityInSphere(edict_t *pStart, const Vector &vecCenter, float flRadius) {
  if(!pStart) pStart = NULL;
  pStart = FIND_ENTITY_IN_SPHERE(pStart, vecCenter, flRadius);
  if(!FNullEnt(pStart)) return pStart;

  return NULL;
}

// gmsgResetHUD
/*void Client_ResetHUD(void* mValue){
  if(mPlayer) {
    if(mPlayer->modeled) {
      MDLL_ClientUserInfoChanged(mPlayer->pEdict, GET_INFOKEYBUFFER(mPlayer->pEdict));
    }
  }
}*/

// gmsgScoreInfo
void Client_ScoreInfo(void* mValue) {
  static int id;
  static int deaths;
  switch(mState++) {
    case 0:
      id = *(int*)mValue;
      break;
    case 2:
      deaths = *(int*)mValue;
      break;
    case 4:
      if(id < 1 || id > gpGlobals->maxClients) break;
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(id);
      pPlayer->deaths = deaths;
      pPlayer->team = *(int*)mValue;
  }
}

// gmsgTeamInfo
void Client_TeamInfo(void* mValue) {
  static int id;
  switch(mState++) {
    case 0:
      id = *(int*)mValue;
      break;
    case 1:
      if(id < 1 || id > gpGlobals->maxClients) break;
      CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);
      char* team = (char*)mValue;
      pPlayer->teamname = team;
  }
}

void Client_TextMsg(void* mValue) {
  if(g_bmod_cstrike && countModelled(false) >= MAX_MODELLED) {
    static bool restart = false;
    static const char* text;
    if(!mPlayer && mState == 1) {
      text = (const char*)mValue;
      if(text[0] == '#' && text[1] == 'G') {
        if(text[6] == 'w' && text[11] == 'r' && text[19] == 'i' && strlen(text) == 21) {
          restart = true;
        }
      }
    }
    if(!mPlayer && mState == 2 && restart == true) {
      restart = false;
      text = (const char*)mValue;
      static CPlayer *pPlayer;
      static int i;
      for(i = 1; i <= gpGlobals->maxClients; ++i) {
        pPlayer = GET_PLAYER_POINTER_I(i);
        if(pPlayer->modeled == true) {
          pPlayer->fModelSet = gpGlobals->time + RANDOM_FLOAT(0.20f, 0.80f);
          pPlayer->SetModel = -2;
          pPlayer->UnSetModel = true;
        }
      }
    }
    ++mState;
  }
}

void Client_SendAudio(void* mValue) {
  if(g_bmod_cstrike && countModelled(false) >= MAX_MODELLED) {
    if(!mPlayer && mState == 1) {
      static const char* text;
      text = (const char*)mValue;
      if(text[0] == '%' && text[1] == '!' && text[2] == 'M') {
        if((text[7] == 't' && text[10] == 'w')
        || (text[7] == 'c' && text[9] == 'w')
        || (text[7] == 'r' && text[12] == 'd')) {
          static CPlayer *pPlayer;
          static int i;
          for(i = 1; i <= gpGlobals->maxClients; ++i) {
            pPlayer = GET_PLAYER_POINTER_I(i);
            if(pPlayer->modeled == true) {
              pPlayer->fModelSet = gpGlobals->time + 4.0f + RANDOM_FLOAT(0.00f, 0.60f);
              pPlayer->SetModel = 0;
              pPlayer->UnSetModel = true;
            }
          }
        }
      }
    }
    ++mState;
  }
}

int countModelled(bool checkUnsetModel) {
  int num = 0;
  int i;
  for(i = 1; i <= gpGlobals->maxClients; ++i) {
    if((GET_PLAYER_POINTER_I(i)->modeled == false) &&
    (checkUnsetModel == false || (checkUnsetModel == true && GET_PLAYER_POINTER_I(i)->UnSetModel == false))) {
      continue;
    }
    ++num;
  }
  return num;
}

bool disableVoiceClientListening() {
  bool same = true;
  int spkflags = -1;
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    if(spkflags == -1) {
      spkflags = (int)GET_PLAYER_POINTER_I(i)->speakflags[0];
    }
    else {
      if((int)GET_PLAYER_POINTER_I(i)->speakflags[0] != spkflags) {
        same = false;
      }
    }
  }
  if(same) {
    return (spkflags == 8) ? false : (spkflags == SPEAK_NORMAL) ? true : false;
  }
  return false;
}
