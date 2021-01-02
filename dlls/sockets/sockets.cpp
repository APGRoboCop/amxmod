/* (c) Copyright 2003, Ivan, -g-s-ivan@web.de
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

#include "sockets.h"

// The functions:

// native socket_open(_hostname[], _port, _protocol = SOCKET_TCP, &_error);
NATIVE(socket_open) {
  unsigned int port = params[2];
  int len;
  char* hostname = MF_GetAmxString(amx,params[1],0,len); // Get the hostname from AMX
  if(len == 0) { // just to prevent to work with a nonset hostname
    params[4] = 2;  // server unknown
    return -1;
  }
  params[4] = 0; // params[4] is error backchannel
  struct sockaddr_in server;
  struct hostent *host_info;
  unsigned long addr;
  int sock;
  int contr;
  // Create a Socket
  sock = socket(AF_INET, params[3] == 1 ? SOCK_STREAM : SOCK_DGRAM, 0);
  if(sock < 0) {
    // Error, couldn't create a socket, so set an error and return.
    params[4] = 1;
    return -1;
  }

  // Clear the server structure (set everything to 0)
  memset( &server, 0, sizeof (server));
  // Test the hostname, and resolve if needed
  if ((addr = inet_addr(hostname)) != INADDR_NONE) {
    // seems like hostname is a numeric ip, so put it into the structure
    memcpy( (char *)&server.sin_addr, &addr, sizeof(addr));
  }
  else {
    // hostname is a domain, so resolve it to an ip
    host_info = gethostbyname(hostname);
    if (host_info == NULL) {
      // an error occured, the hostname is unknown
      params[4] = 2;  // server unknown
      return -1;
    }
    // If not, put it in the Server structure
    memcpy( (char *)&server.sin_addr, host_info->h_addr, host_info->h_length);
  }
  // Set the type of the Socket
  server.sin_family = AF_INET;
  // Change the port to network byte order, and put it into the structure
  server.sin_port = htons(port);

  // Not, let's try to open a connection to the server
  contr = connect(sock, (struct sockaddr*)&server, sizeof( server));
  if(contr < 0) {
    // If an error occured cancel
    params[4] = 3;  //error while connecting
    return -1;
  }
  // Everything went well, so return the socket
  return sock;
}

// native socket_close(_socket);
NATIVE(socket_close) {
  int socket = params[1];
  #ifdef WIN32 // On windows, check whether the sockets are initialized correctly
  // Close the socket (windows)
  closesocket(socket);
  #else
  // Close the socket (linux/unix styled systems)
  close(socket);
  #endif
  return 0;
}

// native socket_change(_socket, _timeout=100000);
// 1 sec =1000000 usec
NATIVE(socket_change) {
  int socket = params[1];
  unsigned int timeout = params[2];
  // We need both a timeout structure and a fdset for our filedescriptor
  fd_set rfds;
  struct timeval tv;
  // Fill in ...
  FD_ZERO(&rfds);
  FD_SET(socket, &rfds);
  tv.tv_sec = 0;
  tv.tv_usec = timeout;
  // Now we "select", which will show us if new data is waiting in the socket's buffer
  if (select(socket+1, &rfds, NULL, NULL, &tv))
     return 1; // Ok, new data, return it
   else
     return 0; // No new data, return it
}

// native socket_recv(_socket, _string[], _length);
NATIVE(socket_recv) {
  int socket = params[1];
  int length = params[3];
  int tmp = -1;
  // First we dynamicly allocate a block of heap memory for recieving our data
  char *tmpchar = new char[length];
  if(tmpchar == NULL) return -1; // If we didn't got a block, we have to quit here to avoid sigsegv
  // And set it all to 0, because the memory could contain old trash
  memset(tmpchar, 0, length);
  // Now we recieve
  tmp = recv(socket, tmpchar, length-1, 0);
  // And put a copy of our recieved data into amx's string
  tmpchar[tmp]='\0';
  int nlen = 0;
  int max = length-1;
  const char* src = tmpchar;
  cell* dest = MF_GetAmxAddr(amx,params[2]);
  while(max--&&nlen<tmp){
   *dest++ = (cell)*src++;
   ++nlen;
  }
  *dest = 0;
  // And we need to free up the space to avoid wasting memory
  delete [] tmpchar;
  // And finally, return the what recv returned
  return tmp;
}

// native socket_send(_socket, _data[], _length);
NATIVE(socket_send) {
  int socket = params[1];
  // We get the string from amx
  int len;
  char* data = MF_GetAmxString(amx,params[2],0,len);
  // And send it to the socket
  return send(socket, data, len, 0);
}

// Natives to register

AMX_NATIVE_INFO sockets_natives[] = {
    {"socket_open",   socket_open},
    {"socket_close",  socket_close},
    {"socket_change", socket_change},
    {"socket_recv",   socket_recv},
    {"socket_send",   socket_send},
    {NULL,            NULL}
};
