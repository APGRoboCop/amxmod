/*
 * Copyright (c) 2002-2011 - Aleksander Naszko, Julien Laurent
 *
 *
 *    By AMX Mod Team.
 *
 *    Some code was taken from AMX Mod X, credits for this code go to them.
 *    The authors of AMX Mod X are:
 *    David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
 *    Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
 *    Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
 *    Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.
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

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "amx_module.h"
#include "mysql/my_global.h"
#include "mysql/mysql.h"
#include "mysql/errmsg.h"

#define MEM_ALLOC_FAILED -20
#define CONNECT_FAILED   -10
#define QUERY_FAILED     -5

typedef struct camx_s {
  AMX *amx;
  struct camx_s *next;
} camx_t;

typedef struct s_my_sql {
  MYSQL mysql;
  MYSQL_RES *result;
  MYSQL_ROW row;
  int res;
  int fields;
  int rows;
} mysql_t;

typedef struct sql_list_s {
  mysql_t* address;
  char* host;
  char* user;
  char* pass;
  char* dbase;
  camx_t *owner;
  struct sql_list_s* next;
} sql_list;

sql_list* list_head;

void add_owner(camx_t** head, AMX *amx) {
  if(!(*head)) {
    camx_t* a = (camx_t*)malloc(sizeof(camx_t));
    if(!a) return;

    a->amx = amx;
    a->next = NULL;
    *head = a;
  }
  else if((*head)->amx != amx) add_owner(&(*head)->next, amx);
}

void delete_owner(camx_t** head, AMX* amx) {
  if(!(*head)) return;

  if((*head)->amx == amx) {
    camx_t* a = *head;
    (*head) = (*head)->next;
    free(a);
  }
  else delete_owner(&(*head)->next, amx);
}

void delete_all_owers(camx_t** head) {
  if(!(*head)) return;

  delete_all_owers(&(*head)->next);
  free(*head);
  *head = NULL;
}

sql_list* sql_exists(sql_list* a, mysql_t* address) {
  if(!address) return NULL;

  while(a){
    if(a->address == address) return a;

    a = a->next;
  }
  return NULL;
}

sql_list* sql_exists(sql_list* a, const char* host, const char* user, const char* pass, const char* dbase) {
  while(a) {
    if(!strcmp(a->host, host) && !strcmp(a->user, user) && !strcmp(a->pass, pass) && !strcmp(a->dbase, dbase)) return a;

    a = a->next;
  }
  return NULL;
}

void delete_sql(sql_list** head, mysql_t* address, AMX* amx) {
  if(!*head) return;

  if((*head)->address == address) {
    sql_list* a = *head;
    delete_owner(&a->owner, amx);
    /* still there are some plugins which use this connection */
    if(a->owner) return;

    *head = (*head)->next;
    mysql_t* mysql = a->address;
    if(mysql) {
      if(mysql->result) mysql_free_result(mysql->result);
      mysql_close(&(mysql->mysql));
      free(mysql);
    }
    free(a->host);
    free(a->user);
    free(a->pass);
    free(a->dbase);
    free(a);
  }
  else delete_sql(&(*head)->next, address, amx);
}

void delete_all_sqls(sql_list** head) {
  sql_list* a = *head;
  while(a) {
    *head = a->next;
    mysql_t* mysql = a->address;
    if(mysql) {
      if(mysql->result) mysql_free_result(mysql->result);
      mysql_close(&(mysql->mysql));
      free(mysql);
    }
    delete_all_owers(&a->owner);
    free(a->host);
    free(a->user);
    free(a->pass);
    free(a->dbase);
    free(a);
    a = *head;
  }
}

sql_list* create_sql(sql_list** head, const char* host, const char* user, const char* pass, const char* dbase) {
  sql_list* a = (sql_list*)malloc(sizeof(sql_list));
  if(!a) return NULL;

  mysql_t* b = (mysql_t*)malloc(sizeof(mysql_t));
  a->address = b;
  if(!b) return NULL;

  memset(b, 0, sizeof(mysql_t));
  a->host = (char*)malloc(sizeof(char)*(strlen(host)+1));
  if(!a->host) return NULL;

  strcpy(a->host, host);
  a->user = (char*)malloc(sizeof(char)*(strlen(user)+1));
  if(!a->user) return NULL;

  strcpy(a->user, user);
  a->pass = (char*)malloc(sizeof(char)*(strlen(pass)+1));
  if(!a->pass) return NULL;

  strcpy(a->pass, pass);
  a->dbase = (char*)malloc(sizeof(char)*(strlen(dbase)+1));
  if(!a->dbase) return NULL;

  strcpy(a->dbase,dbase);
  a->next = *head;
  a->owner = NULL;
  *head = a;
  return a;
}

