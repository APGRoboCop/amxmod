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

#include "rank.h"

#define RANK_VERSION_2005_1 5
#define RANK_VERSION_2006_1 6
#define RANK_VERSION_2006_3 7

#define RANK_VERSION_AMXX 11


#define SZTEMP_LENGTH 127

stats_t* rank_head;
stats_t* rank_tail;

score_plug_t scr_plug;

void load_get_score_plugin(const char* path, char* error){
  if ((MF_LoadAmxScript(&scr_plug.amx,&scr_plug.program,path,error,0)!=AMX_ERR_NONE)||
    (AMX_ALLOT(&scr_plug.amx, 8 , &scr_plug.amx_addr1, &scr_plug.phys_addr1)!=AMX_ERR_NONE)||
    (AMX_ALLOT(&scr_plug.amx, 8 , &scr_plug.amx_addr2, &scr_plug.phys_addr2)!=AMX_ERR_NONE)||
    (AMX_ALLOT(&scr_plug.amx, 12 , &scr_plug.amx_addr3, &scr_plug.phys_addr3)!=AMX_ERR_NONE)||
    (AMX_FINDPUBLIC(&scr_plug.amx,"get_score",&scr_plug.func)!=AMX_ERR_NONE)){
    LOG_CONSOLE( PLID, "Couldn't load plugin (file \"%s\")",path);
    MF_UnloadAmxScript(&scr_plug.amx,&scr_plug.program);
    scr_plug.program = NULL;
  }
}

int get_score(stats_t* p){
  if (scr_plug.program) {
    scr_plug.phys_addr1[0] = p->kills;
    scr_plug.phys_addr1[1] = p->deaths;
    scr_plug.phys_addr1[2] = p->hs;
    scr_plug.phys_addr1[3] = p->tks;
    scr_plug.phys_addr1[4] = p->shots;
    scr_plug.phys_addr1[5] = p->hits;
    scr_plug.phys_addr1[6] = p->damage;
    for(int i = 1; i < 8; ++i)
      scr_plug.phys_addr2[i] = p->body_hits[i];
    scr_plug.phys_addr3[0] = p->had_bomb;
    scr_plug.phys_addr3[1] = p->bomb_planted;
    scr_plug.phys_addr3[2] = p->bomb_exploded;
    scr_plug.phys_addr3[3] = p->bomb_defusions;
    scr_plug.phys_addr3[4] = p->bomb_defused ;
    scr_plug.phys_addr3[5] = p->hostages_touched;
    scr_plug.phys_addr3[6] = p->hostages_rescued;
    scr_plug.phys_addr3[7] = p->was_vip;
    scr_plug.phys_addr3[8] = p->escaped_as_vip;
    scr_plug.phys_addr3[9] = p->killed_vip;
    scr_plug.phys_addr3[10] = p->suicides;
    scr_plug.phys_addr3[11] = p->playtime;
    cell result = 0;
    int err;
    if((err = AMX_EXEC(&scr_plug.amx,&result, scr_plug.func, 3,
      scr_plug.amx_addr1,scr_plug.amx_addr2,scr_plug.amx_addr3)) != AMX_ERR_NONE) {
      LOG_CONSOLE( PLID, "Run time error %d on line %ld (plugin \"%s\")",
        err,scr_plug.amx.curline,LOCALINFO("csstats_score"));
      return p->kills - p->deaths - (2*p->tks);
    }
    return result;
  }
  return p->kills - p->deaths - (2*p->tks);
}

void delete_rank()
{
  while(rank_head) {
    stats_t* p = rank_head;
    rank_head = rank_head->next;
    if(p->flags & (1<<0)) {
      free(p->name);
      free(p->unique);
    }
    free(p);
  }
  rank_tail = NULL;
}

void delete_pos(stats_t* p)
{
  if(p->prev)
    p->prev->next = p->next;
  else
    rank_head = p->next;

  if(p->next)
    p->next->prev = p->prev;
  else
    rank_tail = p->prev;

  if(p->flags&(1<<0)) {
    free(p->name);
    free(p->unique);
  }
  free(p);
}

stats_t* find_name_in_rank(const char* unique, const char* name)
{
  stats_t* p = rank_head;
  while(p) {
    if(strcmp(unique, p->unique) == 0)
      break;
    p = p->next;
  }

  if(!p) {
    p  = (stats_t*)malloc(sizeof(stats_t));
    if (!p) return 0;
    memset(p, 0, sizeof(stats_t));
    p->namelen = strlen(name) + 1;
    p->name = new char[p->namelen];
    if(p->name)
      strcpy(p->name, name);
    else
      p->namelen = 0;
    p->uniquelen = strlen(unique) + 1;
    p->unique = new char[p->uniquelen];
    if(p->unique)
      strcpy(p->unique, unique);
    else
      p->uniquelen = 0;
    p->flags = (1<<0);
    p->id = rank_tail ? (rank_tail->id + 1) : 1;
    p->prev = rank_tail;
    p->next = NULL;
    if(rank_tail)
      rank_tail->next = p;
    else
      rank_head = p;
    rank_tail = p;
  }

  return p;
}

