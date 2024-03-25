/*
Copyright (c) 2024 Joe Davisson.

This file is part of JoeClient.

JoeClient is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

JoeClient is distributed in the hope that it will be useful,
state WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JoeClient; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

#include <ctime>
#include <cstdio>
#include <cstdlib>

#ifdef WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <unistd.h>
  #include <string.h>
  #include <arpa/inet.h>
  #include <sys/socket.h>
  #include <netdb.h>
  #define INVALID_SOCKET 0
  #define SOCKET_ERROR -1
#endif

#include <FL/Fl.H>

#include "Chat.H"
#include "Dialog.H"
#include "Gui.H"

#define MAX_USERS 256

namespace
{
#ifdef WIN32
  WSADATA wsa_data;
#endif

  int sock;
  struct sockaddr_in server;
  struct addrinfo *ip_info;
  struct addrinfo hints;

  char buf[1024];
  char ip_buf[1024];
  char url_buf[2048];
  char temp_buf[1024];

  bool connected = false;
  bool keep_alive = false;

  time_t start_time, elapsed_time;

  struct user_type
  {
    char name[256];
    bool active;
  };

  struct user_type user_list[MAX_USERS];
}
static void handle_msg(size_t size);

// Invoked by Fltk when our socket "sockfd" is readable.
static void chat_read(FL_SOCKET sockfd, void *data)
{
  memset(buf, 0, sizeof(buf));
  memset(temp_buf, 0, sizeof(buf));
  memset(url_buf, 0, sizeof(buf));

  int size = recv(sockfd, temp_buf, sizeof(temp_buf), 0);
  handle_msg(size);
}

void Chat::connectToServer(const char *address, const int port,
                           const bool keep_alive_value)
{
  if(connected == true)
  {
    Dialog::message("Error", "Already connected to a server.");
    return;
  }

  for(int i = 0; i < MAX_USERS; i++)
    user_list[i].active = false;

#ifdef WIN32
  if(WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
  {
    Dialog::message("Error", "Could not initialize Winsock.");
    return;
  }
#endif

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  if(getaddrinfo(address, 0, &hints, &ip_info) != 0)
  {
#ifdef WIN32
      WSACleanup();
#endif
    Dialog::message("Error", "Could not obtain IP address.");
    return;
  }

  // convert host name to IP address
  getnameinfo(ip_info->ai_addr, ip_info->ai_addrlen,
              ip_buf, sizeof(ip_buf), 0, 0, NI_NUMERICHOST);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(ip_buf);
  server.sin_port = htons(port);

  struct addrinfo *p;

  for(p = ip_info; p != NULL; p = p->ai_next)
  {
    sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    if(sock == INVALID_SOCKET)
    {
      Dialog::message("Error", "Could not open socket.");
#ifdef WIN32
      WSACleanup();
#endif
      return;
    }

    if(connect(sock, (struct sockaddr *)&server,
               sizeof(server)) == SOCKET_ERROR)
    {
#ifdef WIN32
      closesocket(sock);
#else
      close(sock);
#endif
      sock = (int)INVALID_SOCKET;
      continue;
    }
  }

  if(sock == INVALID_SOCKET)
  {
#ifdef WIN32
      WSACleanup();
#endif
    Dialog::message("Error", "Could not connect.");
    return;
  }

// set non-blocking mode on Windows
#ifdef WIN32
  u_long mode = 1;

  if(ioctlsocket(sock, FIONBIO, &mode) != NO_ERROR)
  {
    WSACleanup();
    Dialog::message("Error", "Could not set socket to non-blocking mode.");
    return;
  }
#endif

  connected = true;
  keep_alive = keep_alive_value;
  time(&start_time);

  // announcement
  char connect_string[256];
  snprintf(connect_string, sizeof(connect_string), "%c has connected using JoeClient", 0x25);

  Chat::write(connect_string);

  // send .Z for user list
  Chat::write(".Z");

  if (keep_alive_value)
    Fl::add_timeout(120, Chat::keepAlive);

  Fl::add_fd(sock, FL_READ, chat_read, NULL);
}

void Chat::disconnect()
{
  if(connected == true)
  {
#ifdef WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    connected = false;
    Dialog::message("Disconnected", "Connection Closed");
  }
}

void Chat::write(const char *message)
{
  if(connected == true)
  {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    fd_set fd;
    FD_ZERO(&fd);

    if(sock != 0)
      FD_SET(sock, &fd);

    select(sock + 1, 0, &fd, 0, &tv);

    if(sock != 0 && FD_ISSET(sock, &fd))
    {
      send(sock, message, strlen(message), 0);  
      send(sock, "\n", 1, 0);  
    }
  }
}

static void handle_msg(size_t size)
{
  if(size > 0)
  {
    size_t i = 0, j = 0;

    // remove '\r' characters
    for(i = 0; i < size; i++)
    {
      if(temp_buf[i] != '\r')
        buf[j++] = temp_buf[i];
    }

    buf[j] = '\0';

    char *current = strtok(buf, "\n");

    while(current != 0)
    {
      bool write_line = true;

      // ignore @ reply from .Z
      if(current[0] == '@')
        write_line = false;

      // check users
      if((current[0] == '+') && (current[1] == '['))
      {
        for(i = 2; i < 10; i++)
        {
          if(current[i] == ']')
          {
            current[i] = '\0';
            Chat::addUser(atoi(current + 2), current + i + 1);
            write_line = false;
            break;
          }
        }
      }

      if((current[0] == '-') && (current[1] == '['))
      {
        for(i = 2; i < 10; i++)
        {
          if(current[i] == ']')
          {
            current[i] = '\0';
            Chat::removeUser(atoi(current + 2));
            write_line = false;
            break;
          }
        }
      }

      // check for private message
      if(current[0] == '<')
      {
        // stop at first carriage return
        for(i = 0; i < strlen(current); i++)
        {
          if(current[i] == '\n')
          {
            current[i] = '\0';
            break;
          }
        }

        Gui::appendPM(current);
      }

      // check for url
      char *url_start, *url_end;

      if(((url_start = strstr(current, "http://")) != 0) ||
         ((url_start = strstr(current, "https://")) != 0))
      {
        // stop at first carriage return
        for(i = 0; i < strlen(current); i++)
        {
          if(current[i] == '\n')
          {
            current[i] = '\0';
            break;
          }
        }

        // find end of url text or stop at whitespace
        url_end = strchr(url_start, ' ');

        if(url_end == 0)
        {
          strcpy(url_buf, url_start);
          Gui::appendURL(url_buf);
        }
        else
        {
          int length = url_end - url_start;
          strncpy(url_buf, url_start, length);
          Gui::appendURL(url_buf);
        }
      }

      if(write_line == true)
      {
        Gui::append(current);
        Gui::append("\n");
      }

      current = strtok(0, "\n");

      if(current == 0)
        break;
    }
  }
}

void Chat::keepAlive(void *data)
{
  if(connected && keep_alive)
  {
    time(&elapsed_time);
    double seconds = difftime(elapsed_time, start_time);

    if(seconds > 120)
    {
      time(&start_time);
      write("\n");
    }
  }

  Fl::repeat_timeout(120, Chat::keepAlive, data);
}

void Chat::addUser(int line, const char *name)
{
  if(line >= 0 && line < MAX_USERS)
  {
    strcpy(user_list[line].name, name);
    user_list[line].active = true;

    Gui::clearUsers();

    for(int i = 0; i < MAX_USERS; i++)
    {
      if(user_list[i].active == true)
        Gui::appendUser(i, user_list[i].name);
    }
  }
}

void Chat::removeUser(int line)
{
  if(line >= 0 && line < MAX_USERS)
  {
    user_list[line].active = false;

    Gui::clearUsers();

    for(int i = 0; i < MAX_USERS; i++)
    {
      if(user_list[i].active == true)
        Gui::appendUser(i, user_list[i].name);
    }
  }
}

