/*
 * Copyright (c) 2003-2011 Lukasz Wlasinski
 *
 *    Some code was taken from AMX Mod X, credits for this code go to them.
 *    The authors of AMX Mod X are:
 *    David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
 *	  Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
 *		Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
 *	  Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.
 *
 *    This file is part of DoD XMod.
 *
 *    DoD XMod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    DoD XMod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with DoD XMod; if not, write to the Free Software Foundation,
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


#include <extdll.h>
#include <meta_api.h>
#include "CRank.h"
#include "dodxmod.h"

#define RANK_VERSION_2006_2 5
#define RANK_VERSION_2006_3 6


#define SZTEMP_LENGTH 127

// *****************************************************
// class Stats
// *****************************************************
Stats::Stats(){
	hits = shots = damage = hs = tks = points = kills = deaths = 0;
	memset( bodyHits , 0 , sizeof( bodyHits ) );
}
void Stats::commit(Stats* a){
	hits += a->hits;
	shots += a->shots;
	damage += a->damage;
	hs += a->hs;
	tks += a->tks;
	points += a->points;
	kills += a->kills;
	deaths += a->deaths;
	for(int i = 1; i < 8; ++i)
		bodyHits[i] += a->bodyHits[i];
}

// *****************************************************
// class RankSystem
// *****************************************************
RankSystem::RankStats::RankStats( const char* uu, const char* nn, RankSystem* pp ) {
	name = 0;
	namelen = 0;
	unique = 0;
	uniquelen = 0;
	score = 0;
	parent = pp;
	id = ++parent->rankNum;
	next = prev = 0;
	setName( nn );
	setUnique( uu );
}
RankSystem::RankStats::~RankStats() {
	delete[] name;
	delete[] unique;
	--parent->rankNum;
}
void RankSystem::RankStats::setName( const char* nn  )	{
	delete[] name;
	namelen = strlen(nn) + 1;
	name = new char[namelen];
	if ( name )
		strcpy( name , nn );
	else
		namelen = 0;
}
void RankSystem::RankStats::setUnique( const char* nn  )	{
	delete[] unique;
	uniquelen = strlen(nn) + 1;
	unique = new char[uniquelen];
	if ( unique )
		strcpy( unique , nn );	
	else
		uniquelen = 0;
}

RankSystem::RankSystem() { 
	head = 0; 
	tail = 0; 
	rankNum = 0;
	calc.code = 0;
}

RankSystem::~RankSystem() {
	clear();
}

void RankSystem::put_before( RankStats* a, RankStats* ptr ){
	a->next = ptr;
	if ( ptr ){
		a->prev = ptr->prev;
		ptr->prev = a;
	}
	else{
		a->prev = head;
		head = a;
	}
	if ( a->prev )	a->prev->next = a;
	else tail = a;
}
void  RankSystem::put_after( RankStats* a, RankStats* ptr ) {
	a->prev = ptr;
	if ( ptr ){
		a->next = ptr->next;
		ptr->next = a;
	}
	else{
		a->next = tail;
		tail = a;
	}
	if ( a->next )	a->next->prev = a;
	else head = a;
}

void RankSystem::unlink( RankStats* ptr ){
	if (ptr->prev) ptr->prev->next = ptr->next;
	else tail = ptr->next;
	if (ptr->next) ptr->next->prev = ptr->prev;
	else head = ptr->prev;
}

void RankSystem::clear(){
	while( tail ){
		head = tail->next;
		delete tail;
		tail = head;
	}
}

bool RankSystem::loadCalc(const char* filename, char* error)
{
	if ((MF_LoadAmxScript(&calc.amx,&calc.code,filename,error,0)!=AMX_ERR_NONE)||
		(AMX_ALLOT(&calc.amx, 8 , &calc.amxAddr1, &calc.physAddr1)!=AMX_ERR_NONE)||
		(AMX_ALLOT(&calc.amx, 8 , &calc.amxAddr2, &calc.physAddr2)!=AMX_ERR_NONE)||
		(AMX_FINDPUBLIC(&calc.amx,"get_score",&calc.func)!=AMX_ERR_NONE)){
		LOG_CONSOLE(PLID, "Couldn't load plugin (file \"%s\")",filename);
    MF_UnloadAmxScript(&calc.amx,&calc.code);
		calc.code = NULL;
		return false;
	}
	return true;
}

void RankSystem::unloadCalc() {
	if(calc.code) {
		MF_UnloadAmxScript(&calc.amx , &calc.code);
	}
}

RankSystem::RankStats* RankSystem::findEntryInRank(const char* unique, const char* name )
{
	RankStats* a = head;
	
	while ( a )
	{
		if (  strcmp( a->getUnique() ,unique ) == 0 )
			return a;

		a = a->prev;
	}
	a = new RankStats( unique ,name,this );
	if ( a == 0 ) return 0;
	put_after( a  , 0 );
	return a;
}

void RankSystem::updatePos(RankStats* rr ,  Stats* s)
{
	rr->addStats( s );

	if ( calc.code ) {
		calc.physAddr1[0] = rr->kills;
		calc.physAddr1[1] = rr->deaths;
		calc.physAddr1[2] = rr->hs;
		calc.physAddr1[3] = rr->tks;
		calc.physAddr1[4] = rr->shots;
		calc.physAddr1[5] = rr->hits;
		calc.physAddr1[6] = rr->damage;
		calc.physAddr1[7] = rr->points;
		for(int i = 1; i < 8; ++i)
			calc.physAddr2[i] = rr->bodyHits[i];
		cell result = 0;
		int err;
		if((err = AMX_EXEC(&calc.amx,&result,calc.func,2,calc.amxAddr1,calc.amxAddr2)) != AMX_ERR_NONE) {
			LOG_CONSOLE( PLID, "Run time error %d on line %ld (plugin \"%s\")",err,calc.amx.curline,LOCALINFO("dodstats_score"));
			rr->score = rr->kills - rr->deaths - 2*rr->tks;
    }
    else {
		  rr->score = result;
    }
	}
	else rr->score = rr->kills - rr->deaths - 2*rr->tks;

	RankStats* aa = rr->next;
	while ( aa && (aa->score <= rr->score) ) { // try to nominate
		rr->goUp();
		aa->goDown();
		aa = aa->next;		// go to next rank
	}
	if ( aa != rr->next )
	{
		unlink( rr );
		put_before( rr, aa );
	}
	else
	{
		aa = rr->prev;
		while ( aa && (aa->score > rr->score) ) { // go down
			rr->goDown();
			aa->goUp();
			aa = aa->prev;	// go to prev rank
		}
		if ( aa != rr->prev ){
			unlink( rr );
			put_after( rr, aa );
		}
	}

}

void RankSystem::loadRank(const char* filename)
{
  FILE *bfp = fopen(filename, "rb");
  if(!bfp) return;
  int ver = 0;
  if(fread(&ver, sizeof(int), 1, bfp) != 1) {
    fclose(bfp);
    return;
  }
  if(ver == RANK_VERSION_2006_2) {
    g_rank.clear();
    int rank_num = 0;
    if(fread(&rank_num, sizeof(int), 1, bfp) != 1) {
      fclose(bfp);
      return;
    }
    Stats d;
    char name[33], unique[33];
    for(int i = 0; i < rank_num && !feof(bfp); ++i) {
      // Parser fixes By AMXMODX Dev Team
      if(fread(name , sizeof(char) * 32, 1, bfp) != 1) break;
      if(fread(unique , sizeof(char) * 32, 1, bfp) != 1) break;
      if(fread(&d.kills, sizeof(int), 1, bfp) != 1) break;
      if(fread(&d.deaths, sizeof(int), 1, bfp) != 1) break;
      if(fread(&d.hs, sizeof(int), 1, bfp) != 1) break;
      if(fread(&d.tks, sizeof(int), 1, bfp) != 1) break;
      if(fread(&d.shots, sizeof(int), 1, bfp) != 1) break;
      if(fread(&d.hits, sizeof(int), 1, bfp) != 1) break;
      if(fread(&d.damage, sizeof(int), 1, bfp) != 1) break;
      if(fread(&d.points, sizeof(int), 1, bfp) != 1) break;
      if(fread(d.bodyHits, sizeof(d.bodyHits), 1, bfp) != 1) break;
      RankSystem::RankStats* a = findEntryInRank( unique , name );
			if ( a ) a->updatePosition( &d );
    }
  }
  else if (ver == RANK_VERSION_2006_3) {
    g_rank.clear();
    Stats d;
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
      if(fread(&d.points, sizeof(int), 1, bfp) != 1) break;
      if(fread(d.bodyHits, sizeof(d.bodyHits), 1, bfp) != 1) break;
      if(fread(&j, sizeof(short int), 1, bfp) != 1) break;
      RankSystem::RankStats* a = findEntryInRank( unique , name );
			if ( a ) a->updatePosition( &d );
    }
  }
  fclose(bfp);
}

void RankSystem::saveRank(const char* filename)
{
  FILE *bfp;
  bfp = fopen(filename, "wb");
  if (!bfp) return;

  if ( (int)dodstats_maxsize->value > 7000 ) {
    CVAR_SET_FLOAT("dodstats_maxsize",7000.0);
  }
  
  int rank_num = g_rank.getRankNum();
  if ( (int)dodstats_maxsize->value < rank_num ) {
    rank_num = (int)dodstats_maxsize->value;
  }

  if ( dodstats_reset->value )  {
    CVAR_SET_FLOAT("dodstats_reset",0.0);
    fclose(bfp); // don't save data!!!
    return;
  }

  int ver = RANK_VERSION_2006_3;
  fwrite(&ver, 1, sizeof(int) , bfp);
  RankStats* p = head;
  RankStats* a;
  while ( p ) {
    a = p;
    p = p->prev;
    if (a->score == (1<<31)){
      unlink(a);
      --rank_num;
    }
  }
  if (rank_num) {
    int z = 0;
    for (p = head; p ; p = p->prev) {
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
      fwrite(&p->points, 1, sizeof(int), bfp);
      fwrite(p->bodyHits, 1, sizeof(int)*8, bfp);
      if(++z == rank_num) break;
    }
  }
  short int dummy = 0;
	fwrite(&dummy, 1, sizeof(short int), bfp);
  fclose(bfp);
}

bool RankSystem::save_rank_text(const char* filename)
{
  FILE *fp;
  fp = fopen(filename, "wt");
  if (!fp) {
    MF_PrintSrvConsole("[AMX] Couldn't write file \"%s\"\n", filename);
    return false;
  }
  int rank_num = g_rank.getRankNum();
  RankStats* p = head;
  RankStats* a;
  while ( p ) {
    a = p;
    p = p->prev;
    if (a->score == (1<<31)){
      unlink(a);
      --rank_num;
    }
  }
  if (rank_num) {
    char szTemp[SZTEMP_LENGTH+1];
    for (p = head; p ; p = p->prev) {
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%s\n", p->name);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%s\n", p->unique);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->kills);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->deaths);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->hs);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->tks);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->shots);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->hits);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->damage);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d\n", p->points);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "%d,%d,%d,%d,%d,%d,%d,%d\n",p->bodyHits[0],p->bodyHits[1],p->bodyHits[2],
        p->bodyHits[3],p->bodyHits[4],p->bodyHits[5],p->bodyHits[6],p->bodyHits[7]);
      fputs( szTemp , fp );
      szTemp[0] = '\0';
      snprintf(szTemp, SZTEMP_LENGTH, "**************************************************\n");
      fputs( szTemp , fp );
    }
  }
  fclose(fp);
  return true;
}

bool RankSystem::save_rank_sv(const char* filename, int parserType)
{
  FILE *fp;
  fp = fopen(filename, "wt");
  if (!fp) {
    MF_PrintSrvConsole("[AMX] Couldn't write file \"%s\"\n", filename);
    return false;
  }
  int rank_num = g_rank.getRankNum();
  RankStats* p = head;
  RankStats* a;
  while ( p ) {
    a = p;
    p = p->prev;
    if (a->score == (1<<31)){
      unlink(a);
      --rank_num;
    }
  }
  if (rank_num) {
    char szTemp[SZTEMP_LENGTH+1];
    switch(parserType) {
      case 1:
        fputs("\"Name\";\"Unique\";\"Kills\";\"Deaths\";\"HS\";\"TKs\";\"Shots\";\"Hits\";\"Damage\";\"Points\";\"Body Hits\"\n", fp);
        for (p = head; p ; p = p->prev) {
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
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d\";", p->points);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "\"%d,%d,%d,%d,%d,%d,%d,%d\"",p->bodyHits[0],p->bodyHits[1],p->bodyHits[2],
            p->bodyHits[3],p->bodyHits[4],p->bodyHits[5],p->bodyHits[6],p->bodyHits[7]);
          fputs(szTemp, fp);
          fputc('\n', fp);
        }
        break;
      default:
        fputs("Name\tUnique\tKills\tDeaths\tHS\tTKs\tShots\tHits\tDamage\tPoints\tBody Hits\n", fp);
        for (p = head; p ; p = p->prev) {
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
          snprintf(szTemp, SZTEMP_LENGTH, "%d\t", p->points);
          fputs(szTemp, fp);
          szTemp[0] = '\0';
          snprintf(szTemp, SZTEMP_LENGTH, "%d,%d,%d,%d,%d,%d,%d,%d",p->bodyHits[0],p->bodyHits[1],p->bodyHits[2],
            p->bodyHits[3],p->bodyHits[4],p->bodyHits[5],p->bodyHits[6],p->bodyHits[7]);
          fputs(szTemp, fp);
          fputc('\n', fp);
        }
        break;
    }
  }
  fclose(fp);
  return true;
}

bool RankSystem::rank_text_to_binary(char filename[]) {
  FILE *fp;
  fp = fopen(filename, "rt");
  if(!fp){
    MF_PrintSrvConsole("[AMX] Couldn't read file \"%s\"\n", filename);
    return false;
  }

  const char* dodstatsDatFile = MF_BuildPathname("%s", MF_GetLocalInfo("dodstats","$modulesdir/dodstats.dat"));
  FILE *bfp;
  bfp = fopen(dodstatsDatFile, "wb");
  if (!bfp) {
    MF_PrintSrvConsole("[AMX] Couldn't write file \"%s\"\n", dodstatsDatFile);
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
  }
  len = 0;
	fwrite(&len, 1, sizeof(short int), bfp);
  fclose(bfp);
  fclose(fp);
  return true;
}

bool RankSystem::is_valid_text_file(const char* filename) {
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
  if((lines % 12) != 0) {
    MF_PrintSrvConsole("[AMX] \"%s\" is not a valid dodstats text file : the number of lines is incorrect\n", filename);
    fclose(fp);
    return false;
  }
  fclose(fp);
  return true;
}

void RankSystem::ClearCode() {
	calc.code = NULL;
}