void score_stats_in_rank(const char* name, stats_t* p, stats_t* a)
{
  if (!p || !a) return;

  p->kills += a->kills;
  p->deaths += a->deaths;
  p->hs += a->hs;
  p->tks += a->tks;
  p->shots += a->shots;
  p->hits += a->hits;
  p->damage += a->damage;
  
  p->had_bomb += a->had_bomb;
  p->bomb_planted += a->bomb_planted;
  p->bomb_exploded += a->bomb_exploded;
  p->bomb_defusions += a->bomb_defusions;
  p->bomb_defused += a->bomb_defused;
  p->hostages_touched += a->hostages_touched;
  p->hostages_rescued += a->hostages_rescued;
  p->was_vip += a->was_vip;
  p->escaped_as_vip += a->escaped_as_vip;
  p->killed_vip += a->killed_vip;
  p->suicides += a->suicides;
  p->playtime += a->playtime;
  
  short int len = strlen(name) + 1;
  if(len != p->namelen) {
    free(p->name);
    p->namelen = len;
    p->name = new char[p->namelen];
    if(p->name)
      strcpy(p->name, name);
    else
      p->namelen = 0;
  }
  else {
    strcpy(p->name, name);
  }

  for(int i = 1; i < 8; ++i)
    p->body_hits[i] += a->body_hits[i];

  p->score = get_score(p);

  stats_t* d = p->prev;
  while ( d && p->score >= d->score  ) {
    d->id++;
    p->id--;
    d = d->prev;
  }

  if ( d != p->prev ) {
    p->prev->next = p->next;
    if ( p->next )
      p->next->prev = p->prev;
    else
      rank_tail = p->prev;
    if ( d ) {
      p->prev = d;
      p->next = d->next;
      if ( d->next ) d->next->prev = p;
      d->next = p;
    } else {
      rank_head->prev = p;
      p->next = rank_head;
      p->prev = NULL;
      rank_head = p;
    }
  }

  d = p->next;
  while ( d && p->score < d->score  ) {
    d->id--;
    p->id++;
    d = d->next;
  }

  if ( d != p->next ) {
    if ( p->prev )
      p->prev->next = p->next;
    else
      rank_head = p->next;
    p->next->prev = p->prev;
    if ( d ) {
      p->prev = d->prev;
      p->next = d;
      if ( d->prev ) d->prev->next = p;
      d->prev = p;
    } else {
      rank_tail->next = p;
      p->prev = rank_tail;
      p->next = NULL;
      rank_tail = p;
    }
  }
}