// connection
int sqlconnect(MYSQL *mysql, const char* sql_host, const char* sql_user, const char* sql_pass, const char* sql_db, char** sqlerror) {
  // DEC
  char *pos;
  unsigned int sql_port = 0;

  char sql_ip[32];
  strncpy(sql_ip, sql_host, 30);
  sql_ip[30] = 0;

  if(!*sql_host || !*sql_user || !*sql_db) return 0;

  if((pos = strchr(sql_ip, ':')) != NULL){
    *pos = 0;
    sql_port = (unsigned int) atoi(pos+1);
  }

  mysql_init(mysql);
  mysql_real_connect(mysql, sql_ip, sql_user, sql_pass, NULL, sql_port, NULL, 0);

  if(mysql_errno(mysql)){
    *sqlerror = (char *)mysql_error(mysql);
    mysql_close(mysql);
    return 0;
  }

  mysql_select_db(mysql, sql_db);

  if(mysql_errno(mysql)) {
    *sqlerror = (char *)mysql_error(mysql);
    mysql_close (mysql);
    return 0;
  }

  return 1;
}

// By AMXMODX Dev Team
const char *NameToColumn(mysql_t *mysql, const char *field) {
  size_t col = 0;
  int found = 0;
  size_t m_Fields = mysql_num_fields(mysql->result);
  MYSQL_FIELD *mysqlfield;
  const char *name;
  for(size_t i=0; i<m_Fields; ++i) {
    mysqlfield = mysql_fetch_field_direct(mysql->result, i);
    if(!mysqlfield || !mysqlfield->name)
      name = "";
    else
      name = mysqlfield->name;
    if(strcmp(name, field) == 0) {
      col = i;
      found = 1;
      break;
    }
  }

  if(found == 0 || col >= m_Fields)
    return NULL;

  if(mysql->row[col] == NULL)
    return "";
  return mysql->row[col];
}

// By AMXMODX Dev Team
const char *NameOfColumn(mysql_t *mysql, int column) {
  size_t m_Fields = mysql_num_fields(mysql->result);
  if(column < 0 || column >= (int)m_Fields)
    return "";

  MYSQL_FIELD *mysqlfield;
  mysqlfield = mysql_fetch_field_direct(mysql->result, column);
  if(!mysqlfield || !mysqlfield->name)
    return "";
  return mysqlfield->name;
}

// mysql_error(sql,dest[],maxlength)
// - store maxlength characters from mysql error in current row to dest (3 params)
NATIVE(sql_error) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;
  const char *error = mysql_error(&(mysql->mysql));
  return MF_SetAmxString(amx, params[2], error ? error : "", params[3]);
}

// sql = mysql_connect(host[],user[],pass[],dbname[],error[],maxlength) (6 param):
// - open connection
NATIVE(sql_connect) {
  int i;
  const char *host = MF_GetAmxString(amx, params[1], 0, i);
  const char *user = MF_GetAmxString(amx, params[2], 1, i);
  const char *pass = MF_GetAmxString(amx, params[3], 2, i);
  const char *dbname = MF_GetAmxString(amx, params[4], 3, i);

  sql_list* c = sql_exists(list_head, host, user, pass, dbname);

  if(c) {
    add_owner(&c->owner,amx);
    return (cell)c->address;
  }

  c = create_sql(&list_head, host, user, pass, dbname);

  if(!c || !c->address) return MEM_ALLOC_FAILED;

  mysql_t *mysql = c->address;

  char *sqlerror=NULL;

  if(!sqlconnect(&(mysql->mysql), host, user, pass, dbname, &sqlerror)) {
    MF_SetAmxString(amx, params[5], sqlerror ? sqlerror : "unknown error", params[6]);
    return CONNECT_FAILED;
  }

  MF_SetAmxString(amx, params[5], "", params[6]);
  mysql->result = NULL;
  add_owner(&c->owner, amx);

  return (cell)mysql;
}

// mysql_query(sql,query[],{Float,_}:...) (2 params or more):
// - close previous result
// - perform new query
// (can be used not to connect twice in a row)
NATIVE(sql_query) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;

  if(mysql->result) mysql_free_result(mysql->result);
  mysql->result = NULL;

  int len;
  const char* query = MF_FormatAmxString(amx, params, 2, len);

  mysql->res = mysql_query(&(mysql->mysql), query);

  if(mysql->res) return QUERY_FAILED;

  mysql->result = mysql_use_result(&(mysql->mysql));
  mysql->rows = 0;

  return (cell)mysql;
}