void load_rank(const char* filename)
{
  FILE *bfp = fopen(filename, "rb");
  if(!bfp) return;
  short int amxx_ver = 0;
  fread(&amxx_ver, 1, sizeof(short int), bfp);
  if(amxx_ver == RANK_VERSION_AMXX) {
    delete_rank();
    stats_t d;
    char name[33], unique[33];
    short int j = 0;
    int dummy = 0;
    if(fread(&j, sizeof(short int), 1, bfp) != 1) {
      fclose(bfp);
      return;
    }
		while(j && !feof(bfp)) {
      // Parser fixes By AMXMODX Dev Team
			if(fread(name, sizeof(char)*j, 1, bfp) != 1) break;
			if(fread(&j, sizeof(short int), 1, bfp) != 1) break;
			if(fread(unique, sizeof(char)*j, 1, bfp) != 1) break;
			if(fread(&d.tks, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.damage, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.deaths, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.kills, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.shots, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.hits, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.hs, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.bomb_defusions, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.bomb_defused, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.bomb_planted, sizeof(int), 1, bfp) != 1) break;
			if(fread(&d.bomb_exploded, sizeof(int), 1, bfp) != 1) break;
			if(fread(d.body_hits, sizeof(d.body_hits), 1, bfp) != 1) break;
			if(fread(&dummy, sizeof(int), 1, bfp) != 1) break;
			if(fread(&j, sizeof(short int), 1, bfp) != 1) break;
			d.had_bomb = 0;
      d.hostages_touched = 0;
      d.hostages_rescued = 0;
      d.was_vip = 0;
      d.escaped_as_vip = 0;
      d.killed_vip = 0;
      d.suicides = 0;
      d.playtime = 0;
			score_stats_in_rank(name, find_name_in_rank( unique , name ), &d );
		}
    fclose(bfp);
    return;
  }
  else {
    rewind(bfp);
    int ver = 0;
    if(fread(&ver, sizeof(int), 1, bfp) != 1) {
      fclose(bfp);
      return;
    }
    if(ver == RANK_VERSION_2005_1 || ver == RANK_VERSION_2006_1) {
      delete_rank();
      int rank_num = 0;
      if(fread(&rank_num, sizeof(int), 1, bfp) != 1) {
        fclose(bfp);
        return;
      }
      stats_t d;
      char name[33], unique[33];
      for(int i = 0; i < rank_num && !feof(bfp); ++i) {
        // Parser fixes By AMXMODX Dev Team
        if(fread(name, sizeof(char) * 32, 1, bfp) != 1) break;
        if(fread(unique, sizeof(char) * 32, 1, bfp) != 1) break;
        if(fread(&d.kills, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.deaths, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.hs, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.tks, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.shots, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.hits, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.damage, sizeof(int), 1, bfp) != 1) break;
        if(fread(d.body_hits, sizeof(d.body_hits), 1, bfp) != 1) break;
        if(ver == RANK_VERSION_2006_1) {
          if(fread(&d.had_bomb, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.bomb_planted, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.bomb_exploded, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.bomb_defusions, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.bomb_defused, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.hostages_touched, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.hostages_rescued, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.was_vip, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.escaped_as_vip, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.killed_vip, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.suicides, sizeof(int), 1, bfp) != 1) break;
          if(fread(&d.playtime, sizeof(int), 1, bfp) != 1) break;
        }
        else {
          d.had_bomb = 0;
          d.bomb_planted = 0;
          d.bomb_exploded = 0;
          d.bomb_defusions = 0;
          d.bomb_defused = 0;
          d.hostages_touched = 0;
          d.hostages_rescued = 0;
          d.was_vip = 0;
          d.escaped_as_vip = 0;
          d.killed_vip = 0;
          d.suicides = 0;
          d.playtime = 0;
        }
        score_stats_in_rank(name, find_name_in_rank( unique , name ), &d );
      }
    }
    else if(ver == RANK_VERSION_2006_3) {
      delete_rank();
      stats_t d;
      char name[33], unique[33];
      short int j = 0;
      if(fread(&j, sizeof(short int), 1, bfp) != 1) {
        fclose(bfp);
        return;
      }
  		while(j && !feof(bfp)) {
        // Parser fixes By AMXMODX Dev Team
  			if(fread(name, sizeof(char)*j, 1, bfp) != 1) break;
  			name[j] = '\0';
  			if(fread(&j, sizeof(short int), 1, bfp) != 1) break;
  			if(fread(unique, sizeof(char)*j, 1, bfp) != 1) break;
  			unique[j] = '\0';
  			if(fread(&d.kills, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.deaths, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.hs, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.tks, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.shots, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.hits, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.damage, sizeof(int), 1, bfp) != 1) break;
        if(fread(d.body_hits, sizeof(d.body_hits), 1, bfp) != 1) break;
        if(fread(&d.had_bomb, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.bomb_planted, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.bomb_exploded, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.bomb_defusions, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.bomb_defused, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.hostages_touched, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.hostages_rescued, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.was_vip, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.escaped_as_vip, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.killed_vip, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.suicides, sizeof(int), 1, bfp) != 1) break;
        if(fread(&d.playtime, sizeof(int), 1, bfp) != 1) break;
        if(fread(&j, sizeof(short int), 1, bfp) != 1) break;
  			score_stats_in_rank(name, find_name_in_rank( unique , name ), &d );
  		}
    }
    fclose(bfp);
  }
}

void save_rank(const char* filename)
{
  FILE *bfp;
  bfp = fopen(filename, "wb");
  if (!bfp) return;
  if ( (int)csstats_maxsize->value > 7000 ) {
    CVAR_SET_FLOAT("csstats_maxsize",7000.0);
  }
  int rank_num = rank_tail ? rank_tail->id : 0;
  if ( (int)csstats_maxsize->value < rank_num ) {
    rank_num = (int)csstats_maxsize->value;
  }
  if ( csstats_reset->value )  {
    CVAR_SET_FLOAT("csstats_reset",0.0);
    fclose(bfp); // don't save data!!!
    return;
  }
  int ver = RANK_VERSION_2006_3;
  fwrite(&ver, 1, sizeof(int), bfp);
  stats_t* p = rank_tail;
  stats_t* a;
  while ( p ) {
    a = p;
    p = p->prev;
    if (a->score == (1<<31)){
      delete_pos(a);
      --rank_num;
    }
  }
  if(rank_num) {
    int z = 0;
    for(p = rank_head; p ; p = p->next) {
      // Optimizations with ->namelen By AMXMODX Dev Team
      fwrite(&p->namelen, 1, sizeof(short int), bfp);
			fwrite(p->name, p->namelen, sizeof(char), bfp);
			fwrite(&p->uniquelen, 1, sizeof(short int), bfp);
			fwrite(p->unique, p->uniquelen, sizeof(char), bfp);
      fwrite(&p->kills, 1, sizeof(int), bfp);
      fwrite(&p->deaths, 1, sizeof(int), bfp);
      fwrite(&p->hs, 1, sizeof(int), bfp);
      fwrite(&p->tks, 1, sizeof(int), bfp);
      fwrite(&p->shots, 1, sizeof(int), bfp);
      fwrite(&p->hits, 1, sizeof(int), bfp);
      fwrite(&p->damage, 1, sizeof(int), bfp);
      fwrite(p->body_hits, 1, sizeof(int)*8, bfp);
      fwrite(&p->had_bomb, 1, sizeof(int), bfp);
      fwrite(&p->bomb_planted, 1, sizeof(int), bfp);
      fwrite(&p->bomb_exploded, 1, sizeof(int), bfp);
      fwrite(&p->bomb_defusions, 1, sizeof(int), bfp);
      fwrite(&p->bomb_defused, 1, sizeof(int), bfp);
      fwrite(&p->hostages_touched, 1, sizeof(int), bfp);
      fwrite(&p->hostages_rescued, 1, sizeof(int), bfp);
      fwrite(&p->was_vip, 1, sizeof(int), bfp);
      fwrite(&p->escaped_as_vip, 1, sizeof(int), bfp);
      fwrite(&p->killed_vip, 1, sizeof(int), bfp);
      fwrite(&p->suicides, 1, sizeof(int), bfp);
      fwrite(&p->playtime, 1, sizeof(int), bfp);
      if(++z == rank_num) break;
    }
  }
  short int dummy = 0;
	fwrite(&dummy, 1, sizeof(short int), bfp);
  fclose(bfp);
}

bool save_rank_text(const char* filename)
{
  FILE *fp;
  fp = fopen(filename, "wt");
  if(!fp) {
    MF_PrintSrvConsole("[AMX] Couldn't write file \"%s\"\n", filename);
    return false;
  }
  int rank_num = rank_tail ? rank_tail->id : 0;
  stats_t* p = rank_tail;
  stats_t* a;
  while ( p ) {
    a = p;
    p = p->prev;
    if (a->score == (1<<31)){
      delete_pos(a);
      --rank_num;
    }
  }
  if (rank_num) {
    char szTemp[SZTEMP_LENGTH+1];
    for (p = rank_head; p ; p = p->next) {
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%s\n", p->name);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%s\n", p->unique);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->kills);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->deaths);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->hs);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->tks);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->shots);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->hits);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->damage);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d,%d,%d,%d,%d,%d,%d,%d\n",p->body_hits[0],p->body_hits[1],p->body_hits[2],
        p->body_hits[3],p->body_hits[4],p->body_hits[5],p->body_hits[6],p->body_hits[7]);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->had_bomb);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->bomb_planted);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->bomb_exploded);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->bomb_defusions);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->bomb_defused);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->hostages_touched);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->hostages_rescued);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->was_vip);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->escaped_as_vip);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->killed_vip);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->suicides);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->playtime);
      fputs(szTemp, fp);
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "**************************************************\n");
      fputs(szTemp, fp);
    }
  }
  fclose(fp);
  return true;
}

bool save_rank_sv(const char* filename, int parserType)
{
  FILE *fp;
  fp = fopen(filename, "wt");
  if(!fp) {
    MF_PrintSrvConsole("[AMX] Couldn't write file \"%s\"\n", filename);
    return false;
  }
  int rank_num = rank_tail ? rank_tail->id : 0;
  stats_t* p = rank_tail;
  stats_t* a;
  while ( p ) {
    a = p;
    p = p->prev;
    if (a->score == (1<<31)){
      delete_pos(a);
      --rank_num;
    }
  }
  if (rank_num) {
    char szTemp[SZTEMP_LENGTH+1];
    switch(parserType) {
      case 1:
        fputs("\"Name\";\"Unique\";\"Kills\";\"Deaths\";\"HS\";\"TKs\";\"Shots\";\"Hits\";\"Damage\";\"Body Hits\";", fp);
        fputs("\"Had Bomb\";\"Bomb Planted\";\"Bomb Exploded\";\"Bomb Defusing\";\"Bomb Defused\";", fp);
        fputs("\"Hostages Touched\";\"Hostages Rescued\";\"Was VIP\";\"Escaped As VIP\";\"Killed VIP\";\"Suicides\";\"Playtime\"\n", fp);
        for (p = rank_head; p ; p = p->next) {
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%s\";", p->name);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%s\";", p->unique);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->kills);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->deaths);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->hs);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->tks);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->shots);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->hits);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->damage);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d,%d,%d,%d,%d,%d,%d,%d\";",p->body_hits[0],p->body_hits[1],p->body_hits[2],
            p->body_hits[3],p->body_hits[4],p->body_hits[5],p->body_hits[6],p->body_hits[7]);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->had_bomb);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->bomb_planted);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->bomb_exploded);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->bomb_defusions);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->bomb_defused);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->hostages_touched);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->hostages_rescued);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->was_vip);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->escaped_as_vip);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->killed_vip);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->suicides);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\"", p->playtime);
          fputs(szTemp, fp);
          fputc('\n', fp);
        }
        break;
      default:
        fputs("Name\tUnique\tKills\tDeaths\tHS\tTKs\tShots\tHits\tDamage\tBody Hits\t", fp);
        fputs("Had Bomb\tBomb Planted\tBomb Exploded\tBomb Defusing\tBomb Defused\t", fp);
        fputs("Hostages Touched\tHostages Rescued\tWas VIP\tEscaped As VIP\tKilled VIP\tSuicides\tPlaytime\n", fp);
        for (p = rank_head; p ; p = p->next) {
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%s\t", p->name);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%s\t", p->unique);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->kills);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->deaths);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->hs);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->tks);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->shots);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->hits);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->damage);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d,%d,%d,%d,%d,%d,%d,%d\t",p->body_hits[0],p->body_hits[1],p->body_hits[2],
            p->body_hits[3],p->body_hits[4],p->body_hits[5],p->body_hits[6],p->body_hits[7]);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->had_bomb);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->bomb_planted);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->bomb_exploded);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->bomb_defusions);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->bomb_defused);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->hostages_touched);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->hostages_rescued);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->was_vip);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->escaped_as_vip);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->killed_vip);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->suicides);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->playtime);
          fputs(szTemp, fp);
          fputc('\n', fp);
        }
        break;
    }
  }
  fclose(fp);
  return true;
}