// mysql_nextrow(sql) (1 param):
// - read next row
// - return :
//   . number of line
//   . 0 at end
NATIVE(sql_nextrow) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;

  mysql->rows++;
  mysql->row = mysql_fetch_row(mysql->result);

  if(!mysql->row) mysql->rows = 0;

  return mysql->rows;
}

// mysql_close(sql) (1 param):
// - free result
// - close connection
NATIVE(sql_close) {
  delete_sql(&list_head,(mysql_t *)params[1], amx);
  return 1;
}

// mysql_getfield(sql, n, {Float,_}:...)
NATIVE(sql_getfield) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;
  size_t m_Fields = mysql_num_fields(mysql->result);
  int column = (int)params[2] - 1;
  if(column < 0 || column >= (int)m_Fields) {
    return 0;
  }
  const char* data = mysql->row[column];
  if(data == NULL) {
    return 0;
  }
  int type = (*params)/sizeof(cell);
  cell *dest_addr = NULL;
  switch(type) {
    case 2: {
      return atoi(data);
      break;
    }
    case 3: {
      dest_addr = MF_GetAmxAddr(amx, params[3]);
      *dest_addr = (cell)((float)atof(data));
      return 1;
      break;
    }
    case 4: {
      return MF_SetAmxString(amx, params[3], data, params[4]);
      break;
    }
  }

  return 0;
}

// By AMXMODX Dev Team
// mysql_getresult(sql, field[], {Float,_}:...)
NATIVE(sql_getresult) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;
  int len;
  const char *column = MF_GetAmxString(amx, params[2], 0, len);
  const char *data = NameToColumn(mysql, column);
  if(data == NULL) {
    return 0;
  }
  int type = (*params)/sizeof(cell);
  cell *dest_addr = NULL;
  switch(type) {
    case 2: {
      return atoi(data);
      break;
    }
    case 3: {
      dest_addr = MF_GetAmxAddr(amx, params[3]);
      *dest_addr = (cell)((float)atof(data));
      return 1;
      break;
    }
    case 4: {
      return MF_SetAmxString(amx, params[3], data, params[4]);
      break;
    }
  }

  return 0;
}

// mysql_insert_id(sql)
NATIVE(sql_insert_id) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;
  size_t id = static_cast<size_t>(mysql_insert_id(&(mysql->mysql)));
  return id;
}

// mysql_affected_rows(sql)
NATIVE(sql_affected_rows) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;
  size_t num = static_cast<size_t>(mysql_affected_rows(&(mysql->mysql)));
  return num;
}

// mysql_num_fields(sql)
NATIVE(sql_num_fields) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;
  size_t num = mysql_num_fields(mysql->result);
  return num;
}

// mysql_num_rows(sql)
NATIVE(sql_num_rows) {
	mysql_t *mysql = (mysql_t *)params[1];
	if(!sql_exists(list_head, mysql)) return 0;
	size_t num = static_cast<size_t>(mysql_num_rows(mysql->result));
	return num;
}

// By AMXMODX Dev Team
// mysql_field_name(sql, field, name[], maxLength)
NATIVE(sql_field_name) {
  mysql_t *mysql = (mysql_t *)params[1];
  if(!sql_exists(list_head, mysql)) return 0;
  const char *name = NameOfColumn(mysql, (int)params[2]-1);
  if(!name)
    return 0;
  MF_SetAmxString(amx, params[3], name, params[4]);
  return 1;
}

AMX_NATIVE_INFO mysql_natives[] = {
  { "mysql_connect",        sql_connect },
  { "mysql_query",          sql_query },
  { "mysql_nextrow",        sql_nextrow },
  { "mysql_getfield",       sql_getfield },
  { "mysql_getresult",      sql_getresult },
  { "mysql_insert_id",      sql_insert_id },
  { "mysql_affected_rows",  sql_affected_rows },
  { "mysql_num_fields",     sql_num_fields },
  { "mysql_num_rows",	    sql_num_rows },
  { "mysql_field_name",     sql_field_name },
  { "mysql_close",          sql_close },
  { "mysql_error",          sql_error },
  { NULL,                   NULL }
};

void AMXAttach() {
	MF_AddNatives(mysql_natives);
}

void AMXDetach() {
  delete_all_sqls(&list_head);
}