bool rank_text_to_binary(char filename[]) {
  FILE *fp;
  fp = fopen(filename, "rt");
  if(!fp){
    MF_PrintSrvConsole("[AMX] Couldn't read file \"%s\"\n", filename);
    return false;
  }
  
  const char* csstatsDatFile = MF_BuildPathname("%s", MF_GetLocalInfo("csstats","$modulesdir/csstats.dat"));
  FILE *bfp;
  bfp = fopen(csstatsDatFile, "wb");
  if(!bfp) {
    MF_PrintSrvConsole("[AMX] Couldn't write file \"%s\"\n", csstatsDatFile);
    fclose(fp);
    return false;
  }

  int ver = RANK_VERSION_2006_3;
  fwrite(&ver, 1, sizeof(int), bfp);
  char buffor[SZTEMP_LENGTH+1];
  int value;
  int c, j;
  short int len = 0;
  char szTemp[32];
  while(!feof(fp)) {
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    len = strlen(buffor);
    if(len > 0) {
      if(buffor[len-1]=='\n')
        buffor[--len]='\0';
    }
    if(len > 0) {
      if(buffor[len-1]=='\r')
        buffor[--len]='\0';
    }
    fwrite(&len, 1, sizeof(short int), bfp);
    fwrite(&buffor, len, sizeof(char), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    len = strlen(buffor);
    if(len > 0) {
      if(buffor[len-1]=='\n')
        buffor[--len]='\0';
    }
    if(len > 0) {
      if(buffor[len-1]=='\r')
        buffor[--len]='\0';
    }
    fwrite(&len, 1, sizeof(short int), bfp);
    fwrite(&buffor, len, sizeof(char), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    len = strlen(buffor);
    if(len > 0) {
      if(buffor[len-1]=='\n')
        buffor[--len]='\0';
    }
    if(len > 0) {
      if(buffor[len-1]=='\r')
        buffor[--len]='\0';
    }
    j = 0;
    for(c = 0; c < 8; ++c){
      szTemp[0] = '\0';
      while(buffor[j] != ',' && j < len) {
        snprintf(szTemp, 31, "%s%c", szTemp, buffor[j++]);
      }
      value = atoi(szTemp);
      fwrite(&value, 1, sizeof(int), bfp);
      ++j;
    }
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
    if(feof(fp)) break;
    value = atoi(buffor);
    fwrite(&value, 1, sizeof(int), bfp);
    fgets(buffor,SZTEMP_LENGTH,fp);
  }
  len = 0;
	fwrite(&len, 1, sizeof(short int), bfp);
  fclose(bfp);
  fclose(fp);
  return true;
}

bool is_valid_text_file(const char* filename) {
  FILE *fp;
  fp = fopen(filename, "rt");
  if(!fp) {
    MF_PrintSrvConsole("[AMX] Couldn't read file \"%s\"\n", filename);
    return false;
  }
  int lines = 0;
  char line[256];
  while(!feof(fp)) {
    line[0] = '\0';
    fgets(line, 255, fp);
    if(line[0] && line[0] != '\r' && line[0] != '\n') {
      ++lines;
    }
  }
  if((lines % 23) != 0) {
    MF_PrintSrvConsole("[AMX] \"%s\" is not a valid csstats text file : the number of lines is incorrect\n", filename);
    fclose(fp);
    return false;
  }
  fclose(fp);
  return true;
}

void fire_kill(player_t* pVictim, player_t* pKiller, int weapon, int headshot, int teamkilling){
  int index = pKiller->index;
  pVictim->attackers[index].name = (char*)weapon_data[weapon].name;
  pVictim->attackers[index].kills++;
  pVictim->attackers[index].hs += headshot;
  pVictim->attackers[index].tks += teamkilling;
  pVictim->attackers[0].kills++;
  pVictim->attackers[0].hs += headshot;
  pVictim->attackers[0].tks += teamkilling;
  pVictim->weapons[pVictim->weapon->id].deaths++;
  pVictim->weapons[0].deaths++;
  pVictim->life.deaths++;

  pVictim->weapons_rnd[pVictim->weapon->id].deaths++; // DEC-Weapon (round) stats
  pVictim->weapons_rnd[0].deaths++;                   // DEC-Weapon (round) stats

  index = pVictim->index;
  pKiller->victims[index].name = (char*)weapon_data[weapon].name;
  pKiller->victims[index].deaths++;
  pKiller->victims[index].hs += headshot;
  pKiller->victims[index].tks += teamkilling;
  pKiller->victims[0].deaths++;
  pKiller->victims[0].hs += headshot;
  pKiller->victims[0].tks += teamkilling;
  
  pKiller->weapons_rnd[weapon].kills++;                // DEC-Weapon (round) stats
  pKiller->weapons_rnd[weapon].hs += headshot;         // DEC-Weapon (round) stats
  pKiller->weapons_rnd[weapon].tks += teamkilling;     // DEC-Weapon (round) stats
  pKiller->weapons_rnd[0].kills++;                     // DEC-Weapon (round) stats
  pKiller->weapons_rnd[0].hs += headshot;              // DEC-Weapon (round) stats
  pKiller->weapons_rnd[0].tks += teamkilling;          // DEC-Weapon (round) stats

  pKiller->weapons[weapon].kills++;
  pKiller->weapons[weapon].hs += headshot;
  pKiller->weapons[weapon].tks += teamkilling;
  pKiller->weapons[0].kills++;
  pKiller->weapons[0].hs += headshot;
  pKiller->weapons[0].tks += teamkilling;
  pKiller->life.kills++;
  pKiller->life.hs += headshot;
  pKiller->life.tks += teamkilling;
}

void fire_hit(player_t* pVictim, player_t* pAttacker, int weapon, int damage, int body){
  int index = pAttacker->index;
  pVictim->attackers[index].hits++;
  pVictim->attackers[index].damage += damage;
  pVictim->attackers[index].body_hits[body]++;
  pVictim->attackers[0].hits++;
  pVictim->attackers[0].damage += damage;
  pVictim->attackers[0].body_hits[body]++;

  index = pVictim->index;
  pAttacker->victims[index].hits++;
  pAttacker->victims[index].damage += damage;
  pAttacker->victims[index].body_hits[body]++;
  pAttacker->victims[0].hits++;
  pAttacker->victims[0].damage += damage;
  pAttacker->victims[0].body_hits[body]++;


  pAttacker->weapons_rnd[weapon].hits++;              // DEC-Weapon (round) stats
  pAttacker->weapons_rnd[weapon].damage += damage;    // DEC-Weapon (round) stats
  pAttacker->weapons_rnd[weapon].body_hits[body]++;   // DEC-Weapon (round) stats
  pAttacker->weapons_rnd[0].hits++;                   // DEC-Weapon (round) stats
  pAttacker->weapons_rnd[0].damage += damage;         // DEC-Weapon (round) stats
  pAttacker->weapons_rnd[0].body_hits[body]++;        // DEC-Weapon (round) stats

  pAttacker->weapons[weapon].hits++;
  pAttacker->weapons[weapon].damage += damage;
  pAttacker->weapons[weapon].body_hits[body]++;
  pAttacker->weapons[0].hits++;
  pAttacker->weapons[0].damage += damage;
  pAttacker->weapons[0].body_hits[body]++;
  pAttacker->life.hits++;
  pAttacker->life.damage += damage;
  pAttacker->life.body_hits[body]++;
}

void fire_shot(player_t* pPlayer, int weapon){
  pPlayer->victims[0].shots++;
  pPlayer->weapons[weapon].shots++;
  pPlayer->weapons[0].shots++;
  pPlayer->life.shots++;

  pPlayer->weapons_rnd[weapon].shots++;               // DEC-Weapon (round) stats
  pPlayer->weapons_rnd[0].shots++;                    // DEC-Weapon (round) stats
}

void fire_has_bomb(player_t* pPlayer){
  pPlayer->life.had_bomb++;
}

void fire_bomb_planted(player_t* pPlayer){
  pPlayer->life.bomb_planted++;
}

void fire_bomb_exploded(player_t* pPlayer){
  pPlayer->life.bomb_exploded++;
}

void fire_is_defusing(player_t* pPlayer){
  pPlayer->life.bomb_defusions++;
}

void fire_bomb_defused(player_t* pPlayer){
  pPlayer->life.bomb_defused++;
}

void fire_hostages_touched(player_t* pPlayer){
  pPlayer->life.hostages_touched++;
}

void fire_hostages_rescued(player_t* pPlayer){
  pPlayer->life.hostages_rescued++;
}

void fire_is_vip(player_t* pPlayer){
  pPlayer->life.was_vip++;
}

void fire_escaped_as_vip(player_t* pPlayer){
  pPlayer->life.escaped_as_vip++;
}

void fire_killed_vip(player_t* pPlayer){
  pPlayer->life.killed_vip++;
}

void fire_suicide(player_t* pPlayer){
  pPlayer->life.suicides++;
}


int get_stats_num() {
  return rank_tail ? rank_tail->id : 0;
}

NATIVE(get_user_astats) /* 6 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int attacker = params[2];
    if (attacker<0||attacker>gpGlobals->maxClients){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid player id %d", attacker);
      return 0;
    }
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->attackers[attacker].hits){
      int totalShots = 0;
      if(attacker == 0) {
        for(int i = 1; i <= gpGlobals->maxClients; ++i) {
          player_t* pAttacker = GET_PLAYER_POINTER_I(i);
          if(pAttacker->ingame && i != index) {
            totalShots += pAttacker->victims[0].shots;
          }
        }
      }
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      stats_t& stats = pPlayer->attackers[attacker];
      cpStats[0] = stats.kills;
      cpStats[1] = stats.deaths;
      cpStats[2] = stats.hs;
      cpStats[3] = stats.tks;
      cpStats[4] = (attacker > 0) ? GET_PLAYER_POINTER_I(attacker)->victims[0].shots : totalShots;
      cpStats[5] = stats.hits;
      cpStats[6] = stats.damage;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats.body_hits[i];
      if (params[6] && attacker && stats.name)
        MF_SetAmxString(amx,params[5],stats.name,params[6]);
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_vstats) /* 6 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int victim = params[2];
    if (victim<0||victim>gpGlobals->maxClients){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid player id %d", victim);
      return 0;
    }
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->victims[victim].hits){
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      stats_t& stats = pPlayer->victims[victim];
      cpStats[0] = stats.kills;
      cpStats[1] = stats.deaths;
      cpStats[2] = stats.hs;
      cpStats[3] = stats.tks;
      cpStats[4] = pPlayer->victims[0].shots;
      cpStats[5] = stats.hits;
      cpStats[6] = stats.damage;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats.body_hits[i];
      if (params[6] && victim && stats.name)
        MF_SetAmxString(amx,params[5],stats.name,params[6]);
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_wrstats) /* 4 param */ // DEC-Weapon (round) stats (end)
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int weapon = params[2];
    if (weapon<0||weapon>=MAX_WEAPONS){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", weapon);
      return 0;
    }
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->weapons_rnd[weapon].shots){
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      stats_t& stats = pPlayer->weapons_rnd[weapon];
      cpStats[0] = stats.kills;
      cpStats[1] = stats.deaths;
      cpStats[2] = stats.hs;
      cpStats[3] = stats.tks;
      cpStats[4] = stats.shots;
      cpStats[5] = stats.hits;
      cpStats[6] = stats.damage;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats.body_hits[i];
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_wstats) /* 4 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    int weapon = params[2];
    if (weapon<0||weapon>=MAX_WEAPONS){
      MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", weapon);
      return 0;
    }
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if (pPlayer->weapons[weapon].shots){
      cell *cpStats = MF_GetAmxAddr(amx,params[3]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[4]);
      stats_t& stats = pPlayer->weapons[weapon];
      cpStats[0] = stats.kills;
      cpStats[1] = stats.deaths;
      cpStats[2] = stats.hs;
      cpStats[3] = stats.tks;
      cpStats[4] = stats.shots;
      cpStats[5] = stats.hits;
      cpStats[6] = stats.damage;
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = stats.body_hits[i];
      return 1;
    }
  }
  return 0;
}

NATIVE(reset_user_wstats) /* 6 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 0)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    memset(pPlayer->attackers,0,sizeof(stats_t)*33);
    memset(pPlayer->victims ,0,sizeof(stats_t)*33);
    memset(pPlayer->weapons,0,sizeof(stats_t)*MAX_WEAPONS);
    memset(pPlayer->weapons_rnd,0,sizeof(stats_t)*MAX_WEAPONS);  // DEC-Weapon (round) stats
    memset(&pPlayer->life,0,sizeof(stats_t));
    return 1;
  }
  return 0;
}

NATIVE(get_user_stats) /* 3 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!pPlayer->rank) {
      return 0;
    }
    cell *cpStats = MF_GetAmxAddr(amx,params[2]);
    cell *cpBodyHits = MF_GetAmxAddr(amx,params[3]);
    cpStats[0] = pPlayer->rank->kills;
    cpStats[1] = pPlayer->rank->deaths;
    cpStats[2] = pPlayer->rank->hs;
    cpStats[3] = pPlayer->rank->tks;
    cpStats[4] = pPlayer->rank->shots;
    cpStats[5] = pPlayer->rank->hits;
    cpStats[6] = pPlayer->rank->damage;
    cpStats[7] = pPlayer->rank->id;
    for(int i = 1; i < 8; ++i)
      cpBodyHits[i] = pPlayer->rank->body_hits[i];
    return pPlayer->rank->id;
  }
  return 0;
}

NATIVE(get_user_stats2) /* 2 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!pPlayer->rank) {
      return 0;
    }
    cell *cpStats = MF_GetAmxAddr(amx,params[2]);
    cpStats[0] = pPlayer->rank->had_bomb;
    cpStats[1] = pPlayer->rank->bomb_planted;
    cpStats[2] = pPlayer->rank->bomb_exploded;
    cpStats[3] = pPlayer->rank->bomb_defusions;
    cpStats[4] = pPlayer->rank->bomb_defused;
    cpStats[5] = pPlayer->rank->hostages_touched;
    cpStats[6] = pPlayer->rank->hostages_rescued;
    cpStats[7] = pPlayer->rank->was_vip;
    cpStats[8] = pPlayer->rank->escaped_as_vip;
    cpStats[9] = pPlayer->rank->killed_vip;
    cpStats[10] = pPlayer->rank->suicides;
    cpStats[11] = pPlayer->rank->playtime;
    return pPlayer->rank->id;
  }
  return 0;
}

NATIVE(get_user_rstats) /* 3 param */
{
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    cell *cpStats = MF_GetAmxAddr(amx,params[2]);
    cell *cpBodyHits = MF_GetAmxAddr(amx,params[3]);
    cpStats[0] = pPlayer->life.kills;
    cpStats[1] = pPlayer->life.deaths;
    cpStats[2] = pPlayer->life.hs;
    cpStats[3] = pPlayer->life.tks;
    cpStats[4] = pPlayer->life.shots;
    cpStats[5] = pPlayer->life.hits;
    cpStats[6] = pPlayer->life.damage;
    for(int i = 1; i < 8; ++i)
      cpBodyHits[i] = pPlayer->life.body_hits[i];
    return 1;
  }
  return 0;
}

NATIVE(get_stats) /* 3 param */
{
  int index = params[1] + 1;
  for(stats_t* rank = rank_head; rank ; rank = rank->next){
    if (rank->id == index)  {
      cell *cpStats = MF_GetAmxAddr(amx,params[2]);
      cell *cpBodyHits = MF_GetAmxAddr(amx,params[3]);
      cpStats[0] = rank->kills;
      cpStats[1] = rank->deaths;
      cpStats[2] = rank->hs;
      cpStats[3] = rank->tks;
      cpStats[4] = rank->shots;
      cpStats[5] = rank->hits;
      cpStats[6] = rank->damage;
      cpStats[7] = rank->id;
      MF_SetAmxString(amx,params[4],rank->name,params[5]);
      for(int i = 1; i < 8; ++i)
        cpBodyHits[i] = rank->body_hits[i];
      return rank->next ? rank->id : 0;
    }
  }
  return 0;
}

NATIVE(get_stats2) /* 2 param */
{
  int index = params[1] + 1;
  for(stats_t* rank = rank_head; rank ; rank = rank->next){
    if (rank->id == index)  {
      cell *cpStats = MF_GetAmxAddr(amx,params[2]);
      cpStats[0] = rank->had_bomb;
      cpStats[1] = rank->bomb_planted;
      cpStats[2] = rank->bomb_exploded;
      cpStats[3] = rank->bomb_defusions;
      cpStats[4] = rank->bomb_defused;
      cpStats[5] = rank->hostages_touched;
      cpStats[6] = rank->hostages_rescued;
      cpStats[7] = rank->was_vip;
      cpStats[8] = rank->escaped_as_vip;
      cpStats[9] = rank->killed_vip;
      cpStats[10] = rank->suicides;
      cpStats[11] = rank->playtime;
      MF_SetAmxString(amx,params[3],rank->name,params[4]);
      return rank->next ? rank->id : 0;
    }
  }
  return 0;
}

NATIVE(get_statsnum)
{
  return rank_tail ? rank_tail->id : 0;
}

AMX_NATIVE_INFO csstats_natives[] = {
  { "get_stats",         get_stats },
  { "get_stats2",        get_stats2 },
  { "get_statsnum",      get_statsnum },
  { "get_user_astats",   get_user_astats },
  { "get_user_rstats",   get_user_rstats },
  { "get_user_lstats",   get_user_rstats },
  { "get_user_stats",    get_user_stats },
  { "get_user_stats2",   get_user_stats2 },
  { "get_user_vstats",   get_user_vstats },
  { "get_user_wrstats",  get_user_wrstats }, // DEC-Weapon(Round) Stats
  { "get_user_wstats",   get_user_wstats },
  { "reset_user_wstats", reset_user_wstats },
  { NULL,                NULL }
};

